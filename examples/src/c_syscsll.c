#include <unistd.h>

int main(void){

	write(1, "Hello, world\n", 4);
	return 0;
}