TI_GDT equ 0
RPL0 equ 0
SELECTOR_VIDEO equ (0x0003 << 3) + TI_GDT + RPL0

[bits 32]
section .text
global put_char
put_char:
	pushad
	;保证 gs 为显卡的段选择子   
	mov ax, SELECTOR_VIDEO
	mov gs, ax

;获取光标位置
	;先获取高 8 位
	mov dx, 0x03d4
	mov al, 0x0e
	out dx, al
	mov dx, 0x03d5
	in al, dx
	mov ah, al

	; 再获取低 8 位
	mov dx, 0x03d4
	mov al, 0x0f
	out dx, al
	mov dx, 0x03d5
	in al, dx
	mov bx, ax

; 开始打印
	mov ecx, [esp + 36] ; pushad 保存了8个寄存器的值再加上调用函数的返回地址
	cmp cl, 0xd ; 回车
	jz .is_carriage_return

	cmp cl, 0xa ; 换行
	jz .is_line_feed

	cmp cl, 0x8 ;退格
	jz .is_backspace 
	jmp .put_other

.is_backspace:
	dec bx
	shl bx, 1

	mov byte [gs:bx], 0x20
	inc bx
	mov byte [gs:bx], 0x07
	shr bx, 1
	jmp .set_cursor

.put_other:
	shl bx, 1

	mov [gs:bx], cl
	inc bx
	mov byte [gs:bx], 0x07
	shr bx, 1
	inc bx
	cmp bx, 2000
	jl .set_cursor

.is_line_feed:
.is_carriage_return:
	xor dx, dx
	mov ax, bx
	mov si, 80
	div si

	sub bx, dx

.is_carriage_return_end:
	add bx, 80
	cmp bx, 2000
.is_line_feed_end:
	jl .set_cursor











