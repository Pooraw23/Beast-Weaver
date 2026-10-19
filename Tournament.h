#ifndef TOURNAMENT_H
#define TOURNAMENT_H

#include <iostream>
#include <string>
#include "Trainer.h"
#include "StorageBox.h"
#include "BattleArena.h"
#include "Achievement.h"
#include "Bestiary.h"

using namespace std;

class Tournament {
private:
    string tournamentName;
    BattleArena arena; // Class Composition (Slide 05)
    int championshipGrandPrize;

public:
    Tournament(string name = "Grand Imperial Cup", int grandPrize = 500);
    ~Tournament();

    // Runs a sequential multi-stage championship passing references (Slide 05)
    bool enterTournament(Trainer& player, StorageBox& pcBox, AchievementSystem& achievements, Bestiary& dex);
};

#endif
