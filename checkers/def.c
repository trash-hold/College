#ifndef DEF

#define DEF

#define N 8
typedef enum {C, W, B, DW, DB} field_type; // C-clear, W - white, B- black , DW - lvluped white, DB - lvleduped black

typedef struct
{
    field_type field[N][N];
    int turn;       //whos turn;  1 - White, 0 - Black; 
    int capture;    //are there captures
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


#endif
