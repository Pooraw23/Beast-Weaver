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

using namespace std;

// Function prototypes
void saveGame(const Trainer& player, const StorageBox& pcBox);
bool loadGame(Trainer& player, StorageBox& pcBox);
void battleEncounter(Trainer& player, StorageBox& pcBox);
void storageMenu(Trainer& player, StorageBox& pcBox);
void inventoryMenu(Trainer& player);
Beast generateWildBeast();

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    cout << "==================================================" << endl;
    cout << "           WELCOME TO BEAST WEAVER               " << endl;
    cout << "==================================================" << endl;

    Trainer player("Hero");
    StorageBox pcBox(10);
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

        cout << "\nChoose your starter Beast:" << endl;
        cout << "1. Pyrosaur   (Fire  - High Attack, Aggressive)" << endl;
        cout << "2. Aquafang   (Water - High HP, Stun Specialists)" << endl;
        cout << "3. Florataur  (Grass - Balanced, Poison Spores)" << endl;
        cout << "Select starter (1-3): ";
        int sChoice;
        cin >> sChoice;

        Beast starter;
        if (sChoice == 1) {
            starter = Beast("Pyrosaur", 0, 55, 18, 12);
        } else if (sChoice == 2) {
            starter = Beast("Aquafang", 1, 65, 14, 10);
        } else {
            starter = Beast("Florataur", 2, 60, 15, 11);
        }

        player.catchBeast(starter, pcBox);

        // Grant initial consumables for backpack (Requirement 2)
        player.addItem(Item("HealthPotion", "HEAL", 25, "Restores 25 HP."));
        player.addItem(Item("SuperPotion", "HEAL", 45, "Restores 45 HP."));
        player.addItem(Item("AttackElixir", "ATTACK", 4, "Increases base attack power by 4."));
        player.addItem(Item("Antidote", "CURE", 0, "Cures poison, burn, and stun ailments."));
        player.addItem(Item("UltraNet", "TRAP", 15, "Captures beasts with higher health threshold."));
    }

    bool running = true;
    while (running) {
        cout << "\n==================================================" << endl;
        cout << "               BEAST WEAVER - MAIN HUB            " << endl;
        cout << "==================================================" << endl;
        cout << "Trainer: " << player.getName() 
             << " | Active Party: " << player.getPartySize() << "/3"
             << " | Stored in PC: " << pcBox.getCount()
             << " | Total Caught: " << Trainer::getTotalCaught() << endl;
        cout << "--------------------------------------------------" << endl;
        cout << "1. Explore Wild & Battle" << endl;
        cout << "2. View Party & Heal Beasts" << endl;
        cout << "3. Open Backpack (Inventory)" << endl;
        cout << "4. Visit PC Storage Box" << endl;
        cout << "5. Save Game (Stream Serialization)" << endl;
        cout << "6. Exit Game" << endl;
        cout << "Choose action (1-6): ";

        int mainChoice;
        if (!(cin >> mainChoice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        switch (mainChoice) {
            case 1:
                battleEncounter(player, pcBox);
                break;
            case 2:
                player.printParty();
                cout << "\nHeal entire active party? (1 = Yes, 2 = No): ";
                int healChoice;
                cin >> healChoice;
                if (healChoice == 1) {
                    player.healParty();
                }
                break;
            case 3:
                inventoryMenu(player);
                break;
            case 4:
                storageMenu(player, pcBox);
                break;
            case 5:
                saveGame(player, pcBox);
                break;
            case 6:
                cout << "\nWould you like to save before leaving? (1 = Yes, 2 = No): ";
                int exitSave;
                cin >> exitSave;
                if (exitSave == 1) {
                    saveGame(player, pcBox);
                }
                cout << "\nThank you for playing Beast Weaver! Goodbye!" << endl;
                running = false;
                break;
            default:
                cout << "Invalid choice! Please choose 1-6." << endl;
                break;
        }
    }

    return 0;
}

// Generate random wild beasts with different elements
Beast generateWildBeast() {
    int type = rand() % 3; // 0: Fire, 1: Water, 2: Grass
    int roll = rand() % 3;

    if (type == 0) {
        if (roll == 0) return Beast("Wild-Pyrosaur", 0, 45, 14, 10);
        if (roll == 1) return Beast("Cinderhound", 0, 40, 16, 13);
        return Beast("MagmaDrake", 0, 52, 17, 8);
    } else if (type == 1) {
        if (roll == 0) return Beast("Wild-Aquafang", 1, 48, 12, 9);
        if (roll == 1) return Beast("TidalCrab", 1, 55, 11, 7);
        return Beast("VaporSerpent", 1, 42, 14, 14);
    } else {
        if (roll == 0) return Beast("Wild-Florataur", 2, 46, 13, 10);
        if (roll == 1) return Beast("ThornSpitter", 2, 38, 15, 12);
        return Beast("MossGolem", 2, 58, 10, 6);
    }
}

// Battle System with Status Evaluation and Item Usage
void battleEncounter(Trainer& player, StorageBox& pcBox) {
    if (player.getPartySize() == 0) {
        cout << "\nYou have no beasts to battle with!" << endl;
        return;
    }

    // Find first conscious beast in party
    int activeIdx = 0;
    while (activeIdx < player.getPartySize() && player[activeIdx].isFainted()) {
        activeIdx++;
    }
    if (activeIdx >= player.getPartySize()) {
        cout << "\nAll beasts in your active party are fainted! Heal them at the Main Hub first." << endl;
        return;
    }

    Beast wild = generateWildBeast();
    cout << "\n==================================================" << endl;
    cout << "             A WILD BEAST APPEARED!               " << endl;
    cout << "==================================================" << endl;
    cout << "A ferocious " << wild.getName() << " leaps out from the tall grass!" << endl;

    bool inBattle = true;
    while (inBattle && !wild.isFainted() && !player[activeIdx].isFainted()) {
        cout << "\n--------------------------------------------------" << endl;
        cout << "Enemy: " << wild << endl;              // Uses Beast terminal operator<<
        cout << "You:   " << player[activeIdx] << endl;   // Uses Trainer[] and Beast operator<<
        cout << "--------------------------------------------------" << endl;
        cout << "1. Attack" << endl;
        cout << "2. Defend & Heal" << endl;
        cout << "3. Use Backpack Item (operator+=)" << endl;
        cout << "4. Capture Beast" << endl;
        cout << "5. Switch Beast" << endl;
        cout << "6. Run Away" << endl;
        cout << "Choose battle command: ";

        int cmd;
        if (!(cin >> cmd)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        if (cmd == 1) { // Attack
            // Friendly DamageCalculator evaluates combat and status conditions
            DamageCalculator::resolveAttack(player[activeIdx], wild);
            if (!wild.isFainted()) {
                DamageCalculator::resolveAttack(wild, player[activeIdx]);
            }
        } else if (cmd == 2) { // Defend & Heal
            // Cascaded member function call using the 'this' pointer (Slide 05)
            player[activeIdx].defend().heal(8);
            if (!wild.isFainted()) {
                DamageCalculator::resolveAttack(wild, player[activeIdx]);
            }
        } else if (cmd == 3) { // Use Item
            if (player.getInventoryCount() == 0) {
                cout << "\nYour backpack is empty!" << endl;
                continue;
            }
            player.printInventory();
            cout << "Choose item slot to use (0 to cancel): ";
            int iSlot;
            cin >> iSlot;
            if (iSlot > 0 && iSlot <= player.getInventoryCount()) {
                player.useItem(iSlot - 1, player[activeIdx]); // Uses Beast::operator+=
                if (!wild.isFainted()) {
                    DamageCalculator::resolveAttack(wild, player[activeIdx]);
                }
            } else {
                cout << "Canceled item usage." << endl;
                continue;
            }
        } else if (cmd == 4) { // Capture Beast
            Item ultraTrap("UltraNet", "TRAP", 15, "");
            int threshold = 20;

            // Check if player owns a trap item using Item::operator== (Requirement 2)
            if (player.hasItem(ultraTrap)) {
                cout << "\n[TRAP BONUS] Using UltraNet from your backpack for bonus capture range!" << endl;
                threshold += 15;
            }

            if (wild.getHp() <= threshold) {
                // Dynamically handles active party or PC Storage Box (Requirement 4)
                player.catchBeast(wild, pcBox);
                inBattle = false;
            } else {
                cout << "\nThe beast is still too energized to be captured! (HP must be <= " 
                     << threshold << ")" << endl;
                DamageCalculator::resolveAttack(wild, player[activeIdx]);
            }
        } else if (cmd == 5) { // Switch Beast
            player.printParty();
            cout << "Select party slot to send into battle (1-" << player.getPartySize() << "): ";
            int switchSlot;
            cin >> switchSlot;
            if (switchSlot >= 1 && switchSlot <= player.getPartySize()) {
                int newIdx = switchSlot - 1;
                if (player[newIdx].isFainted()) {
                    cout << player[newIdx].getName() << " is fainted and cannot fight!" << endl;
                } else {
                    activeIdx = newIdx;
                    cout << "\nGo! " << player[activeIdx].getName() << " enters the battle!" << endl;
                    DamageCalculator::resolveAttack(wild, player[activeIdx]);
                }
            }
        } else if (cmd == 6) { // Run
            cout << "\nYou managed to flee safely back to camp!" << endl;
            inBattle = false;
            return;
        }

        // ==========================================================
        // Requirement 3: Status Conditions End-of-Turn Ticks
        // Uses prefix operator-- (--beast) to execute status damage
        // ==========================================================
        if (inBattle && !wild.isFainted() && !player[activeIdx].isFainted()) {
            cout << "\n--- END OF TURN STATUS CHECKS ---" << endl;
            --player[activeIdx]; // Prefix decrement operator overload
            --wild;              // Prefix decrement operator overload
        }

        // Check if player's active beast fainted from attacks or status
        if (player[activeIdx].isFainted()) {
            cout << "\n❌ " << player[activeIdx].getName() << " fainted!" << endl;
            // Check for remaining active beasts
            bool hasConscious = false;
            for (int i = 0; i < player.getPartySize(); i++) {
                if (!player[i].isFainted()) {
                    hasConscious = true;
                    activeIdx = i;
                    cout << "Switched to " << player[activeIdx].getName() << "!" << endl;
                    break;
                }
            }
            if (!hasConscious) {
                cout << "\nAll your party beasts have fainted! You blacked out and rushed to camp." << endl;
                inBattle = false;
            }
        }

        if (wild.isFainted()) {
            cout << "\n🎉 " << wild.getName() << " fainted! You won the battle!" << endl;
            // Reward player with a random drop
            int drop = rand() % 3;
            if (drop == 0) player.addItem(Item("HealthPotion", "HEAL", 25, "Restores 25 HP."));
            else if (drop == 1) player.addItem(Item("Antidote", "CURE", 0, "Cleanses all status ailments."));
            else player.addItem(Item("AttackElixir", "ATTACK", 4, "Increases base attack by 4."));
            inBattle = false;
        }
    }
}

// Requirement 4: PC Storage Box Menu
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

// Requirement 2: Inventory & Consumables Menu
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
                // Evaluates player[beastNum - 1] += item (operator+= on Beast)
                player.useItem(itemNum - 1, player[beastNum - 1]);
            } else {
                cout << "Invalid selection!" << endl;
            }
        } else {
            inInv = false;
        }
    }
}

// Requirement 1: Save Game using File Stream Operator Overloading
void saveGame(const Trainer& player, const StorageBox& pcBox) {
    ofstream outFile("save.txt");
    if (!outFile.is_open()) {
        cout << "\n[ERROR] Could not open save.txt for writing!" << endl;
        return;
    }

    // Overloaded ofstream operator<< calls
    outFile << player;
    outFile << pcBox;
    outFile << Trainer::getTotalCaught() << "\n";

    outFile.close();
    cout << "\n💾 [SAVE SUCCESS] Game state serialized and saved to save.txt successfully!" << endl;
}

// Requirement 1: Load Game using File Stream Operator Overloading
bool loadGame(Trainer& player, StorageBox& pcBox) {
    ifstream inFile("save.txt");
    if (!inFile.is_open()) {
        cout << "\n[ERROR] Could not open save.txt for reading!" << endl;
        return false;
    }

    // Overloaded ifstream operator>> calls
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
