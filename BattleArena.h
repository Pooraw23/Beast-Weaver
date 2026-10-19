#ifndef BATTLEARENA_H
#define BATTLEARENA_H

#include <iostream>
#include <string>
#include "Trainer.h"
#include "StorageBox.h"
#include "Damage_Calculator.h"
#include "WorldState.h"

using namespace std;

// Forward declarations (Slide 02)
class AchievementSystem;
class Bestiary;

class BattleArena {
private:
    string arenaName;

    // NPC AI logic with cascaded member function calls (Slide 05)
    void executeNpcTurn(Trainer& npc, int activeNpcIdx, Trainer& player, int activePlayerIdx);

public:
    BattleArena(string name = "Grand Beast Colosseum");
    ~BattleArena();

    // Visual ASCII Battle HUD renderers (Direction 4)
    static string renderHpBar(int current, int max, int length = 16);
    static void renderDuelHud(const Trainer& player, int activePlayerIdx, const Trainer& opponent, int activeOpponentIdx, string location);
    static void renderWildHud(const Trainer& player, int activePlayerIdx, const Beast& wild, string location);
    static void renderWildHud(const string& trainerName, const Beast& playerBeast, const Beast& wild, string location);
    static void printAllCapturedBeasts(const Trainer& player, const StorageBox& pcBox);

    // Runs a turn-based Trainer vs Trainer arena battle passing references (Requirement 1)
    void startMatch(Trainer& player, Trainer& npc, AchievementSystem* achievements = nullptr, Bestiary* dex = nullptr);

    // Runs a wild beast battle encounter with zone support and capture mechanics
    void startWildEncounter(Trainer& player, StorageBox& pcBox, int zone = 1, AchievementSystem* achievements = nullptr, Bestiary* dex = nullptr);
};

#endif
