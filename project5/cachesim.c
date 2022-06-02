#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE* file_pointer;

struct frame{
    int tag;
    struct frame* next;
};

struct set{
    struct frame* first;
};

const int memory_size = 16777215;
unsigned char Main_Memory[memory_size];

int ones(int n){
    int mask = 1;
    mask = 1<<n;
    mask = mask - 1;
    return mask;
}

int log2(int n) {
    int r=0;
    while (n>>=1) r++;
    return r;
}

//-----------String Compare Function-----------------------
int string_equals(char* str1, char* str2){
    int i = 0;
    while(str1[i] != '\0' && str2[i] != '\0'){
        if (str1[i] != str2[i]) return 1;
        i++;
    }

    if (str1[i] == '\0' && str2[i] == '\0'){
        return 0;
    }
    return 1;
}
//---------------------------------------------------------

void move_to_front(struct frame* head, struct frame* current, struct frame* prev){
    (*prev).next = (*current).next;
    (*current).next = head;
    head = current;
    printf("moved to front!\n");
}

char store[8] = "store";
char load[8] = "load";
char operation[8];
int block_offset_trace = 0;
int index_trace = 0;
int tag_trace = 0; 
int hex_address_trace = 0;
int access_size_in_B = 0;
unsigned char byte[1];
unsigned char data_from_memory[10];
char hit_or_miss_STRING[10];
int hit_or_miss = 0;
int load_or_store = 0;
int hex_address_Decimal = 0;
struct frame* current = NULL;
struct frame* prev = NULL;
struct frame* temp = NULL;
int run = 0;

