#ifndef ACHIEVEMENT_H
#define ACHIEVEMENT_H

#include <iostream>
#include <fstream>
#include <string>
#include "Trainer.h"
#include "StorageBox.h"

using namespace std;

class Achievement {
public:
    string id;
    string title;
    string description;
    bool unlocked;

    Achievement();
    Achievement(string aid, string atitle, string adesc, bool aunlocked = false);
};

class AchievementSystem {
private:
    Achievement* badges;
    int count;

    // Static variables tracking class-wide achievements (Slide 05)
    static int totalUnlockedCount;
    static int totalMilestones;

public:
    // Rule of Three (Slide 07)
    AchievementSystem();
    AchievementSystem(const AchievementSystem& other);
    AchievementSystem& operator=(const AchievementSystem& other);
    ~AchievementSystem();

    // Check & unlock milestones
    bool unlock(string id);
    void checkMilestones(const Trainer& player, const StorageBox& pcBox);

    static int getTotalUnlocked();
    static int getTotalMilestones();

    // Stream operator for ASCII Trophy Case UI (Slide 06)
    friend ostream& operator<<(ostream& os, const AchievementSystem& as);
    friend ofstream& operator<<(ofstream& out, const AchievementSystem& as);
    friend ifstream& operator>>(ifstream& in, AchievementSystem& as);
};

#endif
