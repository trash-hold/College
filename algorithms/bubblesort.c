#include <stdio.h>
#define N 5

void bubble_sort(int *t, int n)
{
    for(int i=0; i<n; i++)
    {
        if(t[i]>t[i+1])
        {
            int temp = t[i+1];
            t[i+1] = t[i];
            t[i] = temp;
        }
    }
    n--; 
    n ? bubble_sort(t,n) : 1;
}

void main()
{
    int tab[] = {10, 2, 0, 8, 3};
    int n = N; 
    n--;
    bubble_sort(tab, n);

    for(int i=0; i<=n; i++)
    {
        printf("%d ", tab[i]);
    }
}