#include "WorldState.h"
#include <iomanip>

// Initialize static member variables (one class-wide copy)
int WorldState::globalTurnCount = 0;
int WorldState::currentDay = 1;
int WorldState::timePeriod = 0; // 0: Morning, 1: Afternoon, 2: Dusk, 3: Night

void WorldState::advanceTime(int turns) {
    globalTurnCount += turns;
    timePeriod += turns;
    while (timePeriod >= 4) {
        timePeriod -= 4;
        currentDay++;
        cout << "\n🌅 [NEW DAY] The sun rises! A new day has dawned: Day " << currentDay << "." << endl;
    }
}

void WorldState::restUntilMorning() {
    timePeriod = 0;
    currentDay++;
    globalTurnCount += 2;
    cout << "\n💤 [REST] You slept through the night. It is now a crisp Morning on Day " 
         << currentDay << "!" << endl;
}

string WorldState::getTimeOfDayString() {
    switch (timePeriod) {
        case 0: return "Morning";
        case 1: return "Afternoon";
        case 2: return "Dusk";
        case 3: return "Night";
        default: return "Unknown";
    }
}

string WorldState::getTimeIcon() {
    switch (timePeriod) {
        case 0: return "🌅";
        case 1: return "☀️";
        case 2: return "🌇";
        case 3: return "🌙";
        default: return "⏰";
    }
}

int WorldState::getDay() {
    return currentDay;
}

int WorldState::getTimePeriod() {
    return timePeriod;
}

bool WorldState::isNight() {
    return (timePeriod == 3);
}

int WorldState::getTotalTurns() {
    return globalTurnCount;
}

void WorldState::setWorldTime(int day, int period, int turns) {
    currentDay = day;
    timePeriod = period % 4;
    globalTurnCount = turns;
}

// UI Stream insertion for Global HUD Banner
ostream& operator<<(ostream& os, const WorldState& ws) {
    (void)ws; // Reference unused in static context
    os << "[" << WorldState::getTimeIcon() << " Day " << WorldState::currentDay 
       << " - " << WorldState::getTimeOfDayString() << " | Turn #" << WorldState::globalTurnCount << "]";
    return os;
}

// File Stream insertion for saving
ofstream& operator<<(ofstream& out, const WorldState& ws) {
    (void)ws;
    out << WorldState::currentDay << " " << WorldState::timePeriod << " " << WorldState::globalTurnCount << "\n";
    return out;
}

// File Stream extraction for loading
ifstream& operator>>(ifstream& in, WorldState& ws) {
    (void)ws;
    int d = 1, p = 0, t = 0;
    if (in >> d >> p >> t) {
        WorldState::setWorldTime(d, p, t);
    }
    return in;
}
