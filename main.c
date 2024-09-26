/***************************************************************************************************************************************************
*Author         :MANOHARAN BALAKRISHNAN
*Date           :03/08/2024
*File           :main.c
*Title          :main function for inverted search.
*Description    :This file's contains the main function .
****************************************************************************************************************************************************/
#include "header.h"

int main(int argc, char *argv[]){
    // Check if any files are passed as command line arguments
    if(argc < 2){
        printf("ERROR: please pass files through command line.\n");
        return 0;
    }

    // Initialize the hash table with 28 entries
    hash_t table[28];

    for(int i = 0; i < 28; i++){
        table[i].key = i; // Set the key for each hash table entry
        table[i].link = NULL; // Initialize the link to NULL
    }

    file_node_t *head = NULL; // Initialize the head of the file node list
    int res = 0;

    // Validate each file passed as argument
    for(int i = 1; i < argc; i++){
        res = file_validation(argv[i], &head);
        if(res == SUCCESS){
            // If validation is successful, create a new file node
            file_node_t* new = malloc(sizeof(file_node_t));
            strcpy(new->f_name, argv[i]); // Copy the file name to the new node
            new->link = head; // Link the new node to the existing list
            head = new; // Update the head to the new node
        }
        else{
            // Print an error message if validation fails
            printf("ERROR: File validation for file %s has been failed.\n", argv[i]);
        }
    }

    int flag = 1; // Flag to control the main loop

    // Main loop for the menu-driven program
    while(flag){
        char choice;
        printf("1. Create Database\n2. Display Database\n3. Update Database\n4. Search Database\n5. Save Database\n6. Exit\n");
        printf("Enter your choice: ");
        scanf("%c", &choice); // Get the user choice
        printf("\n");
        getchar(); // Clear the newline character from the buffer

        switch (choice)
        {
        case '1':
            create_db(table, &head); // Create the database
            break;

        case '2':
            display_DB(table); // Display the database
            break;

        case '3': {
            int empty_flag = 1; // Flag to check if the database is empty
            for(int i = 0; i < 28; i++){
                if(table[i].link != NULL) {
                    empty_flag = 0;
                }
            }
            if(empty_flag){
                char str[20];
                printf("Enter the backup file name: ");
                scanf("%s", str); // Get the backup file name from user
                getchar();
                printf("\n");
                int res = backup_file_val(str); // Validate the backup file
                if(res == 1) update_db(table, &head, str); // Update the database if valid
            }
            else {
                // Print an error message if the database is not empty
                printf("ERROR: DataBase is not empty cannot update the database.\n");
            }
            printf("\n");
            break;
        }

        case '4': {
            char str[30];
            printf("Enter the word: ");
            scanf("%s", str); // Get the word to search from user
            getchar();
            printf("\n");
            search_DB(table, str); // Search the database
            break;
        }

        case '5': {
            char str[30];
            printf("Enter the file name: ");
            scanf("%s", str); // Get the file name to save the database
            getchar();
            printf("\n");
            save_DB(table, str); // Save the database to the specified file
            break;
        }

        case '6':
            flag = 0; // Exit the loop and terminate the program
            break;

        default:
            break;
        }
    }
    return 0;
}
