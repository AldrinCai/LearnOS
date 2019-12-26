#ifndef __THREAD_THREAD_H
#define __THREAD_THREAD_H
#include "stdint.h"

/*
 * 自定义通用函数类型
 */
typedef void thread_func(void*);

/*
 *  进程或线程的状态
 */
enum task_status {
    TASK_RUNNING,
    TASK_READY,
    TASK_BLOCKED,
    TASK_WAITING,
    TASK_HANGING,
    TASK_DIED
};

/*
 * 中断栈 此结构用于中断发生时保护程序的上下文环境
 */
struct intr_stack {
    uint32_t vec_no; // VECTOR 中push%1压入的中断号
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp_dummy;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    uint32_t gs;
    uint32_t fs;
    uint32_t es;
    uint32_t ds;

    uint32_t err_code;
    void (*eip) (void);
    uint32_t cs;
    uint32_t eflags;
    void* esp;
    uint32_t ss;
};

/*
 * 线程栈
 */
struct thread_stack {
    uint32_t ebp;
    uint32_t ebx;
    uint32_t edi;
    uint32_t esi;

    void (*eip)(thread_func* func, void* func_arg);

    // 以下供第一次上cpu时使用
    void (*unused_retaddr);
    thread_func* function;
    void* func_arg;
};

/*
 * pcb 程序控制块
 */
struct task_struct {
    uint32_t* self_kstack;
    enum task_status status;
    uint8_t priority; // 线程优先级
    char name[16]; 
    uint8_t ticks;
    uint32_t elapsed_ticks;
    struct list_elem general_tag;
    struct list_elem all_list_tag;
    uint32_t* pgdir;
    uint32_t statck_magic; // 栈的边界标记，检测栈溢出

};

void thread_create(struct task_struct* pthread, thread_func function, void* func_arg);
void init_thread(struct task_struct* pthread, char* name, int prio);
struct task_struct* thread_start(char* name, int prio, thread_func function, void* func_arg);
#endif
