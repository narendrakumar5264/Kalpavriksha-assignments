#include <stdio.h>

int main() {
    int arraySize;
    printf("Enter number of elements: ");
    scanf("%d", &arraySize);

    if (arraySize <= 0) {
        printf("Invalid array size.\n");
        return 0;
    }

    int sourceArray[arraySize], destinationArray[arraySize];
    printf("Enter %d elements for the source array:\n", arraySize);
    for (int inputIndex = 0; inputIndex < arraySize; inputIndex++){
        scanf("%d", sourceArray + inputIndex);
    }

    int *sourcePointer = sourceArray;
    int *destinationPointer = destinationArray;

    while (sourcePointer < sourceArray + arraySize) {
        *destinationPointer = *sourcePointer;
        sourcePointer++;
        destinationPointer++;
    }

    printf("Source Array: ");
    for (int *printPointer = sourceArray; printPointer < sourceArray + arraySize; printPointer++){
        printf("%d ", *printPointer);
    }

    printf("\nCopied Array: ");
    for (int *printPointer = destinationArray; printPointer < destinationArray + arraySize; printPointer++){
        printf("%d ", *printPointer);
    }

    printf("\n");

    return 0;
}
