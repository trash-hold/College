#include <stdio.h>
#include <malloc.h>
#include <assert.h>


typedef struct 
{
    int num;
    void* next;
    void* prev; 
}element;

element *new(element * head, int num)
{
    element *last = head;
    element *beg = NULL; 
    if(head)
    {
        while(last->next)
        {
            if(!beg) beg=head; 
            else beg=beg->next;
            last = last->next;
        }
    }
    
    element* n = calloc(1, sizeof(element));
    n->num = num;
    n->next = NULL;
    n->prev = last;

    if(last)
        {
            last->prev = beg;
            last->next = n;
        }

    return (head ? head : n); //head can be NULL
}

element *delete_num(element* head, int num)
{
    if(num==1)
    {
        element *temp = head;
        if(head->next) head = head->next;
        else 
        {
            printf("Brak elementow do usuniecia \n");
            return NULL;
        }
        return head;
    }
    else
    {
        element *beg = head;
        element *end = head->next;
        for(int i =1; i<num-1; i++)
        {
            assert(end);
            beg = beg->next;
            end = end->next;
        }
        beg->next = end->next;
        if(end->next) end = end->next;
        end->prev = beg;
    }
    return head;
}

void read(element *head)
{
    for(element* i = head, *j = head; i; i=i->next, (i ? j=i->prev : 0))
    {
        printf("now: %d prev: %d \n", i->num, j ? j->num : 0);
    }
}

void main()
{
    element* head = new(NULL, 1); //pierwszy *element 
    head = new(head, 2);
    head = new(head, 3);
    head = new(head, 4);
    head = new(head, 5);
    head = new(head, 6);
    head = delete_num(head, 4);
    read(head);

    free(head);

}
