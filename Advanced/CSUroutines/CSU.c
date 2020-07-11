#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct __node
{
	int data;
	struct __node *next;	
} Node;

typedef struct {
	Node *head;
	Node *crnt;
} List;

static Node *AllocNode(void) {
	return calloc(1, sizeof(Node));
}

static void SetNode(Node *n, int *x, Node *next) {
	n->data = *x;
	n->next = next;
}

void Init(List *list) {
	list->head = NULL;
	list->crnt = NULL;
}

void InsertFront(List *list, int *x) {
	Node *ptr = list->head;
	list->head = list->crnt = AllocNode();
	SetNode(list->head, x, ptr);
}

void InsertRear(List *list, int *x) {
	if (list->head == NULL)
		InsertFront(list, x);
	else {
		Node *ptr = list->head;
		while(ptr->next != NULL)
			ptr = ptr->next;
		ptr->next = list->crnt = AllocNode();
		SetNode(ptr->next, x, NULL);
	}
}



int main(int argc, char **argv) {
	List list;
	Init(&list);
	
	for (int i = 0; i < 10; i++)
		InsertFront(&list, &i);
	return 0;
}
