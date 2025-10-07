#include <stdio.h>
#include <string.h>

#define FILE_NAME "users.txt"

typedef struct {
    int id;
    char name[50];
    int age;
} User;

void createUser() {
    User newUser;
    FILE *filePointer;

    filePointer = fopen(FILE_NAME, "a");
    if (filePointer == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter ID: ");
    scanf("%d", &newUser.id);

    printf("Enter Name: ");
    scanf(" %[^\n]", newUser.name);

    printf("Enter Age: ");
    scanf("%d", &newUser.age);

    fprintf(filePointer, "%d %s %d\n", newUser.id, newUser.name, newUser.age);
    fclose(filePointer);

    printf("User added successfully.\n");
}

void viewUsers() {
    FILE *filePointer;
    char line[100];

    filePointer = fopen(FILE_NAME, "r");
    if (filePointer == NULL) {
        printf("No users found.\n");
        return;
    }

    printf("\n--- All Users ---\n");
    while (fgets(line, sizeof(line), filePointer)) {
        printf("%s", line);
    }

    fclose(filePointer);
}

void updateUser() {
    FILE *filePointer = fopen(FILE_NAME, "r");
    FILE *tempFile = fopen("temp.txt", "w");

    if (filePointer == NULL || tempFile == NULL) {
        printf("Error opening file.\n");
        return;
    }

    User currentUser;
    int targetId, found = 0;

    printf("Enter ID to update: ");
    scanf("%d", &targetId);

    while (fscanf(filePointer, "%d %s %d", &currentUser.id, currentUser.name, &currentUser.age) == 3) {
        if (currentUser.id == targetId) {
            printf("Enter new Name: ");
            scanf("%s", currentUser.name);

            printf("Enter new Age: ");
            scanf("%d", &currentUser.age);

            found = 1;
        }
        fprintf(tempFile, "%d %s %d\n", currentUser.id, currentUser.name, currentUser.age);
    }

    fclose(filePointer);
    fclose(tempFile);

    remove(FILE_NAME);
    rename("temp.txt", FILE_NAME);

    if (found) {
        printf("User updated successfully.\n");
    } else {
        printf("User not found.\n");
    }
}

void deleteUser() {
    FILE *filePointer = fopen(FILE_NAME, "r");
    FILE *tempFile = fopen("temp.txt", "w");

    if (filePointer == NULL || tempFile == NULL) {
        printf("Error opening file.\n");
        return;
    }

    User currentUser;
    int targetId, found = 0;

    printf("Enter ID of user to delete: ");
    scanf("%d", &targetId);

    char line[100];
    while (fgets(line, sizeof(line), filePointer)) {
        if (sscanf(line, "%d %49s %d", &currentUser.id, currentUser.name, &currentUser.age) == 3) {
            if (currentUser.id == targetId) {
                found = 1;
                continue;
            }
            fprintf(tempFile, "%d %s %d\n", currentUser.id, currentUser.name, currentUser.age);
        }
    }

    fclose(filePointer);
    fclose(tempFile);

    if (found) {
        remove(FILE_NAME);
        rename("temp.txt", FILE_NAME);
        printf("User deleted successfully.\n");
    } else {
        remove("temp.txt");
        printf("User with ID %d not found.\n", targetId);
    }
}

int main() {
    int choice;

    while (1) {
        printf("\n..... User Management System .....\n");
        printf("1. Add User\n");
        printf("2. View Users\n");
        printf("3. Update User\n");
        printf("4. Delete User\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                createUser();
                break;
            case 2:
                viewUsers();
                break;
            case 3:
                updateUser();
                break;
            case 4:
                deleteUser();
                break;
            case 5:
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}
