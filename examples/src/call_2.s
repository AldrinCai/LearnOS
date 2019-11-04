SECTION CALL_2 vstart=0x7c00
	mov word [addr], near_proc
	call [addr]
	mov ax, near_proc
	call ax
	jmp $

	addr db 4

near_proc:
	mov ax, 0x1234
	ret

	times 510-($-$$) db 0
	db 0x55, 0xaa
