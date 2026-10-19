#include "BattleArena.h"
#include "Achievement.h"
#include "Bestiary.h"
#include <iostream>
#include <cstdlib>
#include <iomanip>

using namespace std;

BattleArena::BattleArena(string name) {
    arenaName = name;
}

BattleArena::~BattleArena() {
    // Destructor
}

// Direction 4: ASCII Visual HP Bar Generator
string BattleArena::renderHpBar(int current, int max, int length) {
    if (max <= 0) max = 1;
    if (current < 0) current = 0;
    if (current > max) current = max;

    float ratio = static_cast<float>(current) / max;
    int filled = static_cast<int>(ratio * length);
    if (filled == 0 && current > 0) filled = 1;

    string bar = "[";
    for (int i = 0; i < filled; i++) bar += "█";
    for (int i = filled; i < length; i++) bar += "░";
    bar += "] ";

    return bar;
}

// Direction 4: ASCII Dual-Card Duel HUD for Trainer Battles
void BattleArena::renderDuelHud(const Trainer& player, int activePlayerIdx, const Trainer& opponent, int activeOpponentIdx, string location) {
    const Beast& pBeast = player[activePlayerIdx];
    const Beast& oBeast = opponent[activeOpponentIdx];

    string pElem = (pBeast.getElementalType() == 0) ? "🔥 FIRE" : (pBeast.getElementalType() == 1 ? "💧 WATER" : "🌿 GRASS");
    string oElem = (oBeast.getElementalType() == 0) ? "🔥 FIRE" : (oBeast.getElementalType() == 1 ? "💧 WATER" : "🌿 GRASS");

    cout << "\n╔══════════════════════════════════════════════════════════════════════════════╗" << endl;
    cout << "║ LOCATION: " << left << setw(24) << location 
         << " TIME: " << setw(4) << WorldState::getTimeIcon() << " " << setw(12) << WorldState::getTimeOfDayString()
         << " (Day " << WorldState::getDay() << ") ║" << endl;
    cout << "╠══════════════════════════════════════════════════════════════════════════════╣" << endl;
    cout << "║ OPPONENT: " << left << setw(66) << opponent.getName() << "║" << endl;
    cout << "║   [" << setw(14) << oBeast.getName() << "] Lv." << setw(2) << oBeast.getLevel() 
         << " (" << setw(10) << oElem << " | " << setw(9) << oBeast.getStatusString() << ") " 
         << setw(20) << "" << "║" << endl;
    cout << "║   HP: " << renderHpBar(oBeast.getHp(), oBeast.getMaxHp(), 18) 
         << setw(4) << oBeast.getHp() << "/" << setw(4) << oBeast.getMaxHp() 
         << " | ATK: " << setw(3) << oBeast.getAttack() << " | SPD: " << setw(3) << oBeast.getSpeed() << "       ║" << endl;
    cout << "║                                                                              ║" << endl;
    cout << "║                              ⚔️       VS       ⚔️                            ║" << endl;
    cout << "║                                                                              ║" << endl;
    cout << "║ TRAINER: " << left << setw(67) << player.getName() << "║" << endl;
    cout << "║   [" << setw(14) << pBeast.getName() << "] Lv." << setw(2) << pBeast.getLevel() 
         << " (" << setw(10) << pElem << " | " << setw(9) << pBeast.getStatusString() << ") " 
         << setw(20) << "" << "║" << endl;
    cout << "║   HP: " << renderHpBar(pBeast.getHp(), pBeast.getMaxHp(), 18) 
         << setw(4) << pBeast.getHp() << "/" << setw(4) << pBeast.getMaxHp() 
         << " | ATK: " << setw(3) << pBeast.getAttack() << " | SPD: " << setw(3) << pBeast.getSpeed() << "       ║" << endl;
    cout << "╚══════════════════════════════════════════════════════════════════════════════╝" << endl;
}

