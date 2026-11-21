
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"

static int get_index(char *word)
{
    if (word[0] >= 'a' && word[0] <= 'z')
        return word[0] - 'a';

    if (word[0] >= 'A' && word[0] <= 'Z')
        return word[0] - 'A';

    return 26;
}

int create_database(Slist *head, mainnode_t *arr[27])
{
    if (head == NULL)
    {
        printf("No files available to create database\n");
        return FAILURE;
    }
    FILE *fptr;
    char word[25];
    int index;

    Slist *file_temp = head;

    while (file_temp != NULL)
    {
        fptr = fopen(file_temp->filename, "r");

        if (fptr == NULL)
        {
            printf("Unable to open %s\n", file_temp->filename);
            return FAILURE;
        }

        while (fscanf(fptr, "%24s", word) == 1)
        {
            index = get_index(word);

            mainnode_t *mtemp = arr[index];
            mainnode_t *prev = NULL;

            while (mtemp != NULL)
            {
                if (strcmp(mtemp->word, word) == 0)
                    break;

                prev = mtemp;
                mtemp = mtemp->link;
            }

            if (mtemp == NULL)
            {
                mainnode_t *new_main = malloc(sizeof(mainnode_t));
                subnode_t *new_sub = malloc(sizeof(subnode_t));

                if (new_main == NULL || new_sub == NULL)
                {
                    free(new_main);
                    free(new_sub);
                    fclose(fptr);
                    return FAILURE;
                }

                strcpy(new_main->word, word);
                new_main->filecount = 1;
                new_main->sub_link = new_sub;
                new_main->link = NULL;

                strcpy(new_sub->filename, file_temp->filename);
                new_sub->wordcount = 1;
                new_sub->link = NULL;

                if (prev == NULL)
                    arr[index] = new_main;
                else
                    prev->link = new_main;
            }
            else
            {
                subnode_t *stemp = mtemp->sub_link;
                subnode_t *sub_prev = NULL;

                while (stemp != NULL)
                {
                    if (strcmp(stemp->filename,
                               file_temp->filename) == 0)
                        break;

                    sub_prev = stemp;
                    stemp = stemp->link;
                }

                if (stemp != NULL)
                {
                    stemp->wordcount++;
                }
                else
                {
                    subnode_t *new_sub = malloc(sizeof(subnode_t));

                    if (new_sub == NULL)
                    {
                        fclose(fptr);
                        return FAILURE;
                    }

                    strcpy(new_sub->filename, file_temp->filename);
                    new_sub->wordcount = 1;
                    new_sub->link = NULL;

                    sub_prev->link = new_sub;
                    mtemp->filecount++;
                }
            }
        }

        fclose(fptr);
        file_temp = file_temp->link;
    }

    printf("Database created successfully\n");

    return SUCCESS;
}
