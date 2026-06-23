#include <buddy_pmm.h>

// [MA] --> Multi-allocator !

MA_manager g_manager;

// sizes:
uint64_t PAGE_SIZE = 4096;


static inline page_node_t *to_virt(uintptr_t phys_addr) {
  return (page_node_t *)(phys_addr + hhdm_offset);
}


void ma_init() {
  // here we will check the responses
  if (memmap_request.response == NULL || hhdm_request.response == NULL) {
    hal_print("memmap or hhdm response is NULL, Our mem allocator failed",
              0xff0000, 1);
    while (1)
      ;
  }
  hhdm_offset = hhdm_request.response->offset;
  struct limine_memmap_response *memmap = memmap_request.response;

  // zero all the manager vars
  g_manager.total_usable_size = 0;

  int found_first             = 0;
  uint64_t first_usable_base  = 0;

  // lets make a loop in the memory
  for (uint64_t i = 0; i < memmap->entry_count; i++) {
    struct limine_memmap_entry *entry = memmap->entries[i];

    uintptr_t current_addr = entry->base;
    uint64_t remaining_len = entry->length;

    if (entry->type == LIMINE_MEMMAP_USABLE) {
      if (found_first == 0) {
        first_usable_base = current_addr;
        found_first = 1;        // so not repeated again
      }
      g_manager.total_usable_size += remaining_len;
    }
  }

  // Get total pages
  uint64_t total_pages  =  g_manager.total_usable_size / PAGE_SIZE;
  
  for (int i = 0; i < BUDDY_MAX_ORDER; i++) {
    g_manager.buddy_free_list[i] = NULL;
  }

  // HERE OUR BUDDY CODE
  // lets make a loop in the memory
  for (uint64_t i = 0; i < memmap->entry_count; i++) {
    struct limine_memmap_entry *entry = memmap->entries[i];

    uintptr_t current_addr = entry->base;
    uint64_t remaining_len = entry->length;
    if (entry->type == LIMINE_MEMMAP_USABLE) {
      for (int order = BUDDY_MAX_ORDER - 1; order >= 0; order--) {
        // Get size
        uint64_t block_size = (1ULL << order) * PAGE_SIZE;
        while (remaining_len >= block_size) {
          // Ge virsual addr
          page_node_t *block = to_virt(current_addr);
          block->next = (uintptr_t)g_manager.buddy_free_list[order];
          g_manager.buddy_free_list[order] = block;
          current_addr += block_size;
          remaining_len -= block_size;
        }
      }
    }
  }

  // STILL WORKING HERE

  // ------------------------- [ Early Debug Output ] -------------------------
  hal_print("\n[MA_PMM] --- Memory Management Initialized --- \n", 0x00FF00, 1);
  
  hal_print("  [>] Total Usable RAM (Buddy System) : ", 0xFFFFFF, 1);
  hal_print_dec(g_manager.total_usable_size / 1024 / 1024, 0x00FFFF, 1);
  hal_print(" MB\n", 0xFFFFFF, 1);
}





__attribute__((optimize("O0") , noreturn)) void OOM() {
  hal_cls(0x9f0c0a);
  __asm__ volatile ("cli");
  cursor_y = 38;
  hal_print("   !!! BARQ OS - OOM !!!\n", 0xdddddd, 2);
  hal_print("SYSTEM HALTED, Out Of Memory:\n\n", 0xdddddd, 2);
  hal_print("Buy more ram!", 0xb0b0b0, 1);
  while(1) {
    __asm__ volatile ("hlt");
  }
}
// fixed! the Out of memory!





void *buddy_allocator(size_t size) {
  uint64_t pages_needed = (size + 4095) / 4096;
  volatile int req_order = 0;
  while ((1ULL << req_order) < pages_needed) { req_order++; } // get the 2 ^ Order
  volatile int current_order = req_order;
  while (current_order < BUDDY_MAX_ORDER && g_manager.buddy_free_list[current_order] == NULL) {
    current_order++;   // to check if there is a block in free list
  }
  if (current_order == BUDDY_MAX_ORDER) { 
    OOM(); 
    return NULL;
  }
  // edit next in free list
  page_node_t *block = g_manager.buddy_free_list[current_order];
  if (block == NULL) {OOM();}
  g_manager.buddy_free_list[current_order] = (void *)block->next;
  // Now the spliting command
  // first check if needs splitting
  while (current_order > req_order) {
    current_order--;
    uint64_t buddy_size = (1ULL << current_order) * 4096;
    page_node_t *buddy = (page_node_t *)((uintptr_t)block + buddy_size);
    buddy->next = (uintptr_t)g_manager.buddy_free_list[current_order];
    g_manager.buddy_free_list[current_order] = buddy;
  }
  return (void *)block;
}

void *ma_alloc(size_t size) {
  if (size == 0) {
    return NULL;
  }
  size_t estimated_size = (size + 7) & ~(7);
  if (estimated_size > g_manager.total_usable_size) {
    OOM();
  } else {
    // we will use buddy allocator!
    return buddy_allocator(size);
  }
}

/*
void ma_free(void *ptr, size_t size) {
  if (ptr == NULL || size == 0) return;
  uint64_t pages_needed = (size + 4095) / 4096;
  int req_order = 0;
  while ((1ULL << req_order) < pages_needed) { req_order++; }
  uintptr_t block_phys = (uintptr_t)ptr - hhdm_offset;
  int current_addr = req_order;
  while (current_order < (BUDDY_MAX_ORDER - 1)) {
    int current_size = (1ULL << current_order) * PAGE_SIZE;
    uintptr_t buddy_phy = block_phys ^ current_size;
    page_node_t *buddy_virt = to_virt((uintptr_t)buddy_phy) ;
    page_node_t **prev = &g_manager.buddy_free_list[current_order];
    page_node_t *curr = *prev;
    int found = 0;
    while (curr != NULL) {
      if (curr == buddy_virt) {
          found = 1;
          break;
      }
      prev = &curr->next;
      curr = curr->next;
    }
    if (found) {
      *prev = curr->next;
      if (buddy_phy < block_phys) {
        // The buddy is first! we'll start the big block from the buddy
        block_phys = buddy_phy;
      }
    }
  }
}
*/