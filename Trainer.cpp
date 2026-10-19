#include "Trainer.h"
#include <iostream>
#include <cstdlib>

// Initialize static member; one classwide copy
int Trainer::totalBeastsCaught = 0;

// Parameterized constructor with default values
Trainer::Trainer(string name) {
    trainerName = name;
    gold = 100;
    prizeMoney = 0;
    partySize = 0;
    party = nullptr;
    
    inventoryCount = 0;
    inventoryCapacity = 10;
    inventory = new Item[inventoryCapacity];
}

// Custom NPC Constructor dynamically populating party based on difficulty tier (Requirement 5)
Trainer::Trainer(string name, int difficultyTier) {
    trainerName = name;
    inventoryCount = 0;
    inventoryCapacity = 5;
    inventory = new Item[inventoryCapacity];

    if (difficultyTier == 1) { // Novice Tier
        gold = 60;
        prizeMoney = 60;
        partySize = 2;
        party = new Beast[partySize];
        party[0] = Beast("Rattasaur", 0, 36, 13, 10, 0, 2, 0);
        party[1] = Beast("Pebblefin", 1, 42, 11, 8, 0, 2, 0);
    } else if (difficultyTier == 2) { // Intermediate Tier (Hiker / Specialist)
        gold = 150;
        prizeMoney = 150;
        partySize = 2;
        party = new Beast[partySize];
        party[0] = Beast("Boulderfang", 2, 58, 17, 9, 0, 5, 0);
        party[1] = Beast("Pyroclast", 0, 52, 19, 14, 0, 5, 0);
    } else if (difficultyTier >= 3) { // Elite Tier (Champion)
        gold = 350;
        prizeMoney = 350;
        partySize = 3;
        party = new Beast[partySize];
        party[0] = Beast("Leviathan", 1, 75, 22, 16, 0, 8, 0);
        party[1] = Beast("Infernodrake", 0, 72, 25, 18, 0, 8, 0);
        party[2] = Beast("ThornColossus", 2, 85, 20, 12, 0, 9, 0);
    } else {
        gold = 30;
        prizeMoney = 30;
        partySize = 1;
        party = new Beast[partySize];
        party[0] = Beast("Wildling", 0, 30, 10, 8, 0, 1, 0);
    }
}

// Copy constructor (Slide 07)
Trainer::Trainer(const Trainer& other) {
    trainerName = other.trainerName;
    gold = other.gold;
    prizeMoney = other.prizeMoney;
    partySize = other.partySize;
    if (partySize > 0) {
        party = new Beast[partySize];
        for (int i = 0; i < partySize; i++) {
            party[i] = other.party[i];
        }
    } else {
        party = nullptr;
    }

    inventoryCount = other.inventoryCount;
    inventoryCapacity = other.inventoryCapacity;
    inventory = new Item[inventoryCapacity];
    for (int i = 0; i < inventoryCount; i++) {
        inventory[i] = other.inventory[i];
    }
}

// Overloaded assignment operator with self-assignment check (Slide 05 & 07)
Trainer& Trainer::operator=(const Trainer& other) {
    if (this != &other) {
        delete[] party;
        delete[] inventory;

        trainerName = other.trainerName;
        gold = other.gold;
        prizeMoney = other.prizeMoney;
        partySize = other.partySize;
        if (partySize > 0) {
            party = new Beast[partySize];
            for (int i = 0; i < partySize; i++) {
                party[i] = other.party[i];
            }
        } else {
            party = nullptr;
        }

        inventoryCount = other.inventoryCount;
        inventoryCapacity = other.inventoryCapacity;
        inventory = new Item[inventoryCapacity];
        for (int i = 0; i < inventoryCount; i++) {
            inventory[i] = other.inventory[i];
        }
    }
    return *this;
}

// Destructor releasing both dynamic arrays
Trainer::~Trainer() {
    delete[] party;
    delete[] inventory;
}

// Helper to expand inventory backpack
void Trainer::resizeInventory(int newCapacity) {
    Item* temp = new Item[newCapacity];
    for (int i = 0; i < inventoryCount; i++) {
        temp[i] = inventory[i];
    }
    delete[] inventory;
    inventory = temp;
    inventoryCapacity = newCapacity;
}

// Catch beast with StorageBox integration (3-beast active party cap)
bool Trainer::catchBeast(const Beast& newBeast, StorageBox& storage) {
    ++totalBeastsCaught;
    if (partySize < MAX_PARTY) {
        Beast* tempArray = new Beast[partySize + 1];
        for (int i = 0; i < partySize; i++) {
            tempArray[i] = party[i];
        }
        tempArray[partySize] = newBeast;
        delete[] party;
        party = tempArray;
        partySize++;
        cout << "\n>>> " << trainerName << " caught " << newBeast.getName() 
             << "! Added to active party (" << partySize << "/" << MAX_PARTY << ")." << endl;
        return true;
    } else {
        cout << "\n>>> Active party is FULL (" << partySize << "/" << MAX_PARTY 
             << ")! Sending " << newBeast.getName() << " directly to PC Storage Box..." << endl;
        storage.addBeast(newBeast);
        return true;
    }
}

