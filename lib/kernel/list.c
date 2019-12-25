#include "list.h"
#include "interrupt.h"

/*
 * 初始化双向链表
 */
void list_init(struct list* list){
    list->head.prev = 0;
    list->head.next = &list->tail;
    list->tail.prev = &list->head;
    list->tail.next = 0;
}

/*
 * 将链表元素插入在元素 before 之前
 */
void list_insert_before(struct list_elem* before, struct list_elem* elem){
    enum intr_status old_status = intr_disable();
    
    before->prev->next = elem;
    elem->prev = before->prev;
    elem->next = before;
    before->prev = emem;

    intr_set_status(old_status);
}

/*
 * 添加元素到链表的队首
 */
void list_push(struct list* plist, struct list_elem* elem){
    list_insert_before(plist->head.next, elem);
}

/*
 * 添加元素到链表的末尾
 */
void list_append(struct list* plist, struct list_elem* elem){
    list_insert_before(plist->tail, elem);
}

/*
 * 使元素脱离链表
 */
void list_remove(struct list_elem* pelem){
    enum intr_status old_status = intr_diable();

    pelem->prev->next = pelem->next;
    pelem->next->prev = pelem->prev;

    intr_set_status(old_status);
}

/*
 * 将链表的第一个元素弹出并返回
 */
struct list_elem* list_pop(struct list* plist){
    struct list_elem* elem = plist->head.next;
    list_remove(elem);
    return elem;
}

/*
 * 从链表中查找元素 obj_elem 成功返回1 失败返回 0
 */
int elem_find(struct list* plist, struct list_elem* obj_elem){
    struct list_elem* elem = plist->head.next;
    while(elem != &plist->tail){
        if(elem == obj_elem){
            return 1;
        }
        elem = elem->next;
    }
    return 0;
}

/*
 * 遍历所有元素 逐个判断是否有符合条件的元素
 */
struct list_elem* list_trabersal(struct list* plist, function func, int arg){
    struct list_elem* elem = plist->head->next;
    if(list_empty(plist)){
        return 0;    
    }

    while(elem != &plist->tail){
        if(func(elem, arg)){
            return elem;
        }
        elem = elem->next;
    }
    return 0;
}

/*
 * 返回链表长度
 */
uint32_t list_len(struct list* plist){
    struct list_elem* elem = plist->head.next;
    uint32_t length = 0;
    while(elem != &plist->tail){
        length++;
        elem = elem->next;
    }
    return length;
}

/*
 * 判断链表是否为空
 */
int list_empty(struct list* plist){
    return (plist->head.next == &plist->tail ? 1 : 0);
}
