#include <stdio.h>

struct student {
    int Roll;
    char Name[50];
    float Marks[3];
    float Total;
    float Average;
    char Grade;
};

float Calculate_TotalMarks(float marks[]) {
    return marks[0] + marks[1] + marks[2];
}

float Calculate_AverageMarks(float Total) {
    return Total / 3;
}

char Calculate_Student_Grades(float average) {
    if (average >= 85)
        return 'A';
    else if (average >= 70)
        return 'B';
    else if (average >= 50)
        return 'C';
    else if (average >= 35)
        return 'D';
    else
        return 'F';
}

void Display_Performance(char grade) {
    int stars = 0;
    switch (grade) {
        case 'A': stars = 5; break;
        case 'B': stars = 4; break;
        case 'C': stars = 3; break;
        case 'D': stars = 2; break;
        default: stars = 0;
    }
    for (int i = 0; i < stars; i++) {
        printf("*");
    }
}

void Print_Roll_Numbers(struct student students[], int index, int n) {
    if (index == n)
        return;
    printf("%d ", students[index].Roll);

    Print_Roll_Numbers(students, index + 1, n);
}

int main() {
    struct student students[100];
    int n;

    printf("Enter number of students: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        printf("\nEnter details for student %d\n", i + 1);
        printf("Enter Roll Number: ");
        scanf("%d", &students[i].Roll);
        printf("Enter Name: ");
        scanf("%s", students[i].Name);
        printf("Enter marks in 3 subjects: ");
        scanf("%f %f %f", &students[i].Marks[0], &students[i].Marks[1], &students[i].Marks[2]);
        
        students[i].Total = Calculate_TotalMarks(students[i].Marks);
        students[i].Average = Calculate_AverageMarks(students[i].Total);
        students[i].Grade = Calculate_Student_Grades(students[i].Average);
    }

    printf("\n---------- Student Results ----------\n");

    for (int i = 0; i < n; i++) {
        printf("\nRoll: %d\n", students[i].Roll);
        printf("Name: %s\n", students[i].Name);
        printf("Total: %.2f\n", students[i].Total);
        printf("Average: %.2f\n", students[i].Average);
        printf("Grade: %c\n", students[i].Grade);
        if (students[i].Average < 35) {
            continue;
        }
        printf("Performance: ");
        Display_Performance(students[i].Grade);
        printf("\n");
    }

    printf("\nList of Roll Numbers (via recursion): ");
    Print_Roll_Numbers(students, 0, n);
    printf("\n");

    return 0;
}
