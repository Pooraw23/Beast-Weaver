#include "Beast.h"
#include <cstdlib>

// Default constructor
Beast::Beast() {
    name = "Unknown";
    elementalType = 0;
    hp = 10;
    maxHp = 10;
    attackBase = 5;
    speed = 5;
    statusState = 0;
    level = 1;
    exp = 0;
}

// Parameterized constructor defined outside the class
Beast::Beast(string n, int type, int health, int attack, int spd, int status, int lvl, int xp) {
    name = n;
    elementalType = type;
    hp = health;
    maxHp = health;
    attackBase = attack;
    speed = spd;
    statusState = status;
    level = lvl;
    exp = xp;
}

Beast::~Beast() {
    // Destructor
}

// Cascading member function returning a dereferenced this pointer
Beast& Beast::defend() {
    cout << name << " takes a fortified defensive stance!" << endl;
    return *this; 
}

// Cascading member function for healing
Beast& Beast::heal(int amount) {
    hp += amount;
    if (hp > maxHp) hp = maxHp;
    cout << name << " recovers! HP is now " << hp << "/" << maxHp << endl;
    return *this; 
}

// Compound assignment operator overloading to consume items (e.g. beast += potion)
Beast& Beast::operator+=(const Item& item) {
    if (item.getType() == "HEAL") {
        hp += item.getPower();
        if (hp > maxHp) hp = maxHp;
        cout << "\n>>> " << name << " absorbed " << item.getName() << " and healed " << item.getPower() 
             << " HP! (HP: " << hp << "/" << maxHp << ")" << endl;
    } else if (item.getType() == "ATTACK") {
        attackBase += item.getPower();
        cout << "\n>>> " << name << " absorbed " << item.getName() << " and gained +" << item.getPower() 
             << " Attack Base! (Attack: " << attackBase << ")" << endl;
    } else if (item.getType() == "CURE") {
        statusState = 0;
        cout << "\n>>> " << name << " was cleansed of all status ailments with " << item.getName() << "!" << endl;
    } else {
        cout << "\n>>> " << item.getName() << " had no direct effect on " << name << "." << endl;
    }
    return *this; // Enables cascaded operations
}

// Unary prefix decrement operator for end-of-turn status condition damage tick
Beast& Beast::operator--() {
    if (statusState == 1) { // Poison
        int poisonDmg = 6;
        hp -= poisonDmg;
        if (hp < 0) hp = 0;
        cout << "  [STATUS TICK] " << name << " takes " << poisonDmg 
             << " damage from POISON! (HP: " << hp << "/" << maxHp << ")" << endl;
    } else if (statusState == 2) { // Burn
        int burnDmg = 5;
        hp -= burnDmg;
        if (hp < 0) hp = 0;
        cout << "  [STATUS TICK] " << name << " is scorched by its BURN for " << burnDmg 
             << " damage! (HP: " << hp << "/" << maxHp << ")" << endl;
    } else if (statusState == 3) { // Stun
        cout << "  [STATUS TICK] " << name << " is crackling with static electricity!" << endl;
        // 50% chance stun wears off at end of round
        if (rand() % 2 == 0) {
            statusState = 0;
            cout << "  [STATUS RECOVERY] " << name << " broke free from the STUN!" << endl;
        }
    }
    return *this;
}

// Unary prefix increment operator for Level Up & Stat progression (Requirement 2 - Slide 07)
Beast& Beast::operator++() {
    level++;
    maxHp += 8;
    hp += 8; // Bonus health upon leveling
    attackBase += 3;
    speed += 1;
    cout << "\n✨ [LEVEL UP!] " << name << " grew to Level " << level << "!" << endl;
    cout << "   HP +" << 8 << " (Max HP: " << maxHp << ") | ATK +" << 3 
         << " (" << attackBase << ") | SPD +" << 1 << " (" << speed << ")" << endl;
    return *this; // Slide 07: Prefix increment returns *this by reference
}

void Beast::addExp(int amount) {
    exp += amount;
    cout << "  [EXP] " << name << " gained " << amount << " EXP! (" << exp << "/" << (level * 25) << ")" << endl;
    while (exp >= (level * 25)) {
        exp -= (level * 25);
        ++(*this); // Prefix increment invocation
    }
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

int Beast::getMaxHp() const {
    return maxHp;
}

int Beast::getAttack() const {
    return attackBase;
}

int Beast::getSpeed() const {
    return speed;
}

int Beast::getElementalType() const {
    return elementalType;
}

int Beast::getStatus() const {
    return statusState;
}

string Beast::getStatusString() const {
    switch (statusState) {
        case 1: return "Poisoned";
        case 2: return "Burned";
        case 3: return "Stunned";
        default: return "Healthy";
    }
}

int Beast::getLevel() const {
    return level;
}

int Beast::getExp() const {
    return exp;
}

void Beast::setStatus(int s) {
    statusState = s;
}

void Beast::setHp(int health) {
    hp = health;
    if (hp > maxHp) hp = maxHp;
    if (hp < 0) hp = 0;
}

void Beast::restoreHealth() {
    hp = maxHp;
    statusState = 0;
}

// Terminal stream insertion for battle & status display
ostream& operator<<(ostream& output, const Beast& b) {
    string typeName = "Fire";
    if (b.elementalType == 1) typeName = "Water";
    else if (b.elementalType == 2) typeName = "Grass";

    output << "[" << b.name << " (Lv." << b.level << " " << typeName << " | " << b.getStatusString() << ")] - HP: " 
           << b.hp << "/" << b.maxHp << " | ATK: " << b.attackBase << " | SPD: " << b.speed;
    return output;
}

// Dedicated file stream insertion for clean serialization
ofstream& operator<<(ofstream& out, const Beast& b) {
    out << b.name << " " << b.elementalType << " " << b.hp << " " << b.maxHp << " " 
        << b.attackBase << " " << b.speed << " " << b.statusState << " " 
        << b.level << " " << b.exp << "\n";
    return out;
}

// Dedicated file stream extraction for deserialization
ifstream& operator>>(ifstream& in, Beast& b) {
    in >> b.name >> b.elementalType >> b.hp >> b.maxHp >> b.attackBase >> b.speed 
       >> b.statusState >> b.level >> b.exp;
    return in;
}
