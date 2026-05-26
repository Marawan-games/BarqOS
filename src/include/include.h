#ifndef INCLUDE_H
#define INCLUDE_H



#include <screen.h>
#include <port.h>
#include <handlers.h>
#include <gdt.h>
#include "../arch/x86_64/irq.h"

extern void idt_init();

#endif