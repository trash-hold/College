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

void main()
{
    matrix A = mat_construct(4,4);
    for(int i=0; i<A.verse*A.column; i++)
    {
        *(A.ptr + i) = i;
    }

    char *matrix_txt = "Matrix_txt";
    //save_file(A, matrix_txt);

    matrix B = read_file(matrix_txt);
    mat_type(B);
}
