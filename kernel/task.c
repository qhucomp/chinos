#include "include/task.h"
#include "include/kmalloc.h"
task_struct init_task = INIT_TASK(init_task);
task_struct *task_list;
uint64_t pid_bitmap[32];

void add_task(task_struct *task) {
    task_list->prev = task;
    task->prev = NULL;
    task->next = task_list;
    task_list = task;
}

void delete_task(task_struct *task) {
    if (task->prev == NULL) {
        task_list = task->next;
        task_list->prev = NULL;
        kfree(task);
    } else {
        task->next->prev = task->prev;
        task->prev->next = task->next;
        task_list = task->next;
        kfree(task);
    }
}
static pid_t get_new_pid(void) {
    for(int i = 0;i < 32;i++) {
        if (~(bitmap[i] & 0xffffffffffffffff))
            for(int j = 0;j < 64;j++)
                if (bitmap[i] & (1 << j)) {
                    bitmap[i] |= 1 << j;
                    return i + j;
                }
    }
    return 0;
}
task_struct *alloc_task(void) {
    pid_t pid = get_new_pid();
    task_struct *task = kmalloc(sizeof(task_struct));
    task->pid = pid;
    task->next = task->prev = NULL;
    return task;
}