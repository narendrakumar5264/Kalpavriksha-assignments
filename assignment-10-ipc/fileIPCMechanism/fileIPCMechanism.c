#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define FILENAME "file_ipc_data.txt"
#define MAX_ELEMENTS 100

void inputArray(int *numbers, int elementCount);
void printArray(int *numbers, int elementCount, const char *label);
void sortArray(int *numbers, int elementCount);
void readArrayFromFile(char *filename, int *numbers, int *fileElementCount);
void writeArrayToFile(char *filename, int *numbers, int elementCount);

int main()
{
    int elementCount;
    printf("Enter the number of elements: ");
    scanf("%d", &elementCount);

    int numbers[elementCount];
    inputArray(numbers, elementCount);
    printArray(numbers, elementCount, "Before sorting");

    writeArrayToFile(FILENAME, numbers, elementCount);

    pid_t processId = fork();

    if (processId == 0)
    {
        int childNumbers[MAX_ELEMENTS];
        int fileElementCount;

        readArrayFromFile(FILENAME, childNumbers, &fileElementCount);
        sortArray(childNumbers, fileElementCount);
        writeArrayToFile(FILENAME, childNumbers, fileElementCount);
    }
    else
    {
        wait(NULL);

        int parentNumbers[MAX_ELEMENTS];
        int fileElementCount;

        readArrayFromFile(FILENAME, parentNumbers, &fileElementCount);
        printArray(parentNumbers, fileElementCount, "After sorting");
    }

    return 0;
}

void inputArray(int *numbers, int elementCount)
{
    for (int index = 0; index < elementCount; index++)
    {
        scanf("%d", &numbers[index]);
    }
}

void printArray(int *numbers, int elementCount, const char *label)
{
    printf("%s: ", label);
    for (int index = 0; index < elementCount; index++)
    {
        printf("%d ", numbers[index]);
    }
    printf("\n");
}

void sortArray(int *numbers, int elementCount)
{
    for (int index = 0; index < elementCount; index++)
    {
        for (int compareIndex = index + 1; compareIndex < elementCount; compareIndex++)
        {
            if (numbers[compareIndex] < numbers[index])
            {
                int temp = numbers[index];
                numbers[index] = numbers[compareIndex];
                numbers[compareIndex] = temp;
            }
        }
    }
}

void readArrayFromFile(char *filename, int *numbers, int *fileElementCount)
{
    FILE *filePointer = fopen(filename, "r");
    if (!filePointer)
    {
        perror("File open error");
        return;
    }

    fscanf(filePointer, "%d", fileElementCount);
    for (int index = 0; index < *fileElementCount; index++)
    {
        fscanf(filePointer, "%d", &numbers[index]);
    }

    fclose(filePointer);
}

void writeArrayToFile(char *filename, int *numbers, int elementCount)
{
    FILE *filePointer = fopen(filename, "w");
    if (!filePointer)
    {
        perror("File open error");
        return;
    }

    fprintf(filePointer, "%d\n", elementCount);
    for (int index = 0; index < elementCount; index++)
    {
        fprintf(filePointer, "%d ", numbers[index]);
    }

    fprintf(filePointer, "\n");
    fclose(filePointer);
}
