/***************************************************************************************************************************************************
*Author         :MANOHARAN BALAKRISHNAN
*Date           :03/08/2024
*File           :index.c
*Title          :this file has operation functions.
*Description    :This file's contains the 5 main operations for the inverted search.
****************************************************************************************************************************************************/
#include "header.h"

// Function to create a database from a list of files
void create_db(hash_t* table, file_node_t **head) {
    // Check if the head of the file list is NULL
    if (*head == NULL) {
        printf("ERROR: Cannot create database, file list is empty.\n");
        printf("\n");
        return;
    }

    file_node_t *temp = *head; // Temporary pointer to traverse the file list
    char* spl = ".,(){}[]!"; // Delimiters for tokenizing words

    // Loop through each file in the list
    while (temp != NULL) {
        FILE* fptr = fopen(temp->f_name, "r"); // Open the file for reading
        if (fptr == NULL) {
            printf("ERROR: Can't open file %s", temp->f_name);
            continue;
        }

        char str[30]; // Buffer to store words
        char ch = 1; // Variable to read characters

        // Loop until end of file
        while (ch != EOF) {
            do {
                ch = fgetc(fptr); // Read the next character
            } while (isspace(ch)); // Skip whitespace characters

            int i = 0; // Index for the word buffer
            int index; // Index for the hash table

            // Read alphanumeric characters or delimiters into the word buffer
            while (isalnum(ch) || strchr(spl, ch)) {
                str[i] = ch;
                i++;
                ch = fgetc(fptr); // Read the next character
            }
            fseek(fptr, -1, SEEK_CUR); // Move file pointer back one position
            str[i] = '\0'; // Null-terminate the word

            // Determine the index in the hash table based on the first character
            if (isalpha(str[0])) index = (tolower(str[0])) - 97;
            else if (isdigit(str[0])) index = 27;
            else index = 26;
            add_node(table, str, index, temp->f_name); // Add the word to the hash table
        }
        temp = temp->link; // Move to the next file in the list
    }
    printf("INFO: Database created successfully.\n");
    printf("\n");
    return;
}

// Function to display the contents of the database
void display_DB(hash_t* table) {
    int flag = 0; // Flag to check if the database is empty
    for (int i = 0; i < 28; i++) {
        if (table[i].link != NULL) {
            flag = 1;
            break;
        }
    }

    if (flag) {
        printf("--------------------------------------------------------------------------------------------------------------\n");
        printf("Index   word       File_count  Word_count      File_name\n");
        for (int i = 0; i < 28; i++) {
            if (table[i].link != NULL) {
                main_node_t* temp = table[i].link; // Temporary pointer to traverse main nodes
                while (temp != NULL) {
                    sub_node_t* temp1 = temp->sub_link; // Temporary pointer to traverse sub-nodes
                    while (temp1 != NULL) {
                        printf("%-8d%-15s%-12d%-12d%s\n", i, temp->word, temp->f_count, temp1->w_count, temp1->f_name);
                        temp1 = temp1->link; // Move to the next sub-node
                    }
                    temp = temp->link; // Move to the next main node
                }
            }
        }
        printf("--------------------------------------------------------------------------------------------------------------\n");
        return;
    } else {
        printf("INFO: Database is empty, cannot display anything.\n");
    }
}

// Function to update the database from a backup file
void update_db(hash_t* table, file_node_t** head, char *fname) {
    char str[100]; // Buffer to store lines from the file
    char* tk1; // Token for index
    char* word; // Token for word
    char* tk; // Token placeholder
    char* word_count; // Token for word count
    char* file_name; // Token for file name
    FILE* fptr = fopen(fname, "r"); // Open the backup file for reading

    // Read each line from the file
    while ((fscanf(fptr, "%[^\n]%*c", str) != EOF)) {
        tk1 = strtok(str, "#;"); // Tokenize the line
        word = strtok(NULL, "#;");
        tk = strtok(NULL, "#;");
        word_count = strtok(NULL, "#;");
        file_name = strtok(NULL, "#;");

        delete_node(file_name, head); // Delete the old entry from the file list

        int index = atoi(tk1); // Convert index token to integer
        int wc = atoi(word_count); // Convert word count token to integer
        update_hash(table, index, word, wc, file_name); // Update the hash table
    }
    printf("INFO: Database updated successfully.\n");
    printf("\n");
    return;
}

// Function to search for a word in the database
void search_DB(hash_t* table, char *word) {
    int index;

    // Determine the index in the hash table based on the first character
    if (strchr(".[]{}()!/", word[0])) index = 26;
    else if (word[0] > '0' && word[0] <= '9') index = 27;
    else index = tolower(word[0]) - 97;

    if (table[index].link != NULL) {
        main_node_t* temp1 = table[index].link; // Temporary pointer to traverse main nodes
        while (temp1 != NULL) {
            if (strcmp(word, temp1->word) == 0) {
                printf("word \"%s\" is present in %d files\n", temp1->word, temp1->f_count);
                sub_node_t* temp2 = temp1->sub_link; // Temporary pointer to traverse sub-nodes
                while (temp2 != NULL) {
                    printf("%s - %d time(s)\n", temp2->f_name, temp2->w_count);
                    temp2 = temp2->link; // Move to the next sub-node
                }
                printf("\n");
                return;
            }
            temp1 = temp1->link; // Move to the next main node
        }
        printf("word \"%s\" is not present in database.\n", word);
        printf("\n");
        return;
    }
    printf("word \"%s\" is not present in database.\n", word);
    printf("\n");
    return;
}

// Function to save the database to a file
void save_DB(hash_t* table, char *fname) {
    if (strstr(fname, ".txt")) {
        FILE* fptr = fopen(fname, "w"); // Open the file for writing
        for (int i = 0; i < 28; i++) {
            if (table[i].link != NULL) {
                main_node_t* main_t = table[i].link; // Temporary pointer to traverse main nodes
                while (main_t != NULL) {
                    sub_node_t* sub_t = main_t->sub_link; // Temporary pointer to traverse sub-nodes
                    while (sub_t != NULL) {
                        fprintf(fptr, "#%d;%s;%d;%d;%s#\n", i, main_t->word, main_t->f_count, sub_t->w_count, sub_t->f_name);
                        sub_t = sub_t->link; // Move to the next sub-node
                    }
                    main_t = main_t->link; // Move to the next main node
                }
            }
        }
        fclose(fptr); // Close the file
        printf("INFO: Database saved successfully.\n");
        printf("\n");
        return;
    }
    printf("ERROR: Please pass a text file to save the database.\n");
    printf("ERROR: Failed to save the Database.\n");
    return;
}
