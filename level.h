//
//  level.hpp
//  Project 3
//
//  Created by Michael Huang on 5/13/20.
//  Copyright © 2020 Michael Huang. All rights reserved.
//

#ifndef level_h
#define level_h
#include <map>
#include <vector>
#include <string>
using namespace std;

class Actor;
class Player;
class Monster;
class gameObject;
const int MAXROWPOS = 17;
const int MAXCOLPOS = 69;
class Level{
public:
    Level(int g);
    void display();
    //checking status of certain spaces
    int getStatus(int r, int c);
    bool canPlaceActor(int r, int c);
    bool canPlaceItem(int r, int c);
    Monster* whichMonster(int r, int c);
    //setting up the level
    void placeStairs();
    void nextDungeon();
    Player* player() const;
    bool addPlayer(int r, int c);
    bool addMonsters();
    bool addItems();
    
    //cleaning up the level
    void clearItems();
    void clearMonsters();
    bool killMonster(Actor*dead);
    //getting results printout under stats line on display, item/monster interaction
    string pickUpItem();
    string monstersAttack();
    void monstersMove();
    ~Level();
private:
    int lev_num;
    char dungeon[18][70]; //floor plan with staircase
    Player* protag;
    vector<Monster*> monsters;//max number of monsters on a level
    map<vector<int>,gameObject*> items;
    int goblinSmell;
};

class Room{ //helps with generation of levels
public:
    Room();
    //accessors
    int getWidth() const;
    int getHeight() const;
    int leftCornerRow() const;
    int leftCornerCol() const;
    int getArea() const;
    //checking conditions
    bool noOverlaps(vector<Room*>&v);
    bool isInRoom(int r, int c);
    bool rowInRoom(int r);
    bool colInRoom(int c);
private:
    int width;
    int height;
    int coords[2]; //upper left corner coordinates
};
//extra functions to deal with room creation/level creation
int sumAreas(vector<Room*>v);
void connectRooms(vector<Room*>&v, char dun[18][70]);
#endif /* level_hpp */


