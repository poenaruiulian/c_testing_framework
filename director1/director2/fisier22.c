// This program aims to read n number of strings and sort them after the number of vowels.

#include<stdlib.h>
#include<stdio.h>
#include<string.h>

int countVowels(const char* str) {
    int count = 0;
    const char* vowels = "aeiouAEIOU";
    for (size_t i = 0; i < strlen(str); ++i) {
        if (strchr(vowels, str[i]) != NULL) {
            count++;
        }
    }
    return count;
}

int compareStrings(const char* a, const char* b) {
    return countVowels(a) - countVowels(b);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        return 1;
    }

    int numberOfStrings = atoi(argv[1]);

    if (argc != numberOfStrings+2){
        return 1;
    }

    char strings[100][101];
    for(int i=0;i<numberOfStrings;i++){
        strcpy(strings[i], argv[i+2]);
    }

    for(int i=0;i<numberOfStrings-1;i++){
        for(int j=i;j<numberOfStrings;j++){
            if(compareStrings(strings[i],strings[j])>0){
                char *aux = (char*)malloc(strlen(strings[i]));
                strcpy(aux,strings[i]);
                strcpy(strings[i],strings[j]);
                strcpy(strings[j],aux);
            }
        }
    }

    for(int i=0;i<numberOfStrings;i++){
        if(i == numberOfStrings-1){
            printf("%s",strings[i]);
        }else{
            printf("%s ",strings[i]);
        }
    }
}