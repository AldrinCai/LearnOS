#include "interrupt.h"
#include "stdint.h"
#include "global.h"
#include "io.h"

#define IDT_DESC_CNT 0x21  //支持的中断数
#define PIC_M_CTRL 0x20  //主片的控制端口是 0x20
#define PIC_M_DATA 0x21  // 主片的控制端口是0x20
#define PIC_S_CTRL 0xa0  // 从片的控制端口是0xa0
#define PIC_S_DATA 0xa1  // 从片的数据端口是 0xa1

/*中断门描述符结构体*/
struct gate_desc {
     uint16_t func_offset_low_word;
     uint16_t selectot;
     uint8_t dcount;

     uint8_t attribute;
     uint16_t func_offset_high_word;
}; 

static void make_idt_desc(struct gate_desc* p_gdesc, uint8_t attr, intr_handler function);
static struct gate_desc idt[IDT_DESC_CNT]; //中断描述符表

extern intr_handler intr_entry_table[IDT_DESC_CNT]; //声明引用定义在 kernel.s 中的中断处理函数入口

/*初始化中断描述符表 */
static void idt_desc_init(void){
    int i;
    for(i = 0; i < IDT_DESC_CNT; i++){
        make_idt_desc(&idt[i], IDT_DESC_ATTR_DPL0, intr_entry_table[i]);
    }
    put_str("   idt_desc_init done\n");
}

// 初始化可编程中断控制器 8259A
static void pic_init(void){
    // 初始化主片
    outb(PIC_M_CTRL, 0x11); //ICW1: 边沿触发，级联8259，需要 ICW4
    outb(PIC_M_DATA, 0x20); // ICW2: 起始中断向量号为 0x20

    outb(PIC_M_DATA, 0x04); // ICW3: IR2 接从片
    outb(PIC_M_DATA, 0x01); // ICW4: 8086 模式，正常 EOI
    
    //初始化从片
    outb(PIC_S_CTRL, 0x11); //ICW1: 边沿触发，级联 8259，需要ICW4
    outb(PIC_S_DATA, 0x28); //ICW2: 起始中断向量号为 0x28

    outb(PIC_S_DATA, 0x02); //ICW3: 设置从片连接到主片的 IR2 引脚
    outb(PIC_S_DATA, 0x01); // ICW4:8086 模式，正常 EOI

    // 打开主片上的 IR0，也就是目前只接受时钟产生的中断
    outb(PIC_M_DATA, 0xfe);
    outb(PIC_S_DATA, 0xff);

    put_str("   pic_init done\n");

}

// 完成有关中断的所有初始化工作
void idt_init(){
    put_str("idt_init start\n");
    idt_desc_init(); //初始化中断描述符表
    pic_init();  //初始化 8259A

    //加载idt
    uint64_t idt_operand = ((sizeof(idt) - 1) | ((uint64_t)((uint32_t)idt << 16)));
    asm volatile ("lidt %0": :"m"(idt_operand));
    put_str("idt_init done\n");
}
