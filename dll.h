#ifndef DLL_H
#define DLL_H

/*
=============================================================================================
Doubly Linked List

Note: This doubly linked list does not alter the data pointers given to it. It is therefore
the responsibility of the caller to free the data stored in it as calling the delete functions
only removes the list's pointers to the data.
=============================================================================================
*/
typedef struct DLL DLL;

DLL* create_DLL();
void delete_DLL(DLL* list);

void insert_at(DLL* list, char* data, int pos);
void delete_at(DLL* list, int pos);

void prepend(DLL* list, char* data);
void append(DLL* list, char* data);

char* get(DLL* list, int pos);
int size(DLL* list);
void print_DLL(DLL* list);

#endif