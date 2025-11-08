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
        printf("%d", head->data);
        if (head->next) printf(" -> ");
        head = head->next;
    }
    printf("\n");
}

struct Node* deleteDuplicates(struct Node* head) {
    if (!head) return NULL;

    struct Node dummy;
    dummy.next = head;
    struct Node* prev = &dummy;

    while (head) {
        int duplicate = 0;
        while (head->next && head->data == head->next->data) {
            struct Node* temp = head->next;
            head->next = temp->next;
            free(temp);
            duplicate = 1;
        }

        if (duplicate) {
            struct Node* temp = head;
            head = head->next;
            free(temp);
            prev->next = head;
        } else {
            prev = head;
            head = head->next;
        }
    }

    return dummy.next;
}

int main() {
    int n, data;
    struct Node *head = NULL, *tail = NULL;

    printf("Enter number of nodes: ");
    scanf("%d", &n);

    printf("Enter %d sorted elements:\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &data);
        struct Node* newNode = createNode(data);
        if (!head)
            head = tail = newNode;
        else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    printf("\nOriginal List:\n");
    printList(head);

    head = deleteDuplicates(head);

    printf("\nAfter Removing Duplicates:\n");
    printList(head);

    return 0;
}
