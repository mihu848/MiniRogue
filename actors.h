//
//  actors.hpp
//  Project 3
//
//  Created by Michael Huang on 5/13/20.
//  Copyright © 2020 Michael Huang. All rights reserved.
//

#ifndef actors_h
#define actors_h
#include <string>
#include <vector>
#include "utilities.h"
#include "gameobject.h"

const int EMPTY = 0;
const int WALL = 1;
const int ITEM = 2;
const int STAIRS = 3;
const int MONSTER = 4;
const int IDOL = 6;

using namespace std;
class weapon;
class gameObject;
class Level;

class Actor{
public:
    Actor(int max, int arm, int str, int dex, int r, int c, char av, string n,string what_weapon);
    //mutators
    void setPos(int r, int c);
    void changeStats(char c, int amt);
    void cheat();
    void HPregen();
    void weaponChoose(weapon* w);
    void putToSleep();
    void decreaseSleep();
    //accessors
    char getAvatar() const;
    int getRow() const;
    int getCol() const;
    int getHP() const;
    int getArmor() const;
    int getStrength() const;
    int getDexterity() const;
    bool isDead() const;
    bool isAsleep() const;
    string getName() const;
    weapon* getCurrentWeapon() const;
    //battle mechanics
    char attack(Actor*enemy); //returns character indicating result of attack
    string attackResult(char c, Actor*enemy, Level* lev);
    bool nextTo(Actor*other);

    virtual ~Actor();
private:
    int maxHP;
    int HP;
    int armor;
    int strength;
    int dext;
    int sleeptime;
    int posR;
    int posC;
    char avatar;
    weapon* curr;
    string name;
};



class Player: public Actor{
public:
    Player(int r, int c);
    ~Player();
    //player specific actions
    bool addToInventory(gameObject* g);
    string readScroll(Level *lev);
    string wieldWeapon();
    string displayInventory();
    //accessor
    size_t inventorySize();
private:
    vector<gameObject*> inventory;
    int itemcount;
};

class Monster: public Actor{
public:
    Monster(int max, int arm, int str, int dex, int r, int c, char av, string n, string w, int smell);
    bool canSmell(int pr, int pc, int mr, int mc, int max);
    int getSmell() const;
    virtual vector<int> bestMove(int pr, int pc, int mr, int mc);
private:
    int smellDistance;
};

class Bogeyman: public Monster{
public:
    Bogeyman(int r, int c);
private:
};

class Snakewoman: public Monster{
public:
    Snakewoman(int r, int c);
private:
};

class Dragon: public Monster{
public:
    Dragon(int r, int c);
private:
};

class Goblin: public Monster{
public:
    Goblin(int r, int c, int smell);
    vector<int> bestMove(int pr, int pc, Level* l, char dun[18][70]);
    void shortestPathLength(int pr, int pc, int mr, int mc, int max, int steps,Level* l, char dun[18][70],int&least); //recursive algorithm to get the direction with the shortest path to the player
private:
};
#endif /* actors_h */
