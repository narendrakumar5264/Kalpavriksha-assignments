#include <stdio.h>
#include <limits.h>

int findSecondLargest(int arraySize, int numbers[]) {
    if (arraySize < 2) {
        printf("Array size must be at least 2.\n");
        return INT_MIN;
    }

    int largest = INT_MIN;
    int secondLargest = INT_MIN;

    for (int currentIndex = 0; currentIndex < arraySize; currentIndex++) {
        if (numbers[currentIndex] > largest) {
            secondLargest = largest;
            largest = numbers[currentIndex];
        } else if (numbers[currentIndex] > secondLargest && numbers[currentIndex] < largest) {
            secondLargest = numbers[currentIndex];
        }
    }

    if (secondLargest == INT_MIN) {
        printf("No second largest element (all elements are same or only one element).\n");
        return INT_MIN;
    }

    return secondLargest;
}

int findSecondSmallest(int arraySize, int numbers[]) {
    if (arraySize < 2) {
        printf("Array size must be at least 2.\n");
        return INT_MAX;
    }

    int smallest = INT_MAX;
    int secondSmallest = INT_MAX;

    for (int currentIndex = 0; currentIndex < arraySize; currentIndex++) {
        if (numbers[currentIndex] < smallest) {
            secondSmallest = smallest;
            smallest = numbers[currentIndex];
        } else if (numbers[currentIndex] < secondSmallest && numbers[currentIndex] > smallest) {
            secondSmallest = numbers[currentIndex];
        }
    }

    if (secondSmallest == INT_MAX) {
        printf("No second smallest element (all elements are same or only one element).\n");
        return INT_MAX;
    }

    return secondSmallest;
}

int main() {
    int arraySize;
    printf("Enter the size of the array: ");
    scanf("%d", &arraySize);

    if (arraySize <= 0) {
        printf("Invalid array size.\n");
        return 0;
    }

    int numbers[arraySize];
    printf("Enter the array elements: ");
    for (int position = 0; position < arraySize; position++) {
        scanf("%d", &numbers[position]);
    }

    int secondLargest = findSecondLargest(arraySize, numbers);
    if (secondLargest != INT_MIN)
        printf("Second Largest: %d\n", secondLargest);

    int secondSmallest = findSecondSmallest(arraySize, numbers);
    if (secondSmallest != INT_MAX)
        printf("Second Smallest: %d\n", secondSmallest);

    return 0;
}
