#include "GameModeSettings.h"
#include "WorldState.h"

bool GameModeSettings::nuzlockeEnabled = false;
bool GameModeSettings::speedrunEnabled = false;
int GameModeSettings::speedrunMaxTurns = 120;

void GameModeSettings::setNuzlocke(bool enable) {
    nuzlockeEnabled = enable;
    if (nuzlockeEnabled) {
        cout << "\n💀 [CHALLENGE MODE ACTIVE] Nuzlocke Permadeath Enabled! Fainted beasts can NEVER be revived." << endl;
    }
}

void GameModeSettings::setSpeedrun(bool enable, int maxTurns) {
    speedrunEnabled = enable;
    speedrunMaxTurns = maxTurns;
    if (speedrunEnabled) {
        cout << "\n⏱️ [CHALLENGE MODE ACTIVE] Speedrun Clock Enabled! Complete the game within " << maxTurns << " turns." << endl;
    }
}

bool GameModeSettings::isNuzlocke() { return nuzlockeEnabled; }
bool GameModeSettings::isSpeedrun() { return speedrunEnabled; }

int GameModeSettings::getTurnsRemaining() {
    if (!speedrunEnabled) return 9999;
    int remaining = speedrunMaxTurns - WorldState::getTotalTurns();
    return (remaining > 0) ? remaining : 0;
}

bool GameModeSettings::checkSpeedrunFail() {
    if (speedrunEnabled && getTurnsRemaining() <= 0) {
        cout << "\n⌛ [TIME EXPIRED] You failed the Speedrun challenge! (Limit: " << speedrunMaxTurns << " turns)" << endl;
        return true;
    }
    return false;
}

string GameModeSettings::getStatusSummary() {
    string summary = "[Rules: Standard]";
    if (nuzlockeEnabled && speedrunEnabled) summary = "[Rules: 💀 Nuzlocke Permadeath + ⏱️ Speedrun (" + to_string(getTurnsRemaining()) + " turns left)]";
    else if (nuzlockeEnabled) summary = "[Rules: 💀 Nuzlocke Permadeath]";
    else if (speedrunEnabled) summary = "[Rules: ⏱️ Speedrun (" + to_string(getTurnsRemaining()) + " turns left)]";
    return summary;
}
