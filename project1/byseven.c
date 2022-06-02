#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    if(argc != 2){
        printf("Syntax: ./byseven N\n");    
        return 0;
    }
    
    int i;
    int N = atoi(argv[1]);
    
    for (i=1; i<N+1; i++){
        printf("%d\n", 7*i);
    }

    return EXIT_SUCCESS;
    
}