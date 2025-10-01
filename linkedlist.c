#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "colours.h"
#include "linkedlist.h"

int main(int argc, char *argv[])
{
	LinkedList *l = NULL;
	print_list(l);
	bubble_sort(l);
	LinkedList *list = create_list();
	push(5,list);
	print_list(list);
	push(15,list);
	push(25,list);
	print_list(list);
	bubble_sort(list);
	print_list(list);

	pop(list);
	pop(list);
	print_list(list);
	return 0;
}

Node *create_node(const int value)
{
	Node *const node = malloc(sizeof(Node));
	assert(node != NULL);
	node->value = value;
	node->next = NULL;
	return node;
}

LinkedList *create_list(void)
{
	LinkedList *list = malloc(sizeof(LinkedList));
	assert(list != NULL);
	list->head = NULL;
	list->tail = NULL;
	list->length = 0;
	return list;
}

void delete_list(LinkedList *list)
{
	Node *head;
	while(list->head != NULL) {
		head = list->head;
		list->head = list->head->next;
		free(head);
	}
}

void push_p(const int value, Node** head)
{
	struct Node* node = create_node(value);
	node->next = *head;
	*head = node;
}

void push(const int value, LinkedList *list)
{
	push_p(value, &list->head);
	list->length++;
	set_colour(FG_CYAN);
	printf("\n-Push List-\n");
	reset_colour();
	printf(" %d list->length %d\n", list->head->value, list->length);
}

int pop(LinkedList *list)
{
	int value = list->head->value;

	Node *head = list->head;
	list->head = list->head->next;
	free(head);

	list->length--;

	set_colour(FG_MAGENTA);
	printf("\n-Pop List-\n");
	reset_colour();
	printf(" %d list->length %d\n", value, list->length);

	return value;
}

void print_list(LinkedList *list)
{
	if (list == NULL || list->length == 0) {
		set_colour(FG_RED);
		printf("\n-Print List-\n EMPTY\n");
		reset_colour();
		return;
	}

	set_colour(FG_GREEN);
	printf("\n-Print List-\n");
	reset_colour();
	Node *node = list->head;
	int index = 0;
	while (node != NULL) {
		printf(" %d:\t%d\n", index, node->value);
		node = node->next;
		index++;
	}
}

void bubble_sort(LinkedList *list)
{
	if (list == NULL || list->length == 0) {
		set_colour(FG_RED);
		printf("\n-Bubble Sort List-\n EMPTY\n");
		reset_colour();
		return;
	}

	bool sorted = false;
	Node *node;
	int passes = 0;

	set_colour(FG_GREEN);
	printf("\n-Bubble Sort List-\n");
	reset_colour();

	while (!sorted) {
		printf(" pass %d...\n", ++passes);
		sorted = true;
		node = list->head;
		while (node->next != NULL) {
			if (node->value > node->next->value) {
				sorted = false;
				int value = node->value;
				node->value = node->next->value;
				node->next->value = value;
			}
			node = node->next;
		}
	}
	printf("\n ...sorted!\n");
}
