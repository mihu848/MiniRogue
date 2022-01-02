//
//  level.cpp
//  Project 3
//
//  Created by Michael Huang on 5/13/20.
//  Copyright © 2020 Michael Huang. All rights reserved.
//

#include "level.h"
#include "actors.h"
#include "utilities.h"
#include "gameobject.h"
#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <string>
using namespace std;

//level class

Level::Level(int g)
:goblinSmell(g)
{
    //generating initial map
    //fill everything with wall first
    for(int a=0;a<18;a++){
        for(int b=0;b<70;b++){
                dungeon[a][b]='#';
        }
    }
    //generate rooms
    vector<Room*>rooms;
    do{
        Room* r = new Room;
        if(r->noOverlaps(rooms))
            rooms.push_back(r);
        else
            delete r;
    }while(!(sumAreas(rooms)>=250&&rooms.size()>=3));
    
    
    //change rooms into empty floor space
    vector<Room *>::iterator it=rooms.begin();
    for(int a =0;a<rooms.size();a++){
        it= rooms.begin();
        int r = (*(it+a))->leftCornerRow();
        int c = (*(it+a))->leftCornerCol();
        int width = (*(it+a))->getWidth();
        int height = (*(it+a))->getHeight();
        for(int p=r;p<r+height;p++){
            for(int q=c;q<c+width;q++){
                    dungeon[p][q]=' ';
            }
        }
    }
    //connect the rooms with pathways
    connectRooms(rooms,dungeon);
    //delete the rooms
    it=rooms.begin();
    while(it!=rooms.end()){ //get rid of all the rooms
        delete (*it);
        rooms.erase(it);
        it=rooms.begin();
    }
    
    //set dungeon to first level
    lev_num=0;
    //player to nothing
    protag=nullptr;
    //place items
    addItems();
    //place staircase somewhere
    placeStairs();
    //place monsters
    addMonsters();
    
}

bool Level::canPlaceActor(int r, int c){
    if(r<=0||r>=MAXROWPOS||c<=0||c>=MAXCOLPOS)
        return false;
    if(getStatus(r,c)==MONSTER){
        return false;
    }
    if(getStatus(r,c)==EMPTY||getStatus(r,c)==ITEM||getStatus(r,c)==STAIRS||getStatus(r,c)==IDOL){
        return true;
    }
    return false;
}

bool Level::canPlaceItem(int r, int c){
    if(r<=0||r>=MAXROWPOS||c<=0||c>=MAXCOLPOS)
        return false;
    if(dungeon[r][c]==')'||dungeon[r][c]=='&'||dungeon[r][c]=='?'||dungeon[r][c]=='>')
        return false;
    if(getStatus(r,c)==0||getStatus(r,c)==4||getStatus(r,c)==5){
        return true;
    }
    return false;
}

bool Level::addMonsters(){
    int added=0;
    int total = randInt(2,5*(lev_num+1)+1);
    int r,c;
    while(added<total){
        //generate legal coordinates for an monster placement
             r = randInt(1, MAXROWPOS-1);
             c = randInt(1, MAXCOLPOS-1);
             do{
                 r = randInt(1, MAXROWPOS-1);
                 c = randInt(1, MAXCOLPOS-1);
             }while(!canPlaceActor(r, c));
       //separate cases of monster generatino by dungeon level
        if(lev_num<2){
            //choose between goblin or snakewoman
            if(trueWithProbability(0.5))
            monsters.push_back(new Goblin(r,c,goblinSmell));
            else
            monsters.push_back(new Snakewoman(r,c));
        }
        
        if(lev_num==2){ //choose between goblin, snakewoman, or bogeyman
            int select = randInt(1,3);
            switch(select){
                case 1:
                    monsters.push_back(new Goblin(r,c,goblinSmell));
                    break;
                case 2:
                  monsters.push_back(new Snakewoman(r,c));
                    break;
                case 3:
                    monsters.push_back(new Bogeyman(r,c));
                    break;
            }
        }
        if(lev_num>=3){
            //choose between goblin, snakewoman, bogeyman, or dragon
            int select = randInt(1,4);
            switch(select){
                case 1:
                    monsters.push_back(new Goblin(r,c,goblinSmell));
                    break;
                case 2:
                  monsters.push_back(new Snakewoman(r,c));
                    break;
                case 3:
                    monsters.push_back(new Bogeyman(r,c));
                    break;
                case 4:
                    monsters.push_back(new Dragon(r,c));
                    break;
            }
        }
        added++;
    }
    return true;
}

