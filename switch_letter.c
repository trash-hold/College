#include <stdio.h>

int len(char* x)
{
    int s=0;
    for (int i=0; *(x+i); i++) s++;
    return s;
}
void change(char* t, char* wanted, char character)
{
    int len_wanted = len(wanted);
    for(int i = 0; *(t+i); i++)
    {
        int j = 0;
        while(*(t+i+j) == *(wanted+j))
        {
            j++;
        }

        if (j==len_wanted)
        {
            for(int k=0; *(wanted+k); k++)
            {
                *(t+i+k) = character; 
            }
            i--; //for case when multiple wanted characters are next to each other
        }
    }
}

void main()
{
    char t[] = "kaaaaak";
    change(t, "a", '!');
    printf("%s", t);
    
}