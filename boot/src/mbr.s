;主引导程序
;-----------------------------------------------
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
; 输出背景色是绿色，前景为红色，并且跳动的字符串
; a 表示绿色背景闪烁，4 表示前景色是红色
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

	jmp $
	
	times 510-($-$$) db 0
	db 0x55, 0xaa
