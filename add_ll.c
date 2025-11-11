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

struct Node* addTwoNumbers(struct Node* l1, struct Node* l2) {
    struct Node* dummy = createNode(0);
    struct Node* curr = dummy;
    int carry = 0;

    while (l1 || l2 || carry) {
        int sum = carry;
        if (l1) {
            sum += l1->data;
            l1 = l1->next;
        }
        if (l2) {
            sum += l2->data;
            l2 = l2->next;
        }
        carry = sum / 10;
        curr->next = createNode(sum % 10);
        curr = curr->next;
    }
    struct Node* result = dummy->next;
    free(dummy);
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
    
    struct Node* l1 = createNode(2);
    l1->next = createNode(4);
    l1->next->next = createNode(3);

    struct Node* l2 = createNode(5);
    l2->next = createNode(6);
    l2->next->next = createNode(4);

    struct Node* sum = addTwoNumbers(l1, l2);
    printList(sum); 
}
