#include "handlers.h"
#include "port.h"

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

int ticks = 0;

void c_timer_handler() {
    temp_ticks[0] = ticks;
    temp_ticks[1] = '\0';
    print_step("Ticks : " , 0xFFC107 , 1);
    print( temp_ticks , 0xFFC107 , 1);
    ticks++;
}