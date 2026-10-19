#ifndef TRAINERCARD_H
#define TRAINERCARD_H

#include <iostream>
#include <string>
#include "Trainer.h"
#include "Bestiary.h"
#include "Achievement.h"
#include "WorldState.h"
#include "ReputationManager.h"

using namespace std;

class TrainerCard {
private:
    // Class Composition of References (Slide 05)
    const Trainer& player;
    const Bestiary& dex;
    const AchievementSystem& achievements;
    const WorldState& world;
    const ReputationManager& reputation;

public:
    TrainerCard(const Trainer& p, const Bestiary& d, const AchievementSystem& a, const WorldState& w, const ReputationManager& r);

    // Stream operator for visual Trainer ID Card display (Slide 06)
    friend ostream& operator<<(ostream& os, const TrainerCard& tc);
};

#endif
