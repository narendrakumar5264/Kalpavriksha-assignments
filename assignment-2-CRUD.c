#include <stdio.h>
#include <string.h>

#define file_name "users.txt"

// file structure

typedef struct User{
    int id;
    char name[50];
    int age;
} User;


void createuser(){
    User user;
    FILE *fp;
    char line[100];
   

    fp = fopen(file_name, "a");
    if(fp == NULL){
        printf("Error opening file\n");
        return;
    }

    printf("Enter id:");
    scanf("%d", &user.id);
   
  printf("Enter name: ");
scanf(" %[^\n]", user.name);  

    // fgets(user.name, sizeof(user.name), stdin);

    printf("Enter age:");
    scanf("%d", &user.age);


    fprintf(fp , "%d %s %d\n", user.id, user.name, user.age);


    // rewind(fp);
    //      while (fgets(line, sizeof(line), fp)) {
    //     printf("%s", line);   
    // }

    fclose(fp);
    printf("User added successfully\n");


}
void viewuser() {
    FILE *fp;
    User user;
    char line[100];

    fp = fopen(file_name, "r");
    if(fp == NULL) {
        printf("No users found!\n");
        return;
    }


    printf("\n--- All Users ---\n");
    while(fgets(line, sizeof(line), fp)) {
        printf("%s", line);  
    }

    fclose(fp);
}


void updateuser()
{
     FILE *fp = fopen(file_name, "r");
     FILE *temp = fopen("temp.txt", "w");

 
     User u;
     int id, found = 0;
     printf("Enter ID to update: ");
     scanf("%d", &id);
     while (fscanf(fp, "%d %s %d", &u.id, u.name, &u.age) == 3)
     {
          if (u.id == id)
          {
               printf("Enter new Name: ");
               scanf("%s", u.name);

               printf("Enter new Age: ");
               scanf("%d", &u.age);
               found = 1;
          }
          fprintf(temp, "%d %s %d\n", u.id, u.name, u.age);
     }
     fclose(fp);
     fclose(temp);
     remove(file_name);

     rename("temp.txt", file_name);
     if (found){
          printf("User updated successfully.\n");
     }
     else{

          printf("User not found.\n");
     }
}

void deleteuser(){

    int id , found = 0;

     printf("Enter Id of user to delete: ");
    scanf("%d", &id);

    FILE *fp = fopen(file_name, "r");
    FILE *temp = fopen("temp.txt", "w");

    User user;

    char line[100];

    while(fgets(line, sizeof(line), fp)) {
      
         if (sscanf(line, "%d %49s %d", &user.id, user.name, &user.age) == 3) {
         if(user.id == id) {
            found = 1;
            continue; 
         }
           fprintf(temp, "%d %s %d\n", user.id, user.name, user.age);
        }
    }

     fclose(fp);
    fclose(temp);

    if (found) {
        remove(file_name);
        rename("temp.txt", file_name);
        printf("User deleted successfully.\n");
    } else {
        remove("temp.txt");
        printf("User  %d not found.\n", id);
    }

}



int main(){

    int choice;


    while(1){

    printf("..... Users Mangement System .....\n");
    printf("1. add User \n");
    printf("2. view User \n");
    printf("3.update User \n");
    printf("4. delete User \n");
    printf("5. exit \n");


    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch(choice){
        case 1: {
            createuser();
            break;
        }
        case 2: {
            viewuser();
            break;
        }
        case 3: {
            updateuser();
            break;
        }
        case 4: {
            deleteuser();
            break;
        }
        case 5: {
            printf("Exiting...\n");
            return 0;
        }
    default: {
        printf("Invalid choice\n");
        break;
    }
    }
}
    return 0;

    
}

