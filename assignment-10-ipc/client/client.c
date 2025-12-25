
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int connectToServer();
void processTransactions(int sock);

int main() {
    int sock = connectToServer();
    if (sock < 0) {
        return -1;
    }

    processTransactions(sock);

    close(sock);
    return 0;
}

int connectToServer() {
    int sock = 0;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    return sock;
}

void processTransactions(int sock) {
    int choice;
    double amount;
    char buffer[1024] = {0};
    char request[50];

    while (1) {
        printf("\n--- ATM Menu ---\n");
        printf("1. Withdraw\n");
        printf("2. Deposit\n");
        printf("3. Display Balance\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 4) break; 

        amount = 0;
        if (choice == 1 || choice == 2) {
            printf("Enter amount: ");
            scanf("%lf", &amount);
        }

        snprintf(request, sizeof(request), "%d %.2lf", choice, amount);
        send(sock, request, strlen(request), 0);

        memset(buffer, 0, 1024);
        read(sock, buffer, 1024);
        printf("Server Response: %s\n", buffer);
    }
}
