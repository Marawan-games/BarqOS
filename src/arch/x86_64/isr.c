#include "isr.h"
#include <screen.h>
#include "idt.h"


extern void c_divide_by_zero_handler() ;
void BarqOS_ISR_InitializeGates(){
    for (int i = 0; i < 256; i++) 
    {
        SET_IDT_ENTRY(i, (void*)isr_stub_table[i], 0x08, 0x8E);
    }
}

void isr_handler(Registers* regs) {
    if (regs->int_no == 0) {
        c_divide_by_zero_handler();
    }
    else {
        print_step("Interrupt triggered!", 0x00d48d, 1);
        print_step("Vector:", 0x00d48d, 1);
        
        char buf[16];
        uint_to_string(regs->int_no, buf);
        
        print_step(buf, 0xFFC107, 1);
        while(1);
    }
}