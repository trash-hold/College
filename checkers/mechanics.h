#ifndef MECH
#define MECH

#include "def.h"
#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
//#include <time.h>
#include <assert.h>
//Function's declarations and their meaning
//======================================LISTS==================================
//Section 1a - creating list
legal_moves *create_list(Board *A);
legal_moves *new_element(legal_moves* head, move a, int cap);
//Section 1b - adding moves to the list
legal_moves *add_move(Board *A, legal_moves *head, field n);
legal_moves *add_move_adv(Board *A, legal_moves *head, field n, field dir, int i);      //adds queen's moves
int is_overflow(field x, field added);                                                  //for checking if index is over the border limit
//Section 1c - freeing space 
void free_list(legal_moves *head);
//======================================MOVES==================================
//Section 2a - main function
void make_move(Board *A, legal_moves *head, move x);
//Section 2b - helping functions
legal_moves *search(Board *A, legal_moves *head, move x);       //searches for move x
field captured(move x);                                         //finds (x,y) of captured pawn 
move generate_move();                                           //generates one random move
int is_correct(Board *A, move x);                               //checks if move x is correct and if there was a capture
int is_other_capture(Board *A, field x);                        //checks if there is a capture from field x
//======================================NEGAMAX==================================
int negamax(Board *A, int depth, int alfa, int beta);
int Negamax(Board *A, int depth);


//Definitions:

legal_moves *create_list(Board *A)
{
    legal_moves *head = (legal_moves*) NULL;
    field_type player[2];
    if(A->turn) {player[0] = W; player[1] = DW;}
    else {player[0] = B; player[1] = DB;}

    //first searching for fields from where there can be possible move
    for(int i = 0; i<N; i++)
        for(int j = 0; j<N; j++)
        {
            if(A->field[i][j] == player[0])
            {
                head = add_move(A, head, (field){i,j});
            }
            //different rules for queens hence the second if
            else if(A->field[i][j] == player[1])
            {
                field dir[] = {(field){1,1}, (field){1,-1}, (field){-1,1}, (field){-1,-1}};
                for(int n = 0; n<4 ; n++)
                {
                    head = add_move_adv(A, head, (field){i, j}, dir[n], 1); 
                }
            }
        }
    //Two additional lists, because if there are possible captures its neccessary to capture. Capture_adv is for situation when you can capture a queen
    legal_moves *captures = NULL;
    legal_moves *captures_adv = NULL; 
    legal_moves *temp = head; 
    if(head)
    {
        while(temp)
        {
            if(temp->capture == 2) {captures_adv = new_element(captures_adv, temp->m, 2);}
            else if(temp->capture == 1) {captures = new_element(captures, temp->m, 1);}
            temp = temp -> next;
        }
        if(captures_adv) return captures_adv;
        else if(captures) return captures;
    }
    return head;
}

legal_moves *new_element(legal_moves* head, move a, int cap)
{
    legal_moves *ptr = (legal_moves*) calloc(1, sizeof(legal_moves));
    ptr->next = NULL;
    ptr->m = a;
    ptr->capture = cap;
    
    legal_moves *t = head;
    if(head)
    {
        while(t->next)
        {
            t = t -> next;
        }
        t->next = ptr;
    }
    return head ? head : ptr;
}

legal_moves *add_move(Board *A, legal_moves *head, field n)
{
    //First setting who the player and opponent is to know which directions are legal to move for clear fields
    field_type player[2];
    field_type opponent[2];
    field dir[2];

    if(A->turn) {player[0] = W; player[1] = DW; opponent[0] = B; opponent[1] = DB; dir[0] = (field) {-1, -1}; dir[1] = (field) {-1, 1};}
    else {player[0] = B; player[1] = DB; opponent[0] = W; opponent[1] = DW; dir[0] = (field){1, 1}; dir[1] = (field) {1, -1};}

    //For simpler code form function will run two loops: 1) for regular moves 2) for captures
    for(int i=0; i<2; i++)
    {
        if(is_overflow(n, dir[i]))
            if(A->field[n.x + dir[i].x][n.y + dir[i].y] == C)
                head = new_element(head, (move){n, (field){n.x + dir[i].x, n.y + dir[i].y}}, 0);
    }
    for(int j=0; j<2; j++)
    {
        //After first two iterations change direction to check all adjecent fields
        if(j) { dir[0] = (field) {-dir[0].x, -dir[0].y};  dir[1] = (field)  {-dir[1].x , -dir[1].y};} 
        for(int i = 0; i<2; i++)
        {
            if(is_overflow(n, dir[i]) && is_overflow(n, (field) {2*dir[i].x, 2*dir[i].y}))
                if(A->field[n.x + 2*dir[i].x][n.y + 2*dir[i].y] == C)
                {
                    //First if for capture of pawn, second for the queen
                    if(A->field[n.x + dir[i].x][n.y + dir[i].y] == opponent[0])
                        head = new_element(head, (move){n, (field){n.x + 2*dir[i].x, n.y + 2*dir[i].y}}, 1);
                    else if(A->field[n.x + dir[i].x][n.y + dir[i].y] == opponent[1])
                        head = new_element(head, (move){n, (field){n.x + 2*dir[i].x, n.y + 2*dir[i].y}}, 2);
                } 
        }          
    }
    return head;
}

