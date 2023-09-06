#include <stdio.h>
#include <stdlib.h>
#include "to-do_list.h"

void toDoList_menu(const char *file_name)
{
    while (1){
        int opt = 0;
        printf("1. Add\n2. Delete\n3. Update\n4. List\n5. Save\n6. Main Menu\n7. Exit\nOpt: ");
        scanf("%d", &opt);
        switch (opt) {
            char task[MAX_TASK_LEN], date[11], fileName[MAX_FILE_NAME];
            uintmax_t in = 0;
            case 1://add
                printf("Task: ");
                scanf(" %99[^\n]", task);
                printf("Date: ");
                scanf(" %99[^\n]", date);
                node_pushBack(task, lastDate_strToDate(date));
                printf("Task was added to list\n");
                break;
            case 2://delete
                printf("Index: ");
                scanf("%"SCNuMAX, &in);
                node_delete(in - 1);
                printf("Task was deleted to list\n");
                break;
            case 3://update
                printf("Index: ");
                scanf("%jd", &in);
                printf("New Task: ");
                scanf(" %99[^\n]", task);
                printf("New Date: ");
                scanf(" %99[^\n]", date);
                node_update(in + 1, task, lastDate_strToDate(date));
                printf("Task was updated\n");
                break;
            case 4:
                toDoList_display();
                break;
            case 5:
                if(file_name){
                    toDoList_save(file_name);
                    printf("Tasks was saved to %s\n", file_name);
                    break;
                }
                printf("File Name: ");
                scanf(" %99[^\n]", fileName);
                toDoList_save(fileName);
                printf("Tasks was saved to %s\n", fileName);
                break;
            case 6:
                return;
            case 7:
                printf("Exiting...\n");
                exit(EXIT_SUCCESS);
            default:
                fprintf(stderr, "You've entered the wrong option!\n");
                break;
        }
    }
}

int main() {
    while (1){
        system("clear");
        int opt = 0;
        printf("*** Main Menu ***\n\n");
        printf("1. Create\n2. Load\n3. Exit\nOpt: ");
        scanf("%d", &opt);
        switch (opt) {
            char file_name[MAX_FILE_NAME];
            case 1:
                node_clear();
                toDoList_menu(NULL);
                break;
            case 2:
                node_clear();
                printf("File Name: ");
                scanf(" %99[^\n]", file_name);
                if(!toDoList_load(file_name)) {
                    char ret;
                    question:
                    printf("Do you want to continue?(y/n): ");
                    fscanf(stdin," %99[^\n]",&ret);
                    if(ret == 'y') continue;
                    else if(ret == 'n') exit(EXIT_FAILURE);
                    else {
                        printf("You've entered the wrong option!\n");
                        goto question;
                    }
                }
                toDoList_menu(file_name);
                break;
            case 3:
                printf("Exiting...");
                exit(EXIT_SUCCESS);
            default:
                printf("You've entered the worong option!\n");
                break;
        }
    }
}
