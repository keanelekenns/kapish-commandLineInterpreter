/*
==========================================================
CSC 360 Assignment 1: kapish
Name: kapish.c
Author: Keanelek Enns V00875807
Date Created: Jan. 25, 2019
Last Updated: Jan. 31, 2019
Description: A killer application interactive UNIX shell
(i.e. a basic command line interpreter) with 5 built in 
commands: cd, setenv var [value], unsetenv var, history, and exit.
==========================================================
*/

#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

/*
=============================================================================================
Node
=============================================================================================
*/
typedef struct Node{
	char* data;
	struct Node *next;
	struct Node *prev;
}Node;

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

void prepend(DLL* list, char* data){
	insert_at(list, data, 0);
}

void append(DLL* list, char* data){
	insert_at(list, data, list->size);
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

/*
=================================================================
kapish code begins here
=================================================================
*/

DLL* command_history;

void INThandler(int sig){}//Used to ignore ctrl+c while executing child process

void exec_extern_command(char** args){
	//This code is heavily derived from the tutorial given
	//at https://brennan.io/2015/01/16/write-a-shell-in-c/.
	//This is due to the fact that there are very few ways 
	//to implement what the spec requires of kapish,
	//with regard to this code segment that is.
	pid_t pid = fork();
	if(pid == -1){
		perror("kapish");
	}else if(pid == 0){
		execvp(args[0], args);
		//did not need to see if it returned -1
		//because if it returns at all, it failed.
		perror("kapish");
		exit(1);
	}else{
		signal(SIGINT, INThandler);
		int stat;
		if(waitpid(pid, &stat, WUNTRACED) == -1){
			perror("kapish");
		}
	}
}

void cd(char** args){
	if(args[1] == NULL){
		if(chdir(getenv("HOME")) == -1){
			//was going to use fprintf(stderr, "ERROR: Could not change directory to HOME.\n")
			//but I looked into perror after seeing it in
			//the posted tutorial and decided it was better in this situation.
			perror("kapish"); 
		}
	}else if(chdir(args[1]) == -1){
		perror("kapish");
	}
}

void kapish_setenv(char** args){
	if(args[1] == NULL){
		fprintf(stderr, "ERROR: Expected variable name after \"setenv\" command.\n\
		Example: setenv var\n");
	}else if(args[2] == NULL){
		setenv(args[1], "",1);
	}else{
		setenv(args[1], args[2],1);
	}
}

void kapish_unsetenv(char** args){
	if(args[1] == NULL){
		fprintf(stderr, "ERROR: Expected variable name after \"unsetenv\" command.\n\
		Example: unsetenv var\n");
	}else{
		unsetenv(args[1]);
	}
}

//The specs did not say what order to 
//print the commands, so for efficiency
//(and because I don't have a tail pointer
//in my doublylinked list) I decided
//to print commands from newest (starting at 1)
//to oldest (up to 100), despite being opposite to what the 
//typical history command does.
void history(char** args){
	print_DLL(command_history);
}

//Determine whether the first argument is a built-in or if we need to execute an external command.
//Although wordy, this technique is more efficient than comparing our built in names to all the args.
//We could make this more extensible by creating a balanced search tree.
//However, because this assignment only asks for 5 built-in commmands to be implemented,
//making a search tree might be overkill.
int determine_and_execute_command(char** arguments){
	if(arguments[0] == NULL){
		//should have already been checked, but in case 
		//this gets called from somewhere else, this will protect it.
		return 1;
	}
	int stat = strncmp(arguments[0], "history", 8);
	if(stat == 0){
		history(arguments);
	}else if(stat < 0){
		stat = strncmp(arguments[0], "exit", 5);
		if(stat == 0){
			return 0;//no need for a function call, just exit loop
		}else if(stat > 0){
			exec_extern_command(arguments);
		}else{
			stat = strncmp(arguments[0], "cd", 3);
			if(stat == 0){
				cd(arguments);
			}else {
				exec_extern_command(arguments);
			}
		}
	}else{
		stat = strncmp(arguments[0], "unsetenv", 9);
		if(stat == 0){
			kapish_unsetenv(arguments);
		}else if (stat > 0){
			exec_extern_command(arguments);
		}else{
			stat = strncmp(arguments[0], "setenv", 7);
			if(stat == 0){
				kapish_setenv(arguments);
			}else {
				exec_extern_command(arguments);
			}
		}
	}
	return 1;
}

//Returns an array of pointers to tokens in the given buffer.
//The caller is responsible for freeing both the buffer string
//given and the reference that the function returns.
char** tokenize(char* buffer, const char* delim){
	int current_length = 32;
	char** tokens = (char**)malloc(sizeof(char*)*current_length);
	tokens[0] = strtok(buffer, delim);
	int i = 0;
	while(tokens[i]!=NULL){
		i++;
		if( i >= current_length){
			current_length += 32;
			tokens = realloc(tokens, sizeof(char*)*current_length);
		}
		tokens[i] = strtok(NULL, delim);
	}
	return tokens;
}

int read_prompt_loop(){
	const char* delim = " \t\r\n\v\f";
	char* line;
	size_t biggest_line = 512;
	size_t num;
	int exit_stat;
	
	printf("? ");
	
	//read from stdin
	line = (char*)malloc(sizeof(char)*biggest_line);
	num = getline(&line, &biggest_line, stdin);
	if(num == -1){
		printf("exit\n");
		free(line);
		return 0;
	}else if(strlen(line) > 512){
		free(line);
		printf("Input line was above 512 characters.\n");
		return 1;
	}
	
	char* command = (char*)malloc((strlen(line)+1)*sizeof(char));
	command = strncpy(command, line, strlen(line)+1);//make a copy before chopping up line
	
	char** arguments = tokenize(line, delim);
	
	if(arguments[0] == NULL){
		printf("No command given. Please try again.\n");
		free(command);
		command = NULL;
		exit_stat = 1;
	}else if(arguments[0][0] == '!'){
		free(command); //don't need this anymore
		arguments[0]++;//ignore the ! char
		Node* temp = command_history->head;
		int i = 0;
		while(temp!=NULL){
			if(strncmp(arguments[0], temp->data, strlen(arguments[0])) == 0){//found command that matches prefix
				arguments[0]--;//undo
				char* command_copy = (char*)malloc((strlen(temp->data)+1)*sizeof(char));
				command_copy = strncpy(command_copy, temp->data, strlen(temp->data)+1);//make a copy to make new arguments
				char** new_args = tokenize(command_copy,delim);
				
				exit_stat = determine_and_execute_command(new_args);
				
				//be free!
				int i=0;
				while(new_args[i]!=NULL){
					new_args[i] = NULL;
					i++;
				}
				free(new_args);
				new_args = NULL;
				free(command_copy);
				command_copy = NULL;
				break;
			}
			temp = temp->next;
			i++;
		}
		if(i >= size(command_history)){//did not find anything
			printf("Command prefix did not match any previous commands.\n");
			exit_stat = 1;
		}
	}else{
		prepend(command_history, command);
		if(size(command_history) > 100){//number determines how many commands to remember
			free(get(command_history, size(command_history)-1));//need to free the command stored in the node
			delete_at(command_history, size(command_history)-1);
		}
		exit_stat = determine_and_execute_command(arguments);
	}
	
	//be free!
	int i=0;
	while(arguments[i]!=NULL){
		arguments[i] = NULL;
		i++;
	}
	free(arguments);
	arguments = NULL;
	free(line);
	line = NULL;
	return exit_stat;
}

int main(int argc, char** argv){

	char* home = (char*)malloc(sizeof(char)*(strlen(getenv("HOME"))+11));
	if(home == NULL){
		fprintf(stderr, "ERROR:\nAllocation error occured.Did not read .kapishrc.\n");
	}else{
		strncpy(home, getenv("HOME"), strlen(getenv("HOME")));
		home[(int)strlen(getenv("HOME"))+1] = '\0';
		home = strcat(home, "/.kapishrc");
	
		//code for reading file was reused from previous assignment in SENG265
		FILE* fp = fopen(home, "r");
		free(home);
		if(fp == NULL){
			fprintf(stderr, "ERROR:\nEither .kapishrc does not exist, it is not in the HOME directory, or it is unreadable.\n");
		}else{
			fseek(fp, 0L, SEEK_END);
			size_t file_size = ftell(fp);
			rewind(fp);
			char* buffer = (char*)malloc(file_size+1);
			if(buffer==NULL){
				fprintf(stderr, "ERROR:\nAllocation error occured.\n");
				fclose(fp);
			}else{
				fread(buffer, sizeof(char), file_size, fp);
				buffer[file_size] = '\0';
				fclose(fp);
	
				//TOKENIZE
				const char* delim1 = "\n";
				char** lines = tokenize(buffer, delim1);
				int i;
				for(i =0; lines[i]!=NULL;i++){
					printf("? %s\n",lines[i]);
					system(lines[i]);
					lines[i] = NULL;
				}
				free(lines);
				lines = NULL;
				free(buffer);
				buffer = NULL;
			}
		}
	}
	command_history = create_DLL();
	
	while(read_prompt_loop()==1){};//Here is where the magic happens
	
	Node* temp = command_history->head;
	while(temp!= NULL){
		free(temp->data);//free the commands that we allocated in our code
		temp = temp->next;
	}
	delete_DLL(command_history);
	
	printf("Exiting kapish.\n");
	return 0;
}