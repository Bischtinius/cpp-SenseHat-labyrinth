#include "Cell.h"
#include "playground.h"
#include <iostream>

// #include "joystickskrhabe010.h"
// #include "ledmatrixled2472g.h"
// #include <iostream>


int main()
{

    playground test = playground(8,8);
    srand ( time(NULL) );
    int startx = std::rand() % 8;      
    int starty = std::rand() % 8;
    do
    {
        test.setStart(startx,starty);
        test.generate_map();
        while (test.levelFinished() == false && test.playerAlive())
        {
            test.show_map();
            std::cout << "Move The Player" << std::endl;
            test.movePlayer();
        }
        startx = test.PlayerPosX();
        starty = test.PlayerPosY();
    } while (test.playerAlive());

    test.clearBoard();

    return 0;
}