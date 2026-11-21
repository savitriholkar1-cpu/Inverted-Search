
#include <stdio.h>
#include <string.h>
#include "main.h"

int search_database(mainnode_t *arr[27], char *word)
{
    int index;

    if (word[0] >= 'a' && word[0] <= 'z')
        index = word[0] - 'a';
    else if (word[0] >= 'A' && word[0] <= 'Z')
        index = word[0] - 'A';
    else
        index = 26;

    mainnode_t *mtemp = arr[index];

    while (mtemp != NULL)
    {
        if (strcmp(mtemp->word, word) == 0)
        {
            printf("Word %s is present in %d files\n",
                   word, mtemp->filecount);

            subnode_t *stemp = mtemp->sub_link;

            while (stemp != NULL)
            {
                printf("In %s -> %d times\n",
                       stemp->filename,
                       stemp->wordcount);

                stemp = stemp->link;
            }

            return SUCCESS;
        }

        mtemp = mtemp->link;
    }

    printf("Word %s is not present\n", word);

    return FAILURE;
}