bool Trainer::catchBeast(const Beast& newBeast) {
    if (partySize < MAX_PARTY) {
        Beast* tempArray = new Beast[partySize + 1];
        for (int i = 0; i < partySize; i++) {
            tempArray[i] = party[i];
        }
        tempArray[partySize] = newBeast;
        delete[] party;
        party = tempArray;
        partySize++;
        ++totalBeastsCaught;
        cout << "\n>>> " << trainerName << " caught " << newBeast.getName() << "!" << endl;
        return true;
    } else {
        cout << "\n>>> Party full! Beast could not be added without a PC Storage Box." << endl;
        return false;
    }
}

// Requirement 1: Checks if trainer has at least one conscious beast
bool Trainer::hasUsableBeasts() const {
    for (int i = 0; i < partySize; i++) {
        if (!party[i].isFainted()) {
            return true;
        }
    }
    return false;
}

// Returns index of the first conscious beast in the party, or -1 if all fainted
int Trainer::getFirstUsableIndex() const {
    for (int i = 0; i < partySize; i++) {
        if (!party[i].isFainted()) {
            return i;
        }
    }
    return -1;
}

void Trainer::printParty() const {
    cout << "\n==========================================" << endl;
    cout << "           " << trainerName << "'s ACTIVE PARTY (" << partySize << "/" << MAX_PARTY << ")       " << endl;
    cout << "==========================================" << endl;
    if (partySize == 0) {
        cout << "  (Party is empty!)" << endl;
    } else {
        for (int i = 0; i < partySize; i++) {
            cout << "  [" << (i + 1) << "] " << party[i] << endl;
        }
    }
    cout << "==========================================" << endl;
}

int Trainer::getPartySize() const {
    return partySize;
}

string Trainer::getName() const {
    return trainerName;
}

void Trainer::healParty() {
    for (int i = 0; i < partySize; i++) {
        party[i].restoreHealth();
    }
    cout << "\n>>> All beasts in " << trainerName << "'s party have been fully healed!" << endl;
}

void Trainer::swapWithStorage(int partyIndex, int storageIndex, StorageBox& storage) {
    if (partyIndex < 0 || partyIndex >= partySize) {
        cout << "Invalid party slot!" << endl;
        return;
    }
    if (storageIndex < 0 || storageIndex >= storage.getCount()) {
        cout << "Invalid storage slot!" << endl;
        return;
    }

    Beast fromStorage = storage.removeBeast(storageIndex);
    Beast fromParty = party[partyIndex];
    party[partyIndex] = fromStorage;
    storage.addBeast(fromParty);

    cout << "\n>>> Swapped party beast " << fromParty.getName() << " with stored beast " 
         << fromStorage.getName() << "!" << endl;
}

void Trainer::depositToStorage(int partyIndex, StorageBox& storage) {
    if (partySize <= 1) {
        cout << "\n>>> Cannot deposit! You must keep at least 1 beast in your active party." << endl;
        return;
    }
    if (partyIndex < 0 || partyIndex >= partySize) {
        cout << "\n>>> Invalid party slot!" << endl;
        return;
    }

    Beast toDeposit = party[partyIndex];

    Beast* temp = new Beast[partySize - 1];
    int idx = 0;
    for (int i = 0; i < partySize; i++) {
        if (i != partyIndex) {
            temp[idx++] = party[i];
        }
    }
    delete[] party;
    party = temp;
    partySize--;

    storage.addBeast(toDeposit);
    cout << "\n>>> Successfully deposited " << toDeposit.getName() << " into PC Storage Box!" << endl;
}

void Trainer::withdrawFromStorage(int storageIndex, StorageBox& storage) {
    if (partySize >= MAX_PARTY) {
        cout << "\n>>> Active party is already full (3/3)! Deposit a beast or use Swap instead." << endl;
        return;
    }
    if (storageIndex < 0 || storageIndex >= storage.getCount()) {
        cout << "\n>>> Invalid storage slot!" << endl;
        return;
    }

    Beast withdrawn = storage.removeBeast(storageIndex);

    Beast* temp = new Beast[partySize + 1];
    for (int i = 0; i < partySize; i++) {
        temp[i] = party[i];
    }
    temp[partySize] = withdrawn;
    delete[] party;
    party = temp;
    partySize++;

    cout << "\n>>> Withdrew " << withdrawn.getName() << " into active party!" << endl;
}

