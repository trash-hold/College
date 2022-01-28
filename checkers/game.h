#ifndef GAME
#define GAME

#include "def.h"
#include "usr_interface.h"
#include "mechanics.h"

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
//Function's declarations and their meaning
//Section 1 - for turning on game modes
void game_with_AI();
void game_PvP();
void game_AI();
//Section 2 - making a move
void player_move();
void AI_move(int depth);
//Section 3 - managing game status
void generate(Board *A);                //generates starting board
void change_turn(Board *A, move x);     //manges change of turns
int game_state(Board *A);               //returns status of game, >0 advantage in game; <0 loosing;

//Definitions:

void game_with_AI()
{
    int Game_state = 0;
    printf("You are white's and it's your turn! \n");
    while(Game_state != LOST && Game_state!= WON)
    {
        show_board(&A);
        if(A.turn == 1) player_move();
        else AI_move(settings.difficulty);
        Game_state = game_state(&A);
        (Game_state == WON || Game_state == LOST) ? 1 : printf("\n It's %s's move now! \n", A.turn ? "Player" : "AI");
    }
    A.turn = 1;
    Game_state = game_state(&A);
    printf("%s won! \n", Game_state == LOST ? "AI" : "Player");
}

void game_PvP()
{
    int Game_state = 0;
    printf("White's start!\n");
    while(Game_state != LOST && Game_state!= WON)
    {
        show_board(&A);
        if(A.turn == 1) player_move();
        else player_move();
        Game_state = game_state(&A);
        (Game_state == WON || Game_state == LOST) ? 1 : printf("\n It's %s's move now! \n", A.turn ? "White" : "Black");
    }
    A.turn = 1;
    Game_state = game_state(&A);
    printf("%s won! \n", Game_state == LOST ? "Black" : "White");
}

void game_AI()
{
    int Game_state = 0;
    printf("White's start!\n");
    make_move(&A, (legal_moves*) create_list(&A), (move) generate_move());
    while(Game_state != LOST && Game_state!= WON)
    {
        show_board(&A);
        if(A.turn == 1) AI_move(4);
        else AI_move(3);
        Game_state = game_state(&A);
        (Game_state == WON || Game_state == LOST) ? 1 : printf("\n It's %s's move now! \n", A.turn ? "White" : "Black");
    }
    A.turn = 1;
    Game_state = game_state(&A);
    printf("%s won! \n", Game_state == LOST ? "Black" : "White");
}

void player_move()
{
    legal_moves *head = create_list(&A);
    legal_moves *temp = head;
    char p1_move[5];
    fgets(p1_move, 6, stdin);
    while(p1_move[0] == '\n') fgets(p1_move, 6, stdin);
    if(is_call_for_help(p1_move)) show_list(head);
    else {move p1 = trans_move(p1_move); change_turn(&A, p1);}
}

void AI_move(int depth)
{
    legal_moves *list = create_list(&A);
    if(list)
    {
        move *best_move = NULL;
        int state, best = LOST;
        for(legal_moves *t = list; t; t=t->next)
        {
            //printf("Im at: %d, %d \n", t->m.to.x, t->m.to.y);
            Board child = A; 
            make_move(&child, t, list->m);
            state = -Negamax(&child, depth);

            if(state > best)
            {
                best = state;
                best_move = &t->m;
                //printf("Best move: %d, %d, %d \n", best_move->to.x, best_move->to.y, state);
            }
        }
        if(!best_move) 
            best_move = &list->m;
        
        change_turn(&A, *best_move);
        printf("Moved from: %c%c \t to: %c%c \n", (char) best_move->from.y + 'A', (char) best_move->from.x + '1', (char) best_move->to.y + 'A', (char) best_move->to.x + '1');
    }
    else printf("No legal moves \n");
}

void change_turn(Board *A, move x)
{
    legal_moves *head = create_list(A);
    int correct = is_correct(A, x);
        if(correct)
        {
            make_move(A, head, x);
            //Have to check if there was a capture and if there is another one avaible
            if(correct == 2)
            {
                if(!is_other_capture(A, x.to)) A->turn = !A->turn;
            }
            else A->turn = !A->turn;
        }
}

int game_state(Board *A)
{
    int sum[] = {0, 0};
    //sum[0] -> player1 score, sum[1] player2 score 
    field_type player[2];
    field_type opponent[2];
    if(A->turn) {player[0] = W; player[1] = DW; opponent[0] = B; opponent[1] = DB;}
    else {player[0] = B; player[1] = DB; opponent[0] = W; opponent[1] = DW;}
    for(int i = 0; i<N; i++)
    {
        for(int j = 0; j<N; j++)
        {
            if(A->field[i][j] == player[0]) sum[0]++;
            else if(A->field[i][j] == player[1]) sum[0]+=3;
            else if(A->field[i][j] == opponent[0]) sum[1]++;
            else if(A->field[i][j] == opponent[1]) sum[1]+=3;

        }
    }
    //ifs checking whether some player doesnt have any pawns left
    if(!sum[0]) return LOST;
    else if(!sum[1]) return WON;

    return sum[0]-sum[1];
}

void generate(Board *A)
{
    A->turn = 1;
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            if(i>=2 && i<N-3)
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

#endif