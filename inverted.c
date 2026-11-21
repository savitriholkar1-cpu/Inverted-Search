
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"

int valid_file(char *argv[], int argc, Slist **head)
{
    FILE *fptr;
    Slist *new_node, *temp;
    char *ptr;
    int flag;

    for (int i = 1; i < argc; i++)
    {
        if (strlen(argv[i]) >= 25)
        {
            printf("%s : Filename too long\n", argv[i]);
            continue;
        }

        ptr = strrchr(argv[i], '.');

        if (ptr == NULL || strcmp(ptr, ".txt") != 0)
        {
            printf("%s : Invalid extension\n", argv[i]);
            continue;
        }

        fptr = fopen(argv[i], "r");

        if (fptr == NULL)
        {
            printf("%s : File not present\n", argv[i]);
            continue;
        }

        fseek(fptr, 0, SEEK_END);

        if (ftell(fptr) == 0)
        {
            printf("%s : File is empty\n", argv[i]);
            fclose(fptr);
            continue;
        }

        fclose(fptr);

        temp = *head;
        flag = 0;

        while (temp != NULL)
        {
            if (strcmp(temp->filename, argv[i]) == 0)
            {
                flag = 1;
                break;
            }

            temp = temp->link;
        }

        if (flag == 1)
        {
            printf("%s : Duplicate file\n", argv[i]);
            continue;
        }

        new_node = malloc(sizeof(Slist));

        if (new_node == NULL)
            return FAILURE;

        new_node->filename = malloc(strlen(argv[i]) + 1);

        if (new_node->filename == NULL)
        {
            free(new_node);
            return FAILURE;
        }

        strcpy(new_node->filename, argv[i]);
        new_node->link = NULL;

        if (*head == NULL)
        {
            *head = new_node;
        }
        else
        {
            temp = *head;

            while (temp->link != NULL)
                temp = temp->link;

            temp->link = new_node;
        }

        printf("%s : Added successfully\n", argv[i]);
    }

    if (*head == NULL)
        return FAILURE;

    return SUCCESS;
}

void print_list(Slist *head)
{
    while (head != NULL)
    {
        printf("%s -> ", head->filename);
        head = head->link;
    }

    printf("NULL\n");
}

void delete_file(Slist **head, char *filename)
{
    Slist *temp = *head;
    Slist *prev = NULL;

    while (temp != NULL)
    {
        if (strcmp(temp->filename, filename) == 0)
        {
            if (prev == NULL)
                *head = temp->link;
            else
                prev->link = temp->link;

            free(temp->filename);
            free(temp);
            return;
        }

        prev = temp;
        temp = temp->link;
    }
}

void free_file_list(Slist **head)
{
    Slist *temp;

    while (*head != NULL)
    {
        temp = *head;
        *head = temp->link;

        free(temp->filename);
        free(temp);
    }
}

void free_database(mainnode_t *arr[27])
{
    for (int i = 0; i < 27; i++)
    {
        mainnode_t *mtemp = arr[i];

        while (mtemp != NULL)
        {
            mainnode_t *del_main = mtemp;
            mtemp = mtemp->link;

            subnode_t *stemp = del_main->sub_link;

            while (stemp != NULL)
            {
                subnode_t *del_sub = stemp;
                stemp = stemp->link;
                free(del_sub);
            }

            free(del_main);
        }

        arr[i] = NULL;
    }
}
