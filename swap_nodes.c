#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* next;
};

struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

struct Node* swapPairs(struct Node* head) {
    if (!head || !head->next)
        return head;

    struct Node* newHead = head->next;
    struct Node* prev = NULL;

    while (head && head->next) {
        struct Node* first = head;
        struct Node* second = head->next;

        first->next = second->next;
        second->next = first;

        if (prev)
            prev->next = second;

        prev = first;
        head = first->next;
    }

    return newHead;
}

void printList(struct Node* head) {
    while (head) {
        printf("%d ", head->data);
        head = head->next;
    }
    printf("\n");
}

int main() {
  
    struct Node* head = createNode(1);
    head->next = createNode(2);
    head->next->next = createNode(3);
    head->next->next->next = createNode(4);

    struct Node* result = swapPairs(head);
    printList(result); 
}
