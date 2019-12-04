#include "interrupt.h"
#include "stdint.h"
#include "global.h"

#define IDT_DESC_CNT 0x21  //支持的中断数

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

/*完成有关中断的所有初始化工作*/
void idt_init(){
    put_str("idt_init start\n");
    idt_desc_init();
    pic_init();

    /*加载 idt*/
    uint64_t idt_operand = ((sizeof(idt) - 1) | ((uint64_t)((uint32_t)idt << 16)));
    asm volatile("lidt %0": :"m"(idt_operand));
    put_str("idt_init dont\n");
}