// Direction 4: ASCII Visual HUD for Wild Encounters
void BattleArena::renderWildHud(const Trainer& player, int activePlayerIdx, const Beast& wild, string location) {
    const Beast& pBeast = player[activePlayerIdx];

    string pElem = (pBeast.getElementalType() == 0) ? "🔥 FIRE" : (pBeast.getElementalType() == 1 ? "💧 WATER" : "🌿 GRASS");
    string wElem = (wild.getElementalType() == 0) ? "🔥 FIRE" : (wild.getElementalType() == 1 ? "💧 WATER" : "🌿 GRASS");

    cout << "\n╔══════════════════════════════════════════════════════════════════════════════╗" << endl;
    cout << "║ WILD ZONE: " << left << setw(23) << location 
         << " TIME: " << setw(4) << WorldState::getTimeIcon() << " " << setw(12) << WorldState::getTimeOfDayString()
         << " (Day " << WorldState::getDay() << ") ║" << endl;
    cout << "╠══════════════════════════════════════════════════════════════════════════════╣" << endl;
    cout << "║ WILD CREATURE:                                                               ║" << endl;
    cout << "║   [" << left << setw(14) << wild.getName() << "] Lv." << setw(2) << wild.getLevel() 
         << " (" << setw(10) << wElem << " | " << setw(9) << wild.getStatusString() << ") " 
         << setw(20) << "" << "║" << endl;
    cout << "║   HP: " << renderHpBar(wild.getHp(), wild.getMaxHp(), 18) 
         << setw(4) << wild.getHp() << "/" << setw(4) << wild.getMaxHp() 
         << " | ATK: " << setw(3) << wild.getAttack() << " | SPD: " << setw(3) << wild.getSpeed() << "       ║" << endl;
    cout << "║                                                                              ║" << endl;
    cout << "║                              ⚔️       VS       ⚔️                            ║" << endl;
    cout << "║                                                                              ║" << endl;
    cout << "║ YOUR BEAST:                                                                  ║" << endl;
    cout << "║   [" << left << setw(14) << pBeast.getName() << "] Lv." << setw(2) << pBeast.getLevel() 
         << " (" << setw(10) << pElem << " | " << setw(9) << pBeast.getStatusString() << ") " 
         << setw(20) << "" << "║" << endl;
    cout << "║   HP: " << renderHpBar(pBeast.getHp(), pBeast.getMaxHp(), 18) 
         << setw(4) << pBeast.getHp() << "/" << setw(4) << pBeast.getMaxHp() 
         << " | ATK: " << setw(3) << pBeast.getAttack() << " | SPD: " << setw(3) << pBeast.getSpeed() << "       ║" << endl;
    cout << "╚══════════════════════════════════════════════════════════════════════════════╝" << endl;
}

// Direction 3: NPC AI with cascaded member function calls
void BattleArena::executeNpcTurn(Trainer& npc, int activeNpcIdx, Trainer& player, int activePlayerIdx) {
    int currentHp = npc[activeNpcIdx].getHp();
    int maxHp = npc[activeNpcIdx].getMaxHp();

    // Strategy 1: If health is low (<= 25%), defend and heal in a single cascaded expression
    if (currentHp <= (maxHp * 0.25)) {
        cout << "\n🧠 [NPC STRATEGY] " << npc.getName() << " commands " 
             << npc[activeNpcIdx].getName() << " to enter defensive stance and heal!" << endl;
        // Cascaded call using 'this' pointer returning *this (Slide 05)
        npc[activeNpcIdx].defend().heal(12);
    } else {
        // Strategy 2: Attack the player's active beast
        DamageCalculator::resolveAttack(npc[activeNpcIdx], player[activePlayerIdx]);
    }
}

// Direction 4 & 3: Trainer vs Trainer match in Battle Arena
void BattleArena::startMatch(Trainer& player, Trainer& npc, AchievementSystem* achievements, Bestiary* dex) {
    cout << "\n==================================================" << endl;
    cout << "           " << arenaName << " MATCH            " << endl;
    cout << "==================================================" << endl;
    cout << "Challenger " << player.getName() << " VS Rival Trainer " << npc.getName() << "!" << endl;
    cout << "Wager Prize: " << npc.getPrizeMoney() << " Gold!" << endl;
    cout << "==================================================" << endl;

    if (dex) {
        for (int i = 0; i < npc.getPartySize(); i++) {
            dex->recordEncounter(npc[i].getName(), npc[i].getElementalType());
        }
    }

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
        // Render Direction 4 ASCII Battle HUD
        renderDuelHud(player, activePlayerIdx, npc, activeNpcIdx, arenaName);

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

        // Action advances global clock
        WorldState::advanceTime(1);

        if (cmd == 1) { // Attack
            DamageCalculator::resolveAttack(player[activePlayerIdx], npc[activeNpcIdx]);
        } else if (cmd == 2) { // Defend & Heal
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

            // Level up using prefix operator++ (Slide 07)
            ++player[activePlayerIdx];
            player[activePlayerIdx].addExp(45);

            if (npc.hasUsableBeasts()) {
                activeNpcIdx = npc.getFirstUsableIndex();
                cout << "\n" << npc.getName() << " sent out " << npc[activeNpcIdx].getName() << "!" << endl;
            } else {
                break;
            }
        } else {
            // NPC's turn with AI strategy
            executeNpcTurn(npc, activeNpcIdx, player, activePlayerIdx);
        }

        // End of round status checks (unary prefix operator--)
        if (!player[activePlayerIdx].isFainted() && !npc[activeNpcIdx].isFainted()) {
            cout << "\n--- END OF TURN STATUS CHECKS ---" << endl;
            --player[activePlayerIdx];
            --npc[activeNpcIdx];
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
                break;
            }
        }
    }

    // Match Conclusion
    cout << "\n==================================================" << endl;
    cout << "                 MATCH RESOLUTION                 " << endl;
    cout << "==================================================" << endl;

    if (player.hasUsableBeasts()) {
        cout << "🏆 VICTORY! " << player.getName() << " defeated " << npc.getName() << "!" << endl;
        player += npc.getPrizeMoney(); // Overloaded binary operator+=
        if (achievements) {
            achievements->unlock("FIRST_BLOOD");
            if (WorldState::isNight()) achievements->unlock("NIGHT_HUNTER");
            if (npc.getName().find("Cynthia") != string::npos) achievements->unlock("CHAMPION_SLAYER");
        }
    } else {
        cout << "💀 DEFEAT! All your beasts have fainted." << endl;
        cout << npc.getName() << " won the arena match." << endl;
    }
}

