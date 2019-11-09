;主引导程序
;-----------------------------------------------
%include "include/boot.inc"
SECTION MBR vstart=0x7c00
	mov ax, cs
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov fs, ax
	mov sp, 0x7c00
	mov ax, 0xb800
	mov gs, ax

;清屏
;-----------------------------------------------
;功能: 上卷窗口
;ah: 功能号 al: 上卷行数 bh: 上卷行属性
;(cl, ch) 左上角 x、y 位置
;(dl, dh) 右下角 x、y 位置
;无返回值
;-----------------------------------------------
	mov ax, 0x600
	mov bx, 0x700
	mov cx, 0
	mov dx, 0x184f
	int 0x10

;-----------------------------------------------
; 输出背景色是绿色，前景为红色，并且跳动的字符串
; a 表示绿色背景闪烁，4 表示前景色是红色
;-----------------------------------------------
	mov byte [gs:0x00], '1'
	mov byte [gs:0x01], 0xa4

	mov byte [gs:0x02], ' '
	mov byte [gs:0x03], 0xa4

	mov byte [gs:0x04], 'M'
	mov byte [gs:0x05], 0xa4

	mov byte [gs:0x06], 'B'
	mov byte [gs:0x07], 0xa4

	mov byte [gs:0x08], 'R'
	mov byte [gs:0x09], 0xa4

	mov eax, LOADER_START_SECTOR
	mov bx, LOADER_BASE_ADDR
	mov cx, 4     ; loader 大概率会超过一个扇区，直接增加到读取 4 个扇区
	call rd_disk_m_16

	jmp LOADER_BASE_ADDR
;-----------------------------------------------
;功能: 读取硬盘的 n 个扇区
;eax: 扇区号 bx: 数据写入的内存地址 cx:读入的扇区数
;-----------------------------------------------
rd_disk_m_16:
	;备份
	mov esi, eax
	mov di, cx
	
	;设置要读取的扇区数
	mov dx, 0x1f2
	mov al, cl
	out dx, al

	mov eax, esi

	;LBA 7~0 位存入 0x1f3 ~ 0x1f6
	mov dx, 0x1f3
	out dx, al
	
	;LBA 15 ~ 8 位写入0x1f4
	mov cl, 8
	shr eax, cl
	mov dx, 0x1f4
	out dx, al

	;LBA 23 ～ 16 位写入0x1f5
	shr eax, cl
	mov dx, 0x1f5
	out dx, al
	
	shr eax, cl
	and al, 0x0f ;高 4 字节置0 低 4 字节保持, LBA 24 ~ 27 位
	or al, 0xe0 ; 高 4 字节置为 1110(LBA 模式) 低 4 字节保持
	mov dx, 0x1f6
	out dx, al
	
	; 向 0x1f7 端口写入读命令 0x20
	mov dx, 0x1f7
	mov al, 0x20
	out dx, al

;检查硬盘状态
.not_ready:
	nop
	in al, dx ;同一端口写时表示写命令，读时表示读取硬盘状态
	and al, 0x88 ; 第四位为 1 表示已准备好数据， 第七位为1表示硬盘忙
	cmp al, 0x08
	jnz .not_ready ;没有准备好进行下次检测
	
	; 从 0x1f0 端口读取数据
	mov ax, di
	mov dx, 256
	mul dx
	mov cx, ax ;控制循环次数
	mov dx, 0x1f0

.go_on_read:
	in ax, dx
	mov [bx], ax
	add bx, 2
	loop .go_on_read
	ret
	
	times 510-($-$$) db 0
	db 0x55, 0xaa
