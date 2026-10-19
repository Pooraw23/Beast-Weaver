#include "BattleLog.h"
#include <iomanip>

BattleLog::BattleLog(int initialCap) {
    capacity = (initialCap > 0) ? initialCap : 20;
    count = 0;
    events = new string[capacity];
}

BattleLog::BattleLog(const BattleLog& other) {
    capacity = other.capacity;
    count = other.count;
    events = new string[capacity];
    for (int i = 0; i < count; i++) {
        events[i] = other.events[i];
    }
}

BattleLog& BattleLog::operator=(const BattleLog& other) {
    if (this != &other) {
        delete[] events;
        capacity = other.capacity;
        count = other.count;
        events = new string[capacity];
        for (int i = 0; i < count; i++) {
            events[i] = other.events[i];
        }
    }
    return *this;
}

BattleLog::~BattleLog() {
    delete[] events;
}

void BattleLog::resize(int newCap) {
    string* temp = new string[newCap];
    for (int i = 0; i < count; i++) {
        temp[i] = events[i];
    }
    delete[] events;
    events = temp;
    capacity = newCap;
}

void BattleLog::addEvent(const string& ev) {
    if (count >= capacity) {
        resize(capacity * 2);
    }
    events[count++] = ev;
}

void BattleLog::clear() {
    count = 0;
}

int BattleLog::getCount() const {
    return count;
}

void BattleLog::exportToFile(const string& filename) const {
    ofstream out(filename);
    if (!out.is_open()) {
        cout << "Could not open " << filename << " to export battle log!" << endl;
        return;
    }
    out << "=== BEAST WEAVER COMBAT REPLAY TRANSCRIPT ===\n";
    for (int i = 0; i < count; i++) {
        out << "[" << (i + 1) << "] " << events[i] << "\n";
    }
    out.close();
    cout << "📜 Exported battle log (" << count << " turns) to " << filename << endl;
}

ostream& operator<<(ostream& os, const BattleLog& log) {
    os << "\n╔══════════════════════════════════════════════════════════════════════════════╗\n";
    os << "║                        📜  BATTLE CHRONICLE LOG  📜                          ║\n";
    os << "╠══════════════════════════════════════════════════════════════════════════════╣\n";
    if (log.count == 0) {
        os << "║  (No actions recorded yet this duel)                                         ║\n";
    } else {
        int start = (log.count > 10) ? log.count - 10 : 0;
        for (int i = start; i < log.count; i++) {
            os << "║ " << left << setw(4) << (i + 1) << ". " << setw(70) << log.events[i] << " ║\n";
        }
    }
    os << "╚══════════════════════════════════════════════════════════════════════════════╝\n";
    return os;
}
