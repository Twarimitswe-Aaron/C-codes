#include <stdio.h>
#include <stdlib.h>

struct  Node
{
    int data;
    struct Node *next;
};

struct Node *createNode(int data){
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    newNode->data=data;
    newNode->next=NULL;
    return newNode;
}

void readNode(struct Node *head){
    if(head==NULL){
        return;
    }
    printf("%d",head->data);
    printf("hello");
    printf("%d", head->next);
}



int main(){

    struct Node *head=NULL;
    head=createNode(10);
    head->next=createNode(20);
    head->next->next=createNode(30);
    readNode(head);


}
