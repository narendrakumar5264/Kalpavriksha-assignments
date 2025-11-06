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
    struct Node* current = head;
    while (current && current->next) {
        if (current->data == current->next->data) {
            struct Node* temp = current->next;
            current->next = temp->next;
            free(temp);
        } else {
            current = current->next;
        }
    }
    return head;
}

int main() {
    int n, data;
    struct Node *head = NULL, *tail = NULL;

    printf("Enter number of nodes: ");
    scanf("%d", &n);

    printf("Enter %d sorted elements:\n", n);
    for (int index = 0; index < n; index++) {
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

    printf("\nAfter Removing Duplicates :\n");
    printList(head);

    return 0;
}
