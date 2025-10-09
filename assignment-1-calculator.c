#include <stdio.h>
#include <stdbool.h>

bool isSpaceCharacter(char character) {
    return (character == ' ' || character == '\t' || character == '\n');
}

bool isDigitCharacter(char character) {
    return (character >= '0' && character <= '9');
}

int main() {
    char expression[100];
    printf("Enter expression: ");
    fgets(expression, sizeof(expression), stdin);

    int totalResult = 0;
    int currentNumber = 0;
    int previousNumber = 0;
    char currentOperator = '+';

    for (int index = 0; expression[index] != '\0' && expression[index] != '\n'; index++) {
        char currentChar = expression[index];

        if (isSpaceCharacter(currentChar)) {
            continue;
        }

        if (isDigitCharacter(currentChar)) {
            currentNumber = 0;
            while (isDigitCharacter(expression[index])) {
                currentNumber = currentNumber * 10 + (expression[index] - '0');
                index++;
            }
            index--;

            if (currentOperator == '*') {
                previousNumber *= currentNumber;
            } else if (currentOperator == '/') {
                if (currentNumber == 0) {
                    printf("Error: Division by zero.\n");
                    return 0;
                }
                previousNumber /= currentNumber;
            } else if (currentOperator == '+') {
                totalResult += previousNumber;
                previousNumber = currentNumber;
            } else if (currentOperator == '-') {
                totalResult += previousNumber;
                previousNumber = -currentNumber;
            }
        } else if (currentChar == '+' || currentChar == '-' || currentChar == '*' || currentChar == '/') {
            currentOperator = currentChar;
        } else {
            printf("Error: Invalid expression.\n");
            return 0;
        }
    }

    totalResult += previousNumber;
    printf("%d\n", totalResult);

    return 0;
}
