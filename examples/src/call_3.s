SECTION CALL_3 vstart=0x7c00
	
	call 0:far_proc
	jmp $
far_proc:
	mov ax, 0x1234
	retf	
	
    times 510-($-$$) db 0
	db 0x55, 0xaa
