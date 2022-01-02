//
//  actors.cpp
//  Project 3
//
//  Created by Michael Huang on 5/13/20.
//  Copyright © 2020 Michael Huang. All rights reserved.
//

#include "actors.h"
#include "utilities.h"
#include "gameobject.h"
#include "level.h"
#include <iostream>
#include <map>
using namespace std;
//Actor Class
Actor::Actor(int max, int arm, int str, int dex, int r, int c, char av, string n,string what_weapon){
    maxHP=max;
    armor=arm;
    strength=str;
    dext=dex;
    sleeptime=0;
    HP=maxHP;
    posR=r;
    posC=c;
    avatar=av;
    name = n;
    curr = new weapon(what_weapon);
}

void Actor::setPos(int r, int c){
    posR=r;
    posC=c;
}

char Actor::getAvatar() const{
    return avatar;
}


int Actor::getRow() const{
    return posR;
}
int Actor::getCol() const{
    return posC;
}

int Actor::getHP() const{
    return HP;
}
int Actor::getArmor() const{
    return armor;
}
int Actor::getStrength() const{
    return strength;
}
int Actor::getDexterity() const{
    return dext;
}

string Actor::getName() const{
    return name;
}

void Actor::changeStats(char c, int amt){
    switch(c){
        case 'h':
            if((HP+amt)<=maxHP) //check that current HP won't become too high
                   HP+=amt;
            break;
        case 'm':
            if((maxHP+amt)<=99) //check that current HP won't become too high
                   maxHP+=amt;
            else
                maxHP=99;
            break;
        case'd':
            if((dext+amt)<=99)
                dext+=amt;
            else
                dext=99;
            break;
        case 'a':
            if((armor+amt)<=99)
                armor+=amt;
            else
                armor=99;
            break;
        case 's':
            if((strength+amt)<=99)
                strength+=amt;
            else
                strength=99;
            break;
    }
}



bool Actor::isDead() const{
    if(HP<=0)
        return true;
    return false;
}

void Actor::cheat(){
    maxHP=50;
    HP=50;
    strength=9;
}

void Actor::HPregen(){
    if(trueWithProbability(0.1)){
        changeStats('h', 1);
    }
}


void Actor::weaponChoose(weapon* w){
    curr=w;
}

weapon* Actor::getCurrentWeapon() const{
    return curr;
}
char Actor::attack(Actor*enemy){
    bool hit = false;
    int attackerPoints = dext+this->curr->getDextBonus();
    int defenderPoints = enemy->dext+enemy->armor;
    if(randInt(1,attackerPoints)>= randInt(1,defenderPoints))
        hit = true;
    if(hit){
        int damagePoints = -1*randInt(0, this->strength+curr->getDamage()-1);
        enemy->changeStats('h', damagePoints);
        if(enemy->isDead()){
            Monster* kill = dynamic_cast<Monster*>(enemy);
            if(kill!=nullptr){
                return 'k';
                }
            else{
                return 'q'; //killed player
            }
        }
        else{
            if(curr->getName()=="magic fangs"){
                if(trueWithProbability(0.2)){
                    enemy->putToSleep();
                    return 's';
                }
            }
            return 'h';
        }
    }
    else{
        return 'm';

    }
        return ' ';
}

string Actor::attackResult(char c, Actor*enemy, Level*lev){
    string weapon = this->curr->getName();
    string battle = this->name+" "+curr->getAttack()+" "+enemy->name;
    switch(c){
        case 'k':
            battle+=" dealing a final blow.";
            lev->killMonster(enemy);
            break;
        case 'q':
            battle+= " dealing a final blow.";
            break;
        case 'h':
           battle+=" and hits.";
            break;
        case 's':
            battle+=" and hits, putting "+enemy->getName()+" to sleep.";
            break;
        case 'm':
            battle+=" and misses.";
            break;
    }
    return battle;
}

bool Actor::nextTo(Actor*other){
    if(posR==other->posR){
        if(posC==other->posC+1||posC==other->posC-1)
            return true;
    }
    else if(posC==other->posC){
        if(posR==other->posR+1||posR==other->posR-1)
        return true;
    }
    return false;
}

bool Actor::isAsleep() const{
    if(sleeptime>0)
        return true;
    return false;
}

void Actor::putToSleep(){
    int sleep = randInt(2,6);
    sleeptime= max(sleep,sleeptime);
}

void Actor::decreaseSleep(){
    sleeptime--;
}

