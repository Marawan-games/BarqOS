#include <include.h>

// Kernel Entry
void kmain(void) {
  if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false) {
    hcf();
  }
  framebuffer_init();
  hal_cls(0x0A0F1F);
  hal_init();
  ma_init(); // PMM
  //hal_cls(0x0A0F1F);

  hal_print("Starting BarqOS... working \n", 0xFFFF00, 1);

  __asm__ volatile ("sti");

  void *test1 = ma_alloc(100);
  *(char *)test1 = 'A';
  hal_print("Test 1 in 0x", 0xF0FFFF, 1);
  hal_print_hex((uintptr_t)test1, 0xF0FFFF, 1);
  hal_print("\n", 0xF0FFFF, 1);

  void *test2 = ma_alloc(800);
  *(char *)test2 = 'B';
  hal_print("Test 2 in 0x", 0xF0FFFF, 1);
  hal_print_hex((uintptr_t)test2, 0xF0FFFF, 1);
  hal_print("\n", 0xF0FFFF, 1);

  void *test3 = ma_alloc(3000);
  *(char *)test3 = 'C';
  hal_print("Test 3 in 0x", 0xF0FFFF, 1);
  hal_print_hex((uintptr_t)test3, 0xF0FFFF, 1);
  hal_print("\n", 0xF0FFFF, 1);
  void *oom_test = ma_alloc(984ULL * 1024 * 1024);

  // We're done, just hang...
  hcf();
}

// Halt and catch fire function.
void hcf(void) {
  for (;;) {
    asm("hlt");
  }
}