bool Level::addItems(){
    int total=randInt(2,3);
    int added=0;
    int r,c;
    while(added<total){
       //generate legal coordinates for an item placement
        r = randInt(1, MAXROWPOS-1);
        c = randInt(1, MAXCOLPOS-1);
        do{
            r = randInt(1, MAXROWPOS-1);
            c = randInt(1, MAXCOLPOS-1);
        }while(!canPlaceItem(r, c)); //fix to roll through map of items? fix later if necessary
        vector<int> pos;
        pos.push_back(r);
        pos.push_back(c);
        //decide what item to put down
        int which = randInt(1,7);
        switch(which){
            case 1:
                items[pos]=new weapon("short sword");
                dungeon[r][c]=')';
                break;
            case 2:
                items[pos]=new weapon("mace");
                dungeon[r][c]=')';
                break;
            case 3:
                items[pos]=new weapon("long sword");
                dungeon[r][c]=')';
                break;
            case 4:
                items[pos]=new scroll("improve armor");
                dungeon[r][c]='?';
                break;
            case 5:
            items[pos]=new scroll("strength");
            dungeon[r][c]='?';
                break;
            case 6:
                items[pos]=new scroll("enhance health");
                dungeon[r][c]='?';
                    break;
            case 7:
                items[pos]=new scroll("enhance dexterity");
                dungeon[r][c]='?';
                    break;
        }
        added++;
    }
    if(lev_num==4){ //place a golden idol
        r = randInt(1, MAXROWPOS-1);
        c = randInt(1, MAXCOLPOS-1);
        do{
            r = randInt(1, MAXROWPOS-1);
            c = randInt(1, MAXCOLPOS-1);
        }while(!canPlaceItem(r, c));
        vector<int> pos;
        pos.push_back(r);
        pos.push_back(c);
        items[pos]=new idol();
        dungeon[r][c]='&';
    }
    return true;
}

string Level::pickUpItem(){
    string result="";
    vector<int> player_pos;
    player_pos.push_back(protag->getRow());
    player_pos.push_back(protag->getCol());
    
    map<vector<int>,gameObject*>::iterator it=items.find(player_pos);
    if(it!=items.end()){
        if(protag->addToInventory(it->second)){
            string itemName =(it->second->getName());
            switch(dungeon[protag->getRow()][protag->getCol()]){
                case '?':
                    itemName="a"+itemName.substr(1);
                    result="\nYou pick up "+itemName+".";
                    break;
                case ')':
                    result="\nYou pick up "+itemName+".";
                    break;
            }
            items.erase(it);
            dungeon[protag->getRow()][protag->getCol()]=' ';
        }
        else if(protag->inventorySize()==26){
            result="Your knapsack is full; you can't pick that up.";
        }
        else{}
    }
    return result;
}

void Level::placeStairs(){ //place staircase
    int r = randInt(1, MAXROWPOS-1);
    int c = randInt(1, MAXCOLPOS-1);
    do{
        r = randInt(1, MAXROWPOS-1);
        c = randInt(1, MAXCOLPOS-1);
    }while(!canPlaceItem(r, c));
    dungeon[r][c]='>';
}


