#ifndef BEAST_H
#define BEAST_H

#include <iostream>
#include <string>

using namespace std;

// Forward declaration so the compiler knows this class exists
class DamageCalculator; 

class Beast {
    // Friend declarations allow these specific external entities to access private data
    friend class DamageCalculator; 
    friend ostream& operator<<(ostream& output, const Beast& b); 

private:
    string name;
    int elementalType; // e.g., 0 = Fire, 1 = Water, 2 = Grass
    int hp;
    int maxHp;
    int attackBase;
    int speed;

public:
    // Constructors and Destructors
    Beast(); // Default constructor
    Beast(string n, int type, int health, int attack, int spd); // Parameterized constructor
    ~Beast(); // Destructor 

    // Cascading member functions using the 'this' pointer
    Beast& defend(); 
    Beast& heal();   

    // Standard public getters
    bool isFainted() const;
    string getName() const;
    int getHp() const;
};

#endif