// Direction 4 & 3: Wild Expedition Zone Encounters
void BattleArena::startWildEncounter(Trainer& player, StorageBox& pcBox, int zone, AchievementSystem* achievements, Bestiary* dex) {
    if (!player.hasUsableBeasts()) {
        cout << "\nAll of your beasts are fainted! Rest at camp or heal at the center first." << endl;
        return;
    }

    string zoneName = "🌲 Verdant Canopy";
    int type = 2; // Grass
    if (zone == 2) {
        zoneName = "🌋 Obsidian Crags";
        type = 0; // Fire
    } else if (zone == 3) {
        zoneName = "🌊 Azure Reef";
        type = 1; // Water
    }

    // Generate zone-specific wild creature
    int roll = rand() % 3;
    Beast wild;
    if (type == 0) { // Fire Zone
        if (roll == 0) wild = Beast("Wild-Pyrosaur", 0, 48, 16, 11, 0, 3);
        else if (roll == 1) wild = Beast("Cinderhound", 0, 44, 18, 14, 0, 3);
        else wild = Beast("MagmaDrake", 0, 56, 19, 9, 0, 4);
    } else if (type == 1) { // Water Zone
        if (roll == 0) wild = Beast("Wild-Aquafang", 1, 52, 14, 10, 0, 3);
        else if (roll == 1) wild = Beast("TidalCrab", 1, 60, 13, 8, 0, 3);
        else wild = Beast("VaporSerpent", 1, 46, 16, 15, 0, 4);
    } else { // Grass Zone
        if (roll == 0) wild = Beast("Wild-Florataur", 2, 50, 15, 11, 0, 3);
        else if (roll == 1) wild = Beast("ThornSpitter", 2, 42, 17, 13, 0, 3);
        else wild = Beast("MossGolem", 2, 64, 12, 7, 0, 4);
    }

    // Direction 4: Day / Night combat modifier
    if (WorldState::isNight()) {
        cout << "\n🌙 [NIGHT SHROUD] The darkness empowers the wild beast (+2 SPD)!" << endl;
    }

    cout << "\n==================================================" << endl;
    cout << "           ENTERING ZONE: " << zoneName << endl;
    cout << "==================================================" << endl;
    cout << "A wild " << wild.getName() << " appeared from the shadows!" << endl;

    if (dex) {
        dex->recordEncounter(wild.getName(), wild.getElementalType());
    }

    int activeIdx = player.getFirstUsableIndex();

    bool inBattle = true;
    while (inBattle && !wild.isFainted() && player.hasUsableBeasts()) {
        // Render Direction 4 ASCII Wild HUD
        renderWildHud(player, activeIdx, wild, zoneName);

        cout << "1. Attack\n2. Defend & Heal\n3. Use Item\n4. Capture Beast\n5. Switch\n6. Flee Zone\nChoose action: ";
        int cmd;
        if (!(cin >> cmd)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        // Action advances global clock
        WorldState::advanceTime(1);

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
                if (dex) dex->recordCapture(wild.getName());
                if (achievements) achievements->checkMilestones(player, pcBox);
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
            cout << "\nYou retreated back to camp!" << endl;
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
            ++player[activeIdx]; // Level up
            player[activeIdx].addExp(30);
            player += 25; // Bonus exploration gold
            if (achievements) {
                achievements->unlock("FIRST_BLOOD");
                if (WorldState::isNight()) achievements->unlock("NIGHT_HUNTER");
                achievements->checkMilestones(player, pcBox);
            }
            inBattle = false;
        }

        if (player[activeIdx].isFainted()) {
            cout << "\n❌ " << player[activeIdx].getName() << " fainted!" << endl;
            if (player.hasUsableBeasts()) {
                activeIdx = player.getFirstUsableIndex();
                cout << "Sent out " << player[activeIdx].getName() << "!" << endl;
            } else {
                cout << "\nAll beasts fainted! You rushed back to Oakhaven Camp..." << endl;
                inBattle = false;
            }
        }
    }
}
