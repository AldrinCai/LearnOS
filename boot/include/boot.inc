; loader and kernel
;--------------------------------------------------- 
LOADER_BASE_ADDR equ 0x900
LOADER_START_SECTOR equ 0x2
;--------------------------------------------------- 

; gdt 描述符
;--------------------------------------------------- 
DESC_G_4K equ 1_00000000000000000000000b  ;设置 G 位为1，即段界限符单位是 4 kb
DESC_D_32 equ  1_0000000000000000000000b  ;设置 D/B 位为1， 即指令中的有效地址是 32 位
DESC_L    equ   0_000000000000000000000b  ;设置 L 位为0， 即代码段为 32 位
DESC_AVL  equ    0_00000000000000000000b  ;设置 AVL 位为 0
DESC_LIMIT_CODE2 equ 1111_0000000000000000b ; 设置段界限符的第二部分为 1111
DESC_LIMIT_DATA2 equ DESC_LIMIT_CODE2
DESC_LIMIT_VIDEO2 equ 0000_0000000000001011b ;该处书上有错误，改为正确值
DESC_P equ 1_000000000000000b  ;设置 P 位为1 ，即段在内存中存在
DESC_DPL_0 equ 00_0000000000000b
DESC_DPL_1 equ 01_0000000000000b
DESC_DPL_2 equ 10_0000000000000b
DESC_DPL_3 equ 11_0000000000000b   ; DPL 位的四个权限
DESC_S_CODE equ 1_000000000000b   ; 设置 S 位为1，即内存段非系统段
DESC_S_DATA equ DESC_S_CODE 
DESC_S_SYS equ 0_000000000000b  ;设置系统段的 S 位为 1
; 代码段可执行，非一致性，不可读，已访问位清0
DESC_TYPE_CODE equ 1000_00000000b
; 数据的不可执行，向上拓展，可写，已访问位清0
DESC_TYPE_DATA equ 0010_00000000b

;代码段描述符高四个字节
DESC_CODE_HIGH4 equ (0x00 << 24) + DESC_G_4K + DESC_D_32 + \
	DESC_L + DESC_AVL + DESC_LIMIT_CODE2 + \
	DESC_P + DESC_DPL_0 + DESC_S_CODE + \
	DESC_TYPE_CODE + 0x00

;数据段描述符高4字节
DESC_DATA_HIGH4 equ (0x00 << 24) + DESC_G_4K + DESC_D_32 + \
	DESC_L + DESC_AVL + DESC_LIMIT_DATA2 + \
	DESC_P + DESC_DPL_0 + DESC_S_DATA + \
 	DESC_TYPE_DATA + 0x00

DESC_VIDEO_HIGH4 equ (0x00 << 24) + DESC_G_4K + DESC_D_32 + \
	DESC_L + DESC_AVL + DESC_LIMIT_VIDEO2 + DESC_P + \
	DESC_DPL_0 + DESC_S_DATA + DESC_TYPE_DATA + 0x00 

;--------------------------------------------------- 
;选择子属性
RPL0 equ 00b  ;RPL 请求特权级  
RPL1 equ 01b
RPL2 equ 10b
RPL3 equ 11b
TI_GDT equ 000b ;描述符在 GDT 中
TI_LDT equ 100b ;描述符在 LDT 中
;--------------------------------------------------- 

