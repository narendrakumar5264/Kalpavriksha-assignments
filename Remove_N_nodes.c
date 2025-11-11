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

struct Node* removeNthFromEnd(struct Node* head, int n) {
    struct Node* dummy = createNode(0);
    dummy->next = head;

    struct Node* fast = dummy;
    struct Node* slow = dummy;

    for (int i = 0; i <= n; i++)
        fast = fast->next;

    while (fast) {
        fast = fast->next;
        slow = slow->next;
    }

    struct Node* nodeToDelete = slow->next;
    slow->next = slow->next->next;
    free(nodeToDelete);

    struct Node* newHead = dummy->next;
    free(dummy);
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
    head->next->next->next->next = createNode(5);

    head = removeNthFromEnd(head, 2);
    printList(head); 
}
