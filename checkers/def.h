#ifndef DEF
#define DEF

#define N 8
#define WON 36
#define LOST -36
typedef enum {C, W, B, DW, DB} field_type; // C-clear, W - white, B- black , DW - white queen, DB - black queen
typedef enum {PvP, AI, Spectate, Quit} game_mode;

typedef struct
{
    field_type field[N][N];
    int turn;       //whos turn;  1 - White, 0 - Black; 
}Board; 

typedef struct 
{
    int x;          //x is index of verse  <0, N-1>
    int y;          //y is index of column <0, N-1>
}field;

typedef struct
{
    field from; 
    field to; 
}move; 

typedef struct l 
{
    move m;            
    int capture;
    /*
    capture values and its meaning: 
    0 - regular move, 
    1 - the move results in capturing a pawn 
    2 - the move results in capturing a queen 
    */
    struct l* next;
}legal_moves;

typedef struct
{
    game_mode game; 
    int difficulty;     // Easy - 2, Medium - 3, Hard - 7
}game_settings;

Board A;
game_settings settings;
#endif