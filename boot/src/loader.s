%include "include/boot.inc"
section loader vstart=LOADER_BASE_ADDR

; gs 寄存器在 mbr 中已设置
	mov byte [gs:0x0a], '2'
	mov byte [gs:0x0b], 0xa4	

	mov byte [gs:0x0c], ' '
	mov byte [gs:0x0d], 0xa4

	mov byte [gs:0x0e], 'L'
	mov byte [gs:0x0f], 0xa4

	mov byte [gs:0x10], 'O'
	mov byte [gs:0x11], 0xa4

	mov byte [gs:0x12], 'A'
	mov byte [gs:0x13], 0xa4

	mov byte [gs:0x14], 'D'
	mov byte [gs:0x15], 0xa4

	mov byte [gs:0x16], 'E'
	mov byte [gs:0x17], 0xa4

	mov byte [gs:0x18], 'R'
	mov byte [gs:0x19], 0xa4

	jmp $
