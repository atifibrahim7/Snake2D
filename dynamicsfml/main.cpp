#include "game.h"
#include "menu.h"
int main() {
    Menu m;
    m.display_menu();
    Game game;
    game.run();

    return 0;
}

