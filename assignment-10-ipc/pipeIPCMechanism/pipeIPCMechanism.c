#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAXN 100

void inputArray(int *arr, int n);
void printArray(int *arr, int n, const char *label);
void sortArray(int *arr, int n);
void sendArrayToPipe(int *arr, int n, int write_fd);
void receiveArrayFromPipe(int *arr, int *n, int read_fd);

int main()
{
     int n;
     printf("Enter number of elements: ");
     scanf("%d", &n);

     int arr[MAXN];
     inputArray(arr, n);
     printArray(arr, n, "Before sorting");

     int parentToChild[2];
     int childToParent[2];

     if (pipe(parentToChild) == -1 || pipe(childToParent) == -1)
     {
          perror("pipe failed");
          exit(1);
     }

     pid_t pid = fork();

     if (pid == 0)
     {
          close(parentToChild[1]);
          close(childToParent[0]);

          int childArr[MAXN];
          int m;

          receiveArrayFromPipe(childArr, &m, parentToChild[0]);
          sortArray(childArr, m);
          sendArrayToPipe(childArr, m, childToParent[1]);

          close(parentToChild[0]);
          close(childToParent[1]);
          _exit(0);
     }
     else
     {
          close(parentToChild[0]);
          close(childToParent[1]);

          sendArrayToPipe(arr, n, parentToChild[1]);
          wait(NULL);

          int sortedArr[MAXN];
          int m;
          receiveArrayFromPipe(sortedArr, &m, childToParent[0]);
          printArray(sortedArr, m, "After sorting");

          close(parentToChild[1]);
          close(childToParent[0]);
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

void sendArrayToPipe(int *arr, int n, int write_fd)
{
     write(write_fd, &n, sizeof(int));
     write(write_fd, arr, sizeof(int) * n);
}

void receiveArrayFromPipe(int *arr, int *n, int read_fd)
{
     read(read_fd, n, sizeof(int));
     read(read_fd, arr, sizeof(int) * (*n));
}