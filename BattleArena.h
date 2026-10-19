#ifndef BATTLEARENA_H
#define BATTLEARENA_H

#include <iostream>
#include <string>
#include "Trainer.h"
#include "StorageBox.h"
#include "Damage_Calculator.h"

using namespace std;

class BattleArena {
private:
    string arenaName;

    // NPC AI logic with cascaded member function calls
    void executeNpcTurn(Trainer& npc, int activeNpcIdx, Trainer& player, int activePlayerIdx);

public:
    BattleArena(string name = "Grand Beast Colosseum");
    ~BattleArena();

    // Runs a turn-based Trainer vs Trainer arena battle passing references (Requirement 1)
    void startMatch(Trainer& player, Trainer& npc);

    // Runs a wild beast battle encounter with capture mechanics
    void startWildEncounter(Trainer& player, StorageBox& pcBox);
};

#endif
