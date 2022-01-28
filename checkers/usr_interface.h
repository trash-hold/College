#ifndef INTER
#define INTER

#include "def.h"
#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <assert.h>

//Function's declarations and their meaning
//Section 1 - used to interact with user
void show_board(Board *A);                  //displays board
void show_list(legal_moves *head);          //displays all possible moves
void menu(int i);                           //general function for displaying menu messages
game_settings set_up();                     //uses user input to create settings of the game

//Section 2 - used to translate input
void lowercase(char t[]);                   //normalises input for easier managment 
int is_call_for_help(char t[]);             //checks if special help command is used
field trans_index(char a, char b);
move trans_move(char t[]);

//Definitions:
void show_board(Board *A)
{
    printf("\n   A   B   C   D   E   F   G   H ");
    printf("\n ---------------------------------\n");
    for(int i=0; i<N; i++, printf("\n ---------------------------------\n"))
    {
        printf(" |");
        for(int j = 0; j<N; j++)
        {
            switch(A->field[i][j])
            {
                case C:
                    printf(" %c |", '.');
                    break;
                case W:
                    printf(" %c |", 'w');
                    break;
                case B:
                    printf(" %c |", 'b');
                    break;
                case DW:
                    printf(" %c |", 'W');
                    break;
                default:
                    printf(" %c |", 'B');
                    break;
            }
    
        }
        printf(" %d", i+1);
    }
}

void show_list(legal_moves *head)
{
    legal_moves *list = head;
    printf("List of possible moves: \n");
    while(list)
    {
        //format for devs:
        //printf("From: %d %d \t to: %d, %d \n", list->m.from.x, list->m.from.y, list->m.to.x, list->m.to.y);

        //format for users:
        printf("From: %c%c \t To: %c%c \n", (char) list->m.from.y + 'A', (char) list->m.from.x + '1', (char) list->m.to.y + 'A', (char) list->m.to.x + '1');
        list = list -> next;
    }
}

void menu(int i)
{
    switch(i)
    {
        case 0:
            printf("\n Choose one of the options by typing: \n 1. Play \n 2. Tutorial and rules \n 3. Quit \n ");
            break;
        case 1:
            printf("\n Choose the game mode: \n 1. Player vs Player \n 2. Player vs AI \n 3. AI vs AI");
            break;
        case 2:
            printf("Rules \n 1. You can only move diagonally. White pawns move towards top border, black towards bottom. "); 
            printf("\n 2a. If there is an opponent pawn adjacent to your pawn and there is a free field on the same diagonal just after the opponent's pawn, you can capture. *Queens don't have to be adjacent to the captured pawn \n");
            printf(" 2b. If there is a move with capture you must make it. Addtionally you must make the most scored capture (queen capture > pawn capture) \n");
            printf(" 2c. If after a capture you can make another one with same figure you get additional turn. \n 3. If you reach border opposite to the one you started on with a pawn, it's changed into queen. \n \n");
            printf("Tutorial \n Symbols used on the board: \n \t (.) - empty field \n \t (w,W) white pawn, white queen \n \t (b,B) black pawn, black queen \n");
            printf("\n Format of input \n For moves the input should follow the scheme of \n \t [From] (space) [To] \n ");
            printf("Both [From] and [To] take the form of letter + number. Correctly written input should look like: \n");
            printf("\t  D6 C5 \t or \t d6 c5 \n");
            printf("\n If you have troubles with finding avaible moves type: \n");
            printf("\t !h \t or \t !H \n");
            break;
        case 3:
            printf("\n Thank you for your time! \n Quitting...");
            break;
        case 4:
            printf("\n Choose the difficulty: \n 1. Easy \n 2. Medium \n 3. Hard");
            break;
        default:
            printf("\n Wrong input! \n");
            break;
    }
}

game_settings set_up()
{
    game_settings settings;
    int correct_set = 0; 
    menu(0);
    while(!correct_set)
    {
        char input;
        scanf("%c", &input);
        while(input < '1' || input > '3') {scanf("%c", &input);}
    
        menu((int) input - '0');
        if(input == '1')
            {
                input = 'A';
                while(input < '1' || input > '3') {scanf("%c", &input); printf("\n");}
                switch(input)
                {
                    case '1':
                        settings.game = PvP;
                        correct_set = 1;
                        break;
                    case '2':
                        settings.game = AI;
                        input = 'A';
                        menu(4);
                        while(input < '1' || input > '3') {scanf("%c", &input); printf("\n");}

                        if(input == '1') settings.difficulty = 2;
                        else if(input == '2') settings.difficulty = 3;
                        else settings.difficulty = 7;

                        correct_set = 2;
                        break;
                    case '3':
                        settings.game = Spectate;
                        correct_set = 3;
                        break;
                }
            }
        else if(input == '3') {settings.game = Quit; correct_set = -1;}
        else
        {
            printf("\n Press any key to proceed...");
            scanf(" %c", &input);
            printf("\n");
            menu(0);
        }
    }
    return settings;
}

void lowercase(char t[])
{
    for(int i=0; t[i]; i++) if(t[i] >= 97 && t[i] <= 122) t[i] -= 32; 
}

int is_call_for_help(char t[])
{
    lowercase(t);
    if( t[1] == 'H' && t[0] == '!') return 1;
    return 0;
}

field trans_index(char a, char b)
{
    return (field) {(int) (b - '1'), (int) a - 65};
}

move trans_move(char t[])
{
    //in case user uses lowercase characters
    lowercase(t);
    
    return (move) {trans_index(t[0], t[1]), trans_index(t[3], t[4])};
}
#endif