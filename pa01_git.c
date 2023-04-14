#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//fnction prototype
int r_keyFile(int row, int **key_matrix, FILE *k);

int main(int argc, char *argv[]){

    FILE *k =  fopen(argv[1], "r");

    //checking if file opened correctly   
    if(k==NULL){
        printf("Woops! Error. Try again.\n");
    }

    FILE *p =  fopen(argv[2], "r");

    //checking if file opened correctly   
    if(p==NULL){
        printf("Woops! Error. Try again.\n");
    }
    
    int row=0;
    int col=0;
    int count=0;

    //scanning size of key 2d array
    fscanf(k, "%d", &row);
    col = row;
    
    int x=0;
    //using dma since matrix size is unknown
    int **k_matrix = (int**)malloc(row*sizeof(int*));

    for (x; x<row; x++) {
        k_matrix[x] = (int*)malloc(row*sizeof(int));
    }
    //assuring space is available in heap
    if(k_matrix==NULL){
        printf("Unable to allocate memory\n");
        exit(1);
    }

    //calling function to read key file
    r_keyFile(row, k_matrix, k);

    //printing 2d key matrix
    int r = 0;
    int c = 0;

    printf("\n");
    printf("Key matrix:\n");

    for(r; r<row; r++){
        for(c; c<row; c++){
            printf("%d ", k_matrix[r][c]);
        }
        //setting iterator back to 0
        c = 0;
        printf("\n");
    }
    fclose(k);

    int i=0;
    int size_f;

    //moving file pointer & scanning size
    fseek(p, 0, SEEK_END);
    size_f = ftell(p);

    //setting pointer back
    rewind(p);

    //allocating space for original str & checking
    char *o_pt = (char*)malloc(size_f*sizeof(char));
    if(o_pt==NULL){
        printf("Unable to allocate memory\n");
        exit(1);
    }

    //read file
    fread(o_pt, sizeof(char), size_f, p);

    //closing file
    fclose(p);

    //allocating space for cleared str
    char *c_pt = (char*)malloc(size_f*sizeof(char));
    
    //assuring space is available in heap
    if(c_pt==NULL){
        printf("Unable to allocate memory\n");
        exit(1);
    }

    //iterators
    int h;
    int m=0;

    //removing all non-alpha
    for(h=0; o_pt[h]; h++){

        if(isalpha(o_pt[h])){
            //and storing in cleared str
            c_pt[m] = tolower(o_pt[h]);
            m++;
        }
        else{
            continue;
        }
    }
    c_pt[m] = '\0'; /*new end of str*/
    
    //iterators
    int length_k = row;
    int length_p = m;
    int e;

    //if ptext not divisible by kmatrix size
    if((length_p%length_k) != 0){

        int add_x = (length_k - (length_p % row));
        length_p += add_x;

        //allocting correct space now that str cleared
        c_pt = realloc(c_pt, length_p*sizeof(char));
        for(e=length_p-add_x; e<length_p; e++)
        {
            //padding x
            c_pt[e] = 'x';
        }
        //now set new end of str after pad
        c_pt[length_p] = '\0';
    }

    //will be used later
    int value_blocks = (length_p +1)/row;
    int value_total = value_blocks*row;

    //final plaintext in stack memory since we know stdout len
    char final_plaintext[length_p + e + 1];
    strncpy(final_plaintext, c_pt, length_p+1);

    int len = length_p+e;

    //display plaintext after pad
    printf("\n");
    
    printf("Plaintext:\n");
    for(int w=0; w<value_total; w++){
        printf("%c", final_plaintext[w]);
        count++;
        if(count == 80){
            printf("\n");
            count = 0;
        }
    }

    //convert char plaintext to int plaintext

    //allocating space to avoid waste of memory or data leaks
    int *plainToInt = (int*)malloc(len * sizeof(int));

    //assuring space is available in heap
    if(plainToInt==NULL){
        printf("Unable to allocate memory\n");
        exit(1);
    }

    int s=0;

    //convert to int
    for(s; s<len; s++)
    {
       plainToInt[s] = final_plaintext[s] - 97;
    }
    printf("\n");
    printf("\n");

    //result str/converting to letters
    char result[10000];

    int f=0;

    while (f < value_total) 
    {
       for (int A=0; A<row; A++){
            int c_ounter = 0;
            int g = f;

            for (int B=0; B<row; B++){
                c_ounter += (k_matrix[A][B]%26 * plainToInt[g++]%26) % 26;
                c_ounter=c_ounter % 26;
            }
            result[f+A] = (c_ounter+97);
        }
        f += row;
    }
    result[f] = '\0';

    //resetting counter for 80 char limit stdout
    count =0;

    printf("Ciphertext:\n");
    for(int H=0; H<value_total; H++){
        printf("%c", result[H]);
        count++;
        //setting 80 char limit
        if(count == 80){
            printf("\n");
            count = 0;
        }
    }
    printf("\n");

    //freeing dmallocated memory
    free(plainToInt);
    free(c_pt);
    free(o_pt);
    free(k_matrix);

}

int r_keyFile(int row, int **key_matrix, FILE *k){
    //iterators
    int x, z;

    //storing 2d key array in key matrix
    for(x=0; x<row; x++){
        for(z=0; z<row; z++){
            fscanf(k, "%d", &key_matrix[x][z]);         
        }
    }
    return 0;
}