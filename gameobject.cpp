//
//  gameobject.cpp
//  Project 3
//
//  Created by Michael Huang on 5/13/20.
//  Copyright © 2020 Michael Huang. All rights reserved.
//

#include "gameobject.h"
#include "actors.h"
#include "level.h"
#include "utilities.h"
#include <iostream>
using namespace std;
gameObject::gameObject(string n, char ava){
    m_name=n;
    avatar=ava;
}
gameObject::~gameObject(){}

string gameObject::getName() const{
    return m_name;
}

char gameObject::getAvatar() const{
    return avatar;
}
int gameObject::getRow() const{
    return row;
}
int gameObject::getCol() const{
    return col;
}

bool gameObject::operator<(const gameObject &other) const{
    if(other.getName()<getName())
        return true;
    return false;
}

//weapon class
weapon::weapon(string n)
:gameObject(n,')')
{
    if(n=="short sword"){
        damage = 2;
        dextbonus =0;
        attack="slashes";
    }
    else if(n=="mace"){
        damage = 2;
        dextbonus =0;
        attack="swings";
    }
    else if(n=="long sword"){
        damage = 4;
        dextbonus =2;
        attack="swings";
    }
    else if(n=="magic axe"){
        damage = 5;
        dextbonus =5;
        attack="chops";
    }
    else if(n=="magic fangs"){
        damage = 2;
        dextbonus = 3;
        attack="strikes";
    }
    else{
    damage = 0;
        dextbonus = 0;
    }
    
}
weapon::~weapon(){}

int weapon::getDamage() const{
    return damage;
}
int weapon::getDextBonus() const{
    return dextbonus;
}

string weapon::getAttack() const{
    return attack+" "+gameObject::getName()+" at";
}

//scroll class

scroll::scroll(string n)
:gameObject(n,'?')
{

}
scroll::~scroll(){
}

string scroll::scrollEffect(Player*p, Level*l){
    string result="";
    if(gameObject::getName()=="teleportation"){ // give the player a new, legal, position
        int rPlayer = randInt(1, 18);
        int cPlayer = randInt(1, 70);
        do
            {
            rPlayer = randInt(1, 18);
            cPlayer = randInt(1, 70);
        } while (!l->canPlaceActor(rPlayer, cPlayer));
        p->setPos(rPlayer,cPlayer);
        clearScreen();
        l->display();
        result+="\nYou read the"+getName().substr(1)+".\n";
        result+="You feel your body wrenched in space and time.\n";
    }
    if(gameObject::getName()=="improve armor"){
        p->changeStats('a', randInt(1,3));
        clearScreen();
        l->display();
        result+="\nYou read the"+getName().substr(1)+".\n";
        result+="Your armor glows blue.\n";
    }
    if(gameObject::getName()=="strength"){
        p->changeStats('s', randInt(1,3));
        clearScreen();
        l->display();
       result+="\nYou read the"+getName().substr(1)+".\n";
        result+="Your muscles bulge.\n";
    }
    if(gameObject::getName()=="enhance health"){
        p->changeStats('m', randInt(3,8));
        clearScreen();
        l->display();
        result+="\nYou read the"+getName().substr(1)+".\n";
       result+="You feel your heart beating stronger.\n";
    }
    if(gameObject::getName()=="enhance dexterity"){
        p->changeStats('d',+1);
        clearScreen();
        l->display();
        result+="\nYou read the"+getName().substr(1)+".\n";
        result+="You feel like less of a klutz.\n";
    }
    
    return result;
}

string scroll::getName() const{
    string add = "A scroll called scroll of ";
    add+=gameObject::getName();
    return add;
    
}
//idol class
idol::idol()
:gameObject("golden idol",'&')
{
}

idol::~idol(){
}
