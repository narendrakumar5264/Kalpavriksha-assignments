#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <string.h>

#define MAXN 100
#define MSG_KEY 12345

struct message
{
     long mtype;
     int data[MAXN];
     int size;
};
void inputArray(int *arr, int n);
void printArray(int *arr, int n, const char *label);
void sortArray(int *arr, int n);
void sendArrayToQueue(int msgid, int *arr, int n);
void receiveArrayFromQueue(int msgid, int *arr, int *n);

int main()
{
     int n;
     printf("Enter the number of elements: ");
     scanf("%d", &n);

     int arr[n];
     inputArray(arr, n);
     printArray(arr, n, "Before sorting");
     int msgid = msgget(MSG_KEY, IPC_CREAT | 0666);
     if (msgid == -1)
     {
          perror("msgget failed");
          return 1;
     }

     sendArrayToQueue(msgid, arr, n);

     pid_t pid = fork();
     if (pid == 0)
     {
          int childArr[MAXN];
          int m;
          receiveArrayFromQueue(msgid, childArr, &m);
          sortArray(childArr, m);
          sendArrayToQueue(msgid, childArr, m);
     }
     else
     {
          wait(NULL);
          int parentArr[MAXN];
          int m;
          receiveArrayFromQueue(msgid, parentArr, &m);
          printArray(parentArr, m, "After sorting");
          msgctl(msgid, IPC_RMID, NULL);
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

void sendArrayToQueue(int msgid, int *arr, int n)
{
     struct message msg;
     msg.mtype = 1;
     msg.size = n;
     for (int i = 0; i < n; i++)
     {
          msg.data[i] = arr[i];
     }

     if (msgsnd(msgid, &msg, sizeof(msg) - sizeof(long), 0) == -1)
     {
          perror("msgsnd failed");
          exit(1);
     }
}

void receiveArrayFromQueue(int msgid, int *arr, int *n)
{
     struct message msg;

     if (msgrcv(msgid, &msg, sizeof(msg) - sizeof(long), 1, 0) == -1)
     {
          perror("msgrcv failed");
          exit(1);
     }

     *n = msg.size;
     for (int i = 0; i < *n; i++)
     {
          arr[i] = msg.data[i];
     }
}