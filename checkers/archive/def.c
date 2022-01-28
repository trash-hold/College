#ifndef DEF

#define DEF

#define N 8
#define WON 36
#define LOST -36
typedef enum {C, W, B, DW, DB} field_type; // C-clear, W - white, B- black , DW - lvluped white, DB - lvleduped black
typedef enum {PvP, AI, Spectate, Quit} game_mode;

typedef struct
{
    field_type field[N][N];
    int turn;       //whos turn;  1 - White, 0 - Black; 
}Board; 

typedef struct 
{
    int x;
    int y; 
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
    struct l* next;
}legal_moves;

typedef struct
{
    game_mode game;
    // Easy - 2, Medium - 3, Hard - 5
    int difficulty;
}game_settings;

Board A;
game_settings settings;
#endif
