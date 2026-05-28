#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>
#include <include.h>
#include <../arch/x86_64/pic.h>

// Set the base revision to 6 (the latest)

__attribute__((used, section(".limine_requests")))
static volatile uint64_t limine_base_revision[] = LIMINE_BASE_REVISION(6);

// Frambuffer request

__attribute__((used, section(".limine_requests")))
volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
    .revision = 0
};

// The start and end markers for the Limine requests.

__attribute__((used, section(".limine_requests_start")))
static volatile uint64_t limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile uint64_t limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;


// Copied from OSdev wiki for future understanding & really intalizing them


// GCC and Clang reserve the right to generate calls to the following
// 4 functions even if they are not directly called.
// Implement them as the C specification mandates.
// DO NOT remove or rename these functions, or stuff will eventually break!
// They CAN be moved to a different .c file.

/*void *memcpy(void *restrict dest, const void *restrict src, size_t n) {
    uint8_t *restrict pdest = (uint8_t *restrict)dest;
    const uint8_t *restrict psrc = (const uint8_t *restrict)src;

    for (size_t i = 0; i < n; i++) {
        pdest[i] = psrc[i];
    }

    return dest;
}

void *memset(void *s, int c, size_t n) {
    uint8_t *p = (uint8_t *)s;

    for (size_t i = 0; i < n; i++) {
        p[i] = (uint8_t)c;
    }

    return s;
}

void *memmove(void *dest, const void *src, size_t n) {
    uint8_t *pdest = (uint8_t *)dest;
    const uint8_t *psrc = (const uint8_t *)src;

    if ((uintptr_t)src > (uintptr_t)dest) {
        for (size_t i = 0; i < n; i++) {
            pdest[i] = psrc[i];
        }
    } else if ((uintptr_t)src < (uintptr_t)dest) {
        for (size_t i = n; i > 0; i--) {
            pdest[i-1] = psrc[i-1];
        }
    }

    return dest;
}

int memcmp(const void *s1, const void *s2, size_t n) {
    const uint8_t *p1 = (const uint8_t *)s1;
    const uint8_t *p2 = (const uint8_t *)s2;

    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] < p2[i] ? -1 : 1;
        }
    }

    return 0;
}

*/


////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// Kernel Entry ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
void kmain(void) {

    __asm__("cli");

    // Ensure the bootloader actually understands our base revision (Support it)
    if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false) {
        hcf();
    }

    framebuffer_init() ;

    cls(0x111424); 

    uint32_t pad = 20;
    uint32_t border_color = 0xFFC107;

    draw_vertical_line(pad, border_color);
    draw_vertical_line(framebuffer->width - pad, border_color);
    draw_horizental_line(pad, border_color);
    draw_horizental_line(framebuffer->height - pad, border_color);

    current_y = 60; 
    print_step("   ⚡ BarqOS ⚡   ", 0xFFC107, 4); 
    print_step("Fast like lightning (Maybe)", 0xFFFFFF, 1);

    current_y += 20;
    draw_horizental_line(current_y, 0x334155); 

    current_y += 40;
    
    print_step("[ OK ] Framebuffer Initialized.\n", 0x00d48d, 1);
    
    print_step("[..] Loading Global Descriptor Table (GDT)...", 0xFFFFFF, 1);
    gdt_init();
    print_step(" [ DONE ]\n", 0x00d48d, 1);
    
    print_step("[..] Loading Interrupt Descriptor Table (IDT)...", 0xFFFFFF, 1);
    idt_init();
    print_step(" [ DONE ]\n", 0x00d48d, 1);

    print_step( "RIQ intializing" , 0xFFFFFF , 1);
    disable_apic();
    IRQ_Intialize(); 
    print_step(" [ DONE ]\n", 0x00d48d, 1);

    //__asm__ volatile  ("int $0x20");
    //__asm__ volatile  ("int $0x20");
    __asm__ volatile ("sti");

    

    cursor_x = 1;
    cursor_y = 1;
    // We're done, just hang...
    while(1) {
        __asm__ volatile("hlt");
    }
}

//////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
// Halt and catch fire function.
void hcf(void) {
    for (;;) {
        asm ("hlt");
    }
}


void disable_apic() {
    __asm__ volatile ("mov $0x1B, %rcx\nrdmsr\nand $~0x800, %rax\nwrmsr"); 
}