#include "SaveManager.h"
#include <fstream>
#include <iomanip>

string SaveManager::getSlotFilename(int slot) const {
    return "save_slot" + to_string(slot) + ".txt";
}

void SaveManager::refreshSlotMetadata() {
    for (int i = 0; i < maxSlots; i++) {
        int slotNum = i + 1;
        slots[i].slotNumber = slotNum;
        slots[i].exists = false;
        slots[i].trainerName = "[Empty Slot]";
        slots[i].day = 1;
        slots[i].gold = 0;
        slots[i].partySize = 0;

        string fname = getSlotFilename(slotNum);
        ifstream in(fname);
        if (in.is_open()) {
            string tname;
            int g, pm, psize;
            if (in >> tname >> g >> pm >> psize) {
                slots[i].exists = true;
                slots[i].trainerName = tname;
                slots[i].gold = g;
                slots[i].partySize = psize;
            }
            in.close();
        }
    }
}

SaveManager::SaveManager(int numSlots) : maxSlots(numSlots > 0 ? numSlots : 3) {
    slots = new SlotMetadata[maxSlots];
    refreshSlotMetadata();
}

SaveManager::SaveManager(const SaveManager& other) : maxSlots(other.maxSlots) {
    slots = new SlotMetadata[maxSlots];
    for (int i = 0; i < maxSlots; i++) {
        slots[i] = other.slots[i];
    }
}

SaveManager& SaveManager::operator=(const SaveManager& other) {
    if (this != &other) {
        delete[] slots;
        maxSlots = other.maxSlots;
        slots = new SlotMetadata[maxSlots];
        for (int i = 0; i < maxSlots; i++) {
            slots[i] = other.slots[i];
        }
    }
    return *this;
}

SaveManager::~SaveManager() {
    delete[] slots;
}

void SaveManager::printSlots() const {
    cout << *this;
}

bool SaveManager::save(int slot, const Trainer& player, const StorageBox& box, const AchievementSystem& as, const Bestiary& dex) {
    if (slot < 1 || slot > maxSlots) {
        cout << "Invalid save slot!" << endl;
        return false;
    }

    string fname = getSlotFilename(slot);
    ofstream out(fname);
    if (!out.is_open()) {
        cout << "Could not open " << fname << " for writing!" << endl;
        return false;
    }

    out << player;
    out << box;
    out << Trainer::getTotalCaught() << "\n";
    WorldState ws;
    out << ws;
    out << as;
    out << dex;
    out.close();

    refreshSlotMetadata();
    cout << "\n💾 [SAVED] Progress serialized into " << fname << " (Slot " << slot << ") successfully!" << endl;
    return true;
}

bool SaveManager::load(int slot, Trainer& player, StorageBox& box, AchievementSystem& as, Bestiary& dex) {
    if (slot < 1 || slot > maxSlots || !slots[slot - 1].exists) {
        cout << "Cannot load: Slot is empty or invalid!" << endl;
        return false;
    }

    string fname = getSlotFilename(slot);
    ifstream in(fname);
    if (!in.is_open()) {
        cout << "Could not open " << fname << " for reading!" << endl;
        return false;
    }

    in >> player;
    in >> box;
    int caught = 0;
    if (in >> caught) Trainer::setTotalCaught(caught);
    WorldState ws;
    in >> ws;
    in >> as;
    in >> dex;
    in.close();

    cout << "\n📂 [LOADED] Profile restored from " << fname << " (Slot " << slot << ")!" << endl;
    return true;
}

bool SaveManager::slotExists(int slot) const {
    if (slot < 1 || slot > maxSlots) return false;
    return slots[slot - 1].exists;
}

ostream& operator<<(ostream& os, const SaveManager& sm) {
    os << "\n╔══════════════════════════════════════════════════════════════════════════════╗\n";
    os << "║                     💾  SAVED ADVENTURE PROFILE SLOTS  💾                    ║\n";
    os << "╠══════════════════════════════════════════════════════════════════════════════╣\n";

    for (int i = 0; i < sm.maxSlots; i++) {
        string statusTag = sm.slots[i].exists ? "🌟 [ACTIVE] " : "⚪ [EMPTY]  ";
        os << "║ " << statusTag << " Slot " << sm.slots[i].slotNumber << ": " << left << setw(20) << sm.slots[i].trainerName;
        if (sm.slots[i].exists) {
            os << " (Party: " << sm.slots[i].partySize << "/3 | Gold: " << setw(4) << sm.slots[i].gold << "G)           ║\n";
        } else {
            os << " - Ready for New Game                          ║\n";
        }
    }
    os << "╚══════════════════════════════════════════════════════════════════════════════╝\n";
    return os;
}
