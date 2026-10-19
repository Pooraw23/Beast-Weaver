#include "Trainer.h"
#include <iostream>
#include <cstdlib>

// Initialize static member; one classwide copy
int Trainer::totalBeastsCaught = 0;

// Parameterized constructor with defaults
Trainer::Trainer(string name) {
    trainerName = name;
    partySize = 0;
    party = nullptr;
    
    inventoryCount = 0;
    inventoryCapacity = 10;
    inventory = new Item[inventoryCapacity];
}

// Copy constructor (Slide 07)
Trainer::Trainer(const Trainer& other) {
    trainerName = other.trainerName;
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
    // Uses overloaded operator+= on Beast! (Requirement 2)
    target += chosen;

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

// Uses Item::operator== to search inventory
bool Trainer::hasItem(const Item& item) const {
    for (int i = 0; i < inventoryCount; i++) {
        if (inventory[i] == item) {
            return true;
        }
    }
    return false;
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
