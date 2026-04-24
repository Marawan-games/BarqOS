#include <panic.h>


struct interrupt_frame {
    // By pushALL
    uint64_t r15, r14, r13, r12, rbp, rbx, r11, r10, r9, r8, rdi, rsi, rdx, rcx, rax;
    uint64_t ds; // alone
    uint64_t vector;
    uint64_t error_code;
    uint64_t rip, cs, rflags, rsp, ss;
};

void handler_c(struct interrupt_frame *frame){
    if (frame->vector == 3) {
        print_step("(Interrupt Triggered!)" , 0xFFFF00 , 1);
    }
    else {
        //print_step( frame->vector , 0xFF0000 , 1);
        print_step("Interrupt found!" , 0xFF0000 , 1);
        hcf();
    }
    //hcf();
    return;
}