int Level::getStatus(int r,int c){
    if(dungeon[r][c]=='>'){
        for(int a = 0;a<monsters.size();a++){ //check if monster is standing on it
        if(monsters[a]->getRow()==r&&monsters[a]->getCol()==c)
            return 4;//meaning monster
        }
        return 3; //meaning Staircase
    }
    if(dungeon[r][c]=='&'){
        for(int a = 0;a<monsters.size();a++){ //check if monster is standing on it
            if(monsters[a]->getRow()==r&&monsters[a]->getCol()==c)
                return 4;//meaning monster
        }
        return 6; //meaning idol
    }
    if(dungeon[r][c]==')'||dungeon[r][c]=='?'){
        for(int a = 0;a<monsters.size();a++){ //check if monster is standing on it
            if(monsters[a]->getRow()==r&&monsters[a]->getCol()==c)
                return 4;//meaning monster
        }
        return 2; //meaning ITEM
    }
    if(protag!=nullptr){
        if(protag->getRow()==r&&protag->getCol()==c)
        return 5; //meaning player
    }
    for(int a = 0;a<monsters.size();a++){
        if(monsters[a]->getRow()==r&&monsters[a]->getCol()==c)
            return 4; //meaning monster
    }
    if(dungeon[r][c]=='#')
        return 1; //meaning wall
    if(dungeon[r][c]==' ')
        return 0; //meaning empty
    
    return 0;
}

void Level::clearItems(){
    map<vector<int>,gameObject*>::iterator it=items.begin();
    map<vector<int>,gameObject*>::iterator del=items.begin();
    while(it!=items.end()){
        del=it;
        it++;
        delete del->second;
        items.erase(del);
    }
}

void Level::clearMonsters(){
    vector<Monster*>::iterator it=monsters.begin();
    while(it!=monsters.end()){
        delete *it;
        monsters.erase(it);
        it=monsters.begin();
    }
}

bool Level::killMonster(Actor*dead){
    vector<Monster*>::iterator it=monsters.begin();
    Monster* kill = dynamic_cast<Monster*>(dead);
    vector<int> itemdrop;
    if(kill!=nullptr){
    while(kill!=nullptr&&*it!=kill)
        it++;
    if(dead->getName()=="the Snakewoman"){
        if(trueWithProbability(1.0/3.0)&&canPlaceItem(kill->getRow(), kill->getCol())){
                itemdrop.push_back(kill->getRow());
                itemdrop.push_back(kill->getCol());
                items[itemdrop]=new weapon("magic fangs");
            dungeon[kill->getRow()][kill->getCol()]=')';
            }
    }
    if(dead->getName()=="the Bogeyman"){
        if(trueWithProbability(0.1)&&canPlaceItem(kill->getRow(), kill->getCol())){
                itemdrop.push_back(kill->getRow());
                itemdrop.push_back(kill->getCol());
                items[itemdrop]=new weapon("magic axe");
            dungeon[kill->getRow()][kill->getCol()]=')';
                }
        }
    if(dead->getName()=="the Goblin"){
        if(trueWithProbability(1.0/3.0)&&canPlaceItem(kill->getRow(), kill->getCol())){
            itemdrop.push_back(kill->getRow());
            itemdrop.push_back(kill->getCol());
            if(trueWithProbability(0.5)){
            items[itemdrop]=new weapon("magic axe");
                dungeon[kill->getRow()][kill->getCol()]=')';
            }
            else{
            items[itemdrop]=new weapon("magic fangs");
                dungeon[kill->getRow()][kill->getCol()]=')';
            }
            }
    }
    if(dead->getName()=="the Dragon"){
            if(canPlaceItem(kill->getRow(), kill->getCol())){
                itemdrop.push_back(kill->getRow());
                itemdrop.push_back(kill->getCol());
                int which=randInt(1,5);
                switch(which){
                    case 1:
                    items[itemdrop]=new scroll("strength");
                     dungeon[kill->getRow()][kill->getCol()]='?';
                    break;
                    case 2:
                    items[itemdrop]=new scroll("enhance health");
                     dungeon[kill->getRow()][kill->getCol()]='?';
                    break;
                    case 3:
                        items[itemdrop]=new scroll("improve armor");
                         dungeon[kill->getRow()][kill->getCol()]='?';
                        break;
                    case 4:
                        items[itemdrop]=new scroll("enhance dexterity");
                         dungeon[kill->getRow()][kill->getCol()]='?';
                        break;
                    case 5:
                        items[itemdrop]=new scroll("teleportation");
                         dungeon[kill->getRow()][kill->getCol()]='?';
                        break;
                    }
        }
    }
        delete *it;
    monsters.erase(it);
        return true;
    }
    return false;
}


