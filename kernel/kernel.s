[bits 32]
%define ERROR_CODE nop  ;如果异常中 CPU 已经压入了错误码，为了格式统一不做操作

%define ZERO push 0     ;如果异常中 CPU 没有压入错误码，为了统一格式手动压入0

extern put_str;
extern idt_table;

section .data
global intr_entry_table
intr_entry_table:

%macro VECTOR 2
section .text
intr%1entry:
    %2
    push ds
    push es
    push fs
    push gs
    pushad

    mov al, 0x20  ;中断解释命令 EOI
    out 0xa0, al  ;向从片发送
    out 0x20, al  ;向主片发送

    push %1

    call [idt_table + %1*4]
    jmp intr_exit

section .data
    dd intr%1entry  ;存储各个中断入口程序的地址，形成 intr_entry_table 数组
%endmacro

section .text
global intr_exit
intr_exit:
    add esp, 4
    popad
    pop gs
    pop fs
    pop es
    pop ds
    add esp, 4
    iretd

VECTOR 0x00, ZERO
VECTOR 0x01, ZERO
VECTOR 0x02, ZERO
VECTOR 0x03, ZERO
VECTOR 0x04, ZERO
VECTOR 0x05, ZERO
VECTOR 0x06, ZERO
VECTOR 0x07, ZERO
VECTOR 0x08, ERROR_CODE
VECTOR 0x09, ZERO
VECTOR 0x0a, ERROR_CODE
VECTOR 0x0b, ERROR_CODE
VECTOR 0x0c, ZERO
VECTOR 0x0d, ERROR_CODE
VECTOR 0x0e, ERROR_CODE
VECTOR 0x0f, ZERO
VECTOR 0x10, ZERO
VECTOR 0x11, ERROR_CODE
VECTOR 0x12, ZERO
VECTOR 0x13, ZERO
VECTOR 0x14, ZERO
VECTOR 0x15, ZERO
VECTOR 0x16, ZERO
VECTOR 0x17, ZERO
VECTOR 0x18, ERROR_CODE
VECTOR 0x19, ZERO
VECTOR 0x1a, ERROR_CODE
VECTOR 0x1b, ERROR_CODE
VECTOR 0x1c, ZERO
VECTOR 0x1d, ERROR_CODE
VECTOR 0x1e, ERROR_CODE
VECTOR 0x1f, ZERO
VECTOR 0x20, ZERO
