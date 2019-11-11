%include "include/boot.inc"
section loader vstart=LOADER_BASE_ADDR
	LOADER_STACK_TOP equ LOADER_BASE_ADDR
	jmp loader_start

	;构建 GDT 及内部的描述符
	GDT_BASE: dd 0x00000000 
			dd 0x00000000

	CODE_DESC: dd 0x0000FFFF 
			dd DESC_CODE_HIGH4

	DATA_STACK_DESC: dd 0x0000FFFF 
					dd DESC_DATA_HIGH4

	VIDEO_DESC: dd 0x80000007 
				dd DESC_VIDEO_HIGH4

	GDT_SIZE equ $ - GDT_BASE
	GDT_LIMIT equ GDT_SIZE - 1
	times 60 dq 0  ;预留空位
	;保存内存容量，之后会在内核中引用才地址
	total_mem_bytes dd 0

	SELECTOR_CODE equ (0x0001 << 3) + TI_GDT + RPL0
	SELECTOR_DATA equ (0x0002 << 3) + TI_GDT + RPL0
	SELECTOR_VIDEO equ (0x0003 << 3) + TI_GDT + RPL0

	;gdt 指针
	gdt_ptr dw GDT_LIMIT 
			dd GDT_BASE

	;人工对齐
	ards_buf times  244 db 0
	ards_nr dw 0    ;记录 ARDS 结构体数量
	


loader_start:
;------------------------------------------------
;检测可用内存大小
;------------------------------------------------
	xor ebx, ebx ;第一次调用 ebx 为 0
	mov edx, 0x534d4150 ; 固定签名
	mov di, ards_buf   ; ARDS 缓冲区
;------------------------------------------------
;int 0x15 中断 功能号 0x0000e820
;------------------------------------------------
.e820_mem_get_loop:
	mov eax, 0x0000e820 ; 字功能号
	mov ecx, 20    ;ARDS 结构的字节大小
	int 0x15
	jc .e820_failed_so_try_e801 ;CF 位为 0 调用出错进行跳转
	add di, cx
	inc word [ards_nr]
	cmp ebx, 0   ;为 0 说明是最后一个
	jnz .e820_mem_get_loop

	;找出最大值，最大的内存块一定可以被使用
	mov cx, [ards_nr]
	mov ebx, ards_buf
	xor edx, edx ; 先进行清 0 操作 防止误判
.find_max_mem_area:
	mov eax, [ebx]
	add eax, [ebx + 8]
	add ebx, 20  ;指向下一条 ARDS 结构
	cmp edx, eax
	jge .next_ards
	mov edx, eax
.next_ards:
	loop .find_max_mem_area
	jmp .mem_get_ok

;------------------------------------------------
;int 0x15 中断 功能号 0x0000e801
;------------------------------------------------
.e820_failed_so_try_e801:
	mov ax, 0xe801
	int 0x15
	jc .e801_failed_so_try88 ;当前方法失败

	mov cx, 0x400 ; ax 单位为 1 kb
	mul cx
	shl edx, 16       
	and eax, 0x0000FFFF
	or edx, eax        ;相加 
	add edx, 0x100000
	mov esi, edx    ;结果备份到 esi

	xor eax, eax
	mov ax, bx
	mov ecx, 0x10000
	mul ecx

	add esi, eax
	mov edx, esi
	jmp .mem_get_ok

;------------------------------------------------
;int 0x15 中断 功能号 ah=0x88 只能获取 64 M 内存
;------------------------------------------------
.e801_failed_so_try88:
	mov ah, 0x88
	int 0x15
	jc .error_hlt
	and eax, 0x0000FFFF

	mov cx, 0x400
	mul cx
	shl edx, 16
	or edx, eax
	add edx, 0x100000

.mem_get_ok:
	mov [total_mem_bytes],  edx

;--------准备进入保护模式-------
	; 1 打开 A20
	in al, 0x92
	or al, 0000_0010b
	out 0x92, al
	
	;2 加载 GDT
	lgdt [gdt_ptr]

	;3 cr0 寄存器 PE 位置 1
	mov eax, cr0
	or eax, 0x00000001
	mov cr0, eax
	
	jmp dword SELECTOR_CODE:p_mode_start  ; 刷新流水线

.error_hlt:		      ;出错则挂起
   hlt

[bits 32]
p_mode_start:
	mov ax, SELECTOR_DATA
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov esp, LOADER_STACK_TOP
	mov ax, SELECTOR_VIDEO
	mov gs, ax

	mov byte [gs:160], 'P'

	jmp $
