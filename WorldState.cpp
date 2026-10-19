#include "WorldState.h"
#include <cstdlib>

// Initialize static member variables
int WorldState::globalTurnCount = 0;
int WorldState::currentDay = 1;
int WorldState::timePeriod = 0;   // 0: Morning, 1: Afternoon, 2: Dusk, 3: Night
int WorldState::weatherState = 0; // 0: Clear, 1: Scorching Sun, 2: Torrential Rain, 3: Mystic Fog

void WorldState::advanceTime(int turns) {
    globalTurnCount += turns;
    timePeriod += turns;
    
    // Random chance of weather shift when time advances
    if (rand() % 100 < 35) {
        weatherState = rand() % 4;
    }

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
    weatherState = rand() % 4;
    cout << "\n💤 [REST] You slept through the night. It is now Morning on Day " 
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

string WorldState::getWeatherString() {
    switch (weatherState) {
        case 0: return "Clear Skies";
        case 1: return "Scorching Sun";
        case 2: return "Torrential Rain";
        case 3: return "Mystic Fog";
        default: return "Calm";
    }
}

string WorldState::getWeatherIcon() {
    switch (weatherState) {
        case 0: return "🌤️";
        case 1: return "☀️🔥";
        case 2: return "🌧️💧";
        case 3: return "🌫️🌿";
        default: return "✨";
    }
}

int WorldState::getDay() {
    return currentDay;
}

int WorldState::getTimePeriod() {
    return timePeriod;
}

int WorldState::getWeather() {
    return weatherState;
}

bool WorldState::isNight() {
    return (timePeriod == 3);
}

int WorldState::getTotalTurns() {
    return globalTurnCount;
}

void WorldState::setWorldTime(int day, int period, int turns, int weather) {
    currentDay = day;
    timePeriod = period % 4;
    globalTurnCount = turns;
    weatherState = weather % 4;
}

// UI Stream insertion for Global HUD Banner
ostream& operator<<(ostream& os, const WorldState& ws) {
    (void)ws;
    os << "[" << WorldState::getTimeIcon() << " Day " << WorldState::currentDay 
       << " - " << WorldState::getTimeOfDayString() 
       << " | Weather: " << WorldState::getWeatherIcon() << " " << WorldState::getWeatherString()
       << " | Turn #" << WorldState::globalTurnCount << "]";
    return os;
}

// File Stream insertion for saving
ofstream& operator<<(ofstream& out, const WorldState& ws) {
    (void)ws;
    out << WorldState::currentDay << " " << WorldState::timePeriod << " " 
        << WorldState::globalTurnCount << " " << WorldState::weatherState << "\n";
    return out;
}

// File Stream extraction for loading
ifstream& operator>>(ifstream& in, WorldState& ws) {
    (void)ws;
    int d = 1, p = 0, t = 0, w = 0;
    if (in >> d >> p >> t >> w) {
        WorldState::setWorldTime(d, p, t, w);
    }
    return in;
}
