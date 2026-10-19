#include "Item.h"

// Default constructor
Item::Item() {
    name = "Empty";
    type = "NONE";
    power = 0;
    description = "No description.";
}

// Parameterized constructor
Item::Item(string itemName, string itemType, int itemPower, string itemDesc) {
    name = itemName;
    type = itemType;
    power = itemPower;
    description = itemDesc;
}

Item::~Item() {
    // No dynamic memory in Item
}

string Item::getName() const {
    return name;
}

string Item::getType() const {
    return type;
}

int Item::getPower() const {
    return power;
}

string Item::getDescription() const {
    return description;
}

// Overloaded equality operator to search inventory
bool Item::operator==(const Item& other) const {
    return (name == other.name && type == other.type);
}

// Overloaded inequality operator
bool Item::operator!=(const Item& other) const {
    return !(*this == other);
}

// Generic stream insertion (console display)
ostream& operator<<(ostream& os, const Item& item) {
    os << "[" << item.name << " (" << item.type << " +" << item.power << ")] - " << item.description;
    return os;
}

// File stream insertion (clean serialization)
ofstream& operator<<(ofstream& out, const Item& item) {
    out << item.name << " " << item.type << " " << item.power << "\n";
    return out;
}

// File stream extraction (deserialization)
ifstream& operator>>(ifstream& in, Item& item) {
    in >> item.name >> item.type >> item.power;
    if (item.type == "HEAL") item.description = "Restores HP in battle.";
    else if (item.type == "ATTACK") item.description = "Boosts base attack power.";
    else if (item.type == "CURE") item.description = "Cleanses all negative status effects.";
    else if (item.type == "TRAP") item.description = "Improves capture rate on wild beasts.";
    else item.description = "Consumable item.";
    return in;
}
