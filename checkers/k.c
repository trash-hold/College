#include <stdio.h>
#include "def.h"
#include "tools.h"

void main()
{
    generate(&A);
    printf("Welcome to checkers!");
    settings = set_up();
    while(settings.game != Quit)
    {
        if(settings.game == PvP) 
            game_PvP();
        else if(settings.game == AI) game_with_AI();
        else game_AI();
        settings = set_up();
    }
}

/*
1. make_move maybe can remove head? and do create_list; probably should put the if for finding queens in the if(t)
2. rewritting showing board and generating
3. FREEING SPACE

Bugs: bicie do tyłu?
      wyświetlanie złego zwyciężcy maybe fixed

Możliwość wielokrotnego bicia!!!
*/
