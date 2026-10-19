#ifndef BEAST_H
#define BEAST_H

#include <iostream>
#include <fstream>
#include <string>
#include "Item.h"

using namespace std;

// Forward declaration
class DamageCalculator; 

class Beast {
    // Friend declarations allowing external classes and file streams direct access to private data
    friend class DamageCalculator; 
    friend ostream& operator<<(ostream& output, const Beast& b); 
    friend ofstream& operator<<(ofstream& out, const Beast& b);
    friend ifstream& operator>>(ifstream& in, Beast& b);

private:
    string name;
    int elementalType; // 0 = Fire, 1 = Water, 2 = Grass
    int hp;
    int maxHp;
    int attackBase;
    int speed;
    int statusState;   // 0 = Normal, 1 = Poison, 2 = Burn, 3 = Stun
    int level;         // Experience and Leveling
    int exp;

public:
    // Constructors and Destructor
    Beast(); // Default constructor
    Beast(string n, int type, int health, int attack, int spd, int status = 0, int lvl = 1, int xp = 0);
    ~Beast(); // Destructor 

    // Cascading member functions using the 'this' pointer
    Beast& defend(); 
    Beast& heal(int amount = 10);   

    // Compound assignment operator overloading for using items
    Beast& operator+=(const Item& item);

    // Unary prefix decrement operator for end-of-turn status condition tick
    Beast& operator--();

    // Unary prefix increment operator for Level Up & Stat progression (Requirement 2)
    Beast& operator++();

    // Standard public getters and utilities
    bool isFainted() const;
    string getName() const;
    int getHp() const;
    int getMaxHp() const;
    int getAttack() const;
    int getSpeed() const;
    int getElementalType() const;
    int getStatus() const;
    string getStatusString() const;
    int getLevel() const;
    int getExp() const;

    // Mutators
    void setStatus(int s);
    void setHp(int health);
    void restoreHealth();
    void addExp(int amount);
};

#endif
