/*
 Deepa Marti, Fall 2019, ESC 36A
Written 12/6/19

Program to take in an input file with element names, symbols, and weights and compare to the input of a compound and prints its atomic weight.
 
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/* global variable*/
char temp_symbol[3];        /* element symbol when reading the file*/
char temp_name[100];        /* element name when reading the file*/
double temp_weight;         /* element atomic weight when reading the file*/
double atomic_weight;       /* variable for total atomic weight*/
char molecule[32];          /* variable for the user input*/
char curr_element[3];       /* array for current element from the input*/
char curr_digit[30];        /* array for digits from the input */
int el_count;               /* counter for number of elements in curr_element */
int digit_count;            /* ounter for number of elements in curr_digit */
int multiply;
double total;


/*struct definition*/
struct element{
    char symbol[3];
    double weight;
    char* name;
};

typedef struct element el;
/* declaring an array of structures */

el *arr[120];

int nelements = 0; /*number of elements in the file*/

/*function to find the weight given the element using the struct*/
double findweight(char curr_element[3]){
    int i;
    int x;
    
    /* if the curr_element is a null bit then just return 0*/
    if(strcmp(curr_element, "\0") == 0){
        return 0;
    }
    
    /* go through the elements and find a match between the curr_element and the array of structs*/
       for(i = 0; i < nelements; i++){
           if(strcmp(curr_element, arr[i]->symbol) == 0){
               return arr[i]->weight;
           }
       }

    /* will only come to this part if there were no matches, so it will then print out the error messages*/
    total = 0;
    fprintf(stderr, "%s: no such element\n", curr_element);
    fprintf(stderr, "%s: not a valid compound\n", molecule);
    return 0;
}
/*function to find the multiplyer given the position in the input array */
int dig(int i){

    int j;
    multiply = 1;  /* multiply is 1 at first because if there are no digits found then the weight should be multipied by 1 */
    
        for(j = i; j < i+2; j++){
            /* statement executes until the '/0'*/
            if(isdigit(molecule[j])){
                /* using a while loop to make sure it takes in numbers longer than one digit*/
                while(isdigit(molecule[j])){
                    curr_digit[digit_count++]= molecule[j++];
                    /* copying the molecule value of the digit into curr_digit*/
                }
                
            /* ending the array and setting the digit counter back to 0*/
            curr_digit[digit_count] = '\0';
            digit_count = 0;
                
            j--; /* decrementing the counter*/
            multiply = atoi(curr_digit); /* converting string to an integer*/
            
            return multiply;
            }
            
        }
        /*if it didn't find any digits then just return 1 to be the multiplyer*/
        return 1;
    }
/* function look at each individual element in the compound seperately*/
double compound(){
    int i;
    int el_count;
    total = 0;
    el_count = 0;
    
    /* looping through the molecule */
    for(i = 0; i < strlen(molecule); i++){
        /* if the character in the molecule is uppercase add it to the curr_element*/
        if(isupper(molecule[i])){
            curr_element[el_count++] = molecule[i];
            if(islower(molecule[i+1])){
                /* check if the next character is lower (meaning it is still the same element), if it is then add it to curr_element*/
                curr_element[el_count++] = molecule[++i];
            }
        }
        /*end the element by adidng the '\0' and setting element count to 0*/
        curr_element[el_count] = '\0';
        el_count = 0;
        /*call the dig and findweight functions to get the total*/
        total = total + dig(i)*findweight(curr_element);
        
        /* if the total didn't change (and is still 0), just return 0*/
        if(total == 0){
            return total;
        }
        /* reset multiply to 1*/
        multiply = 1;
    }
     /* reset multiply to 1*/
    multiply = 1;
    /*end the element by adidng the '\0'*/
    curr_element[el_count] = '\0';
    
    return total;
}


int main(int argc, char *argv[])
{
    /* be sure there is a file to load! */
    if (argc == 1){
        fprintf(stderr, "Usage: %s atomic_weights [ ... ]\n", argv[0]);
        return(1);
    }
    
    load(argv[1]);
    
        fprintf(stdout,"Chemical composition? ");
        while(fgets(molecule, 32, stdin)!= NULL){
            molecule[strlen(molecule)-1] = '\0';
                    atomic_weight = compound();
            if(total != 0){
                fprintf(stdout,"The atomic weight of %s is %.02f\n", molecule, atomic_weight);
                
            }
                fprintf(stdout,"Chemical composition? ");
         }
        printf("\n");

    return 0;
}

int load(const char *filename){
    FILE *fp;   /*input file pointer*/
    int lineno; /*current line number in the input file also the number of elements*/
    int n;      /*number of inputs scanf read*/
    el *tmp;        /* points to space for new element */
    int x;
    /* open the file*/
    if ((fp = fopen(filename, "r")) == NULL){
        perror(filename);
        exit(1);
    }
    /*read in the file*/
    while((n = fscanf(fp, "%lf %s %s\n", &temp_weight, temp_symbol, temp_name)) == 3){
        /* allocate space for the structure */
        if ((tmp = malloc(sizeof(el))) == NULL){
            perror("malloc 1 failed!");
            exit(1);
        }
        
        /* now, allocate space for the name */
        
        if ((tmp->name = malloc(strlen(temp_name) * sizeof(char))) == NULL){
            perror("malloc 2 failed!");
            exit(1);
        }
        
        /* put the data into the structure */
        (void)strcpy(tmp->name, temp_name);
        (void)strcpy(tmp->symbol, temp_symbol);
        tmp->weight = temp_weight;
        
        /* and now save the structure in the array */
        arr[nelements++] = tmp;
        /* go to the next line */
        lineno++;
        
    }
    /*
     * see why you dropped out
     */
    if (n != EOF){
        fprintf(stderr, "%s: malformed line %d\n", filename, lineno);
        return(0);
    }
    /*
     * now close the file
     */
    (void)fclose(fp);
    
    return(1);
}
