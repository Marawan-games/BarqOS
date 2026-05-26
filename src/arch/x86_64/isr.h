#ifndef ISR_H
#define ISR_H

#include <stdint.h>

typedef struct {
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rbp, rdi, rsi, rdx, rcx, rbx, rax;

    uint64_t int_no, error_code;
    
    uint64_t rip, cs, rflags, rsp, ss;
} __attribute__((packed)) Registers;

/*static const char* exception_names[] = {
    "Divide by Zero",        // 0x00
    "Debug",                 // 0x01
    "NMI",                   // 0x02
    "Breakpoint",            // 0x03
    "Overflow",              // 0x04
    "Bound Range Exceeded",  // 0x05
    "Invalid Opcode",        // 0x06
    "Device Not Available",  // 0x07
    "Double Fault",          // 0x08
    "General Protection",    // 0x0D
    "Page Fault",            // 0x0E
};*/

#endif
