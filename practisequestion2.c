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
    int visited[arraySize];

    printf("Enter %d elements:\n", arraySize);
    for (int position = 0; position < arraySize; position++) {
        scanf("%d", &numbers[position]);
        visited[position] = 0;
    }

    for (int outerIndex = 0; outerIndex < arraySize; outerIndex++) {
        if (visited[outerIndex] == 1)
            continue;

        int count = 1;
        for (int innerIndex = outerIndex + 1; innerIndex < arraySize; innerIndex++) {
            if (numbers[outerIndex] == numbers[innerIndex]) {
                count++;
                visited[innerIndex] = 1;
            }
        }

        printf("Element %d appears %d time%s\n", numbers[outerIndex], count, (count > 1 ? "s" : ""));
    }

    return 0;
}
