#include "print.h"
int main(void){
	put_str("I am kernel\n");
    init_all();
    asm volatile("sti"); //演示中断处理，零时开中断
	while(1);
	return 0;
}
