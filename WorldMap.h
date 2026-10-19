#ifndef WORLDMAP_H
#define WORLDMAP_H

#include <iostream>
#include <string>

using namespace std;

class WorldMap {
private:
    char** grid;       // 2D dynamic char array (Slide 05 & 07)
    int rows;
    int cols;
    int playerR;
    int playerC;

    void initializeGrid();

public:
    // Rule of Three (Slide 07)
    WorldMap(int r = 8, int c = 12);
    WorldMap(const WorldMap& other);
    WorldMap& operator=(const WorldMap& other);
    ~WorldMap();

    // Movement controls returning new tile type
    char move(char direction); // 'N', 'S', 'E', 'W' or 'w', 'a', 's', 'd'
    char getCurrentTile() const;
    string getTileDescription() const;
    int getPlayerRow() const;
    int getPlayerCol() const;

    // Subscript operator overloading with bounds checking (Slide 07)
    char& operator()(int r, int c);
    const char& operator()(int r, int c) const;

    // Stream operator for 2D ASCII Map UI (Slide 06)
    friend ostream& operator<<(ostream& os, const WorldMap& map);
};

#endif
