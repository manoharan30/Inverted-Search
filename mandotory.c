/***************************************************************************************************************************************************
*Author         :MANOHARAN BALAKRISHNAN
*Date           :03/08/2024
*File           :mandatory.c
*Title          :helper functions for inverted search.
*Description    :This file's contains the .
****************************************************************************************************************************************************/
#include "header.h"

int file_validation(char* fname, file_node_t **head) {
    // Check if the file has a .txt extension.
    if (strstr(fname, ".txt")) {
        FILE *fptr = fopen(fname, "r");
        // Check if the file exists.
        if (fptr != NULL) {
            fseek(fptr, EOF, SEEK_END);
            // Check if the file has content.
            if (ftell(fptr)) {
                fclose(fptr);
                file_node_t* temp = *head;
                // Traverse the list to check if the file already exists.
                while (temp != NULL) {
                    if (strcmp(temp->f_name, fname) == 0) {
                        printf("ERROR: File %s already exists.\n", fname);
                        return FAILURE;
                    }
                    temp = temp->link;
                }
                return SUCCESS;
            } else {
                printf("ERROR: File does not have any content.\n");
                return FAILURE;
            }
        } else {
            printf("ERROR: File does not exist.\n");
            return FAILURE;
        }
    } else {
        printf("ERROR: Pass file with .txt extension.\n");
        return FAILURE;
    }
}

void add_node(hash_t* table, char* word, int index, char* fname) {
    // Check if the hash table index is empty.
    if (table[index].link == NULL) {
        main_node_t* newmain = malloc(sizeof(main_node_t));
        sub_node_t* newsub = malloc(sizeof(sub_node_t));
        strcpy(newmain->word, word);
        newmain->link = NULL;
        newmain->f_count = 1;
        strcpy(newsub->f_name, fname);
        newsub->link = NULL;
        newsub->w_count = 1;
        newmain->sub_link = newsub;
        table[index].link = newmain;
        return;
    } else {
        main_node_t* mt = table[index].link;
        main_node_t* mt1 = NULL;
        // Traverse the main nodes at the hash table index.
        while (mt != NULL) {
            mt1 = mt;
            // Check if the word already exists.
            if (strcmp(mt->word, word) == 0) {
                sub_node_t* st = mt->sub_link;
                sub_node_t* st1 = NULL;
                // Traverse the sub nodes for the word.
                while (st != NULL) {
                    st1 = st;
                    // Check if the file already exists for the word.
                    if (strcmp(st->f_name, fname) == 0) {
                        st->w_count++;
                        return;
                    }
                    st = st->link;
                }
                // Add a new sub node for the new file.
                sub_node_t* new = malloc(sizeof(sub_node_t));
                strcpy(new->f_name, fname);
                new->link = NULL;
                new->w_count = 1;
                st1->link = new;
                mt->f_count++;
                return;
            }
            mt = mt->link;
        }
        // Add a new main node for the new word.
        main_node_t* new = malloc(sizeof(main_node_t));
        strcpy(new->word, word);
        new->link = NULL;
        new->f_count = 1;
        sub_node_t* newsub = malloc(sizeof(sub_node_t));
        strcpy(newsub->f_name, fname);
        newsub->w_count = 1;
        newsub->link = NULL;
        new->sub_link = newsub;
        mt1->link = new;
        return;
    }
}

void update_hash(hash_t* table, int index, char* word, int wc, char* fname) {
    // Check if the hash table index is empty.
    if (table[index].link == NULL) {
        main_node_t* newmain = malloc(sizeof(main_node_t));
        sub_node_t* newsub = malloc(sizeof(sub_node_t));
        strcpy(newmain->word, word);
        newmain->link = NULL;
        newmain->f_count = 1;
        strcpy(newsub->f_name, fname);
        newsub->link = NULL;
        newsub->w_count = wc;
        newmain->sub_link = newsub;
        table[index].link = newmain;
        return;
    } else {
        main_node_t* mt = table[index].link;
        main_node_t* mt1 = NULL;
        // Traverse the main nodes at the hash table index.
        while (mt != NULL) {
            mt1 = mt;
            // Check if the word already exists.
            if (strcmp(mt->word, word) == 0) {
                sub_node_t* st = mt->sub_link;
                sub_node_t* st1 = NULL;
                // Traverse the sub nodes for the word.
                while (st != NULL) {
                    st1 = st;
                    // Check if the file already exists for the word.
                    if (strcmp(st->f_name, fname) == 0) {
                        st->w_count = wc;
                        return;
                    }
                    st = st->link;
                }
                // Add a new sub node for the new file.
                sub_node_t* new = malloc(sizeof(sub_node_t));
                strcpy(new->f_name, fname);
                new->link = NULL;
                new->w_count = wc;
                st1->link = new;
                mt->f_count++;
                return;
            }
            mt = mt->link;
        }
        // Add a new main node for the new word.
        main_node_t* new = malloc(sizeof(main_node_t));
        strcpy(new->word, word);
        new->link = NULL;
        new->f_count = 1;
        sub_node_t* newsub = malloc(sizeof(sub_node_t));
        strcpy(newsub->f_name, fname);
        newsub->w_count = wc;
        newsub->link = NULL;
        new->sub_link = newsub;
        mt1->link = new;
        return;
    }
}

int backup_file_val(char* str) {
    // Check if the file has a .txt extension.
    if (strstr(str, ".txt")) {
        FILE* fptr = fopen(str, "r");
        // Check if the file exists.
        if (fptr == NULL) {
            printf("ERROR: file does not exist.\n");
            return 0;
        }
        // Read the first character of the file.
        char ch1 = fgetc(fptr);
        if (ch1 == '#') {
            // If the first character is '#', it's a valid backup file.
            fclose(fptr);
            return 1;
        } else {
            // If the content is invalid, print error and return 0.
            printf("ERROR: Please pass a backup file with valid content.\n");
            fclose(fptr);
            return 0;
        }
    } else {
        // If the file doesn't have a .txt extension, print error and return 0.
        printf("ERROR: Please pass a text file.\n");
        return 0;
    }
}

void delete_node(char* filename, file_node_t** head) {
    // Check if the list is empty.
    if (*head == NULL) {
        return;
    }

    file_node_t* current = *head;
    file_node_t* previous = NULL;

    // Check if the node to delete is the head node.
    if (strcmp(current->f_name, filename) == 0) {
        *head = current->link; // Move head to the next node.
        free(current);
        return;
    }

    // Traverse the list to find the node to delete.
    while (current != NULL && strcmp(current->f_name, filename) != 0) {
        previous = current;
        current = current->link;
    }

    // Node not found.
    if (current == NULL) {
        return;
    }

    // Node found; unlink and free it.
    previous->link = current->link;
    free(current);
    return;
}
