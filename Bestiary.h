#ifndef BESTIARY_H
#define BESTIARY_H

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class BestiaryEntry {
public:
    string name;
    int elementalType; // 0 = Fire, 1 = Water, 2 = Grass
    bool seen;
    bool caught;
    string lore;

    BestiaryEntry();
    BestiaryEntry(string bname, int btype, string blore, bool bseen = false, bool bcaught = false);
};

class Bestiary {
private:
    BestiaryEntry* entries;
    int count;

public:
    // Rule of Three (Slide 07)
    Bestiary();
    Bestiary(const Bestiary& other);
    Bestiary& operator=(const Bestiary& other);
    ~Bestiary();

    // Registry hooks
    void recordEncounter(string name, int type = 0);
    void recordCapture(string name);

    // Subscript operator overloading with bounds checking
    BestiaryEntry& operator[](int index);
    const BestiaryEntry& operator[](int index) const;

    int getCount() const;
    int getTotalSeen() const;
    int getTotalCaught() const;

    // Stream operator for Pokédex ASCII encyclopedia UI (Slide 06)
    friend ostream& operator<<(ostream& os, const Bestiary& b);
    friend ofstream& operator<<(ofstream& out, const Bestiary& b);
    friend ifstream& operator>>(ifstream& in, Bestiary& b);
};

#endif
