/********* 机器模式 *******
    b -- 输出寄存器Qimode 名称，即寄存器中的最低8位 
    w -- 输出寄存器HImode 名称，即寄存器中的两字节
****/

#ifndef __LIB_IO_H
#define __LIB_IO_H
#include "stdio.h"

// 向端口写入一个字节
static inline void outb(uint16_t port, uint8_t data){
    //%b0 对应 al， %w1 对应 dx  N 表示操作数为0-255之间的立即数
    asm volatile ("outb %b0, %w1": :"a"(data), "Nd"(port));
}

// 将 addr 初开始的 word_cnt 个字写入端口 port
static inline void outsw(uint16_t port, const void* addr, uint32_t word_cnt){
    // + 表示此限制即作为输入又作为输出
    asm volatile ("cld; rep outsw":"+S"(addr), "+c"(word_cnt) :"d"(port));
}

// 将从端口 port 读入的一个字节返回
static inline uint8_t inb(uint16_t port){
    uint8_t data;
    asm volatile ("inb %w1, %b0":"=a"(data) :"Nd"(port));
    return data;
}

// 将从端口 port 读入的 word_cnt 个字节写入 addr
static inline void insw(uint16_t port, void* addr, uint32_t word_cnt) {
    asm volatile ("cld; rep insw":"+D"(addr), "+c"(word_cnt) :"d"(port) :"memory");
}
#endif
