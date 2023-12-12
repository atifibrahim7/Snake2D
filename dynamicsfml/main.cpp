#include "game.h"
#include "menu.h"
int main() {
    int flag = 0;
    do
    {

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
        return 0;
    }
  flag =   m.display_end();
    } while (flag);

    cout << "exited";
    return 0;
}