void Level::nextDungeon(){
    //increment level number
    lev_num++;
    //clear all the items
    clearItems();
    //clear all the previous monsters
    clearMonsters();
    //to generate the map
     //fill everything with wall first
     for(int a=0;a<18;a++){
         for(int b=0;b<70;b++){
                 dungeon[a][b]='#';
         }
     }
     //generate rooms
     vector<Room*>rooms;
     do{
         Room* r = new Room;
         if(r->noOverlaps(rooms))
             rooms.push_back(r);
         else
             delete r;
     }while(!(sumAreas(rooms)>=250&&rooms.size()>=3));
     
     
     //change rooms into empty floor space
     vector<Room *>::iterator it=rooms.begin();
     for(int a =0;a<rooms.size();a++){
         it= rooms.begin();
         int r = (*(it+a))->leftCornerRow();
         int c = (*(it+a))->leftCornerCol();
         int width = (*(it+a))->getWidth();
         int height = (*(it+a))->getHeight();
         for(int p=r;p<r+height;p++){
             for(int q=c;q<c+width;q++){
                     dungeon[p][q]=' ';
             }
         }
     }
     //connect the rooms with pathways
     connectRooms(rooms,dungeon);
     //delete the rooms
     it=rooms.begin();
     while(it!=rooms.end()){ //get rid of all the rooms
         delete (*it);
         rooms.erase(it);
         it=rooms.begin();
     }
    //add in the monsters
    addMonsters();
    //add in items
    addItems();
    //get a new player position
    int rPlayer;
    int cPlayer;
    do
        {
        rPlayer = randInt(1, MAXROWPOS-1);
        cPlayer = randInt(1, MAXCOLPOS-1);
    } while (!canPlaceActor(rPlayer, cPlayer));
    protag->setPos(rPlayer,cPlayer);
    
    
    //if level is 4, don't make staircase
    if(lev_num==4){
    }
    else{
        //place staircase if not level 4
        placeStairs();
    }
    clearScreen();
    display();
}

void Level::display(){
    char display_grid[18][70];
    for(int a=0;a<18;a++){
        for(int b=0;b<70;b++){
            display_grid[a][b]=dungeon[a][b]; //incorporates floor and items
        }
    }
    //check through all the monsters to display
    for(int a=0;a<monsters.size();a++){
        display_grid[monsters[a]->getRow()][monsters[a]->getCol()]=monsters[a]->getAvatar();
    }
    
    //display the player
    display_grid[protag->getRow()][protag->getCol()] = protag->getAvatar();
    
    
    for(int a=0;a<18;a++){
        for(int b=0;b<70;b++){
            cout<< display_grid[a][b];
        }
        cout<<endl;
    }
    
    cout<<"Dungeon Level: "<<lev_num<<", Hit points: "<<protag->getHP()<<", Armor: "<<protag->getArmor()<<", Strength: "<<protag->getStrength()<<", Dexterity: "<<protag->getDexterity()<<endl;
    
    
}


Player* Level::player() const
{
    return protag;
}

bool Level::addPlayer(int r, int c){
    if (canPlaceActor(r,c)){
        protag = new Player(r,c);
        return true;
    }
    return false;
}

Monster* Level::whichMonster(int r, int c){
    for(int a = 0;a<monsters.size();a++){
        if(monsters[a]->getRow()==r&&monsters[a]->getCol()==c)
            return monsters[a];
        }
    return nullptr;
}

string Level::monstersAttack(){
    string action="";
    char quit;
    for(int a = 0;a<monsters.size();a++){
        if(monsters[a]->nextTo(protag)){
            quit =monsters[a]->attack(protag);
            action+=monsters[a]->attackResult(quit,protag,this)+"\n";
            if(quit == 'q') //when the player gets killed
                return action + "Press q to exit game.\n";
        }
    }
    return action;
}

