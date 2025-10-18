#include <stdio.h>
#include <stdlib.h>

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void rotateMatrix(int *matrix, int size) {
    if (size <= 0) return;
    for (int row = 0; row < size; row++) {
        for (int col = row + 1; col < size; col++) {
            swap(matrix + (row * size + col), matrix + (col * size + row));
        }
    }
    for (int row = 0; row < size; row++) {
        int *left = matrix + (row * size);
        int *right = matrix + (row * size + size - 1);
        while (left < right) {
            swap(left, right);
            left++;
            right--;
        }
    }
}

void applySmoothingFilter(int *matrix, int size) {
    int *temp = (int *)malloc(size * size * sizeof(int));
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            int sum = 0, count = 0;
            for (int dRow = -1; dRow <= 1; dRow++) {
                for (int dCol = -1; dCol <= 1; dCol++) {
                    int newRow = row + dRow;
                    int newCol = col + dCol;
                    if (newRow >= 0 && newRow < size && newCol >= 0 && newCol < size) {
                        sum += *(matrix + newRow * size + newCol);
                        count++;
                    }
                }
            }
            *(temp + row * size + col) = sum / count;
        }
    }
    for (int i = 0; i < size * size; i++) {
        matrix[i] = temp[i];
    }
    free(temp);
}

int main() {
    int size;
    printf("Enter the size of matrix: ");
    scanf("%d", &size);

    int *matrix = (int *)malloc(size * size * sizeof(int));
    int randomValue = 1;

    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            randomValue = (randomValue * 37 + 11) % 256;
            *(matrix + row * size + col) = randomValue;
        }
    }

    printf("Original:\n");
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            printf("%d\t", *(matrix + row * size + col));
        }
        printf("\n");
    }

    rotateMatrix(matrix, size);
    printf("Rotated:\n");
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            printf("%d\t", *(matrix + row * size + col));
        }
        printf("\n");
    }

    applySmoothingFilter(matrix, size);
    printf("Final Output:\n");
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            printf("%d\t", *(matrix + row * size + col));
        }
        printf("\n");
    }

    free(matrix);
    return 0;
}
