#pragma once

typedef struct Node {
	int value;
	struct Node *next;
} Node;

typedef struct LinkedList {
	Node *head;
	Node *tail;
	size_t length;
} LinkedList;

Node *create_node(const int value);
LinkedList *create_list(void);
void push(const int value, LinkedList *list);
int pop(LinkedList *list);
void print_list(LinkedList *list);
void bubble_sort(LinkedList *list);
