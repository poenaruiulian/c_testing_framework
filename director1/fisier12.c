// This program aims to calculate the sum of all the numbers between an interval,
// but only if the numbers are odd.

#include<stdio.h>
#include<stdlib.h>

int main(int argc, char **argv) {
    if (argc != 3) {
        return 1;
    }

    int lower_bound = atoi(argv[1]);
    int higher_bound = atoi(argv[2]);
    int sum = 0;

    for (int i = lower_bound; i <= higher_bound; i++) {
        if (i % 2 == 0) { // here it is an execution error because the condition refers to the paired number
            sum += i;
        }
    }

    printf("%d", sum);

    return 0;
}
