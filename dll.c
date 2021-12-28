/*
==========================================================
Name: list.c
Author: Keanelek Enns
Date Created: Dec. 28, 2021
Description: A simple doubly linked list implementation
for kapish.
==========================================================
*/

#include <stdlib.h>
#include <stdio.h>
#include "dll.h"

/*
=============================================================================================
Node
=============================================================================================
*/
typedef struct Node {
	char* data;
	struct Node *next;
	struct Node *prev;
} Node;

Node* create_node(char* new_data){
	
	Node* new_node = (Node*)malloc(sizeof(Node));
	new_node->data = new_data;
	new_node->next = NULL;
	new_node->prev = NULL;
	
	return new_node;
}

void delete_node(Node* node){
	if(node->next != NULL && node->prev != NULL){
		if(node->next->prev == node){
			node->next->prev = node->prev;
		}
		if(node->prev->next == node){
			node->prev->next = node->next;
		}
	}else if(node->next != NULL && node->prev == NULL){
		if(node->next->prev == node){
			node->next->prev = NULL;
		}
	}else if(node->next == NULL && node->prev != NULL){
		if(node->prev->next == node){
			node->prev->next = NULL;
		}
	}//if both next and prev are null, then we are free to free node
	
	node->next = NULL;
	node->prev = NULL;//not sure if this is necessary

	free(node);
}

/*
=============================================================================================
Doubly Linked List
=============================================================================================
*/
typedef struct DLL{
	Node* head;
	int size;
}DLL;

DLL* create_DLL(){
	
	DLL* new_dll = (DLL*)malloc(sizeof(DLL));
	new_dll->head = NULL;
	new_dll->size = 0;
	
	return new_dll;
}

void delete_DLL(DLL* list){
	if(list->head == NULL){
		free(list);
		return;
	}
	Node* prev = list->head;
	Node* curr = prev->next;
	while(curr != NULL){
		prev = curr;
		curr = curr->next;
		delete_node(prev->prev);
	}
	delete_node(prev);
	free(list);
}

void insert_at(DLL* list, char* data, int pos){
	
	Node* node = create_node(data);
	
	if(list->head == NULL){
		list->head = node;
		list->size++;
		return;
	}
	
	if(pos < 0 || pos > list->size){
		printf("Position index out of bounds.\n");
		return;
	}
	if(pos == 0){
		node->next = list->head;
		list->head->prev = node;
		list->head = node;
		list->size++;
		return;
	}
	
	Node* temp = list->head;
	
	int i;
	for(i = 0; i < pos-1; i++){
		temp = temp->next;
	}
	
	if(temp == NULL){
		printf("Insertion error, node was not inserted");//shouldn't ever happen if pos is in bounds
		return;
	}
	
	node->next = temp->next;
	node->prev = temp;
	if(node->next != NULL){//if we are inserting at the end, node->next == NULL
		node->next->prev = node;
	}
	node->prev->next = node;
	
	list->size ++;
}

void delete_at(DLL* list, int pos){
	
	if(list->head == NULL){
		printf("No nodes in DLL to delete.\n");
		return;
	}
	
	if(pos < 0 || pos > list->size - 1){
		printf("Position index out of bounds.\n");
		return;
	}
	
	Node* temp = list->head;
	
	if(pos == 0){
		list->head = temp->next;
		delete_node(temp);
		list->size--;
		return;
	}
	
	int i;
	for(i = 0; i < pos; i++){
		temp = temp->next;
	}
	
	delete_node(temp);
	list->size--;
}

void prepend(DLL* list, char* data){
	insert_at(list, data, 0);
}

void append(DLL* list, char* data){
	insert_at(list, data, list->size);
}

char* get(DLL* list, int pos){
	if(pos < 0 || pos > list->size - 1){
		printf("Position index out of bounds.\n");
		return 0;
	}
	if(list->head == NULL){
		return NULL;
	}
	Node* temp = list->head;
	
	int i;
	for(i = 0; i < pos; i++){
		temp = temp->next;
	}
	return temp->data;
}

int size(DLL* list){
	return list->size;
}

void print_DLL(DLL* list){
	
	Node* temp = list->head;
	int i = 1;
	while(temp != NULL){
		printf("%d    %s\n", i, temp->data);
		temp = temp->next;
		i++;
	}
}