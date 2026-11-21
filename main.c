
#include <stdio.h>
#include <stdlib.h>
#include "main.h"

int main(int argc, char *argv[])
{
    Slist *head = NULL;
    mainnode_t *arr[27] = {NULL};

    char word[25];
    int choice;
    int last_operation = 0;

    if (argc < 2)
    {
        printf("ERROR: Pass the file names\n");
        return FAILURE;
    }

    if (valid_file(argv, argc, &head) == FAILURE)
    {
        printf("No valid files\n");
        free_file_list(&head);
        return FAILURE;
    }

    printf("\nValidated file list:\n");
    print_list(head);

    do
    {
        printf("\n1. Create Database\n");
        printf("2. Display Database\n");
        printf("3. Search\n");
        printf("4. Update Database\n");
        printf("5. Save Database\n");
        printf("6. Exit\n");

        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input\n");
            break;
        }

        switch (choice)
        {
            case 1:
                if (last_operation == 0 || last_operation == 2)
                {
                    if (create_database(head, arr) == SUCCESS)
                        last_operation = 1;
                }
                else
                {
                    printf("Create Database is not allowed\n");
                }
                break;

            case 2:
                display_database(arr);
                break;

            case 3:
                printf("Enter the word to search: ");

                if (scanf("%24s", word) == 1)
                    search_database(arr, word);
                break;

            case 4:
                if (last_operation == 0)
                {
                    if (update_database(&head, arr) == SUCCESS)
                        last_operation = 2;
                }
                else
                {
                    printf("Update Database is not allowed\n");
                }
                break;

            case 5:
                save_database(arr);
                break;

            case 6:
                printf("Exiting...\n");
                break;

            default:
                printf("Invalid choice\n");
        }

    } while (choice != 6);

    free_file_list(&head);
    free_database(arr);

    return SUCCESS;
}
