#ifndef __LIB_KERNEL_LIST_H
#define __LIB_KERNEL_LIST_H
#include "global.h"

#define offset(struct_type, member) (int)(&((struct_type*)0)->member)
#define elem2entry(struct_type, struct_member_name, elem_ptr) \
    (struct_type*)((int)elem_ptr - offset(struct_type, struct_member_name))

/*
 * 定义链表节点成员结构
 */
struct list_elem {
    struct list_elem* prev;
    struct list_elem* next;
};

/*
 * 链表结构
 */
struct list {
    struct list_elem head;
    struct list_elem tail;
};

//自定义函数类型 function 用于在 list_trabersal 中做回调函数
typedef int (function)(struct list_elem*, inr arg);

void list_init(struct list*);
void list_insert_before(struct list_elem* before, struct list_elem* elem);
void list_push(struct list* plist, struct list_elem* elem);
void list_iterate(struct list* plist);
void list_append(struct list* plist, struct list_elem* elem);
void list_remove(struct list_elem* pelem);
struct list_elem* list_pop(struct list* plist);
int list_empty(struct list* plist);
uint32_t list_len(struct list* plist);
struct list_elem* list_traversal(struct list* plist, function func, int arg);
int elem_find(struct list* plist, struct list_elem* obj_elem);

#endif