legal_moves *add_move_adv(Board *A, legal_moves *head, field n, field dir, int i)
{
    field_type opponent[2];
    if(A->turn) {opponent[0] = B; opponent[1] = DB;}
    else {opponent[0] = W; opponent[1] = DW;}

    if(n.x + i*dir.x >=0 && n.x + i*dir.x < N && n.y + i*dir.y >= 0 && n.y + i*dir.y < N)
    {
        if(A->field[n.x + i*dir.x][n.y + i*dir.y] == C) {head = new_element(add_move_adv(A, head, n, dir, i+1), (move){n, (field){n.x+ i*dir.x, n.y + i*dir.y}}, 0); return head;}
        else if(n.x + (i+1)*dir.x >=0 && n.x + (i+1)*dir.x < N && n.y + (i+1)*dir.y >= 0 && n.y + (i+1)*dir.y < N)
        {
            if(A->field[n.x + (i+1)*dir.x][n.y + (i+1)*dir.y] == C)
            {
                if(A->field[n.x + i*dir.x][n.y + i*dir.y] == opponent[0]) {head = new_element(head, (move){n, (field){n.x + (i+1)*dir.x, n.y + (i+1)*dir.y}}, 1); return head;}
                else if(A->field[n.x + i*dir.x][n.y + i*dir.y] == opponent[1]) {head = new_element(head, (move){n, (field){n.x + (i+1)*dir.x, n.y + (i+1)*dir.y}}, 2); return head;}
            }
        }
    }
    return head; 
}

int is_overflow(field x, field added)
{
    return (x.x + added.x >= 0 && x.x + added.x < N && x.y + added.y >= 0 && x.y + added.y < N) ? 1 : 0;
}

void free_list(legal_moves *head)
{
    while(head)
    {
        legal_moves *tmp = head;
        head = head->next;
        free(tmp);
    }
}

void make_move(Board *A, legal_moves *head, move x)
{
    legal_moves *t = search(A, head, x);
    //checking if the move is on the list of legal moves if it is the *t isnt NULL
    if(t)
    {
       if(t->capture>0)
       {
            field cap = captured(x); 
            A->field[x.to.x][x.to.y] = A->field[x.from.x][x.from.y];
            A->field[x.from.x][x.from.y] = C;
            A->field[cap.x][cap.y] = C;
       }
       else {A->field[x.to.x][x.to.y] = A->field[x.from.x][x.from.y]; A->field[x.from.x][x.from.y] = C; }
    }
    if(A->turn && x.to.x == 0 || !A->turn && x.to.x == 7)
            {
                if(A->turn) A->field[x.to.x][x.to.y] = DW;
                else  A->field[x.to.x][x.to.y] = DB;
            }
}

legal_moves *search(Board *A, legal_moves *head, move x)
{
    while(head)
    {
        if( head->m.from.x == x.from.x && head->m.from.y == x.from.y && head->m.to.x == x.to.x && head->m.to.y == x.to.y) return head;
        head = head->next;
    }
    //if move is found function returns address of the element 
    return NULL;
}

field captured(move x)
{
    field dir[] = {{1,1}, {1,-1}, {-1,-1}, {-1 , 1}};
    if(x.to.x - x.from.x > 0)
    {
        if(x.to.y - x.from.y > 0) return (field){x.to.x - 1, x.to.y - 1};
        else return (field){x.to.x - 1, x.to.y + 1};
    }
    else
    {
        if(x.to.y - x.from.y > 0) return (field){x.to.x + 1, x.to.y - 1};
        else return (field){x.to.x + 1, x.to.y + 1};
    }
}

move generate_move()
{
    legal_moves *head = create_list(&A);
    legal_moves *temp = head;
    int elements=0; 
    while (temp)
    {
        elements++;
        temp = temp->next;
    }
    if(elements)
    {
        srand(time(0));
        int rand_num = rand()%elements;
        while(rand_num)
        {
            head = head->next;
            rand_num--;
        }
        return (move) head->m;

    }
    //if there aren't any possible moves to pick from returns impossible move
    return (move){(field) {-1,-1}, (field) {-1,-1}};
}

int is_correct(Board *A, move x)
{
    //if move is correct returns !0, additonally returns information whether the move is regular or with capture
    legal_moves *l = create_list(A);
    legal_moves *temp = search(A, l, x);

    if(temp) return temp->capture ? 2 : 1;
    else {printf("Incorrect move! \n"); return 0;}
}

int is_other_capture(Board *A, field x)
{
    legal_moves *head = create_list(A);
    if(!head->capture) return 0;
    while(head)
    {
        if( head->m.from.x == x.x && head->m.from.y == x.y) return 1;
        head = head->next;
    }
    return 0;
}

int negamax(Board *A, int depth, int alfa, int beta)
{
    if(!depth)
        return game_state(A);
    legal_moves *list = create_list(A);
    if(!list)
        return game_state(A);
    legal_moves *ptr = list; 
    Board *child = A;

    int state = LOST; 
    int state_temp, is_better;

    do
    {
        make_move(child, list, list->m);
        state_temp = negamax(child, depth -1, -beta, -alfa);
        if(state_temp > state)
        {
            state = state_temp;
            if(state > alfa)
                alfa = state;
        }
        if(alfa >= beta)
        {
            state = beta;
            break;
        }
        list = list->next;

    } while (list);

    return state;
     
}

int Negamax(Board *A, int depth)
{
    return negamax(A, depth, LOST, WON);
}
#endif