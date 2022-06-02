#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    char string[64];
    FILE *ptr = fopen("PizzaCalc_input_0.txt", "r");
    fscanf(ptr,"%s", string);
    printf("%s\n", string);
    return 0;

}