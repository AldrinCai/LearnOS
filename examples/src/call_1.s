SECTION CALL_1 vstart=0x7c00
call near near_proc
jmp $
addr dd 4
near_proc:
	mov ax, 0x1234
	ret

times 510-($-$$) db 0
db 0x55, 0xaa