void Trainer::addItem(const Item& item) {
    if (inventoryCount >= inventoryCapacity) {
        resizeInventory(inventoryCapacity * 2);
    }
    inventory[inventoryCount] = item;
    inventoryCount++;
    cout << "Added item: " << item.getName() << " to backpack." << endl;
}

bool Trainer::useItem(int itemIndex, Beast& target) {
    if (itemIndex < 0 || itemIndex >= inventoryCount) {
        cout << "Invalid item selection!" << endl;
        return false;
    }

    Item chosen = inventory[itemIndex];
    target += chosen; // Uses Beast::operator+=

    // Remove consumed item and shift array
    for (int i = itemIndex; i < inventoryCount - 1; i++) {
        inventory[i] = inventory[i + 1];
    }
    inventoryCount--;
    return true;
}

void Trainer::printInventory() const {
    cout << "\n==========================================" << endl;
    cout << "            " << trainerName << "'s BACKPACK             " << endl;
    cout << "==========================================" << endl;
    if (inventoryCount == 0) {
        cout << "  (Backpack is empty!)" << endl;
    } else {
        for (int i = 0; i < inventoryCount; i++) {
            cout << "  [" << (i + 1) << "] " << inventory[i] << endl;
        }
    }
    cout << "==========================================" << endl;
}

int Trainer::getInventoryCount() const {
    return inventoryCount;
}

Item& Trainer::getItem(int index) {
    if (index >= 0 && index < inventoryCount) {
        return inventory[index];
    } else {
        cout << "Inventory: Index out of bounds!" << endl;
        abort();
    }
}

bool Trainer::hasItem(const Item& item) const {
    for (int i = 0; i < inventoryCount; i++) {
        if (inventory[i] == item) {
            return true;
        }
    }
    return false;
}

// Binary Operator Overloading for Currency (Requirement 3)
Trainer& Trainer::operator+=(int amount) {
    gold += amount;
    cout << "💰 [WALLET] " << trainerName << " received " << amount 
         << " Gold! Total: " << gold << " Gold." << endl;
    return *this;
}

Trainer& Trainer::operator-=(int amount) {
    gold -= amount;
    if (gold < 0) gold = 0;
    cout << "💳 [PAYMENT] " << trainerName << " spent " << amount 
         << " Gold. Remaining: " << gold << " Gold." << endl;
    return *this;
}

int Trainer::getGold() const {
    return gold;
}

int Trainer::getPrizeMoney() const {
    return prizeMoney;
}

void Trainer::setGold(int amount) {
    gold = amount;
}

void Trainer::setPrizeMoney(int amount) {
    prizeMoney = amount;
}

Beast& Trainer::operator[](int index) {
    if (index >= 0 && index < partySize) {
        return party[index];
    } else {
        cout << "Trainer party index out of bounds!" << endl;
        abort();
    }
}

const Beast& Trainer::operator[](int index) const {
    if (index >= 0 && index < partySize) {
        return party[index];
    } else {
        cout << "Trainer party index out of bounds!" << endl;
        abort();
    }
}

int Trainer::getTotalCaught() {
    return totalBeastsCaught;
}

void Trainer::setTotalCaught(int count) {
    totalBeastsCaught = count;
}

ofstream& operator<<(ofstream& out, const Trainer& t) {
    out << t.trainerName << "\n";
    out << t.gold << " " << t.prizeMoney << "\n";
    out << t.partySize << "\n";
    for (int i = 0; i < t.partySize; i++) {
        out << t.party[i]; // Beast file stream overload
    }
    out << t.inventoryCount << "\n";
    for (int i = 0; i < t.inventoryCount; i++) {
        out << t.inventory[i]; // Item file stream overload
    }
    return out;
}

ifstream& operator>>(ifstream& in, Trainer& t) {
    in >> t.trainerName;
    in >> t.gold >> t.prizeMoney;
    int loadedPartySize = 0;
    if (in >> loadedPartySize) {
        delete[] t.party;
        t.partySize = loadedPartySize;
        t.party = new Beast[t.partySize];
        for (int i = 0; i < t.partySize; i++) {
            in >> t.party[i];
        }
    }

    int loadedInvCount = 0;
    if (in >> loadedInvCount) {
        delete[] t.inventory;
        t.inventoryCount = loadedInvCount;
        t.inventoryCapacity = (loadedInvCount > 10) ? loadedInvCount + 5 : 10;
        t.inventory = new Item[t.inventoryCapacity];
        for (int i = 0; i < t.inventoryCount; i++) {
            in >> t.inventory[i];
        }
    }
    return in;
}
