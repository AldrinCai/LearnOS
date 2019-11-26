TI_GDT equ 0
RPL0 equ 0
SELECTOR_VIDEO equ (0x0003 << 3) + TI_GDT + RPL0

section .data
put_int_buffer dq 0

[bits 32]
section .text
global put_int
put_int:
	pushad
	mov ebp, esp
	mov eax, [ebp + 4*9]
	mov edx, eax
	mov edi, 7
	mov ecx, 8
	mov ebx, put_int_buffer ;缓冲区

.16based_4bits:
	and edx, 0x0000000F
	cmp edx, 9
	jg .is_A2F
	add edx, '0'
	jmp .store
.is_A2F:
	sub edx, 10
	add edx, 'A'
.store:
	mov [ebx + edi], dl
	dec edi
	shr eax, 4
	mov edx, eax
	loop .16based_4bits

.ready_to_print:
	mov edi, 0
.skip_prefix_0:
	cmp edi, 8
	je .full0
.go_on_skip:
	mov cl, [put_int_buffer + edi]
	inc edi
	cmp cl, '0'
	je .skip_prefix_0
	dec edi
	jmp .put_each_num

.full0:
	mov cl, '0'
.put_each_num:
	push ecx
	call put_char
	add esp, 4
	inc edi
	mov cl, [put_int_buffer + edi]
	cmp edi, 8
	jl .put_each_num
	popad
	ret


;put_str 通过 put_char 来打印 \0 结尾的字符串
global put_str
put_str:
	push ebx
	push ecx
	xor ecx, ecx
	mov ebx, [esp + 12]
.goon:
	mov cl, [ebx]
	cmp cl, 0
	jz .str_over
	push ecx 
	call put_char
	add esp, 4
	inc ebx
	jmp .goon
.str_over:
	pop ecx
	pop ebx
	ret


global put_char
put_char:
	pushad
	;保证 gs 为显卡的段选择子   
	mov ax, SELECTOR_VIDEO
	mov gs, ax

;获取光标位置
	;先获取高 8 位
	mov dx, 0x03d4
	mov al, 0x0e ;低 8 位光标寄存器索引
	out dx, al
	mov dx, 0x03d5
	in al, dx
	mov ah, al

	; 再获取低 8 位
	mov dx, 0x03d4
	mov al, 0x0f ;高 8 位寄存器光标索引 
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

; 超出屏幕范围开始滚屏
.roll_screen:
	cld
	mov ecx, 960 ; 2000 - 80 = 1920, 1920*2/4 = 960

	mov esi, 0xc00b80a0 ; 第 1 行行首
	mov edi, 0xc00b8000 ; 第 0 行行首
	rep movsd

	;最后一行填充为空白
	mov ebx, 3840
	mov ecx, 80

.cls:
	mov word [gs:ebx], 0x0720
	add ebx, 2
	loop .cls
	mov bx, 1920

.set_cursor:
	;先设置高 8 位
	mov dx, 0x03d4
	mov al, 0x0e
	out dx, al
	mov dx, 0x03d5
	mov al, bh
	out dx, al
	;设置低 8 位
	mov dx, 0x03d4
	mov al, 0x0f
	out dx, al
	mov dx, 0x03d5
	mov al, bl
	out dx, al
	
.put_char_done:
	popad
	ret









