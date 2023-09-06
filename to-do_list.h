#ifndef TO_DO_LIST_H
#define TO_DO_LIST_H

#include <inttypes.h>

#define MAX_TASK_LEN 128
#define MAX_FILE_NAME 32

typedef struct LastDate LastDate;
typedef struct Node Node;
typedef LastDate *lastDate_ptr;
typedef Node *node_ptr;

lastDate_ptr lastDate_init(uint16_t d, uint16_t m, uint16_t y);
lastDate_ptr lastDate_strToDate(const char *date);
void lastDate_display(lastDate_ptr date);
void node_pushBack(char *task, lastDate_ptr last_date);
void node_delete(uintmax_t index);
void toDoList_display();
void node_clear();
void toDoList_save(const char *file_name);
void node_update(uintmax_t index, char *new_task, lastDate_ptr new_date);
int toDoList_load(const char *file_name);

#endif