# ЗАВДАННЯ (7 ВАРІАНТ)

## Умова:

Напишіть програму на ANSI C, яка читає з stdin довільне ціле число — основу системи числення, потім ціле число у цій системі числення, а далі виводить його значення у десятковій системі. Ігноруйте всі недопустимі символи.

Приклад:
Вхід:   8 77   
Вихід:  63 (значення 77 у системі числення з основою 8)

Додайте обробку значень із плаваючою точкою для десяткової системи числення.

## Код задачі:

```c
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_INPUT 100

long convert_integer(const char *num_str, int base) {
    long result = 0;
    while (*num_str) {
        if (isdigit(*num_str)) {
            result = result * base + (*num_str - '0');
        } else if (*num_str >= 'A' && *num_str <= 'Z') {
            result = result * base + (*num_str - 'A' + 10);
        } else if (*num_str >= 'a' && *num_str <= 'z') {
            result = result * base + (*num_str - 'a' + 10);
        }
        num_str++;
    }
    return result;
}

double convert_float(const char *num_str, int base) {
    double result = 0.0;
    double fraction = 0.0;
    int fraction_part = 0;
    double base_power = 1.0;

    while (*num_str) {
        if (*num_str == '.') {
            fraction_part = 1;
            num_str++;
            continue;
        }

        int value = -1;
        if (isdigit(*num_str)) {
            value = *num_str - '0';
        } else if (*num_str >= 'A' && *num_str <= 'Z') {
            value = *num_str - 'A' + 10;
        } else if (*num_str >= 'a' && *num_str <= 'z') {
            value = *num_str - 'a' + 10;
        }

        if (value >= 0 && value < base) {
            if (fraction_part) {
                base_power /= base;
                fraction += value * base_power;
            } else {
                result = result * base + value;
            }
        }
        num_str++;
    }
    return result + fraction;
}

int main() {
    char input[MAX_INPUT];
    int base;
    char number[MAX_INPUT];

    printf("Введіть основу системи числення (2-36): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    if (sscanf(input, "%d", &base) != 1 || base < 2 || base > 36) {
        printf("Некоректна основа системи числення!\n");
        return 1;
    }

    printf("Введіть число у цій системі числення: ");
    if (fgets(number, sizeof(number), stdin) == NULL) {
        return 1;
    }

    number[strcspn(number, "\n")] = '\0';

    if (strchr(number, '.') != NULL) {
        double result = convert_float(number, base);
        printf("Десяткове представлення: %f\n", result);
    } else {
        long result = convert_integer(number, base);
        printf("Десяткове представлення: %ld\n", result);
    }

    return 0;
}
```

## Як працює програма

1. Користувач вводить ціле число (основу системи числення) від 2 до 36. Програма перевіряє, чи значення основи є коректним.

2. Користувач вводить число у вказаній системі числення. Програма обробляє вхідні дані, ігноруючи некоректні символи.

3. Якщо число є цілим (не містить символу `.`), використовується функція `convert_integer`, яка конвертує рядок у десяткове число. Якщо число має дробову частину (містить символ `.`), використовується функція `convert_float`, яка обробляє як цілу, так і дробову частини числа.

4. Програма виводить значення числа у десятковій системі числення.
