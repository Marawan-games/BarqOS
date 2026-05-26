#include "handlers.h"
#include "port.h"
void c_divide_by_zero_handler() {
    cls(0x4A0000); 

    current_y = 267;
    
    print_step(" !!! BARQ OS - KERNEL PANIC !!! " , 0xFFFFFF , 1);
    print_step("SYSTEM HALTED DUE TO UNHANDLED EXCEPTION:" , 0xFFC107 , 1);
    print_step("-> EXCEPTION 0x00: DIVIDE BY ZERO ERROR" , 0xFFFFFF , 1);
    print_step("The processor was forced to halt to protect your hardware." , 0xDDDDDD , 1);
    
    while(1) {
        __asm__("hlt");
    }
}

void c_keyboard_handler() {
    uint8_t volatile scancode_num = inb(0x60);  //reading the scancode
    char temp_str[2]; 
    temp_str[0] = scancode_num;
    temp_str[1] = '\0';
    print_step("Keboard interrupt" , 0xFFC107 , 1);
    print_step("Scancode -> " , 0xFFC107 , 1);
    print(temp_str , 0xFFC107 , 1);
    outb(0x20, 0x20);
}