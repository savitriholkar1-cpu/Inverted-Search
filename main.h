
#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>

#define SUCCESS 0
#define FAILURE -1

typedef struct node
{
    char *filename;
    struct node *link;
} Slist;

typedef struct snode
{
    char filename[25];
    int wordcount;
    struct snode *link;
} subnode_t;

typedef struct mnode
{
    char word[25];
    int filecount;
    subnode_t *sub_link;
    struct mnode *link;
} mainnode_t;

int valid_file(char *argv[], int argc, Slist **head);
void print_list(Slist *head);
void delete_file(Slist **head, char *filename);
void free_file_list(Slist **head);

int create_database(Slist *head, mainnode_t *arr[27]);
int display_database(mainnode_t *arr[27]);
int search_database(mainnode_t *arr[27], char *word);
int update_database(Slist **head, mainnode_t *arr[27]);
int save_database(mainnode_t *arr[27]);
void free_database(mainnode_t *arr[27]);

#endif
