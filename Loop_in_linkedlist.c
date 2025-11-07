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

int countLoopLength(struct Node* loopNode) {
    struct Node* temp = loopNode;
    int count = 1;
    while (temp->next != loopNode) {
        count++;
        temp = temp->next;
    }
    return count;
}

int findLoopLength(struct Node* head) {
    struct Node *slow = head, *fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast)
            return countLoopLength(slow);
    }
    return 0;
}

int main() {
    struct Node* head = createNode(1);
    head->next = createNode(2);
    head->next->next = createNode(3);
    head->next->next->next = createNode(4);
    head->next->next->next->next = createNode(5);
    head->next->next->next->next->next = head->next->next;
    printf("%d", findLoopLength(head));
    return 0;
}
