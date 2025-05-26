#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct Node
{
    int data;
    struct Node *next;
};
//create node
struct Node *createNode(struct Node **head, int data)
{
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;
    if (*head == NULL)
    {
        *head = newNode;
    }
    else
    {
        struct Node *last = *head;
        while (last->next != NULL)
        {
            last = last->next;
        }
        last->next = newNode;
    }
}

struct Node *creNo(struct Node *head, int data)
{
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
    ;
}

struct Node *insertNodeAtTheBeginning(struct Node *head, int data)
{
    struct Node *newNode = creNo(head, data);
    newNode->next = head;
    printf("Inserted %d at the beginning\n", data);
    return newNode;
}
void traverse(struct Node *head)
{
    if (head == NULL)
    {
        return;
    }

    printf("%d ", head->data);
    traverse(head->next);
};

bool search(struct Node *head, int key)
{
    if (head == NULL)
    {
        return 0;
    }
    if (head->data == key)
    {
        printf("Found %d\n", key);
        return 1;
    }
    return search(head->next, key);
}

void countNodes(struct Node *head)
{
    int count = 0;
    while (head != NULL)
    {
        count++;
        head = head->next;
    }
    printf("Number of nodes is %d\n", count);
}

int main()
{
    struct Node *head = NULL;
    createNode(&head, 2);
    createNode(&head, 3);
    createNode(&head, 4);
    createNode(&head, 5);

    head = insertNodeAtTheBeginning(head, 1);

    search(head, 1);
    traverse(head);
    countNodes(head);
    return 0;
}