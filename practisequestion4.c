#include <stdio.h>

int main() {
    int arraySize;
    printf("Enter number of elements: ");
    scanf("%d", &arraySize);

    if (arraySize <= 0) {
        printf("Invalid array size.\n");
        return 0;
    }

    int numbers[arraySize];
    printf("Enter %d elements:\n", arraySize);
    for (int elementIndex = 0; elementIndex < arraySize; elementIndex++){
        scanf("%d", numbers + elementIndex);
    }

    int newSize = arraySize;

    for (int *currentElement = numbers; currentElement < numbers + newSize; currentElement++) {
        for (int *nextElement = currentElement + 1; nextElement < numbers + newSize; nextElement++) {
            if (*currentElement == *nextElement) {
                for (int *shiftPointer = nextElement; shiftPointer < numbers + newSize - 1; shiftPointer++) {
                    *shiftPointer = *(shiftPointer + 1);
                }
                newSize--;
                nextElement--;
            }
        }
    }

    printf("Array after removing duplicates: ");
    for (int *printPointer = numbers; printPointer < numbers + newSize; printPointer++){
        printf("%d ", *printPointer);
    }

    printf("\nNew length: %d\n", newSize);

    return 0;
}
