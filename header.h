/***************************************************************************************************************************************************
*Author         :MANOHARAN BALAKRISHNAN
*Date           :03/08/2024
*File           :header.c
*Title          :header function for inverted search.
*Description    :This file's contains the function prototypes and structures for inverted search.
****************************************************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SUCCESS 0                // Define macro for successful operation
#define FAILURE -1               // Define macro for failed operation
#define NOELEMENT -2            // Define macro for no element found
#define EMPTYLIST -3            // Define macro for an empty list
#define REPEATED -4             // Define macro for repeated element
#define FILE_EMPTY -5          // Define macro for empty file
#define NOT_PRESENT -6         // Define macro for element not present

#define SIZE 26                 // Define macro for the size of hash table (26 letters)
#define BUFF_SIZE 255           // Define macro for buffer size
#define NAMELENGTH 20           // Define macro for length of names

/* subnode structure for word occurrences in files */
typedef struct sub_node
{
    char f_name[NAMELENGTH];    // Filename where the word occurs
    int w_count;                // Count of occurrences of the word in the file
    struct sub_node *link;      // Pointer to the next subnode
} sub_node_t;

/* main node structure for words in the hash table */
typedef struct node
{
    char word[NAMELENGTH];      // The word stored in the main node
    struct node *link;          // Pointer to the next main node
    sub_node_t *sub_link;       // Pointer to the first subnode
    int f_count;                // Count of files containing the word
} main_node_t;

/* file node structure for storing filenames in a linked list */
typedef struct file_node
{
    char f_name[NAMELENGTH];    // Filename stored in the file node
    struct file_node *link;     // Pointer to the next file node
} file_node_t;

/* hash table structure */
typedef struct hash
{
    int key;                    // Hash table key (index)
    main_node_t* link;          // Pointer to the first main node
} hash_t;

// Function prototypes
void create_db(hash_t* table, file_node_t **head);  // Create database from file nodes
void display_DB(hash_t* table);                     // Display the database content
void update_db(hash_t* table, file_node_t** head, char* fname); // Update the database with a new file
void search_DB(hash_t* table, char *word);          // Search for a word in the database
void save_DB(hash_t* table, char *fname);           // Save the database to a file

int file_validation(char* fname, file_node_t **head); // Validate the file and update the file node list

void add_node(hash_t*, char*, int, char*);   // Add a new word node to the hash table
void update_hash(hash_t*, int, char*, int, char*); // Update the hash table with new information
int backup_file_val(char* str);              // Validate backup file
void delete_node(char*, file_node_t**);     // Delete a file node from the linked list
