#include <stdio.h>

#include <stdbool.h>


bool IsSpace(char c){
     if(c == ' ' || c == '\t' || c == '\n'){
        return true;
     }
     return false;
 }

bool IsDigit(char c){
    if(c >= '0' && c <= '9'){
        return true;
    }
    return false;
}



int main() {
    char exp[100];

    printf("Enter expression:");
    fgets(exp, sizeof(exp), stdin);

    int result = 0;       
    int current = 0;      
    int last = 0; 

    char op = '+';   

    for (int i = 0; exp[i] != '\0' && exp[i] != '\n'; i++) {
        char c = exp[i];
        // check for spaces

        if (IsSpace(c)) 
        {
            continue ;
        } 
    
        // check for digits

        if (IsDigit(c)) {
            current = 0;
            while (IsDigit(exp[i])) {
                current = current * 10 + (exp[i] - '0');
                i++;
            }
            i--; 

            if (op == '*') { 
                last = last * current;
            }
            else if (op == '/') {
                if (current == 0) {
                    printf("Error: Division by zero.\n");
                    return 0;
                }
                last = last / current;
            }
            else if (op == '+') {
                result += last; 
                last = current;
            }
            else if (op == '-') {
                result += last; 
                last = -current;
            }
        }
        // check for operators
        else if (c == '+' || c == '-' || c == '*' || c == '/') {
            op = c; 
        }
        // invalid expression
        else {
            printf("Error: Invalid expression.\n");
            return 0;
        }
    }

    result += last; 

    printf("%d\n", result);

    return 0;
}
