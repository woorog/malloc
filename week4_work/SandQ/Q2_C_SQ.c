//////////////////////////////////////////////////////////////////////////////////

/* CE1007/CZ1007 Data Structures
Lab Test: Section C - Stack and Queue Questions
Purpose: Implementing the required functions for Question 2 */

//////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

#define MIN_INT -1000
//////////////////////////////////////////////////////////////////////////////////

typedef struct _listnode
{
	int item;
	struct _listnode* next;
} ListNode;	// You should not change the definition of ListNode

typedef struct _linkedlist
{
	int size;
	ListNode* head;
} LinkedList;	// You should not change the definition of LinkedList

typedef struct _stack
{
	LinkedList ll;
}Stack;  // You should not change the definition of Stack

///////////////////////// function prototypes ////////////////////////////////////

// You should not change the prototypes of these functions
void createStackFromLinkedList(LinkedList* ll, Stack* stack);
void removeEvenValues(Stack* s);

void push(Stack* s, int item);
int pop(Stack* s);
int isEmptyStack(Stack* s);
void removeAllItemsFromStack(Stack* s);

void printList(LinkedList* ll);
ListNode* findNode(LinkedList* ll, int index);
int insertNode(LinkedList* ll, int index, int value);
int removeNode(LinkedList* ll, int index);
void removeAllItems(LinkedList* ll);

//////////////////////////// main() //////////////////////////////////////////////

int main()
{
	int c, i;
	LinkedList ll;
	Stack s;

	c = 1;
	// Initialize the linked list as an empty linked list
	ll.head = NULL;
	ll.size = 0;

	// Initalize the stack as an empty stack
	s.ll.head = NULL;
	s.ll.size = 0;

	printf("1: Insert an integer into the linked list:\n");
	printf("2: Create the stack from the linked list:\n");
	printf("3: Remove even numbers from the stack:\n");
	printf("4: Pop from the stack:\n");
	printf("0: Quit:\n");

	while (c != 0)
	{
		printf("Please input your choice(1/2/3/4/0): ");
		scanf_s("%d", &c);

		switch (c)
		{
		case 1:
			printf("Input an integer that you want to add to the linked list: ");
			scanf_s("%d", &i);
			insertNode(&ll, ll.size, i);
			printf("The resulting linked list is: ");
			printList(&ll);
			break;
		case 2:
			createStackFromLinkedList(&ll, &s); // You need to code this function
			printf("The resulting stack is: ");
			printList(&(s.ll));
			break;
		case 3:
			removeEvenValues(&s); // You need to code this function
			printf("The resulting stack after removing even integers is: ");
			printList(&(s.ll));
			removeAllItemsFromStack(&s);
			removeAllItems(&ll);
			break;
		case 4:
			pop(&s); // You need to code this function
			printf("The resulting queue after pop is: ");
			printList(&(s.ll));
			break;
		case 0:
			removeAllItemsFromStack(&s);
			removeAllItems(&ll);
			break;
		default:
			printf("Choice unknown;\n");
			break;
		}

	}

	return 0;
}


//////////////////////////////////////////////////////////////////////////////////

//void stackFromLinkedListRecursive(ListNode* cur, Stack* s)
//{
//	if (cur == NULL)
//		return;
//
//	stackFromLinkedListRecursive(cur->next, s);
//	push(s, cur->item);
//}

void createStackFromLinkedList(LinkedList* ll, Stack* s)
{
	if (ll == NULL || s == NULL)
		return;

	if (!isEmptyStack(s))
		removeAllItemsFromStack(s);

	ListNode* temp = ll->head;
	// push 함수와 pop 함수가 인덱스 0을 파라미터로 넣어주고 있기 때문에 재귀 구현 필요x
	//stackFromLinkedListRecursive(temp, s);
	while (temp != NULL)
	{
		push(s, temp->item);
		temp = temp->next;
	}
}

void removeEvenValues(Stack* s)
{
	if (s == NULL || (s->ll).head == NULL)
		return;

	ListNode* cur = (s->ll).head;
	int index = 0;
	while (cur != NULL)
	{
		int ins_item = cur->item;
		cur = cur->next;
		if (ins_item % 2 == 0) {
			if (removeNode(&(s->ll), index) == 0)
				printf("%d번 노드 삭제됨.\n", ins_item);
			else
				printf("잘못된 참조!\n");
		}
		else
			index++;
	}
}

//////////////////////////////////////////////////////////////////////////////////

void push(Stack* s, int item)
{
	insertNode(&(s->ll), 0, item);
}

int pop(Stack* s)
{
	int item;
	if (s->ll.head != NULL)
	{
		item = ((s->ll).head)->item;
		removeNode(&(s->ll), 0);
		return item;
	}
	else
		return MIN_INT;
}

int isEmptyStack(Stack* s)
{
	if ((s->ll).size == 0)
		return 1;
	else
		return 0;
}


void removeAllItemsFromStack(Stack* s)
{
	if (s == NULL)
		return;
	while (s->ll.head != NULL)
	{
		pop(s);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////


void printList(LinkedList* ll) {

	ListNode* cur;
	if (ll == NULL)
		return;
	cur = ll->head;
	if (cur == NULL)
		printf("Empty");
	while (cur != NULL)
	{
		printf("%d ", cur->item);
		cur = cur->next;
	}
	printf("\n");
}


void removeAllItems(LinkedList* ll)
{
	ListNode* cur = ll->head;
	ListNode* tmp;

	while (cur != NULL) {
		tmp = cur->next;
		free(cur);
		cur = tmp;
	}
	ll->head = NULL;
	ll->size = 0;
}


ListNode* findNode(LinkedList* ll, int index) {

	ListNode* temp;

	if (ll == NULL || index < 0 || index >= ll->size)
		return NULL;

	temp = ll->head;

	if (temp == NULL || index < 0)
		return NULL;

	while (index > 0) {
		temp = temp->next;
		if (temp == NULL)
			return NULL;
		index--;
	}

	return temp;
}

int insertNode(LinkedList* ll, int index, int value) {

	ListNode* pre, * cur;

	if (ll == NULL || index < 0 || index > ll->size + 1)
		return -1;

	// If empty list or inserting first node, need to update head pointer
	if (ll->head == NULL || index == 0) {
		cur = ll->head;
		ll->head = malloc(sizeof(ListNode));
		if (ll->head == NULL)
		{
			exit(0);
		}
		ll->head->item = value;
		ll->head->next = cur;
		ll->size++;
		return 0;
	}


	// Find the nodes before and at the target position
	// Create a new node and reconnect the links
	if ((pre = findNode(ll, index - 1)) != NULL) {
		cur = pre->next;
		pre->next = malloc(sizeof(ListNode));
		if (pre->next == NULL)
		{
			exit(0);
		}
		pre->next->item = value;
		pre->next->next = cur;
		ll->size++;
		return 0;
	}

	return -1;
}


int removeNode(LinkedList* ll, int index) {

	ListNode* pre, * cur;

	// Highest index we can remove is size-1
	if (ll == NULL || index < 0 || index >= ll->size)
		return -1;

	// If removing first node, need to update head pointer
	if (index == 0) {
		cur = ll->head->next;
		free(ll->head);
		ll->head = cur;
		ll->size--;
		return 0;
	}

	// Find the nodes before and after the target position
	// Free the target node and reconnect the links
	if ((pre = findNode(ll, index - 1)) != NULL) {

		if (pre->next == NULL)
			return -1;

		cur = pre->next;
		pre->next = cur->next;
		free(cur);
		ll->size--;
		return 0;
	}

	return -1;
}
