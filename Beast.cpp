#include "Beast.h"

// Default constructor
Beast::Beast() {
    name = "Unknown";
    elementalType = 0;
    hp = 10;
    maxHp = 10;
    attackBase = 5;
    speed = 5;
}

// Parameterized constructor defined outside the class[cite: 32]
Beast::Beast(string n, int type, int health, int attack, int spd) {
    name = n;
    elementalType = type;
    hp = health;
    maxHp = health;
    attackBase = attack;
    speed = spd;
}

Beast::~Beast() {
    // Destructor (No dynamic memory inside Beast itself, so this remains empty)
}

// Cascading member function returning a dereferenced this pointer[cite: 43, 67]
Beast& Beast::defend() {
    cout << name << " takes a defensive stance!" << endl;
    // Implementation of defense buff would go here
    return *this; 
}

// Cascading member function[cite: 43]
Beast& Beast::heal() {
    hp += 10;
    if (hp > maxHp) hp = maxHp;
    cout << name << " heals! HP is now " << hp << "/" << maxHp << endl;
    return *this; 
}

bool Beast::isFainted() const {
    return hp <= 0;
}

string Beast::getName() const {
    return name;
}

int Beast::getHp() const {
    return hp;
}

// Overloaded output operator as a friend function[cite: 71]
ostream& operator<<(ostream& output, const Beast& b) {
    string typeName = "";
    if (b.elementalType == 0) typeName = "Fire";
    else if (b.elementalType == 1) typeName = "Water";
    else if (b.elementalType == 2) typeName = "Grass";

    output << "[" << b.name << " (" << typeName << ")] - HP: " << b.hp << "/" << b.maxHp;
    return output; // Enables cascading[cite: 71]
}
