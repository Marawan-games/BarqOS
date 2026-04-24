[bits 64]
default rel

global idt_init

section .bss

align 16
idt: 
    resb 256 * 16    



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
section .data

align 16
idtr:
    dw (256 * 16) - 1
    dq idt

%macro ISR 1
global isr%1
isr%1:
    push 0
    push %1
    jmp common_interrupt_handler
%endmacro

%macro ISR_ERR 1
global isr%1
isr%1:
    push %1
    jmp common_interrupt_handler
%endmacro

%macro set_gate 3
    mov rax, %2             ; Load address directly
    lea rdi, [rel idt]      
    mov rsi, %1
    shl rsi, 4              
    add rdi, rsi        

    mov [rdi], ax           ; Low 16 bits
    mov word [rdi + 2], %3  ; Selector
    mov byte [rdi + 5], 0x8E ; Attributes
    
    shr rax, 16
    mov [rdi + 6], ax       ; Mid 16 bits
    
    shr rax, 16
    mov [rdi + 8], eax      ; High 32 bits
%endmacro

%macro pushAll 0
    push rax
    push rcx
    push rdx
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push rbx
    push rbp
    push r12
    push r13
    push r14
    push r15
%endmacro

%macro popAll 0
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbp
    pop rbx
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rax
%endmacro

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
section .text

idt_init:
    %assign i 0
    %rep 256
        set_gate i, isr %+ i, 0x08
        %assign i i+1
    %endrep

    ; 3. Load IDT
    lidt [rel idtr]
    ret

ISR             0x0
ISR             0x1
ISR             0x2
ISR             0x3
ISR             0x4
ISR             0x5
ISR             0x6
ISR             0x7
ISR_ERR         0x8
ISR             0x9
ISR_ERR         0xA
ISR_ERR         0XB
ISR_ERR         0XC
ISR_ERR         0XD
ISR_ERR         0XE
ISR             0XF
ISR             0X10
ISR_ERR         0X11
ISR             0X12
ISR             0X13
ISR             0X14
ISR             0X15
ISR             0X16
ISR             0X17
ISR             0X18
ISR             0X19
ISR             0X1A
ISR             0X1B
ISR             0X1C
ISR_ERR         0X1D
ISR_ERR         0X1E
ISR             0X1F
;complete the ISRs
%assign i 0x20
%rep    0xFF - 0x20 + 1
    ISR i
    %assign i i+1
%endrep


extern handler_c

common_interrupt_handler:
    pushAll

    mov ax, ds
    push rax

    mov        ax, 0x10
    mov        ds, ax
    mov        es, ax
    mov        fs, ax
    mov        gs, ax

    mov rdi, rsp
    call    handler_c

    pop        rax
    mov        ds, ax
    mov        es, ax
    mov        fs, ax
    mov        gs, ax

    popAll

    add        rsp, 16
    sti
    iretq