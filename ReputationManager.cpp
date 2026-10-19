#include "ReputationManager.h"
#include <iomanip>

int ReputationManager::globalFamePoints = 0;

ReputationManager::ReputationManager() {
    currentTitle = "Novice Wanderer";
    checkRank();
}

ReputationManager& ReputationManager::addFame(int pts) {
    globalFamePoints += pts;
    cout << "\n🎖️ [FAME GAINED] +" << pts << " Renown Points! (Total: " << globalFamePoints << " Fame)" << endl;
    return *this; // Slide 05: Cascaded call returning *this
}

ReputationManager& ReputationManager::checkRank() {
    string prev = currentTitle;
    if (globalFamePoints >= 500) currentTitle = "👑 Beast Sovereign / Grandmaster";
    else if (globalFamePoints >= 250) currentTitle = "⚔️ Gladiator of Beasts";
    else if (globalFamePoints >= 100) currentTitle = "🛡️ Colosseum Contender";
    else currentTitle = "🌱 Novice Wanderer";

    if (prev != currentTitle && prev != "") {
        cout << "\n🌟 [RANK PROMOTION!] You have earned the title: " << currentTitle << "!" << endl;
    }
    return *this;
}

int ReputationManager::getFame() { return globalFamePoints; }
void ReputationManager::setFame(int pts) { globalFamePoints = pts; }
string ReputationManager::getTitle() const { return currentTitle; }

ostream& operator<<(ostream& os, const ReputationManager& rm) {
    os << "[" << rm.currentTitle << " | " << ReputationManager::globalFamePoints << " Fame Points]";
    return os;
}
