#include <stdio.h>
#include <math.h>

int len(char n[])
{
    int sum=0; 
    for (int i=0; n[i]; i++)
    {
        sum++;
    }
    return sum; 
}

void backwards(char n[])
{
    char c;
    for(char *end = &n[len(n)-1], *begin = n; begin < end; begin++, end--)
    {
        c = *begin;
        *begin = *end; 
        *(end) = c;
    }
}

void back(char *t)
{
    if(*(t+1)) back(t+1);
    printf("%c", *t);
} // second version


void main()
{   
    char n[] = "ABCD";
    backwards(n);
    for (char *i=n; *i; i++)
    {
        printf("%c", *i); 
    }
}
