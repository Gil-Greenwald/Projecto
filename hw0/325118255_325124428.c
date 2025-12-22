#include <stdio.h>


void print_next_char(int target_base, unsigned long input_number) {
    int next_digit = input_number % target_base;
    char next_digit_char;

    if (input_number == 0)
        return;

    print_next_char(target_base, input_number / target_base);

    if (0 <= next_digit && next_digit <= 9)
        next_digit_char = (char)('0' + next_digit);
    else if (10 <= next_digit && next_digit <= 15)
        next_digit_char = (char)('a' - 10 + next_digit);
    else
        return;

    printf("%c", next_digit_char);
    
}

int main() {
    int source_base;
    int target_base;
    char chr;
    int digit;
    unsigned long input_number = 0;

    printf("enter the source base:\n");
    scanf("%d", &source_base);

    if (source_base < 2 || source_base > 16) {
        printf("Invalid source base!");
        return 0;
    }

    printf("enter the target base:\n");
    scanf("%d", &target_base);

    if (target_base < 2 || target_base > 16) {
        printf("Invalid target base!");
        return 0;
    }

    printf("enter a number in base %d:\n", source_base);
    getchar();

    while ((chr = getchar()) != '\n' && chr != EOF) {
        if ('0' <= chr && chr <= '9')
            digit = chr - '0';
        else if ('a' <= chr && chr <= 'f')
            digit = chr - 'a' + 1;
        else
            printf("Invalid input number!");

        if (digit < 0 || digit >= source_base) {
            printf("Invalid input number!");
            return 0;
        }

        input_number *= (unsigned long)source_base;
        input_number += (unsigned long)digit;
    }

    printf("the number in base %d is:\n", target_base);

    print_next_char(target_base, input_number);
    printf("\n");

    return 0;
}