Actor::~Actor(){
    if(curr!=nullptr)
    delete curr;
}



//Player Class
Player::Player(int r, int c)
:Actor(20,2,2,2,r,c,'@',"Player","short sword")
{
    inventory.push_back(getCurrentWeapon());
}

bool Player::addToInventory(gameObject* g){
    if(inventory.size()<=25){
        inventory.push_back(g);
        return true;
    }
    return false;
}


string Player::displayInventory(){
    clearScreen();
    char order = 'a';
    cout<<"Inventory:"<<endl;
    for(int a=0;a<inventory.size();a++){
        cout<<(char)(order+a)<<". "<<inventory[a]->getName()<<endl;
    }
    return "i";
}

string Player::readScroll(Level* lev){
    string result="";
    clearScreen();
    char order = 'a';
    cout<<"Inventory:"<<endl;
    map<char, gameObject*> inven;
    map<char, gameObject*>::iterator it;
    for(int a=0;a<inventory.size();a++){
        inven[(char)(order+a)]=inventory[a];
        cout<<(char)(order+a)<<". "<<inventory[a]->getName()<<endl;
    }
    char selection = getCharacter();
    it=inven.find(selection);
    int position = it->first-'a';
    
    if(it==inven.end()){ //invalid letter
        return result;
    }
    scroll* reading= dynamic_cast<scroll*>(it->second);
    if(reading!=nullptr){
        clearScreen();
        lev->display();
        result=reading->scrollEffect(this,lev);
        delete reading;
        inventory.erase(inventory.begin()+position);
    }
    else{
        clearScreen();
        lev->display();
        result+="\nYou can't read a "+it->second->getName()+".";
    }
    return result;
}

string Player::wieldWeapon(){
    string result="";
    clearScreen();
    char order = 'a';
    cout<<"Inventory:"<<endl;
    map<char, gameObject*> inven;
    map<char, gameObject*>::iterator it;
    for(int a=0;a<inventory.size();a++){
        inven[(char)(order+a)]=inventory[a];
        cout<<(char)(order+a)<<". "<<inventory[a]->getName()<<endl;
    }
    char selection = getCharacter();
    it=inven.find(selection);
    
    if(it==inven.end()){ //invalid letter
        return result;
    }
    weapon* select= dynamic_cast<weapon*>(it->second);
    if(select!=nullptr){
        weaponChoose(select);
        result="\nYou are wielding "+select->getName()+".";
    }
    
    else{
        result="\nYou can't wield "+it->second->getName().substr(16)+".";
    }
    return result;
}
size_t Player::inventorySize(){
    return inventory.size();
}

Player::~Player(){
    vector<gameObject*>::iterator it=inventory.begin();
    while(it!=inventory.end()){
        delete *it;
        inventory.erase(it);
        it=inventory.begin();
    }
    weaponChoose(nullptr);
}



//Monster
Monster::Monster(int max, int arm, int str, int dex, int r, int c, char av,string n,string w, int smell)
:Actor(max, arm, str, dex, r, c, av,n,w), smellDistance(smell)
{}

bool Monster::canSmell(int pr, int pc, int mr, int mc, int max){ //dumb monster version of can the monster smell the player
    if(abs(mr-pr)>max||abs(mc-pc)>max||abs(mr-pr)+abs(mc-pc)>max)
    return false;
    
    return true;
}

vector<int> Monster::bestMove(int pr, int pc, int mr, int mc){ //dumb best move algorithm
    vector<int> move;
    if(pr>mr){
        move.push_back(mr+1);
        move.push_back(mc);
    }
    else if(mr>pr){
        move.push_back(mr-1);
        move.push_back(mc);
    }
    else{
        move.push_back(mr);
        if(pc>mc)
            move.push_back(mc+1);
        else if(mc>pc)
            move.push_back(mc-1);
        else
           move.push_back(mc);
    }
   
    return move;
    
}

int Monster::getSmell() const{
    return smellDistance;
}
//Bogeyman Class
Bogeyman::Bogeyman(int r, int c)
:Monster(randInt(5,10),2,randInt(2,3),randInt(2,3),r,c,'B',"the Bogeyman","short sword",5)
{
}


//Snakewoman Class
Snakewoman::Snakewoman(int r, int c)
:Monster(randInt(3,6),3,2,3,r,c,'S',"the Snakewoman","magic fangs",3)
{
}


