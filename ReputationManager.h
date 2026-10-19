#ifndef REPUTATIONMANAGER_H
#define REPUTATIONMANAGER_H

#include <iostream>
#include <string>

using namespace std;

class ReputationManager {
private:
    static int globalFamePoints; // Static class variable (Slide 05)
    string currentTitle;

public:
    ReputationManager();

    // Cascaded member function returning dereferenced *this (Slide 05)
    ReputationManager& addFame(int pts);
    ReputationManager& checkRank();

    static int getFame();
    static void setFame(int pts);
    string getTitle() const;

    // Stream operator for title card (Slide 06)
    friend ostream& operator<<(ostream& os, const ReputationManager& rm);
};

#endif
