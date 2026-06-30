#include <include.h>

// Kernel Entry
void kmain(void) {
  if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false) {
    hcf();
  }
  framebuffer_init();
  hal_cls(0x0A0F1F);
  hal_init();

  hal_print("Starting BarqOS... working \n", 0xFFFF00, 1);

  __asm__ volatile ("sti");

  // We're done, just hang...
  hcf();
}

// Halt and catch fire function.
void hcf(void) {
  for (;;) {
    asm("hlt");
  }
}