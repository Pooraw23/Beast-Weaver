#ifndef ITEM_H
#define ITEM_H

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Item {
private:
    string name;
    string type;       // "HEAL", "ATTACK", "CURE", "TRAP"
    int power;         // Value of healing, attack buff, or trap bonus
    string description;

public:
    // Constructors and Destructor
    Item();
    Item(string itemName, string itemType, int itemPower, string itemDesc);
    ~Item();

    // Getters
    string getName() const;
    string getType() const;
    int getPower() const;
    string getDescription() const;

    // Relational operator overloading for inventory search
    bool operator==(const Item& other) const;
    bool operator!=(const Item& other) const;

    // Stream operator overloading
    friend ostream& operator<<(ostream& os, const Item& item);
    friend ofstream& operator<<(ofstream& out, const Item& item);
    friend ifstream& operator>>(ifstream& in, Item& item);
};

#endif
