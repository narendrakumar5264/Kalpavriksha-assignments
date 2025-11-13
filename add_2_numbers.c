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

struct Node* addTwoNumbers(struct Node* first, struct Node* second) {
    struct Node dummy;
    struct Node* tail = &dummy;
    dummy.next = NULL;
    int carry = 0;
    while (first || second || carry) {
        int sum = carry;
        if (first) {
            sum += first->value;
            first = first->next;
        }
        if (second) {
            sum += second->value;
            second = second->next;
        }
        carry = sum / 10;
        tail->next = createNode(sum % 10);
        tail = tail->next;
    }
    return dummy.next;
}

void printList(struct Node* head) {
    while (head) {
        printf("%d ", head->value);
        head = head->next;
    }
}

int main() {
    struct Node* num1 = createNode(2);
    num1->next = createNode(4);
    num1->next->next = createNode(3);
    struct Node* num2 = createNode(5);
    num2->next = createNode(6);
    num2->next->next = createNode(4);
    struct Node* result = addTwoNumbers(num1, num2);
    printList(result);
    return 0;
}
