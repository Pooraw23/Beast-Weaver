#include "BattleArena.h"
#include <iostream>
#include <cstdlib>

using namespace std;

BattleArena::BattleArena(string name) {
    arenaName = name;
}

BattleArena::~BattleArena() {
    // Destructor
}

// Requirement 4: NPC AI with cascaded commands using the 'this' pointer
void BattleArena::executeNpcTurn(Trainer& npc, int activeNpcIdx, Trainer& player, int activePlayerIdx) {
    int currentHp = npc[activeNpcIdx].getHp();
    int maxHp = npc[activeNpcIdx].getMaxHp();

    // Strategy 1: If health is low (<= 25%), defend and heal in a single cascaded expression
    if (currentHp <= (maxHp * 0.25)) {
        cout << "\n🧠 [NPC STRATEGY] " << npc.getName() << " commands " 
             << npc[activeNpcIdx].getName() << " to enter defensive stance and heal!" << endl;
        // Cascaded call using 'this' pointer returning *this
        npc[activeNpcIdx].defend().heal(12);
    } else {
        // Strategy 2: Attack the player's active beast
        DamageCalculator::resolveAttack(npc[activeNpcIdx], player[activePlayerIdx]);
    }
}

// Requirement 1: Trainer vs Trainer match passing references
void BattleArena::startMatch(Trainer& player, Trainer& npc) {
    cout << "\n==================================================" << endl;
    cout << "           " << arenaName << " MATCH            " << endl;
    cout << "==================================================" << endl;
    cout << "Challenger " << player.getName() << " VS Rival Trainer " << npc.getName() << "!" << endl;
    cout << "Wager Prize: " << npc.getPrizeMoney() << " Gold!" << endl;
    cout << "==================================================" << endl;

    if (!player.hasUsableBeasts()) {
        cout << "\nAll of " << player.getName() << "'s beasts are fainted! Heal your party first." << endl;
        return;
    }
    if (!npc.hasUsableBeasts()) {
        cout << "\n" << npc.getName() << " has no conscious beasts left to fight!" << endl;
        return;
    }

    int activePlayerIdx = player.getFirstUsableIndex();
    int activeNpcIdx = npc.getFirstUsableIndex();

    cout << "\n" << npc.getName() << " sends out " << npc[activeNpcIdx].getName() << "!" << endl;
    cout << player.getName() << " sends out " << player[activePlayerIdx].getName() << "!" << endl;

    // Run until one trainer's entire party is fainted
    while (player.hasUsableBeasts() && npc.hasUsableBeasts()) {
        cout << "\n--------------------------------------------------" << endl;
        cout << "Enemy (" << npc.getName() << "): " << npc[activeNpcIdx] << endl;
        cout << "You   (" << player.getName() << "): " << player[activePlayerIdx] << endl;
        cout << "--------------------------------------------------" << endl;
        cout << "1. Attack" << endl;
        cout << "2. Defend & Heal (Cascaded this)" << endl;
        cout << "3. Use Backpack Item (operator+=)" << endl;
        cout << "4. Switch Beast" << endl;
        cout << "5. Forfeit Match" << endl;
        cout << "Choose arena command: ";

        int cmd;
        if (!(cin >> cmd)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        if (cmd == 1) { // Attack
            DamageCalculator::resolveAttack(player[activePlayerIdx], npc[activeNpcIdx]);
        } else if (cmd == 2) { // Defend & Heal
            // Cascaded member function call using this pointer
            player[activePlayerIdx].defend().heal(10);
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
                player.useItem(iSlot - 1, player[activePlayerIdx]); // Uses operator+=
            } else {
                cout << "Canceled item usage." << endl;
                continue;
            }
        } else if (cmd == 4) { // Switch Beast
            player.printParty();
            cout << "Select next active beast (1-" << player.getPartySize() << "): ";
            int switchSlot;
            cin >> switchSlot;
            if (switchSlot >= 1 && switchSlot <= player.getPartySize()) {
                int newIdx = switchSlot - 1;
                if (player[newIdx].isFainted()) {
                    cout << player[newIdx].getName() << " is fainted and cannot fight!" << endl;
                    continue;
                } else if (newIdx == activePlayerIdx) {
                    cout << player[newIdx].getName() << " is already on the field!" << endl;
                    continue;
                } else {
                    activePlayerIdx = newIdx;
                    cout << "\nSwitch! Go " << player[activePlayerIdx].getName() << "!" << endl;
                }
            } else {
                cout << "Invalid choice!" << endl;
                continue;
            }
        } else if (cmd == 5) {
            cout << "\nYou forfeited the match!" << endl;
            return;
        } else {
            cout << "Invalid action!" << endl;
            continue;
        }

        // Check if enemy active beast fainted
        if (npc[activeNpcIdx].isFainted()) {
            cout << "\n💥 " << npc.getName() << "'s " << npc[activeNpcIdx].getName() << " fainted!" << endl;

            // Requirement 2: Level up / stat progression using prefix operator++
            ++player[activePlayerIdx]; // Unary operator++ overload!
            player[activePlayerIdx].addExp(40);

            if (npc.hasUsableBeasts()) {
                // Force NPC to select next valid index
                activeNpcIdx = npc.getFirstUsableIndex();
                cout << "\n" << npc.getName() << " sent out " << npc[activeNpcIdx].getName() << "!" << endl;
            } else {
                break; // NPC out of beasts!
            }
        } else {
            // NPC's turn with AI logic
            executeNpcTurn(npc, activeNpcIdx, player, activePlayerIdx);
        }

        // End of round status checks
        if (!player[activePlayerIdx].isFainted() && !npc[activeNpcIdx].isFainted()) {
            cout << "\n--- END OF TURN STATUS CHECKS ---" << endl;
            --player[activePlayerIdx]; // Prefix operator-- overload
            --npc[activeNpcIdx];       // Prefix operator-- overload
        }

        // Check if player active beast fainted
        if (player[activePlayerIdx].isFainted()) {
            cout << "\n❌ " << player[activePlayerIdx].getName() << " fainted!" << endl;
            if (player.hasUsableBeasts()) {
                cout << "\nChoose your next beast to send into the arena:" << endl;
                player.printParty();
                bool switched = false;
                while (!switched) {
                    cout << "Select valid slot (1-" << player.getPartySize() << "): ";
                    int pick;
                    cin >> pick;
                    if (pick >= 1 && pick <= player.getPartySize() && !player[pick - 1].isFainted()) {
                        activePlayerIdx = pick - 1;
                        cout << "\nGo! " << player[activePlayerIdx].getName() << " enters the arena!" << endl;
                        switched = true;
                    } else {
                        cout << "That beast is unable to fight! Choose another." << endl;
                    }
                }
            } else {
                break; // Player out of beasts!
            }
        }
    }

    // Match Conclusion
    cout << "\n==================================================" << endl;
    cout << "                 MATCH RESOLUTION                 " << endl;
    cout << "==================================================" << endl;

    if (player.hasUsableBeasts()) {
        cout << "🏆 VICTORY! " << player.getName() << " defeated " << npc.getName() << "!" << endl;
        // Requirement 3: Prize Money via Binary operator+=
        player += npc.getPrizeMoney(); // Overloaded binary operator+=
    } else {
        cout << "💀 DEFEAT! All your beasts have fainted." << endl;
        cout << npc.getName() << " won the arena match." << endl;
    }
}

