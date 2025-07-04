#include <stdlib.h>
#include "Game.h"


int main (int argc, char *argv[])
{
    Game game;

    game.init();
    game.Run();
    game.CleanUP();
        
    return 0; 

}
