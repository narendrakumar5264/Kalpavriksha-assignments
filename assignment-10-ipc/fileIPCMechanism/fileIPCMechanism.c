#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#define FILENAME "file_ipc_data.txt"
#define MAXN 100

void inputArray(int *arr, int n);
void printArray(int *arr, int n, const char *label);
void sortArray(int *arr, int n);
void readArrayFromFile(char *filename, int *arr, int *n);
void writeArrayToFile(char *filename, int *arr, int n);

int main()
{
    int n;
    printf("Enter the number of elements: ");
    scanf("%d", &n);

    int arr[n];
    inputArray(arr, n);
    printArray(arr, n, "Before sorting");

    writeArrayToFile(FILENAME, arr, n);

    pid_t pid = fork();
    if (pid == 0)
    {
        int childArr[MAXN];
        int m;
        readArrayFromFile(FILENAME, childArr, &m);
        sortArray(childArr, m);
        writeArrayToFile(FILENAME, childArr, m);
    }
    else
    {
        wait(NULL);
        int parentArr[MAXN];
        int m;
        readArrayFromFile(FILENAME, parentArr, &m);
        printArray(parentArr, m, "After sorting");
    }

    return 0;
}

void inputArray(int *arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &arr[i]);
    }
}

void printArray(int *arr, int n, const char *label)
{
    printf("%s: ", label);
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void sortArray(int *arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (arr[j] < arr[i])
            {
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

void readArrayFromFile(char *filename, int *arr, int *n)
{
    FILE *fileptr = fopen(filename, "r+");
    if (!fileptr)
    {
        perror("fopen error (read)");
        return;
    }
    fscanf(fileptr, "%d", n);
    for (int i = 0; i < *n; i++)
    {
        fscanf(fileptr, "%d", &arr[i]);
    }
    fclose(fileptr);
}

void writeArrayToFile(char *filename, int *arr, int n)
{
    FILE *fileptr = fopen(filename, "w");
    if (!fileptr)
    {
        perror("fopen error (write)");
        return;
    }
    fprintf(fileptr, "%d\n", n);
    for (int i = 0; i < n; i++)
    {
        fprintf(fileptr, "%d ", arr[i]);
    }
    fprintf(fileptr, "\n");
    fclose(fileptr);
}