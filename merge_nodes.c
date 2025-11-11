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

struct Node* mergeNodes(struct Node* head) {
    struct Node *result = NULL, *tail = NULL;
    int sum = 0;
    head = head->next;

    while (head) {
        if (head->data == 0) {
            struct Node* newNode = createNode(sum);
            if (!result) result = tail = newNode;
            else {
                tail->next = newNode;
                tail = newNode;
            }
            sum = 0;
        } else {
            sum += head->data;
        }
        head = head->next;
    }
    return result;
}

void printList(struct Node* head) {
    while (head) {
        printf("%d ", head->data);
        head = head->next;
    }
    printf("\n");
}

int main() {

    struct Node* head = createNode(0);
    head->next = createNode(3);
    head->next->next = createNode(1);
    head->next->next->next = createNode(0);
    head->next->next->next->next = createNode(4);
    head->next->next->next->next->next = createNode(5);
    head->next->next->next->next->next->next = createNode(2);
    head->next->next->next->next->next->next->next = createNode(0);

    struct Node* result = mergeNodes(head);
    printList(result);
}
