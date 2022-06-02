#include <stdio.h>
#include <stdlib.h>

int f(int N){
        if (N==0){
            return 2;
        }
        else{
            return 3*(N-1) + f(N-1) +1;
        }
    }



int main(int argc, char* argv[]){
    if(argc != 2){
        printf("Syntax: ./recurse N");
    }
    int N = atoi(argv[1]);
    int result = f(N);

    printf("%d\n", result);

    return EXIT_SUCCESS;
    
}