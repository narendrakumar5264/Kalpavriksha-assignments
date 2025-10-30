#include <stdio.h>

int main() {
    int arraySize;
    printf("Enter the number of elements in the array: ");
    scanf("%d", &arraySize);

    if (arraySize <= 0) {
        printf("Invalid array size.\n");
        return 0;
    }

    int numbers[arraySize];
    printf("Enter %d elements:\n", arraySize);
    for (int position = 0; position < arraySize; position++) {
        scanf("%d", &numbers[position]);
    }

    int *leftPointer = numbers;
    int *rightPointer = numbers + arraySize - 1;
    int isPalindrome = 1;

    while (leftPointer < rightPointer) {
        if (*leftPointer != *rightPointer) {
            isPalindrome = 0;
            break;
        }
        leftPointer++;
        rightPointer--;
    }

    if (isPalindrome)
        printf("Palindrome\n");
    else
        printf("Not a palindrome\n");

    return 0;
}
