//
//  gameobject.h
//  Project 3
//
//  Created by Michael Huang on 5/13/20.
//  Copyright © 2020 Michael Huang. All rights reserved.
//

#ifndef gameobject_h
#define gameobject_h
#include <string>
class Player;
class Level;
using namespace std;
class gameObject{
public:
    gameObject(string n, char ava);
    virtual ~gameObject();
    virtual string getName() const;
    char getAvatar() const;
    int getRow() const;
    int getCol() const;
    bool operator<(const gameObject &other) const;
private:
    string m_name;
    char avatar;
    int row;
    int col;
};
class weapon: public gameObject{
public:
    weapon(string n);
    ~weapon();
    int getDamage() const;
    int getDextBonus() const;
    string getAttack() const;
private:
    int damage;
    int dextbonus;
    string attack;
};
class scroll: public gameObject{
public:
    scroll(string n);
    ~scroll();
    string scrollEffect(Player*p, Level*l);
    string getName() const;
private:
};
class idol: public gameObject{
public:
    idol();
    ~idol();
};



#endif /* gameobject_h */
