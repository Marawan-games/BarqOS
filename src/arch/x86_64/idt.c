#include "idt.h"
#include <stdint.h>


typedef struct {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t type_attr;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t Reserved;
} __attribute__((packed)) IDTEntry;

typedef struct {                 // that's what we'll load
    uint16_t Limit;             // sizeof(idt) - 1
    uint64_t Base;             // base addres of IDT
}__attribute__((packed)) IDTR;

IDTEntry g_IDT[256];

IDTR IDTDescriptor = {
    .Limit = sizeof(g_IDT) - 1,
    .Base = (uint64_t)&g_IDT
};


void IDT_load(IDTR* IDTDescriptor_ptr);

void SET_IDT_ENTRY(int interrupt_num , void* offset , uint16_t segment_selector , uint8_t flags) {
    g_IDT[interrupt_num].offset_low = ((uint64_t)offset) & 0xffff;
    g_IDT[interrupt_num].selector = segment_selector;
    g_IDT[interrupt_num].ist = 0; // that's only for now 
    g_IDT[interrupt_num].type_attr = flags;
    g_IDT[interrupt_num].offset_mid = ((((uint64_t) offset) >> 16) & 0xffff);
    g_IDT[interrupt_num].offset_high = ((((uint64_t) offset) >> 32) & 0xffffffff);
    g_IDT[interrupt_num].Reserved = 0;
}

void idt_init() {
    for (int i = 0; i < 256; i++) {
        SET_IDT_ENTRY(i, (void*)isr_stub_table[i], 0x08, 0x8E);
    }

    IDT_load(&IDTDescriptor);
}