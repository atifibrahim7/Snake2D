#include "game.h"
#include "menu.h"
int main() {
    Menu m;
    m.display_menu();
    Game game;
    game.run(1);

    return 0;
}