int main (int argc, char* argv[]){

    //----INITIALIZING MEMORY TO ZERO----

    for (int i=0; i<(2^24)-1; i++){
        Main_Memory[i] = 0;
    }
    //printf("Size of memory: %ld\n", sizeof(Main_Memory));

    //----INITIALIZING CACHE----

    int cache_size_in_B = atoi(argv[2])*1024;
    //sscanf(*argv[2], "%d", &cache_size_in_B);
    int associativity = atoi(argv[3]);
    int block_size = atoi(argv[4]);

    int number_of_frames = cache_size_in_B/block_size;
    int number_of_sets = number_of_frames/associativity;

    //printf("Frames: %d\n", number_of_frames);
    //printf("Sets: %d\n", number_of_sets);
    

    struct set cache[number_of_sets];

    for(int i = 0; i<number_of_sets; i++){
        cache[i].first = NULL;
    }

    //-----START READING FROM FILE STUFF------

    file_pointer = fopen(argv[1], "r");

    while(1){
        
        run++;
        int hit_or_miss = 0;
        //printf("\n");
        //printf("\n");
        //printf("Run: %d\n", run);
        //printf("%s", operation);
        int scan_result = fscanf(file_pointer, "%s", operation);
        //printf("%s", operation);
        if(scan_result == EOF){
            return 0;
        }

        load_or_store = string_equals(operation, load);
        //printf("Load = 0 and Store = 1: %d\n", load_or_store); 
        //printf("%s", operation);

        fscanf(file_pointer, "%x", &hex_address_trace);
        //printf("Address: %x\n", hex_address_trace);
        index_trace = (hex_address_trace/block_size) & ones(log2(number_of_sets));
        tag_trace = (hex_address_trace/block_size)/number_of_sets;
        block_offset_trace = hex_address_trace & ones(log2(block_size));
        //printf("Index: %x\n", index_trace);
        //printf("Tag: %x\n", tag_trace);
        //printf("Block Offset: %x\n", block_offset_trace);

        //-----CHECK IF HIT OR MISS--------
        
        if(cache[index_trace].first == NULL){
            hit_or_miss = 0;
            //printf("line 140\n");
        }
        else{
            current = cache[index_trace].first;
            //printf("first tag: %d\n", (*current).tag);
            if((*current).tag == tag_trace){ //it's a hit on the first node!!! --> just update variable to 1
                hit_or_miss = 1;
                //printf("line 147\n");
            }
        
            //If hit or miss != 1, let's check the rest of the set (linked list)
            
            if(hit_or_miss == 0){
                //printf("line 153\n");
                prev = current;
                current = (*current).next;
                while(current != NULL){
                    //printf("loop\n");
                    if((*current).tag == tag_trace){ //it's a hit on the current node!!! --> update variable to 1 and move to front of the set
                        hit_or_miss = 1;
                        (*prev).next = (*current).next;
                        (*current).next = cache[index_trace].first;
                        cache[index_trace].first = current;
                        //move_to_front(cache[index_trace].first, current, prev);
                        break; //break bc no need to search the list anymore, already found it
                    }
                    prev = current;
                    current = (*current).next;
                }

            }

        }
        fscanf(file_pointer, "%d", &access_size_in_B);
        //printf("Access Size: %d bytes\n", access_size_in_B);

        //If hit_or_miss == 1 here, then it's IN cache :)
        //If hit_or_miss == 0 here, then it's NOT IN cache :(
        
        if(hit_or_miss == 1 && load_or_store == 0){ //hit and it's a load --> we don't scan data to be stored and go look for bytes in memory
            //printf("I got here if!!!");
            for(int i = 0; i<access_size_in_B; i++){
                data_from_memory[i] = Main_Memory[hex_address_trace+i];
            }
        }
        
        if(hit_or_miss == 0 && load_or_store == 0){ //miss and it's a load --> we don't scan data to be stored; add to cache (put as first node and free last node) and go look for bytes in memory
            //add to beggining of set
            //printf("ADDING TO CACHE!!!\n");
            temp = (struct frame*) malloc(sizeof(struct frame));
            (*temp).tag = tag_trace;
            (*temp).next = cache[index_trace].first;
            cache[index_trace].first = temp;

            //loop through set to see if we need to evict
            current = cache[index_trace].first;
            prev = NULL;
            int ways_count = 0;

            while((*current).next != NULL){
                prev = current;
                current = (*current).next;
                ways_count++;
            }
            ways_count++;
        
           // printf("Filled: %d\n", ways_count);
            ///printf("Associativity: %d\n", associativity);

            //here if count > associativity then we should free(prev)
            if(ways_count > associativity){
                //printf("Evicting\n");
                (*prev).next = NULL;
                free(current);
            }

            //cache part is done, so just get it from memory
            for(int i = 0; i<access_size_in_B; i++){
                data_from_memory[i] = Main_Memory[hex_address_trace+i];
            }
        }
        
        ///printf("%s", operation);
        if(hit_or_miss == 1 && load_or_store == 1){ //hit and it's a store --> scan data to be stored and write it to memory
            for(int i = 0; i<access_size_in_B; i++){
                fscanf(file_pointer, "%2hhx", byte);
                //printf("%x ", *byte);
                Main_Memory[hex_address_trace+i] = *byte;
            }
            //printf("\n");
        }
        
        //printf("memory stuff::: %x\n", Main_Memory[hex_address_trace+1]);
        //printf("I got here!!! 202\n");
        //printf("%d\n", hit_or_miss);
        //printf("%d\n", load_or_store);

        
        if(hit_or_miss == 0 && load_or_store == 1){ //miss and it's a store --> write no-allocate = just write to memory
            for(int i = 0; i<access_size_in_B; i++){
                fscanf(file_pointer, "%2hhx", byte);
                //printf("%x ", *byte);
                Main_Memory[hex_address_trace+i] = *byte;
                //printf("%x", data_trace[i]);
                //printf("%x", Main_Memory[hex_address_trace+i]);
            }
        }
        
        if(hit_or_miss == 1){
            strcpy(hit_or_miss_STRING, "hit");
        }
        else{
            strcpy(hit_or_miss_STRING, "miss");
        }
        
        if(load_or_store == 0){
            //printf("%s\n", operation);
            printf("%s 0x%x %s ", operation, hex_address_trace, hit_or_miss_STRING);
            int j = 0;
            while(j<access_size_in_B){
                printf("%02hhx", *(data_from_memory+j));
                j++;
            }
            printf("\n");
            
        }
        else{
            printf("%s 0x%x %s\n", operation, hex_address_trace, hit_or_miss_STRING);
        }
        
        
           
    }
    

    return 0;

}