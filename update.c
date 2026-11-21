
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

static int get_index(char *word)
{
    if (word[0] >= 'a' && word[0] <= 'z')
        return word[0] - 'a';

    if (word[0] >= 'A' && word[0] <= 'Z')
        return word[0] - 'A';

    return 26;
}

static void free_temp_node(mainnode_t *mtemp)
{
    subnode_t *stemp = mtemp->sub_link;

    while (stemp != NULL)
    {
        subnode_t *del = stemp;
        stemp = stemp->link;
        free(del);
    }

    free(mtemp);
}

static void free_temp_database(mainnode_t *arr[27])
{
    free_database(arr);
}

static int load_database(FILE *fp, mainnode_t *temp_arr[27])
{
    int index, filecount, wordcount;
    char word[25], filename[25], ch;

    while (1)
    {
        int result = fscanf(fp, " \n#%d;%24[^;];%d;",
                            &index, word, &filecount);

        if (result == EOF)
            return SUCCESS;

        if (result != 3 || index < 0 || index >= 27 ||
            filecount <= 0 || get_index(word) != index)
            return FAILURE;

        mainnode_t *new_main = malloc(sizeof(mainnode_t));

        if (new_main == NULL)
            return FAILURE;

        strcpy(new_main->word, word);
        new_main->filecount = filecount;
        new_main->sub_link = NULL;
        new_main->link = NULL;

        subnode_t *sub_tail = NULL;
        int status = SUCCESS;

        for (int i = 0; i < filecount; i++)
        {
            if (fscanf(fp, "%24[^;];%d;",
                       filename, &wordcount) != 2 ||
                wordcount <= 0)
            {
                status = FAILURE;
                break;
            }

            if (strchr(filename, '/') != NULL ||
                strchr(filename, '\\') != NULL ||
                strlen(filename) == 0)
            {
                status = FAILURE;
                break;
            }

            subnode_t *stemp = new_main->sub_link;

            while (stemp != NULL)
            {
                if (strcmp(stemp->filename, filename) == 0)
                {
                    status = FAILURE;
                    break;
                }

                stemp = stemp->link;
            }

            if (status == FAILURE)
                break;

            subnode_t *new_sub = malloc(sizeof(subnode_t));

            if (new_sub == NULL)
            {
                status = FAILURE;
                break;
            }

            strcpy(new_sub->filename, filename);
            new_sub->wordcount = wordcount;
            new_sub->link = NULL;

            if (new_main->sub_link == NULL)
                new_main->sub_link = new_sub;
            else
                sub_tail->link = new_sub;

            sub_tail = new_sub;
        }

        if (status == SUCCESS)
        {
            if (fscanf(fp, "%c", &ch) != 1 || ch != '#')
                status = FAILURE;
        }

        if (status == FAILURE)
        {
            free_temp_node(new_main);
            return FAILURE;
        }

        mainnode_t *mtemp = temp_arr[index];

        while (mtemp != NULL)
        {
            if (strcmp(mtemp->word, word) == 0)
            {
                free_temp_node(new_main);
                return FAILURE;
            }

            mtemp = mtemp->link;
        }

        if (temp_arr[index] == NULL)
        {
            temp_arr[index] = new_main;
        }
        else
        {
            mtemp = temp_arr[index];

            while (mtemp->link != NULL)
                mtemp = mtemp->link;

            mtemp->link = new_main;
        }
    }
}

int update_database(Slist **head, mainnode_t *arr[27])
{
    char backup_file[25];

    mainnode_t *temp_arr[27] = {NULL};

    Slist *new_head = NULL;
    Slist *tail = NULL;

    printf("Enter the backup file name: ");

    if (scanf("%24s", backup_file) != 1)
        return FAILURE;

    FILE *fp = fopen(backup_file, "r");

    if (fp == NULL)
    {
        printf("Unable to open backup file\n");
        return FAILURE;
    }

    if (load_database(fp, temp_arr) == FAILURE)
    {
        printf("Invalid database format\n");
        fclose(fp);
        free_temp_database(temp_arr);
        return FAILURE;
    }

    fclose(fp);

    printf("Database created successfully\n");

    Slist *original = *head;
    int status = SUCCESS;

    while (original != NULL)
    {
        int found = 0;

        for (int i = 0; i < 27 && found == 0; i++)
        {
            mainnode_t *mtemp = temp_arr[i];

            while (mtemp != NULL && found == 0)
            {
                subnode_t *stemp = mtemp->sub_link;

                while (stemp != NULL)
                {
                    if (strcmp(stemp->filename,
                               original->filename) == 0)
                    {
                        found = 1;
                        break;
                    }

                    stemp = stemp->link;
                }

                mtemp = mtemp->link;
            }
        }

        if (found == 1)
        {
            printf("%s : File already exists\n", original->filename);
        }
        else
        {
            Slist *new_node = malloc(sizeof(Slist));

            if (new_node == NULL)
            {
                status = FAILURE;
                break;
            }

            new_node->filename = original->filename;
            new_node->link = NULL;

            if (new_head == NULL)
                new_head = new_node;
            else
                tail->link = new_node;

            tail = new_node;
        }

        original = original->link;
    }

    if (status == SUCCESS && new_head != NULL)
    {
        if (create_database(new_head, temp_arr) == FAILURE)
            status = FAILURE;
    }

    /* Free only the temporary list nodes.
       The filename pointers belong to the original list. */

    while (new_head != NULL)
    {
        Slist *del = new_head;
        new_head = new_head->link;
        free(del);
    }

    if (status == FAILURE)
    {
        printf("Database update failed\n");
        free_temp_database(temp_arr);
        return FAILURE;
    }

    /* Replace the original database with the updated one. */

    free_database(arr);

    for (int i = 0; i < 27; i++)
        arr[i] = temp_arr[i];

    /* All command-line files have now been processed. */

    free_file_list(head);

    printf("Database updated successfully\n");

    return SUCCESS;
}
