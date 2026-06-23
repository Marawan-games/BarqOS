#ifndef buddy_PMM_H
#define buddy_PMM_H

// Includes
#include <limine.h>
#include <stddef.h>
#include <hal.h>

#define BUDDY_MAX_ORDER 16      // From 0 to 15 ()

typedef struct {
    uint64_t total_usable_size; // To get ml_size and buddy_size

    void* buddy_free_list[BUDDY_MAX_ORDER]; 
} MA_manager;

typedef struct {
    uintptr_t next;
}page_node_t;

extern MA_manager g_manager;
extern int current_y;
// Requests
extern uintptr_t hhdm_offset;
extern volatile struct limine_memmap_request memmap_request;
extern volatile struct limine_hhdm_request hhdm_request;

// Forward declarations
void ma_init() ;
void *ma_alloc(size_t size);

#endif