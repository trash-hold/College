#include <stdio.h>
#include "def.h"
#include "tools.h"

void main()
{
    //Board A;
    generate(&A); 
    A.field[4][4] = B;
    A.field[1][7] = C;
    /*move x = {(field){5,1}, (field){3,3}};
    
    A.field[4][2] = B;
    A.field[4][4] = DB;
    legal_moves *l = create_list(&A);
    show_board(&A);
    play();
    A.turn = 0;
    show_board(&A);
    play();
    show_board(&A);
    show_list(l);
    
    printf("game: %d \n", game_state(&A));*/
    
    int Game_state = 0;
    //game with AI
    while(Game_state != LOST && Game_state!= WON)
    {
        show_board(&A);
        legal_moves *head = create_list(&A);
        if(A.turn == 1)
        {
            char p1_move[5];
            gets(p1_move);
            move p1 = trans_move(p1_move);
            
            change_turn(&A, p1);
            /*int correct = is_correct(&A, trans_move(p1_move));
            if(correct)
            {
                move p1 = trans_move(p1_move);
                make_move(&A, head, p1);
                //Have to check if there was a capture and if there is another one avaible
                if(correct == 2)
                {
                    if(!is_other_capture(&A, p1.to)) A.turn = 0;
                }
                else A.turn = 0;
            }*/
        }
        else {play();}
        Game_state = game_state(&A);
    }
}

