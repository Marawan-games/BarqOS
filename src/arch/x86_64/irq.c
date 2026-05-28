#include "pic.h"
#include "irq.h"
#include "isr.h"
#include <screen.h>


typedef void (*IRQHandler)(Registers* regs);
IRQHandler g_IRQHandler[16];

void IRQs_Handler(Registers* regs) {
    int irq = regs->int_no - 0x20;
    if (g_IRQHandler[irq] != NULL) {
        g_IRQHandler[irq](regs);
    }
    else {
        print("Unhandled IRQ", 0xFFFFFF, 1);
    }
    PIC_SendEOI(irq);
}

void IRQ_Intialize() {
    pic_remap();
    __asm__ volatile ("sti");
    g_IRQHandler[0] = timer_handler;
}