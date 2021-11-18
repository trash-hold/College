#include <stdio.h>
#include <malloc.h>
#include <assert.h>

typedef struct {int verse, column; float *ptr; } matrix;

matrix mat_construct(int verse, int column)
{
    float *ptr = calloc(verse * column, sizeof(float));
    assert(ptr); 
    return (matrix) {verse, column, ptr};
}

void mat_type(matrix x)
{
    float *ptr = x.ptr;
    for(int i=0; i < x.verse; i++, printf("\n"))
    {
        for(int j=0; j<x.column; j++)
        {
            printf("%4.2f \t", *ptr++);
        }
    }
    printf("\n");
}

//Normal saving and reading file
void save_file(matrix A, char* txt_name)
{
    FILE *file = fopen( txt_name ,"w+");
    fprintf(file, "verse %d column %d, ", A.verse, A.column);
    for(int i=0; i< A.column*A.verse; i++)
    {
        fprintf(file, "%2.2f, ", *(A.ptr+i)); 
    }
    fclose(file);

}

matrix read_file(char *txt_name)
{
    FILE *file = fopen(txt_name, "r");
    matrix A;
    fscanf(file, "verse %d column %d, ", &A.verse, &A.column); 
    A = mat_construct(A.verse, A.column);

    for (int i=0; i<A.verse*A.column; i++)
    {
        fscanf(file, "%f, " ,  A.ptr+i);
    }
    fclose(file);
    return A;
}


//Saving and reading file in binary form
void save_file_bin(matrix A, char* txt_name)
{
    FILE *f = fopen(txt_name, "w+");
    int B[]={A.column, A.verse};
    fwrite(B, sizeof(int), 2, f);
    fwrite(A.ptr, sizeof(float), A.column*A.verse, f);
    fclose(f);
}

matrix read_bin(char *txt)
{
    FILE* f = fopen(txt, "r");
    int t[2];
    fread(t, sizeof(int), 2, f);

    float *ptr = calloc(t[0]*t[1], sizeof(float));
    fread(ptr, sizeof(float), t[0]*t[1], f);
    return (matrix){*t, *(t+1), ptr};
    fclose(f);
}

void main()
{
    matrix A = mat_construct(4,4);
    for(int i=0; i<A.verse*A.column; i++)
    {
        *(A.ptr + i) = i;
    }

    char *matrix_txt = "matrix_txt";
    char *bin = "bin_txt";
    //save_file(A, matrix_txt);
    save_file_bin(A, bin);
    matrix C = read_bin(bin);

    //matrix B = read_file(matrix_txt);
    mat_type(C);
}