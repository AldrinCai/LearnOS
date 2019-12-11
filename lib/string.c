#include "string.h"
#include "global.h"
#include "debug.h"

/*
 * 将 dst_ 起始的 size 个字节置为 value
 */
void memset(void* dst_, uint8_t value, uint32_t size){
    ASSERT(dst_ != NULL);
    uint8_t* dst = (uint8_t*)dst_;
    while(size-- > 0){
        *dst++ = value;     
    }
}

/*
 * 将 src_ 起始的 size 个字节复制到 dst_
 */
void memcpy(void* dst_, const void* src_, uint32_t size){
    ASSERT(dst_ != NULL && src_ != NULL);
    uint8_t* dst = (uint8_t*)dst_;
    const uint8_t* src = src_;
    while(size-- > 0){
        *dst++ = *src++;
    }
}

/*连续比较以地址a_ 和 地址 b_ 开头的 size 个字节
 * 若相等返回 0
 * 若 a_ 大于 b_ 返回 1
 * 若 a_ 小于 b_ 返回 -1
 */
int memcmp(const void* a_, const void* b_, uint32_t size){
    const char* a = a_;
    const char* b = b_;
    ASSERT(a != NULL && b != NULL);
    while(size-- > 0){
        if(*a != *b){
           return *a > *b ? 1 : -1; 
        }
        *a++;
        *b++;
    }
    return 0;
}

/*
 * 返回字符串的长度
 */
uint32_t strlen(const char* str){
    ASSERT(str != NULL);
    const char* p = str;
    while(*p++);
    return p - str - 1;
}

/*
 * 将字符串从 src_ 复制到 dst_
 */
char* strcpy(char* dst_, const char* src_){
    char* dst = dst_;
    const char* src = src_;
    uint32_t size = strlen(src);
    memcmp(dst, src, size +1);
    return dst_; 
}
