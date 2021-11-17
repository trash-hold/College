#include <stdio.h>

char *earase(char *t, char x)
{
    for(int i=0; *(t+i); i++)
    {
        if(*(t+i)==x)
        {
            for(int j=i; *(t+j); j++)
            {
                if(*(t+j+1) !=x ) *(t+j) = *(t+j+1);
            }
            i--;
        }
    }
    return t;
}

void main()
{
    char n[]="Napaaaais";
    printf("%s \n", earase(n, 'a')); 
}