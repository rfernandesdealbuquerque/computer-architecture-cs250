# include "stdio.h"
#include "stdlib.h"

void foo(){
    unsignded int a = 6;
    int b = -20;
    (a+b > 6) ? puts ("> 6") : puts ("<= 6");

}

int main(){
    foo();
    return 0;

}