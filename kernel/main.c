#include "print.h"
#include "init.h"
#include "debug.h"
#include "memory.h"

int main(void){
	put_str("I am kernel\n");
    init_all();
    init_mem();
	while(1);
	return 0;
}
