// This program aims to calculate the sum of all divisors of a given number.

#include<stdio.h>
#include<stdlib.h>

int main(int argc, char **argv) {

    if (argc != 2) {
        exit(1);
    }

    int number = atoi(argv[1]);
    int sum = 0;
    for (int i = 1; i <= number; i++) {
        if (number % i == 0) {
            sum += i;
        }
    }

    printf("%d", sum);

    exit(0);
}
