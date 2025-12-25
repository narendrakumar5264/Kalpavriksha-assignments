#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <string.h>

#define MAXN 100
#define SHM_KEY 12345

void inputArray(int *arr, int n);
void printArray(int *arr, int n, const char *label);
void sortArray(int *arr, int n);

int main()
{
     int n;
     printf("Enter the number of elements: ");
     scanf("%d", &n);

     int shmid = shmget(SHM_KEY, sizeof(int) * (n + 1), IPC_CREAT | 0666);
     if (shmid == -1)
     {
          perror("shmget failed");
          return 1;
     }

     int *arr = (int *)shmat(shmid, NULL, 0);
     if (arr == (int *)-1)
     {
          perror("shmat failed");
          return 1;
     }
     arr[0] = n;

     inputArray(&arr[1], n);
     printArray(&arr[1], n, "Before sorting");

     pid_t pid = fork();
     if (pid == 0)
     {
          int *childArr = (int *)shmat(shmid, NULL, 0);
          if (childArr == (int *)-1)
          {
               perror("shmat failed in child");
               return 1;
          }

          sortArray(&childArr[1], childArr[0]);
          printf("Child sorted the array.\n");
          shmdt(childArr);
          exit(0);
     }
     else
     {
          wait(NULL);
          printArray(&arr[1], n, "After sorting");
          shmdt(arr);
          shmctl(shmid, IPC_RMID, NULL);
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