
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define DB_FILE "../resource/accountDB.txt" 

pthread_mutex_t file_mutex; 

double getBalance();
void updateBalance(double new_balance);
void *handleClient(void *socket_desc);

int main() {
    int server_fd, new_socket, *new_sock;
    struct sockaddr_in address;
    int addrlen = sizeof(address);


    pthread_mutex_init(&file_mutex, NULL);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

  
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("ATM Server listening on port %d...\n", PORT);

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("Accept failed");
            continue;
        }

        printf("New client connected.\n");

     
        pthread_t thread_id;
        new_sock = malloc(1);
        *new_sock = new_socket;
        
        if (pthread_create(&thread_id, NULL, handleClient, (void *)new_sock) < 0) {
            perror("Could not create thread");
            return 1;
        }
        pthread_detach(thread_id); 
    }

    pthread_mutex_destroy(&file_mutex);
    return 0;
}


double getBalance() {
    FILE *file = fopen(DB_FILE, "r");
    double balance;
    fscanf(file, "%lf", &balance);
    fclose(file);
    return balance;
}

void updateBalance(double new_balance) {
    FILE *file = fopen(DB_FILE, "w");
    if (file) {
        fprintf(file, "%.2lf", new_balance);
        fclose(file);
    }
}

void *handleClient(void *socket_desc) {
    int sock = *(int *)socket_desc;
    free(socket_desc);
    char buffer[1024] = {0};
    int choice;
    double amount;

    while (1) {
        int valread = read(sock, buffer, 1024);
        if (valread <= 0){
               break;
        }  

        sscanf(buffer, "%d %lf", &choice, &amount);
        char response[1024];

        pthread_mutex_lock(&file_mutex);
        
        double current_balance = getBalance();
        
        if (choice == 1) { 
            if (amount > current_balance) {
                snprintf(response, sizeof(response), "Error: Insufficient funds. Balance: %.2lf", current_balance);
            } else {
                current_balance -= amount;
                updateBalance(current_balance);
                snprintf(response, sizeof(response), "Withdrawal successful. New Balance: %.2lf", current_balance);
            }
        } else if (choice == 2) { 
            current_balance += amount;
            updateBalance(current_balance);
            snprintf(response, sizeof(response), "Deposit successful. New Balance: %.2lf", current_balance);
        } else if (choice == 3) { 
            snprintf(response, sizeof(response), "Current Balance: %.2lf", current_balance);
        } else {
            strcpy(response, "Invalid option");
        }

        pthread_mutex_unlock(&file_mutex);

        send(sock, response, strlen(response), 0);
    }

    close(sock);
    return NULL;
}
