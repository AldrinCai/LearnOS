section .data
str_c_lib: db "c library says: hello world!", 0xa
str_lib_len equ  $-str_c_lib

str_syscall: db "syscall says: hello world!", 0xa
str_syscall_len equ $-str_syscall

section .text
global _start
_start:
;方式1
	push str_lib_len
	push str_c_lib
	push 1

	call simu_write
	add esp, 12

;方式2
	mov eax, 4
	mov ebx, 1
	mov ecx, str_syscall
	mov edx, str_syscall_len
	int 0x80
	mov eax, 1
	mov ebx, 0
	int 0x80

simu_write:
	push ebp
	mov ebp, esp
	mov eax, 4
	mov ebx, [ebp + 8]
	mov ecx, [ebp + 12]
	mov edx, [ebp + 16]
	int 0x80
	pop ebp
	ret