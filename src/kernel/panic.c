#include "panic.h"
void c_divide_by_zero_handler() {
    // 1. اقلب الخلفية كلها لون أحمر غامق بتاعة الـ Panic الشيك (مثلاً 0x8B0000 أو 0x550000)
    cls(0x4A0000); 

    current_y = 267;
    
    print_step(" !!! BARQ OS - KERNEL PANIC !!! " , 0xFFFFFF , 1);
    print_step("SYSTEM HALTED DUE TO UNHANDLED EXCEPTION:" , 0xFFC107 , 1);
    print_step("-> EXCEPTION 0x00: DIVIDE BY ZERO ERROR" , 0xFFFFFF , 1);
    print_step("The processor was forced to halt to protect your hardware." , 0xDDDDDD , 1);
    
    // 4. اشنق المعالج هنا
    while(1) {
        __asm__("hlt");
    }
}