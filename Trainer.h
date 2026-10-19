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

    // Static data member to track global progress across all Trainer instances
    static int totalBeastsCaught; 

    void resizeInventory(int newCapacity);

public:
    // Constructors and Destructor (Rule of Three - Slide 07)
    explicit Trainer(string name = "Trainer");
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
