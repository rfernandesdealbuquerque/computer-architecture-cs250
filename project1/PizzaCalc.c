#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PI 3.14159265358979323846


struct pizza {
        char pizzaname[64];
        float pizzaperdollar;
        struct pizza* next;
    };
struct pizza* head = NULL;
struct pizza* tail = NULL;
struct pizza* trav = NULL;
struct pizza* pre = NULL;
FILE *file_pointer;
char done[10] = "DONE";

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

void print_list(struct pizza* node){
    //printf("Printing list:\n");
    while(node != NULL){
        printf("%s %f\n", (*node).pizzaname, (*node).pizzaperdollar);
        node = (*node).next;
    } 
}

void free_list(struct pizza* trav, struct pizza* pre){
    trav = (*head).next;
    pre = head;
    while(trav != NULL){
        free(pre);
        pre = trav;
        trav = (*trav).next;
    }
    free(pre);
}

// A program seg faults when it tries to read/write to a memory location that it shouldn't access

int main(int argc, char* argv[]){

    file_pointer = fopen(argv[1], "r");
    char string_scanned[64];
    int result_string_comparison;
    float diameter;
    float price;
    int success_or_fail;
    int runcount = 0;
    int scan_result;
    while (1){
    
        runcount++;
        //printf("Run: %d\n",runcount);
        scan_result = fscanf(file_pointer, "%s", string_scanned); //scan pizzaname of pizza
        if(scan_result == EOF){
            printf("PIZZA FILE IS EMPTY");
            return 0;
        }
        result_string_comparison = string_equals(done, string_scanned); //compare DONE with pizzaname to see if file has ended
        if(result_string_comparison == 0) {
            //printf("EXITING\n");
            break;
        }
        
        struct pizza* temp = (struct pizza*) malloc(sizeof(struct pizza));
        strcpy((*temp).pizzaname, string_scanned);
    
        fscanf(file_pointer, "%s", string_scanned); //scan DIAMETER of pizza
    
        success_or_fail = sscanf(string_scanned, "%f", &diameter);
        
        if(success_or_fail == 0){
            printf("Conversion string to diameter float failed\n");
            break;
        }
        fscanf(file_pointer,"%s", string_scanned); //scan price of pizza
        success_or_fail = sscanf(string_scanned, "%f", &price);

        if(success_or_fail == 0){
            printf("Conversion string to diameter float failed\n");
            break;
        }
        if(price == 0 || diameter == 0){
            (*temp).pizzaperdollar = 0;
            (*temp).next = NULL;
        }
        else{
            (*temp).pizzaperdollar = (PI*diameter*diameter/4)/price;
            (*temp).next = NULL;
        }

        //printf("%s\n",(*temp).pizzaname);
        //printf("%f\n",(*temp).pizzaperdollar);

//------------------------------BEGIN LINKED LIST STUFF-----------------------------------------------------------------
        int compare = 0;
        struct pizza* current = head;
        struct pizza* prev = NULL;
        
        if (head == NULL){
            //printf("beggining inserting head\n");
            head = temp;
            //printf("%s\n",(*head).pizzaname);
            continue;
        }

       //printf("%s\n",(*head).pizzaname);
        //printf("%s\n",(*current).pizzaname);

        if((*head).pizzaperdollar < (*temp).pizzaperdollar){
                //printf("beggining change head\n");
                (*temp).next = head;
                head = temp;
                continue;
        }

        if((*head).pizzaperdollar == (*temp).pizzaperdollar){
            compare = strcmp((*head).pizzaname, (*temp).pizzaname);
            if(compare > 0){
                (*temp).next = head;
                head = temp;
                continue;
            }
        }

        if((*current).next == NULL && (*current).pizzaperdollar > (*temp).pizzaperdollar){
                //printf("beggining insertion 2nd node\n");
                //printf("%s\n",(*current).pizzaname);
                prev = current;
                (*current).next = temp;
                current = (*current).next;
                tail = current;
                //printf("%s\n",(*current).pizzaname);
                continue;
        }
        prev = current;
        current = (*head).next;
        while(1){
            if(current == NULL){
                (*prev).next = temp;
                tail = temp;\
                break;
            }
            if((*current).pizzaperdollar < (*temp).pizzaperdollar){
                (*prev).next = temp;
                (*temp).next = current;
                break;
            }

            if((*current).pizzaperdollar == (*temp).pizzaperdollar){
                //printf("begin equals\n");
                //printf("%s %s\n", (*current).pizzaname, (*temp).pizzaname);
                compare = strcmp((*current).pizzaname, (*temp).pizzaname);
                if(compare > 0){
                    (*prev).next = temp;
                    (*temp).next = current;
                    break;
                }
                else{
                    prev = current;
                    current = (*current).next;
                    continue;
                }
                
            }
            else{
                prev = current;
                current = (*current).next;
            }
        }   
    
    }

    struct pizza* node = head;
    print_list(node);
    free_list(trav, pre);
    fclose(file_pointer);
    return 0;
    
}