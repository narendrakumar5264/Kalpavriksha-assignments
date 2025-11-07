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

void printList(struct Node* head) {
    while (head) {
        printf("%d ", head->data);
        head = head->next;
    }
}

struct Node* sort012(struct Node* head) {
    struct Node *zeroH = NULL, *zeroT = NULL;
    struct Node *oneH = NULL, *oneT = NULL;
    struct Node *twoH = NULL, *twoT = NULL;
    struct Node* curr = head;

    while (curr) {
        if (curr->data == 0) {
            if (!zeroH) zeroH = zeroT = curr;
            else { zeroT->next = curr; zeroT = curr; }
        } else if (curr->data == 1) {
            if (!oneH) oneH = oneT = curr;
            else { oneT->next = curr; oneT = curr; }
        } else {
            if (!twoH) twoH = twoT = curr;
            else { twoT->next = curr; twoT = curr; }
        }
        curr = curr->next;
    }

    if (zeroT) zeroT->next = oneH ? oneH : twoH;
    if (oneT) oneT->next = twoH;
    if (twoT) twoT->next = NULL;

    return zeroH ? zeroH : (oneH ? oneH : twoH);
}

int main() {
    struct Node* head = createNode(1);
    head->next = createNode(2);
    head->next->next = createNode(0);
    head->next->next->next = createNode(1);
    head->next->next->next->next = createNode(2);
    head->next->next->next->next->next = createNode(0);
    head->next->next->next->next->next->next = createNode(1);
    head = sort012(head);
    printList(head);
    return 0;
}
