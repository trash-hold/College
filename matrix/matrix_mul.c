#include <stdio.h>
#include <assert.h>
#include <malloc.h>

typedef struct {int verse, column; float *ptr; } matrix;

matrix mat_construct(int verse, int column)
{
    float *ptr = calloc(verse * column, sizeof(float));
    assert(ptr); 
    return (matrix) {verse, column, ptr};
}

matrix mat_multiply(matrix A, matrix B)
{
    assert (A.column == B.verse);
    matrix res = mat_construct(A.verse, B.column);
    float *temp = res.ptr; 
    for(int ver=0; ver < A.verse; ver++)
    {
        for(int col = 0; col < B.column; col++)
        {
            for(int i=0; i< B.verse; i++)
            {
                *(res.ptr) += *(A.ptr + ver*A.column + i) * *(B.ptr + B.column*i + col);

            }
            res.ptr++;
        }
    }
    return (matrix) {A.verse, B.column, temp};
}


//showing elements of matrix
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

void main()
{
    matrix A = mat_construct(2,4), B = mat_construct(4,2);
    for(int i=0; i<8; i++)
    {
        *(A.ptr + i) = i;
        *(B.ptr + i) = i*2;
    }
    mat_type(A);
    mat_type(B); //example matrixes

    matrix C = mat_multiply(A, B);
    mat_type(C);
    
    free(A.ptr);
    free(B.ptr);
    free(C.ptr);
}