// Wild Beast Encounter with capture support into StorageBox
void BattleArena::startWildEncounter(Trainer& player, StorageBox& pcBox) {
    if (!player.hasUsableBeasts()) {
        cout << "\nAll of your beasts are fainted! Heal at the hub first." << endl;
        return;
    }

    // Generate wild beast
    int type = rand() % 3;
    int roll = rand() % 3;
    Beast wild;
    if (type == 0) {
        if (roll == 0) wild = Beast("Wild-Pyrosaur", 0, 45, 14, 10, 0, 2);
        else if (roll == 1) wild = Beast("Cinderhound", 0, 42, 16, 13, 0, 3);
        else wild = Beast("MagmaDrake", 0, 55, 18, 9, 0, 4);
    } else if (type == 1) {
        if (roll == 0) wild = Beast("Wild-Aquafang", 1, 48, 13, 9, 0, 2);
        else if (roll == 1) wild = Beast("TidalCrab", 1, 56, 12, 7, 0, 3);
        else wild = Beast("VaporSerpent", 1, 44, 15, 14, 0, 4);
    } else {
        if (roll == 0) wild = Beast("Wild-Florataur", 2, 46, 13, 10, 0, 2);
        else if (roll == 1) wild = Beast("ThornSpitter", 2, 40, 15, 12, 0, 3);
        else wild = Beast("MossGolem", 2, 60, 11, 6, 0, 4);
    }

    cout << "\n==================================================" << endl;
    cout << "              WILD BEAST ENCOUNTER                " << endl;
    cout << "==================================================" << endl;
    cout << "A wild " << wild.getName() << " appeared!" << endl;

    int activeIdx = player.getFirstUsableIndex();

    bool inBattle = true;
    while (inBattle && !wild.isFainted() && player.hasUsableBeasts()) {
        cout << "\n--------------------------------------------------" << endl;
        cout << "Wild Beast: " << wild << endl;
        cout << "Your Beast: " << player[activeIdx] << endl;
        cout << "--------------------------------------------------" << endl;
        cout << "1. Attack\n2. Defend & Heal\n3. Use Item\n4. Capture Beast\n5. Switch\n6. Run\nChoose: ";
        int cmd;
        if (!(cin >> cmd)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        if (cmd == 1) {
            DamageCalculator::resolveAttack(player[activeIdx], wild);
            if (!wild.isFainted()) {
                DamageCalculator::resolveAttack(wild, player[activeIdx]);
            }
        } else if (cmd == 2) {
            player[activeIdx].defend().heal(10);
            if (!wild.isFainted()) {
                DamageCalculator::resolveAttack(wild, player[activeIdx]);
            }
        } else if (cmd == 3) {
            if (player.getInventoryCount() == 0) {
                cout << "\nBackpack is empty!" << endl;
                continue;
            }
            player.printInventory();
            cout << "Choose item slot (0 to cancel): ";
            int slot;
            cin >> slot;
            if (slot > 0 && slot <= player.getInventoryCount()) {
                player.useItem(slot - 1, player[activeIdx]);
                if (!wild.isFainted()) {
                    DamageCalculator::resolveAttack(wild, player[activeIdx]);
                }
            }
        } else if (cmd == 4) {
            Item trap("UltraNet", "TRAP", 15, "");
            int threshold = 22;
            if (player.hasItem(trap)) {
                cout << "\n[TRAP BONUS] Using UltraNet for enhanced capture threshold!" << endl;
                threshold += 15;
            }

            if (wild.getHp() <= threshold) {
                player.catchBeast(wild, pcBox);
                inBattle = false;
                return;
            } else {
                cout << "\nThe beast broke free! HP must be <= " << threshold << "." << endl;
                DamageCalculator::resolveAttack(wild, player[activeIdx]);
            }
        } else if (cmd == 5) {
            player.printParty();
            cout << "Select beast (1-" << player.getPartySize() << "): ";
            int pick;
            cin >> pick;
            if (pick >= 1 && pick <= player.getPartySize() && !player[pick - 1].isFainted()) {
                activeIdx = pick - 1;
                cout << "\nGo " << player[activeIdx].getName() << "!" << endl;
                DamageCalculator::resolveAttack(wild, player[activeIdx]);
            }
        } else if (cmd == 6) {
            cout << "\nGot away safely!" << endl;
            inBattle = false;
            return;
        }

        // Status ticks
        if (!wild.isFainted() && !player[activeIdx].isFainted()) {
            cout << "\n--- END OF TURN STATUS CHECKS ---" << endl;
            --player[activeIdx];
            --wild;
        }

        if (wild.isFainted()) {
            cout << "\n🎉 " << wild.getName() << " fainted! You won!" << endl;
            ++player[activeIdx]; // Level up!
            player[activeIdx].addExp(25);
            // Grant some wild prize gold
            player += 20;
            inBattle = false;
        }

        if (player[activeIdx].isFainted()) {
            cout << "\n❌ " << player[activeIdx].getName() << " fainted!" << endl;
            if (player.hasUsableBeasts()) {
                activeIdx = player.getFirstUsableIndex();
                cout << "Sent out " << player[activeIdx].getName() << "!" << endl;
            } else {
                cout << "\nAll beasts fainted! You blacked out..." << endl;
                inBattle = false;
            }
        }
    }
}
