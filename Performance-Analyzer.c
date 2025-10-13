#include <stdio.h>
#include <stdbool.h>

#define NUM_SUBJECTS 3
#define MAX_STUDENTS 100

struct Student {
    int rollNumber;
    char name[50];
    float marks[NUM_SUBJECTS];
    float totalMarks;
    float averageMarks;
    char grade;
};

bool isValidMark(float mark) {
    return mark >= 0 && mark <= 100;
}

float calculateTotalMarks(float marks[]) {
    float total = 0;
    for (int subjectIndex = 0; subjectIndex < NUM_SUBJECTS; subjectIndex++) {
        total += marks[subjectIndex];
    }
    return total;
}

float calculateAverageMarks(float totalMarks) {
    return totalMarks / NUM_SUBJECTS;
}

char calculateStudentGrade(float average) {
    if (average >= 85) {
        return 'A';
    }
    else if (average >= 70) {
        return 'B';
    }
    else if (average >= 50) {
        return 'C';
    }
    else if (average >= 35) {
         return 'D';
    }
    else {
        return 'F';
    }
}

void displayPerformance(char grade) {
    int stars = 0;
    switch (grade) {
        case 'A': stars = 5; break;
        case 'B': stars = 4; break;
        case 'C': stars = 3; break;
        case 'D': stars = 2; break;
        default: stars = 0;
    }
    for (int count = 0; count < stars; count++) {
        printf("*");
    }
}

void printRollNumbers(struct Student studentList[], int currentIndex, int totalStudents) {
    if (currentIndex == totalStudents){
        return;
    }
    printf("%d ", studentList[currentIndex].rollNumber);
    printRollNumbers(studentList, currentIndex + 1, totalStudents);
}

int main() {
    struct Student studentList[MAX_STUDENTS];
    int totalStudents;

    printf("Enter number of students: ");

    if (scanf("%d", &totalStudents) != 1 || totalStudents <= 0 || totalStudents > MAX_STUDENTS) {
        printf("Invalid number of students. Must be between 1 and %d.\n", MAX_STUDENTS);
        return 1;
    }

    for (int studentIndex = 0; studentIndex < totalStudents; studentIndex++) {
        printf("\nEnter details for student %d\n", studentIndex + 1);

        printf("Enter Roll Number: ");
        if (scanf("%d", &studentList[studentIndex].rollNumber) != 1 || studentList[studentIndex].rollNumber <= 0) {
            printf("Invalid roll number. Must be a positive integer.\n");
            return 1;
        }

        printf("Enter Name: ");
        if (scanf("%49s", studentList[studentIndex].name) != 1) {
            printf("Invalid name input.\n");
            return 1;
        }

        printf("Enter marks in %d subjects: ", NUM_SUBJECTS);
        for (int subjectIndex = 0; subjectIndex < NUM_SUBJECTS; subjectIndex++) {
            if (scanf("%f", &studentList[studentIndex].marks[subjectIndex]) != 1 ||
                !isValidMark(studentList[studentIndex].marks[subjectIndex])) {
                printf(" Invalid mark. Must be between 0 and 100.\n");
                return 1;
            }
        }

        studentList[studentIndex].totalMarks = calculateTotalMarks(studentList[studentIndex].marks);
        studentList[studentIndex].averageMarks = calculateAverageMarks(studentList[studentIndex].totalMarks);
        studentList[studentIndex].grade = calculateStudentGrade(studentList[studentIndex].averageMarks);
    }

    printf("\n--------- Student Results ----------\n");

    for (int studentIndex = 0; studentIndex < totalStudents; studentIndex++) {
        printf("\nRoll: %d\n", studentList[studentIndex].rollNumber);
        printf("Name: %s\n", studentList[studentIndex].name);
        printf("Total: %.2f\n", studentList[studentIndex].totalMarks);
        printf("Average: %.2f\n", studentList[studentIndex].averageMarks);
        printf("Grade: %c\n", studentList[studentIndex].grade);

        if (studentList[studentIndex].averageMarks >= 35) {
            printf("Performance: ");
            displayPerformance(studentList[studentIndex].grade);
            printf("\n");
        }
    }

   
    printRollNumbers(studentList, 0, totalStudents);
    printf("\n");

    return 0;
}
