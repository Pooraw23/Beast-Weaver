#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

#include "Beast.h"
#include "Item.h"
#include "Trainer.h"
#include "StorageBox.h"
#include "Damage_Calculator.h"
#include "BattleArena.h"
#include "WorldState.h"

using namespace std;

// Direction 4: Function prototypes for UI and World State
void saveGame(const Trainer& player, const StorageBox& pcBox);
bool loadGame(Trainer& player, StorageBox& pcBox);
void storageMenu(Trainer& player, StorageBox& pcBox);
void inventoryMenu(Trainer& player);
void shopMenu(Trainer& player);
void npcChallengeMenu(Trainer& player, BattleArena& arena);
void exploreWildernessMenu(Trainer& player, StorageBox& pcBox, BattleArena& arena);
void campMenu(Trainer& player, StorageBox& pcBox);

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    cout << "==========================================================================" << endl;
    cout << "             🌲 🌋 🌊  WELCOME TO BEAST WEAVER  🌊 🌋 🌲                 " << endl;
    cout << "              Object-Oriented Tactical RPG Adventure                     " << endl;
    cout << "==========================================================================" << endl;

    Trainer player("Hero");
    player.setGold(150); // Starts with 150 Gold
    StorageBox pcBox(10);
    BattleArena arena("Imperial Colosseum");
    bool gameLoaded = false;

    // Check for existing save.txt (Requirement 1)
    ifstream checkFile("save.txt");
    if (checkFile.is_open()) {
        checkFile.close();
        cout << "\n[SAVE FOUND] A saved adventure was detected in save.txt!" << endl;
        cout << "1. Resume Saved Game\n2. Start New Adventure\nChoose: ";
        int startChoice;
        if (cin >> startChoice && startChoice == 1) {
            gameLoaded = loadGame(player, pcBox);
        }
    }

    if (!gameLoaded) {
        cout << "\nEnter your Trainer Name: ";
        string tName;
        cin >> tName;
        player = Trainer(tName);
        player.setGold(150);

        cout << "\nChoose your starter Beast companion:" << endl;
        cout << "1. 🔥 Pyrosaur   (Fire  - High Attack, Aggressive DPS)" << endl;
        cout << "2. 💧 Aquafang   (Water - High HP, Stun Crowd Control)" << endl;
        cout << "3. 🌿 Florataur  (Grass - Balanced Stats, Toxic Spores)" << endl;
        cout << "Select starter (1-3): ";
        int sChoice;
        cin >> sChoice;

        Beast starter;
        if (sChoice == 1) {
            starter = Beast("Pyrosaur", 0, 55, 18, 12, 0, 1, 0);
        } else if (sChoice == 2) {
            starter = Beast("Aquafang", 1, 65, 14, 10, 0, 1, 0);
        } else {
            starter = Beast("Florataur", 2, 60, 15, 11, 0, 1, 0);
        }

        player.catchBeast(starter, pcBox);

        // Initial inventory backpack
        player.addItem(Item("HealthPotion", "HEAL", 25, "Restores 25 HP."));
        player.addItem(Item("AttackElixir", "ATTACK", 4, "Increases base attack by 4."));
        player.addItem(Item("Antidote", "CURE", 0, "Cures poison, burn, and stun ailments."));
        player.addItem(Item("UltraNet", "TRAP", 15, "Captures beasts with higher health."));
    }

    bool running = true;
    while (running) {
        WorldState ws;
        cout << "\n==========================================================================" << endl;
        cout << "                        BEAST WEAVER: OVERWORLD MAP                       " << endl;
        cout << "==========================================================================" << endl;
        cout << " " << ws << endl;
        cout << " Trainer: " << player.getName() 
             << " | Gold: " << player.getGold() << "G"
             << " | Active Party: " << player.getPartySize() << "/3"
             << " | Stored in PC: " << pcBox.getCount()
             << " | Caught: " << Trainer::getTotalCaught() << endl;
        cout << "--------------------------------------------------------------------------" << endl;
        cout << "  [1] 🌲 🌋 🌊 Explore Wild Habitats  - Venture into Canopy, Crags, or Reef" << endl;
        cout << "  [2] 🏛️ Imperial Colosseum          - Wager Gold vs Rival NPC Trainers" << endl;
        cout << "  [3] 🏪 The Grand BeastMart          - Buy Potions, Boosters & Ultra Nets" << endl;
        cout << "  [4] 🏕️ Oakhaven Basecamp           - Rest (Day/Night), Heal, PC Storage" << endl;
        cout << "  [5] 🎒 Open Backpack                - Use Consumables & Inspect Items" << endl;
        cout << "  [6] 📜 Inspect Active Team          - View Level, Stats & Status" << endl;
        cout << "  [7] 💾 Save Adventure               - Serialize Progress to save.txt" << endl;
        cout << "  [8] 🚪 Exit Game" << endl;
        cout << "--------------------------------------------------------------------------" << endl;
        cout << "Choose destination (1-8): ";

        int mainChoice;
        if (!(cin >> mainChoice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        switch (mainChoice) {
            case 1:
                exploreWildernessMenu(player, pcBox, arena);
                break;
            case 2:
                npcChallengeMenu(player, arena);
                break;
            case 3:
                shopMenu(player);
                break;
            case 4:
                campMenu(player, pcBox);
                break;
            case 5:
                inventoryMenu(player);
                break;
            case 6:
                player.printParty();
                break;
            case 7:
                saveGame(player, pcBox);
                break;
            case 8:
                cout << "\nSave before exiting? (1 = Yes, 2 = No): ";
                int exitSave;
                cin >> exitSave;
                if (exitSave == 1) {
                    saveGame(player, pcBox);
                }
                cout << "\nThank you for exploring Beast Weaver! Farewell, Game Master!" << endl;
                running = false;
                break;
            default:
                cout << "Invalid destination! Please choose 1-8." << endl;
                break;
        }
    }

    return 0;
}

// Direction 4: World Map Zone Expedition Menu
void exploreWildernessMenu(Trainer& player, StorageBox& pcBox, BattleArena& arena) {
    cout << "\n==================================================" << endl;
    cout << "           SELECT WILD EXPEDITION ZONE            " << endl;
    cout << "==================================================" << endl;
    cout << "1. 🌲 Verdant Canopy  (Dense Jungle | Grass & Nature Beasts)" << endl;
    cout << "2. 🌋 Obsidian Crags  (Active Volcano | Fire & Magma Beasts)" << endl;
    cout << "3. 🌊 Azure Reef      (Ocean Coastline | Water & Deep Beasts)" << endl;
    cout << "4. Return to Overworld Map" << endl;
    cout << "Select destination (1-4): ";

    int zoneChoice;
    if (cin >> zoneChoice && zoneChoice >= 1 && zoneChoice <= 3) {
        arena.startWildEncounter(player, pcBox, zoneChoice);
    }
}

// Direction 4: Oakhaven Basecamp with Day/Night Rest & PC Storage
void campMenu(Trainer& player, StorageBox& pcBox) {
    bool inCamp = true;
    while (inCamp) {
        WorldState ws;
        cout << "\n==================================================" << endl;
        cout << "               🏕️  OAKHAVEN BASECAMP               " << endl;
        cout << "==================================================" << endl;
        cout << " " << ws << endl;
        cout << "1. 💤 Rest Until Morning (Advance Day & Restore Full Party)" << endl;
        cout << "2. 💖 Nurse Joy's Tent (Heal Party Immediately)" << endl;
        cout << "3. 💻 PC Storage Box Terminal (Deposit/Withdraw/Swap)" << endl;
        cout << "4. Return to Overworld Map" << endl;
        cout << "Choose camp action: ";

        int cChoice;
        if (!(cin >> cChoice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        if (cChoice == 1) {
            WorldState::restUntilMorning();
            player.healParty();
        } else if (cChoice == 2) {
            player.healParty();
        } else if (cChoice == 3) {
            storageMenu(player, pcBox);
        } else if (cChoice == 4) {
            inCamp = false;
        }
    }
}

// Custom Parameterized NPC Constructors & Battle Arena
void npcChallengeMenu(Trainer& player, BattleArena& arena) {
    cout << "\n==================================================" << endl;
    cout << "           COLOSSEUM NPC TOURNAMENT               " << endl;
    cout << "==================================================" << endl;
    cout << "1. Novice Scout Timmy     (Tier 1 | Wager: 60 Gold)" << endl;
    cout << "2. Specialist Hiker Brock (Tier 2 | Wager: 150 Gold)" << endl;
    cout << "3. Elite Champion Cynthia (Tier 3 | Wager: 350 Gold)" << endl;
    cout << "4. Back to Overworld Map" << endl;
    cout << "Choose opponent (1-4): ";

    int tierPick;
    if (!(cin >> tierPick) || tierPick < 1 || tierPick > 4) {
        cin.clear();
        cin.ignore(1000, '\n');
        return;
    }

    if (tierPick == 1) {
        Trainer npc("Scout Timmy", 1);
        arena.startMatch(player, npc);
    } else if (tierPick == 2) {
        Trainer npc("Hiker Brock", 2);
        arena.startMatch(player, npc);
    } else if (tierPick == 3) {
        Trainer npc("Champion Cynthia", 3);
        arena.startMatch(player, npc);
    }
}

// Shop Menu with Binary Operator-=
void shopMenu(Trainer& player) {
    bool inShop = true;
    while (inShop) {
        WorldState ws;
        cout << "\n==================================================" << endl;
        cout << "              🏪  THE GRAND BEASTMART             " << endl;
        cout << "==================================================" << endl;
        cout << " " << ws << endl;
        cout << "Trainer: " << player.getName() << " | Wallet: " << player.getGold() << " Gold" << endl;
        cout << "--------------------------------------------------" << endl;
        cout << "1. Health Potion (+25 HP)        -  30 Gold" << endl;
        cout << "2. Super Potion  (+45 HP)        -  60 Gold" << endl;
        cout << "3. Attack Elixir (+4 Base ATK)   -  50 Gold" << endl;
        cout << "4. Antidote      (Cures Ailments)-  25 Gold" << endl;
        cout << "5. Ultra Net     (+15 Trap Bonus)-  75 Gold" << endl;
        cout << "6. Back to Overworld Map" << endl;
        cout << "Choose item to purchase: ";

        int pick;
        if (!(cin >> pick)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        WorldState::advanceTime(1); // Shopping advances time

        if (pick == 1) {
            int price = 30;
            if (player.getGold() >= price) {
                player -= price;
                player.addItem(Item("HealthPotion", "HEAL", 25, "Restores 25 HP."));
            } else cout << "\n❌ Not enough gold to buy Health Potion!" << endl;
        } else if (pick == 2) {
            int price = 60;
            if (player.getGold() >= price) {
                player -= price;
                player.addItem(Item("SuperPotion", "HEAL", 45, "Restores 45 HP."));
            } else cout << "\n❌ Not enough gold to buy Super Potion!" << endl;
        } else if (pick == 3) {
            int price = 50;
            if (player.getGold() >= price) {
                player -= price;
                player.addItem(Item("AttackElixir", "ATTACK", 4, "Increases base attack by 4."));
            } else cout << "\n❌ Not enough gold to buy Attack Elixir!" << endl;
        } else if (pick == 4) {
            int price = 25;
            if (player.getGold() >= price) {
                player -= price;
                player.addItem(Item("Antidote", "CURE", 0, "Cures poison, burn, and stun ailments."));
            } else cout << "\n❌ Not enough gold to buy Antidote!" << endl;
        } else if (pick == 5) {
            int price = 75;
            if (player.getGold() >= price) {
                player -= price;
                player.addItem(Item("UltraNet", "TRAP", 15, "Captures beasts with higher health."));
            } else cout << "\n❌ Not enough gold to buy Ultra Net!" << endl;
        } else if (pick == 6) {
            inShop = false;
        }
    }
}

void storageMenu(Trainer& player, StorageBox& pcBox) {
    bool inStorage = true;
    while (inStorage) {
        cout << "\n==================================================" << endl;
        cout << "              💻  PC STORAGE SYSTEM               " << endl;
        cout << "==================================================" << endl;
        cout << "Active Party: " << player.getPartySize() << "/3"
             << " | Stored in PC: " << pcBox.getCount() << endl;
        cout << "1. View Active Party" << endl;
        cout << "2. View PC Storage Box" << endl;
        cout << "3. Deposit Beast from Party to PC" << endl;
        cout << "4. Withdraw Beast from PC to Party" << endl;
        cout << "5. Swap Party Beast with PC Beast" << endl;
        cout << "6. Return to Camp" << endl;
        cout << "Choose storage action: ";

        int sChoice;
        if (!(cin >> sChoice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        if (sChoice == 1) {
            player.printParty();
        } else if (sChoice == 2) {
            pcBox.printBox();
        } else if (sChoice == 3) {
            player.printParty();
            cout << "Select party slot to deposit (1-" << player.getPartySize() << "): ";
            int slot;
            cin >> slot;
            player.depositToStorage(slot - 1, pcBox);
        } else if (sChoice == 4) {
            pcBox.printBox();
            if (pcBox.getCount() > 0) {
                cout << "Select PC slot to withdraw (1-" << pcBox.getCount() << "): ";
                int slot;
                cin >> slot;
                player.withdrawFromStorage(slot - 1, pcBox);
            }
        } else if (sChoice == 5) {
            player.printParty();
            pcBox.printBox();
            if (player.getPartySize() > 0 && pcBox.getCount() > 0) {
                cout << "Select Party slot to swap (1-" << player.getPartySize() << "): ";
                int pSlot;
                cin >> pSlot;
                cout << "Select PC slot to swap (1-" << pcBox.getCount() << "): ";
                int bSlot;
                cin >> bSlot;
                player.swapWithStorage(pSlot - 1, bSlot - 1, pcBox);
            } else {
                cout << "\nYou need at least 1 beast in party and 1 beast in storage to swap!" << endl;
            }
        } else if (sChoice == 6) {
            inStorage = false;
        }
    }
}

void inventoryMenu(Trainer& player) {
    bool inInv = true;
    while (inInv) {
        player.printInventory();
        cout << "\n1. Use Item on a Party Beast\n2. Return to Overworld\nChoose: ";
        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        if (choice == 1) {
            if (player.getInventoryCount() == 0) {
                cout << "\nNo items in backpack to use!" << endl;
                continue;
            }
            cout << "Select item number (1-" << player.getInventoryCount() << "): ";
            int itemNum;
            cin >> itemNum;

            player.printParty();
            cout << "Select beast to receive item (1-" << player.getPartySize() << "): ";
            int beastNum;
            cin >> beastNum;

            if (itemNum >= 1 && itemNum <= player.getInventoryCount() &&
                beastNum >= 1 && beastNum <= player.getPartySize()) {
                player.useItem(itemNum - 1, player[beastNum - 1]);
            } else {
                cout << "Invalid selection!" << endl;
            }
        } else {
            inInv = false;
        }
    }
}

void saveGame(const Trainer& player, const StorageBox& pcBox) {
    ofstream outFile("save.txt");
    if (!outFile.is_open()) {
        cout << "\n[ERROR] Could not open save.txt for writing!" << endl;
        return;
    }

    outFile << player;
    outFile << pcBox;
    outFile << Trainer::getTotalCaught() << "\n";
    WorldState ws;
    outFile << ws;

    outFile.close();
    cout << "\n💾 [SAVE SUCCESS] Game state serialized and saved to save.txt successfully!" << endl;
}

bool loadGame(Trainer& player, StorageBox& pcBox) {
    ifstream inFile("save.txt");
    if (!inFile.is_open()) {
        cout << "\n[ERROR] Could not open save.txt for reading!" << endl;
        return false;
    }

    inFile >> player;
    inFile >> pcBox;
    int caught = 0;
    if (inFile >> caught) {
        Trainer::setTotalCaught(caught);
    }
    WorldState ws;
    inFile >> ws;

    inFile.close();
    cout << "\n📂 [LOAD SUCCESS] Game state deserialized from save.txt successfully!" << endl;
    cout << "Welcome back, " << player.getName() << "! Loaded on Day " 
         << WorldState::getDay() << " (" << WorldState::getTimeOfDayString() << ")." << endl;
    return true;
}
