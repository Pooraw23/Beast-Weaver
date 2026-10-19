#ifndef BATTLELOG_H
#define BATTLELOG_H

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class BattleLog {
private:
    string* events;     // Dynamically allocated array of combat events (Slide 05)
    int count;
    int capacity;

    void resize(int newCap);

public:
    // Rule of Three (Slide 07)
    BattleLog(int initialCap = 20);
    BattleLog(const BattleLog& other);
    BattleLog& operator=(const BattleLog& other);
    ~BattleLog();

    // Log management
    void addEvent(const string& ev);
    void clear();
    int getCount() const;
    void exportToFile(const string& filename) const;

    // Stream operator for battle feed terminal view (Slide 06)
    friend ostream& operator<<(ostream& os, const BattleLog& log);
};

#endif
