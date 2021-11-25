#ifndef TOOLS
#define TOOLS

#include "def.h"
#include <stdio.h>
#include <malloc.h>
#include <assert.h>

void generate(Board *A)
{
    A->turn = 1;
    A->capture = 0;
    //A->end = 0; 
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            if(i>2 && i<N-3)
            {
                A->field[i][j] = C;
            }
            else
            {
               if((j+1)%2)
               {
                   if((i+1)%2)
                   {
                       A->field[i][j] = W;
                       A->field[N-i-1][N-j-1] = B;
                   }
                   else
                   {
                        A->field[i][j] = C;
                        A->field[N-i-1][N-j-1] = C;
                   }
               }
               else
               {
                   if(!((i+1)%2))
                   {
                       A->field[i][j] = W;
                       A->field[N-i-1][N-j-1] = B;
                   }
                   else
                   {
                        A->field[i][j] = C;
                        A->field[N-i-1][N-j-1] = C;
                   }
               }
            }
        }
    }
}
void show_board(Board *A)
{
    printf("   A   B   C   D   E   F   G   H ");
    printf("\n ---------------------------------\n |");
    for(int i=0; i<N; i++, printf("\n ---------------------------------\n |"))
    {
        //printf("\n---------------------------------\n |");
        for(int j = 0; j<N; j++)
        {
            switch(A->field[i][j])
            {
                case C:
                    printf(" %c |", '.');
                    break;
                case W:
                    printf(" %c |", 'B');
                    break;
                case B:
                    printf(" %c |", 'W');
                    break;
                default:
                    printf(" %d |", 1);
                    break;
            }
    
        }
        printf(" %d", i+1);
    }
}

field index(char a, char b)
{
    return (field) {(int) a - 65, (int) b - 1};
}

void move_fun(Board *A, move *m)
{
    if(m->to.x >= 0 && m->to.x<N && m->to.y >= 0 && m->to.x<N) {printf("Incorrect move \n"); assert(0);}
    else if( (A->field[m->from.x][m->from.y] == W && A->turn == 1) || (A->field[m->from.x][m->from.y] == B && A->turn == 0) )
    {
        if (A->field[m->to.x][m->to.y]!= C){printf("Incorrect move \n"); assert(0);}
    }
    else {printf("Incorrect move \n"); assert(0);}


}

#endif