#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include <iostream>
#include <string>
#include "Trainer.h"
#include "StorageBox.h"
#include "Achievement.h"
#include "Bestiary.h"
#include "WorldState.h"

using namespace std;

struct SlotMetadata {
    int slotNumber;
    bool exists;
    string trainerName;
    int day;
    int gold;
    int partySize;
};

class SaveManager {
private:
    SlotMetadata* slots; // Dynamically allocated array of save profiles (Slide 05 & 07)
    int maxSlots;

    string getSlotFilename(int slot) const;
    void refreshSlotMetadata();

public:
    // Rule of Three (Slide 07)
    SaveManager(int numSlots = 3);
    SaveManager(const SaveManager& other);
    SaveManager& operator=(const SaveManager& other);
    ~SaveManager();

    void printSlots() const;
    bool save(int slot, const Trainer& player, const StorageBox& box, const AchievementSystem& as, const Bestiary& dex);
    bool load(int slot, Trainer& player, StorageBox& box, AchievementSystem& as, Bestiary& dex);
    bool slotExists(int slot) const;

    // Stream operator for multi-slot UI (Slide 06)
    friend ostream& operator<<(ostream& os, const SaveManager& sm);
};

#endif
