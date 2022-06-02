#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, const char** argv){

    char string[50] = "ola";

    char letra_char = *(string + 2);
    int letra_int = string[2];

    printf("%p\n", string + 2);
    printf("%s\n", &letra_char);
    printf("%d\n", &letra_int);
    printf("%s\n", string + 2);

    return 0;
}