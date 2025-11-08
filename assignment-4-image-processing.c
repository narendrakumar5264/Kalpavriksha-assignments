#include <stdio.h>
#include <stdlib.h>

void swapElements(int *firstElement, int *secondElement) {
    int tempValue = *firstElement;
    *firstElement = *secondElement;
    *secondElement = tempValue;
}

void rotateMatrix(int *matrixPtr, int matrixSize) {
    if (matrixSize <= 0) 
    {
        return; 
    }
    for (int row = 0; row < matrixSize; row++) {
        for (int col = row + 1; col < matrixSize; col++) {
            swapElements(matrixPtr + (row * matrixSize + col),
                         matrixPtr + (col * matrixSize + row));
        }
    }
    for (int row = 0; row < matrixSize; row++) {
        int *leftPtr = matrixPtr + (row * matrixSize);
        int *rightPtr = leftPtr + (matrixSize - 1);
        while (leftPtr < rightPtr) {
            swapElements(leftPtr, rightPtr);
            leftPtr++;
            rightPtr--;
        }
    }
}

void applySmoothingFilter(int *matrixPtr, int matrixSize) {
    int *rowBuffer = (int *)malloc(matrixSize * sizeof(int));
    
    for (int row = 0; row < matrixSize; row++) {
        int *matrixRowPtr = matrixPtr + (row * matrixSize);
        for (int col = 0; col < matrixSize; col++) {
            int sum = 0, neighborCount = 0;
            for (int dRow = -1; dRow <= 1; dRow++) {
                int newRow = row + dRow;
                if (newRow < 0 || newRow >= matrixSize)
                    continue;
                int *neighborRowPtr = matrixPtr + (newRow * matrixSize);
                for (int dCol = -1; dCol <= 1; dCol++) {
                    int newCol = col + dCol;
                    if (newCol < 0 || newCol >= matrixSize)
                        continue;
                    sum += *(neighborRowPtr + newCol);
                    neighborCount++;
                }
            }
            *(rowBuffer + col) = sum / neighborCount;
        }
        for (int col = 0; col < matrixSize; col++) {
            *(matrixRowPtr + col) = *(rowBuffer + col);
        }
    }
    free(rowBuffer);
}

int main() {
    int matrixSize;
    printf("Enter the size of matrix (2 to 50): ");
    if (scanf("%d", &matrixSize) != 1 || matrixSize < 2 || matrixSize > 50) {
        printf("Invalid matrix size! Must be between 2 and 50.\n");
        return 1;
    }

    int *matrixPtr = (int *)malloc(matrixSize * matrixSize * sizeof(int));
    if (!matrixPtr) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    int seedValue = 1;
    for (int row = 0; row < matrixSize; row++) {
        for (int col = 0; col < matrixSize; col++) {
            seedValue = (seedValue * 37 + 11) % 256;
            *(matrixPtr + row * matrixSize + col) = seedValue;
        }
    }

    printf("\nOriginal Matrix:\n");
    for (int row = 0; row < matrixSize; row++) {
        for (int col = 0; col < matrixSize; col++) {
            printf("%3d ", *(matrixPtr + row * matrixSize + col));
        }
        printf("\n");
    }

    rotateMatrix(matrixPtr, matrixSize);
    printf("\nRotated Matrix:\n");
    for (int row = 0; row < matrixSize; row++) {
        for (int col = 0; col < matrixSize; col++) {
            printf("%3d ", *(matrixPtr + row * matrixSize + col));
        }
        printf("\n");
    }

    applySmoothingFilter(matrixPtr, matrixSize);
    printf("\nSmoothed Matrix:\n");
    for (int row = 0; row < matrixSize; row++) {
        for (int col = 0; col < matrixSize; col++) {
            printf("%3d ", *(matrixPtr + row * matrixSize + col));
        }
        printf("\n");
    }

    free(matrixPtr);
    return 0;
}
