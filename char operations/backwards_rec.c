#include <stdio.h>

int len(char *t)
{
    int n = 0;
    for (int i=0; t[i]; i++) n++;
    return n;
    
}

void backwards(char *t, int n)
{
    if(n>=0) 
    {
        printf("%c", *(t+n));
        backwards(t, n-1);
    }
}


void main()
{
    char t[] = "ABCDEFG";
    backwards(t, len(t));
}