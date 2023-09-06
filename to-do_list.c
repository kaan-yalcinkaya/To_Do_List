#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include "to-do_list.h"

struct LastDate{
    uint16_t day, mon, year;
};
struct Node{
    char *task;
    lastDate_ptr last_date;
    struct Node *prev, *next;
};

node_ptr head = NULL, tail = NULL;

lastDate_ptr
lastDate_init(uint16_t d, uint16_t m, uint16_t y){
    lastDate_ptr pd = malloc(sizeof *pd);
    if(d > 31 || m > 12 || y > 2100 || y < 2023) return NULL;
    pd->day = d, pd->mon = m, pd->year = y;
    return pd;
}
void lastDate_display(lastDate_ptr date)
{
    if(date){
        printf("%"PRIu16".%"PRIu16".%"PRIu16, date->day, date->mon, date->year);
    }
}
static int lastDate_isValid(const char *str)
{
    uintmax_t i = 0, count = 0;
    size_t len = strlen(str);
    while (str[i]){
        if(!isdigit(str[i])) {
            if(ispunct(str[i]) && ++count < 3 && i != 0 && i != len - 1) ;
            else return 0;
        }
        i++;
    }
    return 1;
}
lastDate_ptr lastDate_strToDate(const char *date){
    if(!date || !lastDate_isValid(date)) return NULL;
    char day[3], mon[3], year[5];
    size_t i = 0, j = 0, count = 0;
    while(date[i]){
        if(date[i] == '.'){
            count++, j = 0, i++;
            continue;
        }
        switch(count){
            case 0:
                day[j++] = date[i++];
                day[j] = '\0';
                break;
            case 1:
                mon[j++] = date[i++];
                mon[j] = '\0';
                break;
            case 2:
                year[j++] = date[i++];
                year[j] = '\0';
                break;
            default:
                break;
        }
    }
    uintmax_t d = strtoumax(day, NULL, 10);
    uintmax_t m = strtoumax(mon, NULL, 10);
    uintmax_t y = strtoumax(year, NULL, 10);
    if(d > 31 || m > 12 || y > 2100 || y < 2023) return NULL;
    return lastDate_init(d, m, y);
}

static node_ptr node_init(char *task, lastDate_ptr last_date)
{
    size_t len_task = strlen(task);
    if(len_task >= MAX_TASK_LEN) exit(EXIT_FAILURE);
    node_ptr node = (node_ptr) malloc(sizeof *node);
    node->task = (char *) malloc(sizeof(char)*(len_task + 1));
    node->last_date = last_date;
    node->prev = node->next = NULL;
    strcpy(node->task, task);
    return node;
}

static void node_destroy(node_ptr *node)
{
    free((*node)->task);
    free((*node)->last_date);
    free(*node);
    *node = NULL;
}

void node_pushBack(char *task, lastDate_ptr last_date)
{
    if(!task) exit(EXIT_FAILURE);
    if(!head){
        head = node_init(task, last_date);
        tail = head;
        return;
    }
    tail->next = node_init(task, last_date);
    tail->next->prev = tail;
    tail = tail->next;
}

void node_update(uintmax_t index, char *new_task, lastDate_ptr new_date)
{
    if(!new_task) exit(EXIT_FAILURE);
    node_ptr iter = head;
    uintmax_t i = 0;
    while (iter){
        if(i == index){
            free(iter->task);
            free(iter->last_date);
            iter->task = (char *) malloc(sizeof(char) * (strlen(new_task)+1));
            iter->last_date = new_date;
            strcpy(iter->task, new_task);
            break;
        }
        iter = iter->next, i++;
    }
}

void node_clear(){
    node_ptr iter = head;
    while (iter){
        node_ptr temp = iter;
        iter = iter->next;
        node_destroy(&temp);
    }
    head = tail = NULL;
}

void node_delete(uintmax_t index)
{
    uintmax_t i = 0;
    node_ptr iter = head;
    while (iter){
        if(i == index){
            if(iter == head){
                if(!head->next){
                    node_destroy(&head);
                    tail = NULL;
                    break;
                }
                node_ptr temp = head;
                head = head->next;
                head->prev = NULL;
                node_destroy(&temp);
            }
            else if(iter == tail){
                node_ptr temp = tail;
                tail = tail->prev;
                tail->next = NULL;
                node_destroy(&temp);
            }
            else{
                iter->prev->next = iter->next;
                iter->next->prev = iter->prev;
                node_destroy(&iter);
            }
        }
        iter = iter->next, i++;
    }
}

void toDoList_display()
{
    node_ptr iter = head;
    uintmax_t i = 0;
    printf("%-35s %s\n\n", "Task", "Date");
    while (iter){
        printf("%ju. %-33s", ++i, iter->task);
        lastDate_display(iter->last_date);
        puts("");
        iter = iter->next;
    }
    puts("");
    puts("");
}

void toDoList_save(const char *file_name)
{
    if(!file_name) exit(EXIT_FAILURE);
    FILE *file = fopen(file_name, "w");
    if(!file) perror("Error"), exit(EXIT_FAILURE);
    node_ptr iter = head;
    while (iter){
        if(iter->last_date) fprintf(file, "%s:%u.%u.%u\n", iter->task, iter->last_date->day, iter->last_date->mon, iter->last_date->year);
        else fprintf(file, "%s:\n", iter->task);
        iter = iter->next;
    }
    fclose(file);
}

int toDoList_load(const char *file_name)
{
    if(!file_name) exit(EXIT_FAILURE);
    FILE *file = fopen(file_name, "r");
    if(!file) {
        perror("Error");
        return 0;
    }
    char buffer[256];
    while (fgets(buffer, sizeof buffer, file)){
        char task[MAX_TASK_LEN], date[11] = {'\0'};
        uintmax_t i = 0, j = 0;
        while (buffer[i] != ':'){
            task[j++] = buffer[i++];
        }
        task[i++] = '\0';
        j = 0;
        while (buffer[i] != '\n' && buffer[i] != '\0'){
            date[j++] = buffer[i++];
        }
        date[j] = '\0';
        if(date[0] == '\0') node_pushBack(task, NULL);
        else node_pushBack(task, lastDate_strToDate(date));
    }
    return 1;
}