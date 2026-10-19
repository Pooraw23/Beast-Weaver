#include "Achievement.h"
#include <iomanip>

int AchievementSystem::totalUnlockedCount = 0;
int AchievementSystem::totalMilestones = 7;

Achievement::Achievement() {
    id = "";
    title = "";
    description = "";
    unlocked = false;
}

Achievement::Achievement(string aid, string atitle, string adesc, bool aunlocked) {
    id = aid;
    title = atitle;
    description = adesc;
    unlocked = aunlocked;
}

AchievementSystem::AchievementSystem() {
    count = 7;
    badges = new Achievement[count];

    badges[0] = Achievement("FIRST_BLOOD", "First Blood", "Win your first battle against a wild beast or trainer.");
    badges[1] = Achievement("PARTY_FULL", "Beast Master", "Assemble a full active party of 3 beasts.");
    badges[2] = Achievement("PC_HOARDER", "Digital Collector", "Transfer at least one captured beast to the PC Storage Box.");
    badges[3] = Achievement("DEEP_POCKETS", "Golden Hoard", "Accumulate 300 or more Gold in your trainer wallet.");
    badges[4] = Achievement("VETERAN_BEAST", "Ascended Warrior", "Train any companion beast to reach Level 3 or higher.");
    badges[5] = Achievement("CHAMPION_SLAYER", "Arena Conqueror", "Defeat Elite Champion Cynthia in the Imperial Colosseum.");
    badges[6] = Achievement("NIGHT_HUNTER", "Shadow Stalker", "Survive a battle encounter during the dark of Night.");

    // Update static count
    totalUnlockedCount = 0;
    for (int i = 0; i < count; i++) {
        if (badges[i].unlocked) totalUnlockedCount++;
    }
}

// Copy Constructor
AchievementSystem::AchievementSystem(const AchievementSystem& other) {
    count = other.count;
    badges = new Achievement[count];
    for (int i = 0; i < count; i++) {
        badges[i] = other.badges[i];
    }
}

// Assignment Operator
AchievementSystem& AchievementSystem::operator=(const AchievementSystem& other) {
    if (this != &other) {
        delete[] badges;
        count = other.count;
        badges = new Achievement[count];
        for (int i = 0; i < count; i++) {
            badges[i] = other.badges[i];
        }
    }
    return *this;
}

AchievementSystem::~AchievementSystem() {
    delete[] badges;
}

bool AchievementSystem::unlock(string id) {
    for (int i = 0; i < count; i++) {
        if (badges[i].id == id && !badges[i].unlocked) {
            badges[i].unlocked = true;
            totalUnlockedCount++;
            cout << "\n🏆 [ACHIEVEMENT UNLOCKED!] ★ " << badges[i].title << " ★" << endl;
            cout << "   \"" << badges[i].description << "\"" << endl;
            return true;
        }
    }
    return false;
}

void AchievementSystem::checkMilestones(const Trainer& player, const StorageBox& pcBox) {
    if (player.getPartySize() >= 3) unlock("PARTY_FULL");
    if (pcBox.getCount() >= 1) unlock("PC_HOARDER");
    if (player.getGold() >= 300) unlock("DEEP_POCKETS");
    for (int i = 0; i < player.getPartySize(); i++) {
        if (player[i].getLevel() >= 3) unlock("VETERAN_BEAST");
    }
}

int AchievementSystem::getTotalUnlocked() {
    return totalUnlockedCount;
}

int AchievementSystem::getTotalMilestones() {
    return totalMilestones;
}

// Direction 4: Formatted ASCII Trophy Case
ostream& operator<<(ostream& os, const AchievementSystem& as) {
    os << "\n╔══════════════════════════════════════════════════════════════════════════════╗\n";
    os << "║                     🏆  TRAINER TROPHY CASE & BADGES  🏆                     ║\n";
    os << "║                 Progress: [" << as.totalUnlockedCount << "/" << as.count << " Milestones Unlocked]                   ║\n";
    os << "╠══════════════════════════════════════════════════════════════════════════════╣\n";

    for (int i = 0; i < as.count; i++) {
        string statusIcon = as.badges[i].unlocked ? "🌟 [UNLOCKED]" : "🔒 [LOCKED]  ";
        os << "║ " << statusIcon << " " << left << setw(20) << as.badges[i].title 
           << " - " << setw(44) << as.badges[i].description << "║\n";
    }
    os << "╚══════════════════════════════════════════════════════════════════════════════╝\n";
    return os;
}

ofstream& operator<<(ofstream& out, const AchievementSystem& as) {
    out << as.count << "\n";
    for (int i = 0; i < as.count; i++) {
        out << as.badges[i].id << " " << (as.badges[i].unlocked ? 1 : 0) << "\n";
    }
    return out;
}

ifstream& operator>>(ifstream& in, AchievementSystem& as) {
    int savedCount = 0;
    if (in >> savedCount) {
        AchievementSystem::totalUnlockedCount = 0;
        for (int i = 0; i < savedCount; i++) {
            string sid;
            int u;
            if (in >> sid >> u) {
                for (int j = 0; j < as.count; j++) {
                    if (as.badges[j].id == sid) {
                        as.badges[j].unlocked = (u == 1);
                        if (as.badges[j].unlocked) AchievementSystem::totalUnlockedCount++;
                        break;
                    }
                }
            }
        }
    }
    return in;
}