//Dragon Class
Dragon::Dragon(int r, int c)
:Monster(randInt(20,25),4,4,4,r,c,'D',"the Dragon","long sword",0)
{
}

//Goblin Class
Goblin::Goblin(int r, int c, int smell)
:Monster(randInt(20,25),1,3,1,r,c,'G',"the Goblin","short sword",smell)
{
}

vector<int> Goblin::bestMove(int pr, int pc, Level* l, char dun[18][70]){ //goblin version of can the monster smell

int mr = getRow();
int mc = getCol();
int max = getSmell();




vector<int> bestpos;
int north = max+1;
int south = max+1;
int west = max+1;
int east = max+1;
char n_arr[18][70],e_arr[18][70],w_arr[18][70],s_arr[18][70];


for(int r=0;r<18;r++){ //initialize the direction arrays so they can edit their own array
    for(int c=0;c<70;c++){
        n_arr[r][c]=e_arr[r][c]=w_arr[r][c]=s_arr[r][c]=dun[r][c];
    }
}

    if(!(abs(mr-pr)>max||abs(mc-pc)>max||abs(mr-pr)+abs(mc-pc)>max)){ //not too impossibly far from player
    

if(l->canPlaceActor(mr-1,mc)){//north case
   shortestPathLength(pr,pc,mr-1,mc,max,1,l,n_arr,north);
}

if(l->canPlaceActor(mr+1,mc)){//south case
   shortestPathLength(pr,pc,mr+1,mc,max,1,l,s_arr,south);
}

if(l->canPlaceActor(mr,mc-1)){//west case
   shortestPathLength(pr,pc,mr,mc-1,max,1,l,w_arr,west);
}

if(l->canPlaceActor(mr,mc+1)){//east case
   shortestPathLength(pr,pc,mr,mc+1,max,1,l,e_arr,east);
}
}

if(north==max+1&&south==north&&west==north&&east==north){ //goblin can't even reach player
  bestpos.push_back(getRow());
  bestpos.push_back(getCol());
  return bestpos;
}
else{
int best = min(north,south);
best=min(best,west);
best=min(best,east);
    
    
if(north==best){
  bestpos.push_back(getRow()-1);
  bestpos.push_back(getCol());
  return bestpos;
}

if(south==best){
  bestpos.push_back(getRow()+1);
  bestpos.push_back(getCol());
  return bestpos;
}

if(west==best){
  bestpos.push_back(getRow());
  bestpos.push_back(getCol()-1);
  return bestpos;
}

if(east==best){
  bestpos.push_back(getRow());
  bestpos.push_back(getCol()+1);
  return bestpos;
}


    bestpos.push_back(getRow());
    bestpos.push_back(getCol());
    return bestpos;
    
}
}

void Goblin::shortestPathLength(int pr, int pc, int mr, int mc, int max, int steps,Level* l, char dun[18][70],int&least){

if(steps>max) //not legal by smell distance
    return;
if(mr==pr&&mc==pc){ //goblin reach the player without breaking smell maximum limit
    least=min(steps,least);
      return;
}
    
    if(abs(mr-pr)>max||abs(mc-pc)>max||abs(mr-pr)+abs(mc-pc)>max) //too impossibly far from player
    return;
    
    dun[mr][mc]='x'; //mark location as visited
    
    if(!((l->getStatus(mr-1,mc)==WALL)||(l->getStatus(mr-1,mc)==MONSTER)||dun[mr-1][mc]=='x')) //go north legal?
        shortestPathLength(pr,pc,mr-1,mc,max,steps+1,l,dun,least);
    if(!((l->getStatus(mr,mc+1)==WALL)||(l->getStatus(mr,mc+1)==MONSTER)||dun[mr][mc+1]=='x'))// go east legal?
        shortestPathLength(pr,pc,mr,mc+1,max,steps+1,l,dun,least);
    if(!((l->getStatus(mr,mc-1)==WALL)||(l->getStatus(mr,mc-1)==MONSTER)||dun[mr][mc-1]=='x'))// go west legal?
        shortestPathLength(pr,pc,mr,mc-1,max,steps+1,l,dun,least);
    if(!((l->getStatus(mr+1,mc)==WALL)||(l->getStatus(mr+1,mc)==MONSTER)||dun[mr+1][mc]=='x')) //go south legal?
        shortestPathLength(pr,pc,mr+1,mc,max,steps+1,l,dun,least);
    
    dun[mr][mc]=' '; //open up location for crossing again in case path crossed the spot, but was not ideal path
    

}
