
#include <stdio.h>
#include "main.h"

int display_database(mainnode_t *arr[27])
{
    mainnode_t *mtemp;
    subnode_t *stemp;

    printf("Index\tWord\tFile Count\tWord Count\tFilename\n");
    printf("---------------------------------------------------------------\n");

    for (int i = 0; i < 27; i++)
    {
        mtemp = arr[i];

        while (mtemp != NULL)
        {
            stemp = mtemp->sub_link;

            while (stemp != NULL)
            {
                printf("%d\t%s\t%d\t\t%d\t\t%s\n",
                       i,
                       mtemp->word,
                       mtemp->filecount,
                       stemp->wordcount,
                       stemp->filename);

                stemp = stemp->link;
            }

            mtemp = mtemp->link;
        }
    }

    return SUCCESS;
}
