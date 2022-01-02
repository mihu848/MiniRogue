// Game.cpp

#include "Game.h"
#include "utilities.h"
#include "level.h"
#include "actors.h"
#include <iostream>
using namespace std;

// Implement these and other Game member functions you may have added.

Game::Game(int goblinSmellDistance)
{
    //create new level
    lev= new Level(goblinSmellDistance);
    //place the player on the board
    int rPlayer = randInt(1, 18);
    int cPlayer = randInt(1, 70);
    do
        {
        rPlayer = randInt(1, 18);
        cPlayer = randInt(1, 70);
    } while (!lev->addPlayer(rPlayer, cPlayer));
    player = lev->player();
}

void Game::play()
{
    char quit=' ';
    while(quit!='q'&&quit!='w'){ //while takePlayerTurn() doesn't say the player has quit or won
        
        if(quit!='p'){ //takePlayerTurn() entered sequence (attack or item interaction) that took care of refreshing display, so pause refresh
        clearScreen();
        lev->display();
        }
        
    if(!player->isDead()){ //while player isn't dead, take a turn
       
        if(player->isAsleep()){//if player is asleep, keep going through new turns until waking up
            do{
                quit = getCharacter(); //take in character, but don't go to player turn
                if(quit=='q')//quit the game
                    return;
                quit = monsterActions(); //have the monsters attack and move, will be q if death or p to pause screen refresh
                if(quit=='q') //game ends because player died
                    return;
                player->decreaseSleep(); //
            }while(player->isAsleep());
        }
        
        quit = takePlayerTurn();
        if(quit=='w'){ //player wins
            cout<<"\nCongratulations, you won!"<<endl;
            cout<<"Press q to quit."<<endl;
            char end=getCharacter();
            while(end!='q'){ //while not quitting
                end = getCharacter();
            }
            return;
        }
            if(quit=='q'||player->isDead())//if monsters killed player after attack sequence
            return;
            if(quit!='p'){//if monster attack sequence or item pickup sequence has not already triggered monsters to attack or move
                quit = monsterActions();
            }
        }
    }
}

char Game::monsterActions(){  //takes care of monsters attacking and printing out hit/miss/which monsters and telling the level to move the monsters
    string monsterAttacks = "";
    monsterAttacks=lev->monstersAttack();
    lev->monstersMove();
        if(monsterAttacks!=""){ 
        clearScreen();
        lev->display();
        cout<<endl<<monsterAttacks;
            if(player->isDead()){ //if the attack kills the player
                char end = getCharacter();
                while(end!='q'){ //while not quitting, wait for q
                    end = getCharacter();
                }
                return 'q'; //end program
            }
        return 'p'; //prevent a screen refresh after monster attack information is shown
        }
    return ' ';//no monster attacks, no result that needs to be preserved on the screen
}

char Game::playerAttacks(int r, int c){ //when the player attacks a monster
    string battle="";
    
    battle=(player->attackResult(player->attack(lev->whichMonster(r,c)),lev->whichMonster(r,c),lev))+"\n"; //develop the results text that pops up under stats
    battle+=lev->monstersAttack(); //track what the monsters do to the player
    if(player->isDead()){ //player died
        clearScreen();
        lev->display();
        cout<<endl<<battle<<endl;
        char end = getCharacter();
        while(end!='q'){ //while not quitting
            end = getCharacter();
        }
        return 'q';
    }
    lev->monstersMove(); //let monsters move after the attack sequence
    clearScreen();
    lev->display();
    cout<<endl<<battle<<endl; //display results text
    //takePlayerTurn(); //take another turn
    //return 'a';
    return 'p'; //pause a screen refresh
    
}

char Game::itemInteract(string result){//when the player opens inventory, wields weapon, reads scroll
    string monsterAttacks=""; //take monster attacks and save to print to screen after inventory window is closed
    char delay;
    monsterAttacks=lev->monstersAttack();
    lev->monstersMove();
        if(monsterAttacks!=""){
            if(result =="i"){
                delay = getCharacter();
            }
        clearScreen();
        lev->display();
        
            if(result !="i")
        cout<<result;
            
        cout<<endl<<monsterAttacks;
            if(player->isDead()){
                char end = getCharacter();
                while(end!='q'){ //while not quitting
                    end = getCharacter();
                }
                return 'q';
            }
        }
        else if(result == "i"){
            delay = getCharacter();
            clearScreen();
            lev->display();
            delay = ' ';
            return delay;
        }
        else if(result!=""){
            clearScreen();
            lev->display();
            cout<<result;
            return 'p';
        }
        else{
           clearScreen();
            lev->display();
            return 'p';
        }
    return 'p';
}

char Game::takePlayerTurn() //uses user input to the keyboard in order to determine what the turn will be spent on
{
        char move = getCharacter();
        int r = player->getRow();
        int c = player ->getCol();
        switch(move){
            case ARROW_UP:
                r-=1;
                if(lev->canPlaceActor(r,c)){
                    player->setPos(r,c);
                }
                else if(lev->getStatus(r,c)==MONSTER){//if player walked into a monster
                    return playerAttacks(r,c);
                }
                break;
            case ARROW_DOWN:
                r+=1;
                if(lev->canPlaceActor(r,c))
                    player->setPos(r,c);
                if(lev->getStatus(r,c)==MONSTER){//if player walked into a monster
                    return playerAttacks(r,c);
                }
                break;
            case ARROW_LEFT:
                 c-=1;
                if(lev->canPlaceActor(r,c))
                    player->setPos(r,c);
                if(lev->getStatus(r,c)==MONSTER){//if player walked into a monster
                    return playerAttacks(r,c);
                }
                break;
            case ARROW_RIGHT:
                c+=1;
                if(lev->canPlaceActor(r,c))
                    player->setPos(r,c);
                if(lev->getStatus(r,c)==MONSTER){//if player walked into a monster
                    return playerAttacks(r,c);
                }
                break;
            case 'q':
                return 'q';
                break;
            case '>':
                if(lev->getStatus(r, c)==STAIRS){
                    lev->nextDungeon();
                }
                return 'p';
                break;
            case 'c':
                player->cheat();
                break;
            case 'g':
                if(lev->getStatus(r, c)==IDOL)
                    return 'w';
                return itemInteract(lev->pickUpItem());
                break;
            case 'i':
                return itemInteract(player->displayInventory());
                break;
            case 'r':
                return itemInteract(player->readScroll(lev));
                break;
            case 'w':
                return itemInteract(player->wieldWeapon());
                return 'p';
                break;
        }
    return ' ';
}


Game::~Game(){ //delete level
    delete lev;
}

// You will presumably add to this project other .h/.cpp files for the
// various classes of objects you need to play the game:  player, monsters,
// weapons, etc.  You might have a separate .h/.cpp pair for each class
// (e.g., Player.h, Boegeyman.h, etc.), or you might put the class
// declarations for all actors in Actor.h, all game objects in GameObject.h,
// etc.
