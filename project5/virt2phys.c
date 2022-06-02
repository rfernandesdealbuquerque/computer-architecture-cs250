#include <stdio.h>
#include <stdlib.h>

int countBits(int n){
   int bits = 0;
   while (n)
   {
        bits++;
        n >>= 1;
    }
    return bits;
}

int ones(int n){
    int mask = 1;
    mask = 1<<n;
    mask = mask - 1;
    return mask;
}

FILE *file_pointer;

int main (int argc, char* argv[]){

    file_pointer = fopen(argv[1], "r");
    int address_bits;
    int page_size;
    int ppn;
    int memory_size = 1;
    int number_of_pages = 0;
    int address;
    int page_number;
    int loop_count = 0;
    int bits = 0;
    int phys_address;

    fscanf(file_pointer, "%d", &address_bits);
    fscanf(file_pointer, "%d", &page_size);

    //printf("Word Size: %d\n", address_bits);
    //printf("Page Size: %d\n", page_size);
    //printf("%d\n", ppn);

    memory_size = 1<<address_bits;
    number_of_pages = memory_size/page_size;

    //printf("Size: %d\n", memory_size);
    //printf("Number of pages: %d\n", number_of_pages);
    sscanf(argv[2], "%x", &address);
    //printf("Address in hex: %x\n", address);
    bits = countBits(page_size-1);
    //printf("Bits: %d\n", bits);
    page_number = address >> bits;
    //printf("Page Number: %d\n", page_number);

    while(1){
        fscanf(file_pointer, "%d", &ppn);
        if (loop_count == page_number){
            break;
        }
        loop_count++;
    }

    if (ppn == -1){
        printf("PAGE FAULT\n");
    }
    else{
        ppn = ppn << bits;
        //printf("%x\n", address);
        address = address & ones(bits);
        //printf("%x\n", address);
        phys_address = ppn + address;
        printf("%x\n", phys_address);
    }


    return 0;
}