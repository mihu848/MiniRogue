// Game.h

#ifndef GAME_INCLUDED
#define GAME_INCLUDED

class Level;
class Player;
#include <string>
using namespace std;
// You may add data members and other member functions to this class.

class Game
{
public:
    Game(int goblinSmellDistance);
    void play();
    char takePlayerTurn();
    char playerAttacks(int r, int c); //sequence when player attacks (monsters also attack)
    char itemInteract(string result); //sequence when player opens inventory, wields weapon, reads scroll
    char monsterActions(); //atack and move monsters
    ~Game();
private:
    Level* lev;
    Player* player;
};

#endif // GAME_INCLUDED
