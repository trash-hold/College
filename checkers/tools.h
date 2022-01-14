#ifndef TOOLS
#define TOOLS

#include "def.h"
#include <stdio.h>
#include <malloc.h>
#include <assert.h>

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
legal_moves *add_move(Board *A, legal_moves *head, field n);
legal_moves *add_move_adv(Board *A, legal_moves *head, field n, field dir, int i);
legal_moves *search(Board *A, legal_moves *head, move x);

void lowercase(char t[])
{
    for(int i=0; t[i]; i++) if(t[i] >= 97 && t[i] <= 122) t[i] -= 32; 
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
    //printf("NEW ITEM \n");
    return head ? head : ptr;
}

legal_moves *create_list(Board *A)
{
    legal_moves *head = (legal_moves*) NULL;
    field_type player[2];
    if(A->turn) {player[0] = W; player[1] = DW;}
    else {player[0] = B; player[1] = DB;}

    //first searching for fields from where there can be possible moves
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

int is_overflow(field x, field added)
{
    return (x.x + added.x >= 0 && x.x + added.x < N && x.y + added.y >= 0 && x.y + added.y < N) ? 1 : 0;
}

legal_moves *add_move(Board *A, legal_moves *head, field n)
{
    //First setting who the player and opponent is to know which directions are legal to move for clear fields
    field_type player[2];
    field_type opponent[2];
    field dir[2];

    if(A->turn) {player[0] = W; player[1] = DW; opponent[0] = B; opponent[1] = DB; dir[0] = (field) {-1, -1}; dir[1] = (field) {-1, 1};}
    else {player[0] = B; player[1] = DB; opponent[0] = W; opponent[1] = DW; dir[0] = (field){1, 1}; dir[1] = (field) {1, -1};}

    //For simpler code form function will run two loops: 1) for normal moves 2) for captures
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

/*legal_moves *add_move(Board *A, legal_moves *head, field n)
{
    field_type player[2];
    field_type opponent[2];
    field dir[2];
    if(A->turn) {player[0] = W; player[1] = DW; opponent[0] = B; opponent[1] = DB; dir[0] = (field) {-1, -1}; dir[1] = (field) {-1, 1};}
    else {player[0] = B; player[1] = DB; opponent[0] = W; opponent[1] = DW; dir[0] = (field){1, 1}; dir[1] = (field) {1, -1};}

    for(int i=0; i<4; i++)
    {
        if(n.x + dir[i%2].x >= 0 && n.x + dir[i%2].x < N && n.y + dir[i%2].y >= 0 && n.y + dir[i%2].y<N)
        {
            if(i < 2)
            {
                if(A->field[n.x + dir[i%2].x][n.y + dir[i%2].y] == C)
                {
                    head = new_element(head, (move){n, (field){n.x + dir[i%2].x, n.y + dir[i%2].y}}, 0);
                }
                else if(n.x + 2*dir[i%2].x >= 0 && n.x + 2*dir[i%2].x < N && n.y + 2*dir[i%2].y >= 0 && n.y + 2*dir[i%2].y<N && A->field[n.x + 2*dir[i%2].x][n.y + 2*dir[i%2].y] == C)
                {
                    if(A->field[n.x + dir[i%2].x][n.y + dir[i%2].y] == opponent[0]) 
                        head = new_element(head, (move){n, (field){n.x + 2*dir[i%2].x, n.y + 2*dir[i%2].y}}, 1);
                    else if(A->field[n.x + dir[i%2].x][n.y + dir[i%2].y] == opponent[1]) 
                        head = new_element(head, (move){n, (field){n.x + 2*dir[i%2].x, n.y + 2*dir[i%2].y}}, 2);
                }
            }
            else 
            {
                if(n.x + (-2)*dir[i%2].x >= 0 && n.x + (-2)*dir[i%2].x < N && n.y + (-2)*dir[i%2].y >= 0 && n.y + (-2)*dir[i%2].y<N && A->field[n.x + (-2)*dir[i%2].x][n.y + (-2)*dir[i%2].y] == C)
                {
                    if(A->field[n.x - dir[i%2].x][n.y - dir[i%2].y] == opponent[0]) 
                        head = new_element(head, (move){n, (field){n.x + (-2)*dir[i%2].x, n.y + (-2)*dir[i%2].y}}, 1);
                    else if(A->field[n.x - dir[i%2].x][n.y - dir[i%2].y] == opponent[1]) 
                        head = new_element(head, (move){n, (field){n.x + (-2)*dir[i%2].x, n.y + (-2)*dir[i%2].y}}, 2);
                }

            }
        }
    }
    return head;
}*/

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

//searches for a move in a list of all legal moves
legal_moves *search(Board *A, legal_moves *head, move x)
{
    while(head)
    {
        if( head->m.from.x == x.from.x && head->m.from.y == x.from.y && head->m.to.x == x.to.x && head->m.to.y == x.to.y) return head;
        head = head->next;
    }
    return NULL;
}

int is_other_capture(Board *A, field x)
{
    legal_moves *head = create_list(A);
    printf("Avaible moves for other capture \n");
    show_list(head);
    if(!head->capture) return 0;
    while(head)
    {
        if( head->m.from.x == x.x && head->m.from.y == x.y) return 1;
        head = head->next;
    }
    return 0;
}


/*int legal_move(Board *A, legal_moves *head, move x)
{
    field_type player[2];
    if(A->turn) {player[0] = W; player[1] = DW;}
    else {player[0] = B; player[1] = DB;}

    if(x.from.x < 0 || x.from.y < 0 || x.to.x < 0 || x.to.y < 0 || x.from.x >= N || x.from.y >= N || x.to.x >= N || x.to.y >= N ) {printf("Incorrect move \n"); return 0;}
    if(A->field[x.from.x][x.from.y] != player[0] && A->field[x.from.x][x.from.y] != player[1]) {printf("Incorrect move \n"); return 0;}

    
    legal_moves *captures = NULL;
    legal_moves *captures_adv = NULL; 
    legal_moves *temp = head; 

    int found = 0;
    while(temp)
    {
        if(temp->capture == 2) {captures_adv = new_element(captures_adv, temp->m, 2);}
        else if(temp->capture == 1) {captures = new_element(captures, temp->m, 1);}
        temp = temp -> next;
    }
    if(captures_adv)
    {
        found = search(A, captures_adv, x);
        return found ? 3 : 0;
    }
    else if(captures)
    {
        found = search(A, captures, x);
        return found ? 2 : 0;
    }
    else
    {
        found = search(A, head, x);
        return found ? 1 : 0;
    }
    return found;
}*/

//finds the field of captured pawn 
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

//makes a move if its legal
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

int game_state(Board *A)
{
    int sum[] = {0, 0};
    //sum[0] -> player1 score, sum[1] player2/AI score 
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

//if move is correct and there is a capture return 2; if its only correct return 1; else return 0
int is_correct(Board *A, move x)
{
    legal_moves *l = create_list(A);
    legal_moves *temp = search(A, l, x);

    if(temp) return temp->capture ? 2 : 1;
    else {printf("Incorrect move! \n"); return 0;}
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

void AI_move()
{
    int depth = 4; 
    //scanf("%d", &depth);
    
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

int is_call_for_help(char t[])
{
    lowercase(t);
    if( t[1] == 'H' && t[0] == '!') return 1;
    return 0;
}

int is_same(char input[], char template[])
{
    lowercase(input);
    for(int i=0; input[i]; i++) if(input[i] != template[i]) return 0;
    return 1;
}

void menu(int i)
{
    switch(i)
    {
        case 0:
            printf("Welcome to checkers! \n Choose one of the options by typing: \n 1. Play \n 2. Tutorial and rules \n 3. Quit \n ");
            break;
        case 1:
            printf("\n Choose the game mode: \n 1. Player vs Player \n 2. Player vs AI \n 3. AI vs AI \n ");
            break;
        case 2:
            printf("Twarda bania i zacisniete piesci jebac leszczy \n");
            printf("Rules \n 1. You can only move diagonally. White pawns move towards top border, black towards bottom. "); 
            printf("\n 2a. If there is an opponent pawn adjacent to your pawn and there is a free field on the same diagonal just after the opponent's pawn, you can capture. *Queens don't have to be adjacent to the captured pawn \n");
            printf("2b. If there is a move with capture you must make it. Addtionally you must make the most scored capture (queen capture > pawn capture) \n");
            printf(" 2c. If after a capture you can make another one with same figure you get additional turn. \n 3. If you reach border opposite to the one you started on with a pawn, it's changed into queen. \n \n");
            printf("Tutorial \n Symbols used on the board: \n (.) - empty field \n (w,W) white pawn, white queen \n (b,B) black pawn, black queen \n");
            printf("\n Format of input: \n For moves the input should follow the scheme of \n From (space) To \n ");
            printf("Both from and to take the form of letter + number. Correctly written input should look like: \n");
            printf("D6 C5 \t or \t d6 c5 \n");
            printf("If you have troubles with finding avaible move type: \n");
            printf("!h or \t !H \n");
            break;
        case 3:
            printf("Thank you for your time! \n Quitting...");
            break;
        case 4:
            printf("Choose the difficulty: \n 1. Easy \n 2. Medium \n 3. Hard \n");
            break;
        default:
            printf("Wrong input! \n");
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
        printf("%c", input);
    
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
                        else settings.difficulty = 5;

                        correct_set = 2;
                        break;
                    case '3':
                        settings.game = Spectate;
                        correct_set = 3;
                        break;
                }
            }
        else if(input == '3') {menu(3); settings.game = Quit; correct_set = -1;}
    }
}

void player_move()
{
    legal_moves *head = create_list(&A);
    char p1_move[5];
    gets(p1_move);
    if(is_call_for_help(p1_move)) show_list(head);
    else {move p1 = trans_move(p1_move); change_turn(&A, p1);}
}

void game_with_AI()
{
    int Game_state = 0;
    while(Game_state != LOST && Game_state!= WON)
    {
        show_board(&A);
        if(A.turn == 1) player_move();
        else AI_move();
        Game_state = game_state(&A);
        printf("\n It's %s's move now! \n", A.turn ? "Player" : "AI");
    }
    A.turn = 1;
    printf("%s won! \n", Game_state == LOST ? "AI" : "Player");
}
#endif
