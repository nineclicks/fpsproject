#include <iostream>
#include "game.h"
#include "view.h"
#include "input.h"
#include "vec.h"

using namespace std;

int main()
{
    
    Game game;
    View view(&game);
    Input input(&game, &view);

    int done=0;
    
    while(!done) {
        done = input.CheckInput();
        
    }
    view.cleanupXWindows();
    

    return 0;
}
