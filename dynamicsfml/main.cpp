#include "game.h"
#include "menu.h"
int main() {
    int option = 0;
    Menu m;
    m.display_menu(option);
    Game game;
    if (option == 1)
    {
    game.run(1);
    }
    else if (option == 2)
    {
        game.run(2);
    }
    else {
        ;
    }
    cout << "exited";
    return 0;
}

