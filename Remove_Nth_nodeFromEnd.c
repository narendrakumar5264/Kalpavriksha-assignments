#include <stdio.h>
#include <stdlib.h>

struct Node {
    int value;
    struct Node* next;
};

struct Node* createNode(int value) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->value = value;
    newNode->next = NULL;
    return newNode;
}

struct Node* removeNthFromEnd(struct Node* head, int positionFromEnd) {
    struct Node dummy;
    dummy.next = head;
    struct Node* fast = &dummy;
    struct Node* slow = &dummy;
    for (int step = 0; step <= positionFromEnd; step++) fast = fast->next;
    while (fast) {
        fast = fast->next;
        slow = slow->next;
    }
    struct Node* nodeToDelete = slow->next;
    slow->next = slow->next->next;
    free(nodeToDelete);
    return dummy.next;
}

void printList(struct Node* head) {
    while (head) {
        printf("%d ", head->value);
        head = head->next;
    }
}

int main() {
    struct Node* head = createNode(1);
    head->next = createNode(2);
    head->next->next = createNode(3);
    head->next->next->next = createNode(4);
    head->next->next->next->next = createNode(5);
    int n = 2;
    head = removeNthFromEnd(head, n);
    printList(head);
    return 0;
}
