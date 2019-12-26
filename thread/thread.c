#include "thread.h"
#include "stdint.h"
#include "string.h"
#include "global.h"
#include "memory.h"
#include "assert.h"
#include "list.h"

#define PG_SIZE 4096

struct task_struct* main_thread; // 主线程 PCB
struct list thread_ready_list; // 就绪队列
struct list thread_all_list; // 所有任务队列
static struct list_elem* thread_tag; // 用于保存队列中的线程节点

extern void switch_to(struct task_struct* cur, struct task_struct* next);

/*
 * 获取当前线程 pcb 指针
 */
struct task_struct* running_thread(){
    uint32_t esp;
    asm("mov %%esp, %0":"=g"(esp));
    // 获取 esp 所在的页起始地址
    return (struct task_struct*)(esp & 0xfffff000);
}

/*
 * 由 kernel_thread 执行 function(func_arg)
 */
static void kernel_thread(thread_func* function, void* func_arg){
    //执行前需要开中断，避免后面的中断被屏蔽导致无法调度其他线程
    intr_enable();
    function(func_arg);
}

/*
 * 初始化线程栈 thread_stack
 */
void thread_create(struct task_struct* pthread, thread_func function, void* func_arg){
    // 先预留中断使用的栈
    pthread->self_kstack -= sizeof(struct intr_stack);

    // 再留出线程栈空间
    pthread->self_kstack -= sizeof(struct thread_stack);
    struct thread_stack* kthread_stack = (struct thread_stack*)pthread->self_kstack;
    kthread_stack->eip = kernel_thread;
    kthread_stack->function = function;
    kthread_stack->func_arg = func_arg;
    kthread_stack->ebp = kthread_stack->ebx = \
                         kthread_stack->esi = kthread_stack->edi = 0;
}

/*
 * 初始化线程基本信息
 */
void init_thread(struct task_struct* pthread, char* name, int prio){
    memset(pthread, 0, sizeof(*pthread));
    strcpy(pthread->name, name);
    
    if(pthread == main_thread){
        pthread->status = TASK_RUNNING;    
    }else{
        pthread->status = TASK_READY;
    }

    pthread->priority = prio;
    pthread->self_kstack = (uint32_t*)((uint32_t)pthread + PG_SIZE);
    pthread->ticks = prio;
    pthread->elapsed_ticks = 0;
    pthread->pgdir =(void*) 0;
    pthread->statck_magic = 0x19870916;
}

struct task_struct* thread_start(char* name, int prio, thread_func function, void* func_arg){
    struct task_struct* thread = get_kernel_pages(1);
    init_thread(thread, name, prio);
    thread_create(thread, function, func_arg);

    ASSERT(!elem_find(&thread_ready_list, &thread->general_tag));
    list_append(&thread_ready_list, &thread->general_tag);

    ASSERT(!elem_find(&thread_all_list, &thread->all_list_tag));
    list_append(&thread_all_list, &thread->all_list_tag);

    return thread;
}

/*
 * 将 kernel 中的 main 函数完善为主线程
 */
static void make_main_thread(void){
    // loader.s 中预留了 pcb 不需要分配内存
    main_thread = running_thread();
    init_thread(main_thread, "main", 31);
    ASSERT(!elem_find(&thread_all_list, &main_thread->all_list_tag));
    list_append(&thread_all_list, &main_thread->all_list_tag);
}

