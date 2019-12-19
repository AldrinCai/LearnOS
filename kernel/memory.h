#ifndef __KERNEL_MEMORY_H
#define __KERNEL_MEMORY_H
#include "stdint.h"
#include "bitmap.h"

// 虚拟地址池，用于虚拟地址管理
struct virtual_addr{
    struct bitmap vaddr_bitmap;
    uint32_t vaddr_start;
};

extern struct pool kernel_pool, user_pool;
void init_mem(void);

enum pool_flags {
    PF_KERNEL = 1,
    PF_USER = 2
};

#define PG_P_1 1  // 页表项或页目录项存在属性位
#define PG_P_0 0  //  页表项或页目录项存在属性位
#define PG_RW_R 0 // 读 执行
#define PG_RW_W 2 // 读 写 执行
#define PG_US_S 0 // 系统级
#define PG_US_U 4 // 用户级

#endif
