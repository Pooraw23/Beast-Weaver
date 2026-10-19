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

using namespace std;

// Function prototypes
void saveGame(const Trainer& player, const StorageBox& pcBox);
bool loadGame(Trainer& player, StorageBox& pcBox);
void storageMenu(Trainer& player, StorageBox& pcBox);
void inventoryMenu(Trainer& player);
void shopMenu(Trainer& player);
void npcChallengeMenu(Trainer& player, BattleArena& arena);

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    cout << "==================================================" << endl;
    cout << "           WELCOME TO BEAST WEAVER               " << endl;
    cout << "==================================================" << endl;

    Trainer player("Hero");
    player.setGold(150); // Starts with 150 Gold
    StorageBox pcBox(10);
    BattleArena arena("Imperial Colosseum");
    bool gameLoaded = false;

    // Check for existing save.txt (Requirement 1)
    ifstream checkFile("save.txt");
    if (checkFile.is_open()) {
        checkFile.close();
        cout << "\n[SAVE FOUND] A saved adventure was detected (save.txt)!" << endl;
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

        cout << "\nChoose your starter Beast:" << endl;
        cout << "1. Pyrosaur   (Fire  - High Attack, Aggressive)" << endl;
        cout << "2. Aquafang   (Water - High HP, Stun Specialists)" << endl;
        cout << "3. Florataur  (Grass - Balanced, Poison Spores)" << endl;
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

        // Initial inventory
        player.addItem(Item("HealthPotion", "HEAL", 25, "Restores 25 HP."));
        player.addItem(Item("AttackElixir", "ATTACK", 4, "Increases base attack by 4."));
        player.addItem(Item("Antidote", "CURE", 0, "Cures poison, burn, and stun ailments."));
        player.addItem(Item("UltraNet", "TRAP", 15, "Captures beasts with higher health."));
    }

    bool running = true;
    while (running) {
        cout << "\n==================================================" << endl;
        cout << "               BEAST WEAVER - MAIN HUB            " << endl;
        cout << "==================================================" << endl;
        cout << "Trainer: " << player.getName() 
             << " | Gold: " << player.getGold() << "G"
             << " | Party: " << player.getPartySize() << "/3"
             << " | PC Box: " << pcBox.getCount()
             << " | Caught: " << Trainer::getTotalCaught() << endl;
        cout << "--------------------------------------------------" << endl;
        cout << "1. Challenge Rival NPC Trainers (BattleArena)" << endl;
        cout << "2. Explore Wild & Battle (Wild Encounter)" << endl;
        cout << "3. Visit Beast Mart / Shop (Currency Overloading)" << endl;
        cout << "4. View Party & Heal Beasts" << endl;
        cout << "5. Open Backpack (Inventory)" << endl;
        cout << "6. Visit PC Storage Box" << endl;
        cout << "7. Save Game (Stream Serialization)" << endl;
        cout << "8. Exit Game" << endl;
        cout << "Choose action (1-8): ";

        int mainChoice;
        if (!(cin >> mainChoice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        switch (mainChoice) {
            case 1:
                npcChallengeMenu(player, arena);
                break;
            case 2:
                arena.startWildEncounter(player, pcBox);
                break;
            case 3:
                shopMenu(player);
                break;
            case 4:
                player.printParty();
                cout << "\nHeal entire active party? (1 = Yes, 2 = No): ";
                int healChoice;
                cin >> healChoice;
                if (healChoice == 1) {
                    player.healParty();
                }
                break;
            case 5:
                inventoryMenu(player);
                break;
            case 6:
                storageMenu(player, pcBox);
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
                cout << "\nThank you for playing Beast Weaver! Goodbye!" << endl;
                running = false;
                break;
            default:
                cout << "Invalid choice! Please choose 1-8." << endl;
                break;
        }
    }

    return 0;
}

// Requirement 5: Custom NPC Constructors instantiated for Battle Arena matches
void npcChallengeMenu(Trainer& player, BattleArena& arena) {
    cout << "\n==========================================" << endl;
    cout << "           SELECT NPC CHALLENGER          " << endl;
    cout << "==========================================" << endl;
    cout << "1. Novice Scout Timmy     (Tier 1 | Wager: 60 Gold)" << endl;
    cout << "2. Specialist Hiker Brock (Tier 2 | Wager: 150 Gold)" << endl;
    cout << "3. Elite Champion Cynthia (Tier 3 | Wager: 350 Gold)" << endl;
    cout << "4. Back to Main Hub" << endl;
    cout << "Choose opponent (1-4): ";

    int tierPick;
    if (!(cin >> tierPick) || tierPick < 1 || tierPick > 4) {
        cin.clear();
        cin.ignore(1000, '\n');
        return;
    }

    if (tierPick == 1) {
        Trainer npc("Scout Timmy", 1); // Parameterized NPC constructor
        arena.startMatch(player, npc); // Passing references to BattleArena
    } else if (tierPick == 2) {
        Trainer npc("Hiker Brock", 2);
        arena.startMatch(player, npc);
    } else if (tierPick == 3) {
        Trainer npc("Champion Cynthia", 3);
        arena.startMatch(player, npc);
    }
}

// Requirement 3: Shop with Binary Operator-=
void shopMenu(Trainer& player) {
    bool inShop = true;
    while (inShop) {
        cout << "\n==========================================" << endl;
        cout << "           BEAST WEAVER MART / SHOP       " << endl;
        cout << "==========================================" << endl;
        cout << "Trainer: " << player.getName() << " | Current Wallet: " << player.getGold() << " Gold" << endl;
        cout << "------------------------------------------" << endl;
        cout << "1. Health Potion (+25 HP)        -  30 Gold" << endl;
        cout << "2. Super Potion  (+45 HP)        -  60 Gold" << endl;
        cout << "3. Attack Elixir (+4 Base ATK)   -  50 Gold" << endl;
        cout << "4. Antidote      (Cures Ailments)-  25 Gold" << endl;
        cout << "5. Ultra Net     (+15 Trap Bonus)-  75 Gold" << endl;
        cout << "6. Back to Main Hub" << endl;
        cout << "Choose item to purchase: ";

        int pick;
        if (!(cin >> pick)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        if (pick == 1) {
            int price = 30;
            if (player.getGold() >= price) {
                player -= price; // Overloaded binary operator-=
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
        cout << "\n==========================================" << endl;
        cout << "           POKEMON / BEAST PC SYSTEM      " << endl;
        cout << "==========================================" << endl;
        cout << "Active Party: " << player.getPartySize() << "/3"
             << " | Stored in PC: " << pcBox.getCount() << endl;
        cout << "1. View Active Party" << endl;
        cout << "2. View PC Storage Box" << endl;
        cout << "3. Deposit Beast from Party to PC" << endl;
        cout << "4. Withdraw Beast from PC to Party" << endl;
        cout << "5. Swap Party Beast with PC Beast" << endl;
        cout << "6. Return to Main Hub" << endl;
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
        cout << "\n1. Use Item on a Party Beast\n2. Return to Main Hub\nChoose: ";
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

    inFile.close();
    cout << "\n📂 [LOAD SUCCESS] Game state deserialized from save.txt successfully!" << endl;
    cout << "Welcome back, " << player.getName() << "!" << endl;
    return true;
}
