// This program aims to read two strings in an order and return them in the reverse order

#include<stdio.h>
#include<stdlib.h>

int main(int argc, char **argv) {
    if(argc!=3){
        exit(1);
    }

    printf("%s ", argv[2]);
    printf("%s", argv[1]);
}