#include "WorldMap.h"
#include <iomanip>
#include <cstdlib>

void WorldMap::initializeGrid() {
    grid = new char*[rows];
    for (int i = 0; i < rows; i++) {
        grid[i] = new char[cols];
        for (int j = 0; j < cols; j++) {
            grid[i][j] = '.'; // Plains
        }
    }

    // Fixed landmark positions
    grid[3][5] = 'T'; // Oakhaven Town (Center)
    grid[3][6] = 'M'; // Grand BeastMart
    grid[2][5] = 'C'; // Imperial Colosseum

    // Wilderness Zones
    grid[0][1] = 'F'; grid[0][2] = 'F'; grid[1][1] = 'F'; grid[1][2] = 'F'; // Verdant Canopy (Northwest)
    grid[1][9] = 'V'; grid[1][10] = 'V'; grid[2][10] = 'V';               // Obsidian Crags (Northeast)
    grid[6][2] = 'O'; grid[6][3] = 'O'; grid[7][3] = 'O'; grid[7][4] = 'O'; // Azure Reef (South)
}

WorldMap::WorldMap(int r, int c) : rows(r), cols(c), playerR(3), playerC(5) {
    initializeGrid();
}

WorldMap::WorldMap(const WorldMap& other) : rows(other.rows), cols(other.cols), playerR(other.playerR), playerC(other.playerC) {
    grid = new char*[rows];
    for (int i = 0; i < rows; i++) {
        grid[i] = new char[cols];
        for (int j = 0; j < cols; j++) {
            grid[i][j] = other.grid[i][j];
        }
    }
}

WorldMap& WorldMap::operator=(const WorldMap& other) {
    if (this != &other) {
        for (int i = 0; i < rows; i++) delete[] grid[i];
        delete[] grid;

        rows = other.rows;
        cols = other.cols;
        playerR = other.playerR;
        playerC = other.playerC;

        grid = new char*[rows];
        for (int i = 0; i < rows; i++) {
            grid[i] = new char[cols];
            for (int j = 0; j < cols; j++) {
                grid[i][j] = other.grid[i][j];
            }
        }
    }
    return *this;
}

WorldMap::~WorldMap() {
    for (int i = 0; i < rows; i++) {
        delete[] grid[i];
    }
    delete[] grid;
}

char WorldMap::move(char direction) {
    int newR = playerR;
    int newC = playerC;

    if (direction == 'N' || direction == 'n' || direction == 'w') newR--;
    else if (direction == 'S' || direction == 's' || direction == 's') newR++;
    else if (direction == 'E' || direction == 'e' || direction == 'd') newC++;
    else if (direction == 'W' || direction == 'w' || direction == 'a') newC--;

    if (newR >= 0 && newR < rows && newC >= 0 && newC < cols) {
        playerR = newR;
        playerC = newC;
    } else {
        cout << "\n🚫 You hit the natural boundary of the realm!" << endl;
    }

    return getCurrentTile();
}

char WorldMap::getCurrentTile() const {
    return grid[playerR][playerC];
}

string WorldMap::getTileDescription() const {
    char tile = getCurrentTile();
    switch (tile) {
        case 'T': return "🏡 Oakhaven Town (Safe haven, Healer & PC Storage)";
        case 'M': return "🏪 The Grand BeastMart (Potions & Traps)";
        case 'C': return "🏛️ Imperial Colosseum (Trainer Arenas & Tournaments)";
        case 'F': return "🌲 Verdant Canopy (Dense grass habitat)";
        case 'V': return "🌋 Obsidian Crags (Scorching magma peaks)";
        case 'O': return "🌊 Azure Reef (Shimmering oceanic shore)";
        default:  return "🌾 Open Wildlands (Rolling meadows)";
    }
}

int WorldMap::getPlayerRow() const { return playerR; }
int WorldMap::getPlayerCol() const { return playerC; }

char& WorldMap::operator()(int r, int c) {
    if (r >= 0 && r < rows && c >= 0 && c < cols) {
        return grid[r][c];
    }
    cout << "Map coordinate out of bounds!" << endl;
    abort();
}

const char& WorldMap::operator()(int r, int c) const {
    if (r >= 0 && r < rows && c >= 0 && c < cols) {
        return grid[r][c];
    }
    cout << "Map coordinate out of bounds!" << endl;
    abort();
}

ostream& operator<<(ostream& os, const WorldMap& map) {
    os << "\n╔══════════════════════════════════════════════════════════════════════════════╗\n";
    os << "║                     🗺️  2D EXPEDITION OVERWORLD MAP  🗺️                     ║\n";
    os << "╠══════════════════════════════════════════════════════════════════════════════╣\n";

    for (int i = 0; i < map.rows; i++) {
        os << "║                 ";
        for (int j = 0; j < map.cols; j++) {
            if (i == map.playerR && j == map.playerC) {
                os << " [@]"; // Player
            } else {
                os << " [" << map.grid[i][j] << "]";
            }
        }
        os << "                 ║\n";
    }

    os << "╠══════════════════════════════════════════════════════════════════════════════╣\n";
    os << "║ LEGEND: [@] You  | [T] Town  | [M] Mart  | [C] Arena  | [F] Canopy            ║\n";
    os << "║         [V] Crags | [O] Reef  | [.] Plains                                   ║\n";
    os << "║ CURRENT LOCATION: " << left << setw(58) << map.getTileDescription() << "║\n";
    os << "╚══════════════════════════════════════════════════════════════════════════════╝\n";
    return os;
}
