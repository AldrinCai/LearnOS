#include <stdio.h>
int main(void){
    int ret_cnt = 0, test = 0;
    char* fmt = "hello,world\n";
    asm(
    "pushl %1; \
    call printf; \
    addl $4, %%esp; \
    movl $6, %2"
    :"=&a"(ret_cnt)
    :"m"(fmt),"r"(test)
    );
    printf("the number of bytes written is %d\n", ret_cnt);
    return 0;
}
