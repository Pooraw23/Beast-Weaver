#include "Bestiary.h"
#include <iomanip>
#include <cstdlib>

BestiaryEntry::BestiaryEntry() {
    name = "Unknown";
    elementalType = 0;
    seen = false;
    caught = false;
    lore = "Undiscovered creature.";
}

BestiaryEntry::BestiaryEntry(string bname, int btype, string blore, bool bseen, bool bcaught) {
    name = bname;
    elementalType = btype;
    lore = blore;
    seen = bseen;
    caught = bcaught;
}

Bestiary::Bestiary() {
    count = 12;
    entries = new BestiaryEntry[count];

    // Fire Beasts
    entries[0] = BestiaryEntry("Pyrosaur", 0, "A fiery quadruped beast that spits concentrated embers.");
    entries[1] = BestiaryEntry("Wild-Pyrosaur", 0, "Untamed Pyrosaur roaming active tectonic rifts.");
    entries[2] = BestiaryEntry("Cinderhound", 0, "Fast volcanic hound coated in smoldering coal fur.");
    entries[3] = BestiaryEntry("MagmaDrake", 0, "Heavy armored draconic beast infused with liquid magma.");
    entries[4] = BestiaryEntry("Infernodrake", 0, "Legendary draconic titan commanding blazing infernos.");

    // Water Beasts
    entries[5] = BestiaryEntry("Aquafang", 1, "Serpentine predator with razor teeth that channels oceanic tides.");
    entries[6] = BestiaryEntry("Wild-Aquafang", 1, "Feral Aquafang hunting through coastal coral reefs.");
    entries[7] = BestiaryEntry("TidalCrab", 1, "Armored crustacean capable of absorbing crushing pressures.");
    entries[8] = BestiaryEntry("VaporSerpent", 1, "Elusive mist dweller whose swift strikes paralyze foes.");
    entries[9] = BestiaryEntry("Leviathan", 1, "Colossal oceanic serpent worshipped as the ruler of the deep.");

    // Grass Beasts
    entries[10] = BestiaryEntry("Florataur", 2, "Stout minotaur-like beast bearing toxic flora and vines.");
    entries[11] = BestiaryEntry("MossGolem", 2, "Ancient animated stone covered in healing moss and roots.");
}

Bestiary::Bestiary(const Bestiary& other) {
    count = other.count;
    entries = new BestiaryEntry[count];
    for (int i = 0; i < count; i++) {
        entries[i] = other.entries[i];
    }
}

Bestiary& Bestiary::operator=(const Bestiary& other) {
    if (this != &other) {
        delete[] entries;
        count = other.count;
        entries = new BestiaryEntry[count];
        for (int i = 0; i < count; i++) {
            entries[i] = other.entries[i];
        }
    }
    return *this;
}

Bestiary::~Bestiary() {
    delete[] entries;
}

void Bestiary::recordEncounter(string name, int type) {
    for (int i = 0; i < count; i++) {
        if (entries[i].name == name) {
            if (!entries[i].seen) {
                entries[i].seen = true;
                cout << "\n📖 [BESTIARY DISCOVERY] Encountered new species: " << name << "!" << endl;
            }
            return;
        }
    }
    // Dynamic expansion if new custom beast is encountered
    BestiaryEntry* temp = new BestiaryEntry[count + 1];
    for (int i = 0; i < count; i++) temp[i] = entries[i];
    temp[count] = BestiaryEntry(name, type, "A newly documented beast in the wild.", true, false);
    delete[] entries;
    entries = temp;
    count++;
    cout << "\n📖 [BESTIARY DISCOVERY] Added new catalog entry: " << name << "!" << endl;
}

void Bestiary::recordCapture(string name) {
    for (int i = 0; i < count; i++) {
        if (entries[i].name == name) {
            entries[i].seen = true;
            if (!entries[i].caught) {
                entries[i].caught = true;
                cout << "📖 [BESTIARY LOG] Captured and logged data for: " << name << "!" << endl;
            }
            return;
        }
    }
}

BestiaryEntry& Bestiary::operator[](int index) {
    if (index >= 0 && index < count) return entries[index];
    cout << "Bestiary index out of bounds!" << endl;
    abort();
}

const BestiaryEntry& Bestiary::operator[](int index) const {
    if (index >= 0 && index < count) return entries[index];
    cout << "Bestiary index out of bounds!" << endl;
    abort();
}

int Bestiary::getCount() const { return count; }

int Bestiary::getTotalSeen() const {
    int total = 0;
    for (int i = 0; i < count; i++) if (entries[i].seen) total++;
    return total;
}

int Bestiary::getTotalCaught() const {
    int total = 0;
    for (int i = 0; i < count; i++) if (entries[i].caught) total++;
    return total;
}

// Direction 4: ASCII Pokédex Encyclopedia Display
ostream& operator<<(ostream& os, const Bestiary& b) {
    os << "\n╔══════════════════════════════════════════════════════════════════════════════╗\n";
    os << "║                     📖  BEAST WEAVER POKÉDEX / BESTIARY                      ║\n";
    os << "║                 Species Seen: [" << b.getTotalSeen() << "/" << b.count 
       << "]  |  Species Caught: [" << b.getTotalCaught() << "/" << b.count << "]               ║\n";
    os << "╠══════════════════════════════════════════════════════════════════════════════╣\n";

    for (int i = 0; i < b.count; i++) {
        string statusIcon = "❓ [UNKNOWN] ";
        string elem = "???";
        string displayName = "???";
        string desc = "Data not yet discovered in the wild.";

        if (b.entries[i].seen) {
            statusIcon = b.entries[i].caught ? "⭐ [CAUGHT]  " : "👁️  [SEEN]    ";
            displayName = b.entries[i].name;
            elem = (b.entries[i].elementalType == 0) ? "🔥 FIRE" : 
                   (b.entries[i].elementalType == 1 ? "💧 WATER" : "🌿 GRASS");
            desc = b.entries[i].lore;
        }

        os << "║ #" << right << setw(2) << (i + 1) << " " << statusIcon << " "
           << left << setw(14) << displayName << " (" << setw(8) << elem << ") "
           << "- " << setw(38) << desc << "║\n";
    }
    os << "╚══════════════════════════════════════════════════════════════════════════════╝\n";
    return os;
}

ofstream& operator<<(ofstream& out, const Bestiary& b) {
    out << b.count << "\n";
    for (int i = 0; i < b.count; i++) {
        out << b.entries[i].name << " " << b.entries[i].elementalType << " "
            << (b.entries[i].seen ? 1 : 0) << " " << (b.entries[i].caught ? 1 : 0) << "\n";
    }
    return out;
}

ifstream& operator>>(ifstream& in, Bestiary& b) {
    int savedCount = 0;
    if (in >> savedCount) {
        for (int i = 0; i < savedCount; i++) {
            string sname;
            int stype, sseen, scaught;
            if (in >> sname >> stype >> sseen >> scaught) {
                for (int j = 0; j < b.count; j++) {
                    if (b.entries[j].name == sname) {
                        b.entries[j].seen = (sseen == 1);
                        b.entries[j].caught = (scaught == 1);
                        break;
                    }
                }
            }
        }
    }
    return in;
}
