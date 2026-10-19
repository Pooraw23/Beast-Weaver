#ifndef TRAINER_H
#define TRAINER_H

#include <string>
#include <fstream>
#include "Beast.h"
#include "Item.h"
#include "StorageBox.h"

using namespace std;

class Trainer {
private:
    string trainerName;
    
    // Dynamic array for active party (Capped at MAX_PARTY)
    Beast* party;
    int partySize;
    static const int MAX_PARTY = 3;

    // Dynamic array for inventory backpack
    Item* inventory;
    int inventoryCount;
    int inventoryCapacity;

    // Financial wallet and wager
    int gold;
    int prizeMoney;

    // Static data member to track global progress across all Trainer instances
    static int totalBeastsCaught; 

    void resizeInventory(int newCapacity);

public:
    // Player Constructor (Default parameter supported - Slide 04)
    explicit Trainer(string name = "Trainer");
    
    // Custom NPC Constructor with difficulty tier (Requirement 5)
    Trainer(string name, int difficultyTier);

    Trainer(const Trainer& other);
    Trainer& operator=(const Trainer& other);
    ~Trainer(); 

    // Beast catching and party management
    bool catchBeast(const Beast& newBeast, StorageBox& storage);
    bool catchBeast(const Beast& newBeast);
    void printParty() const;
    int getPartySize() const;
    string getName() const;
    void healParty();

    // Battle readiness checks (Requirement 1)
    bool hasUsableBeasts() const;
    int getFirstUsableIndex() const;

    // Storage Box integration
    void swapWithStorage(int partyIndex, int storageIndex, StorageBox& storage);
    void depositToStorage(int partyIndex, StorageBox& storage);
    void withdrawFromStorage(int storageIndex, StorageBox& storage);

    // Inventory operations
    void addItem(const Item& item);
    bool useItem(int itemIndex, Beast& target);
    void printInventory() const;
    int getInventoryCount() const;
    Item& getItem(int index);
    bool hasItem(const Item& item) const; // Uses Item::operator==

    // Binary Operator Overloading for Wallet Management (Requirement 3)
    Trainer& operator+=(int amount); // Wager winnings (e.g. player += npc.getPrizeMoney())
    Trainer& operator-=(int amount); // Shop purchases (e.g. player -= potionPrice)

    // Wallet getters and setters
    int getGold() const;
    int getPrizeMoney() const;
    void setGold(int amount);
    void setPrizeMoney(int amount);

    // Array subscript operator overloading for party indexing
    Beast& operator[](int index); 
    const Beast& operator[](int index) const; 

    // Static member functions
    static int getTotalCaught(); 
    static void setTotalCaught(int count);

    // File stream operators for persistent save/load
    friend ofstream& operator<<(ofstream& out, const Trainer& t);
    friend ifstream& operator>>(ifstream& in, Trainer& t);
};

#endif
