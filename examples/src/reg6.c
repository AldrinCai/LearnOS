#include <stdio.h>
int main(void){
    int in_a = 1, in_b = 2;
    asm(
    "addl %%ebx, %%eax;"
    :"+a"(in_a)
    :"b"(in_b)
    );
    printf("in_a is %d\n", in_a);
    return 0;
}