void Level::monstersMove(){ //need to distinguish between other monsters and goblin, dynamic cast?
    for(int a = 0;a<monsters.size();a++){
        
        if(!(monsters[a]->isAsleep())){ //monster is not asleep and can move
            if(!(monsters[a]->nextTo(protag))){ //if the monster isn't already next to the player, move monster towards player
            char check[18][70];
            for(int r=0;r<18;r++){
                for(int c=0;c<70;c++){
                    check[r][c]=dungeon[r][c];
                }
            }
            bool smell=false;
           Goblin* gob = dynamic_cast<Goblin*>(monsters[a]);
            vector <int> newpos;
            if(gob!=nullptr){ //the monster is a goblin
                newpos=gob->bestMove(protag->getRow(), protag->getCol(), this, check);
                if(canPlaceActor(newpos[0],newpos[1]))
                monsters[a]->setPos(newpos[0],newpos[1]);
            }
            else{//monster not goblin
                smell=monsters[a]->canSmell(protag->getRow(), protag->getCol(), monsters[a]->getRow(),monsters[a]->getCol(), monsters[a]->getSmell());
                if(smell){
                newpos = monsters[a]->bestMove(protag->getRow(), protag->getCol(), monsters[a]->getRow(),monsters[a]->getCol());
                    if(canPlaceActor(newpos[0],newpos[1]))
                        monsters[a]->setPos(newpos[0],newpos[1]);
                }
                
        }
        }
               }
        
        else{
            monsters[a]->decreaseSleep();
        }
    }
}

Level::~Level(){
    delete protag;
    clearItems();
    clearMonsters();
}


//room class
Room::Room(){
    width = randInt(8,20);
    height = randInt(3,6);
    //generate position of the left corner of the room, ring of # will always create frame around level
    int row = randInt(3,16-height);
    int col = randInt(3,68-width);
    coords[0]=row;
    coords[1]=col;
}

int Room::getWidth() const{
    return width;
}
int Room::getHeight() const{
    return height;
}
int Room::getArea() const{
    return width*height;
}
int Room::leftCornerRow() const{
    return coords[0];
}

int Room::leftCornerCol() const{
    return coords[1];
}

bool Room::isInRoom(int r, int c){
    if((r>=coords[0]&&r<coords[0]+height)&&(c>=coords[1]&&c<coords[1]+width))
        return true;
    return false;
}

bool Room::rowInRoom(int r){
    if(r>=coords[0]&&r<coords[0]+height)
        return true;
    return false;
}

bool Room::colInRoom(int c){
    if(c>=coords[1]&&c<coords[1]+width)
        return true;
    return false;
}

bool Room::noOverlaps(vector<Room*>&v){ //also gives a little bit of extra spacing
    for(int a=0;a<v.size();a++){
        for(int r=coords[0]-2;r<=coords[0]+height+1;r++){
            for(int c=coords[1]-2;c<=coords[1]+width+1;c++){
                if(v[a]->isInRoom(r,c))
                    return false;
        }
    }
    
}
   return true;
}

//sum areas function
int sumAreas(vector<Room*>v) {
    int sum=0;
    for(int a=0;a<v.size();a++){
        sum+=v[a]->getArea();
    }
    return sum;
}



void connectRooms(vector<Room*>&v, char dun[18][70]){
    int r1=0;
    int r2=0;
    int c1=0;
    int c2=0;
    for(int a=0;a<v.size()-1;a++){
        r1 = v[a]->leftCornerRow()+v[a]->getHeight()/2;
        c1 = v[a]->leftCornerCol()+v[a]->getWidth()/2;
        r2 = v[a+1]->leftCornerRow()+v[a+1]->getHeight()/2;
        c2 = v[a+1]->leftCornerCol()+v[a+1]->getWidth()/2;
        //determine whether to go right or left column wise to create path to the other room
        
        if(r1<r2){
            while(!(v[a+1]->rowInRoom(r1))){
                r1++;
                dun[r1][c1]=' ';
                
            }
        }
        else{
           while(!(v[a+1]->rowInRoom(r1))){
               r1--;
               dun[r1][c1]=' ';

            }
        }



        
        if(!(v[a+1]->isInRoom(r1,c1))){  //now adjust by columns if necessary, which is when the cleared out space isn't already in the other room
        
        if(c1<c2){
            while(!(v[a+1]->isInRoom(r1,c1))){
                c1++;
                dun[r1][c1]=' ';
            }
        }
        
        else{  //c1>c2
            while(!(v[a+1]->isInRoom(r1,c1))){
                c1--;
                dun[r1][c1]=' ';
            }
        }
        //
        }
        
        
        
        
    }
}
