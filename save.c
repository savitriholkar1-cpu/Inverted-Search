
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"

int save_database(mainnode_t *arr[27])
{
    char filename[25];
    char *ptr;

    printf("Enter the filename: ");

    if (scanf("%24s", filename) != 1)
        return FAILURE;

    ptr = strrchr(filename, '.');

    if (ptr == NULL || strcmp(ptr, ".txt") != 0)
    {
        printf("Invalid extension\n");
        return FAILURE;
    }

    FILE *fptr = fopen(filename, "w");

    if (fptr == NULL)
    {
        printf("Unable to open file\n");
        return FAILURE;
    }

    for (int i = 0; i < 27; i++)
    {
        mainnode_t *mtemp = arr[i];

        while (mtemp != NULL)
        {
            fprintf(fptr, "#%d;%s;%d;",
                    i, mtemp->word, mtemp->filecount);

            subnode_t *stemp = mtemp->sub_link;

            while (stemp != NULL)
            {
                fprintf(fptr, "%s;%d;",
                        stemp->filename,
                        stemp->wordcount);

                stemp = stemp->link;
            }

            fprintf(fptr, "#\n");

            mtemp = mtemp->link;
        }
    }

    if (fclose(fptr) != 0)
    {
        printf("Error while saving database\n");
        return FAILURE;
    }

    printf("Database saved successfully\n");

    return SUCCESS;
}
