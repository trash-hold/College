#include <stdio.h>
#include <malloc.h>

typedef struct 
{
    int column, verse;
    char *t; 
}board;

board new_board(board A)
{
    char *temp = calloc(A.column*A.verse, sizeof(char));
    return (board){A.column, A.verse, temp};
}

int find(board A, int n)
{
    for(int verse = 0; verse < A.verse; verse++)
    {
        for(int column = 0; column < A.column; column++)
        {
            if(*(A.t + verse*A.verse + column)) check(A, A.t + verse*A.verse + column,n);
        }
    }
}

int check(board A, char *t, int left)
{
    int direction[0][2] = {{1,0}, {1,1}, {-1,1}};
}