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
#include "Achievement.h"
#include "Bestiary.h"
#include "Tournament.h"

// 10 Game Master Direction 4 Extensions
#include "TerminalHUD.h"
#include "BattleLog.h"
#include "WorldMap.h"
#include "EventDispatcher.h"
#include "BountyBoard.h"
#include "Marketplace.h"
#include "ReputationManager.h"
#include "TrainerCard.h"
#include "GameModeSettings.h"
#include "SaveManager.h"

using namespace std;

// Direction 4: Function prototypes for UI and Overworld systems
void storageMenu(Trainer& player, StorageBox& pcBox);
void inventoryMenu(Trainer& player);
void campMenu(Trainer& player, StorageBox& pcBox);
void npcChallengeMenu(Trainer& player, StorageBox& pcBox, BattleArena& arena, AchievementSystem& achievements, Bestiary& dex, Tournament& tourney, ReputationManager& rep);
void exploreWildernessMenu(Trainer& player, StorageBox& pcBox, BattleArena& arena, AchievementSystem& achievements, Bestiary& dex, ReputationManager& rep, BountyBoard& bounties);
void mapNavigationLoop(Trainer& player, StorageBox& pcBox, BattleArena& arena, WorldMap& map, AchievementSystem& achievements, Bestiary& dex, ReputationManager& rep, BountyBoard& bounties, Marketplace& market);

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    EventDispatcher::initialize();

    TerminalHUD::printHeaderBanner("🌲 🌋 🌊  WELCOME TO BEAST WEAVER: EXPEDITION EDITION  🌊 🌋 🌲");
    cout << TerminalHUD::colorize("             Master Object-Oriented Tactical RPG Adventure            \n", TerminalHUD::YELLOW);

    Trainer player("Hero");
    player.setGold(150);
    StorageBox pcBox(10);
    BattleArena arena("Imperial Colosseum");
    Tournament tourney("Grand Imperial Cup", 500);
    AchievementSystem achievements;
    Bestiary dex;
    BountyBoard bounties;
    Marketplace market;
    ReputationManager reputation;
    WorldMap worldMap(8, 12);
    SaveManager saveManager(3);

    bool gameLoaded = false;

    // Save Profile Slot Selection / Quick-Resume
    cout << "\nChoose an entry mode:" << endl;
    cout << "1. Start New Adventure" << endl;
    cout << "2. Quick-Resume from save.txt" << endl;
    cout << "3. Manage / Load Save Slots (Multi-Save Manager)" << endl;
    cout << "Select (1-3): ";
    int bootChoice;
    if (cin >> bootChoice) {
        if (bootChoice == 2) {
            ifstream inFile("save.txt");
            if (inFile.is_open()) {
                inFile >> player >> pcBox;
                int caught = 0;
                if (inFile >> caught) Trainer::setTotalCaught(caught);
                WorldState ws; inFile >> ws;
                inFile >> achievements >> dex;
                inFile.close();
                gameLoaded = true;
                TerminalHUD::flashMessage("Loaded save.txt successfully! Welcome back, " + player.getName() + "!", TerminalHUD::GREEN);
            } else {
                cout << "No save.txt found! Starting fresh adventure." << endl;
            }
        } else if (bootChoice == 3) {
            saveManager.printSlots();
            cout << "Select slot to load (1-3, or 0 to start new): ";
            int slotPick;
            if (cin >> slotPick && slotPick >= 1 && slotPick <= 3 && saveManager.slotExists(slotPick)) {
                gameLoaded = saveManager.load(slotPick, player, pcBox, achievements, dex);
            }
        }
    }

    if (!gameLoaded) {
        // Optional Challenge Game Mode Selector (Feature #10)
        cout << "\n==================================================" << endl;
        cout << "           SELECT ADVENTURE RULES / CHALLENGE     " << endl;
        cout << "==================================================" << endl;
        cout << "1. Standard Mode       (Casual, fainted beasts can be revived)" << endl;
        cout << "2. 💀 Nuzlocke Mode    (Hardcore: Permadeath on all fainted beasts)" << endl;
        cout << "3. ⏱️ Speedrun Clock   (Defeat Tournament within 120 turns)" << endl;
        cout << "Choose rule preset (1-3): ";
        int ruleChoice;
        if (cin >> ruleChoice) {
            if (ruleChoice == 2) GameModeSettings::setNuzlocke(true);
            else if (ruleChoice == 3) GameModeSettings::setSpeedrun(true, 120);
        }

        cout << "\nEnter your Trainer Name: ";
        string tName;
        cin >> tName;
        player = Trainer(tName);
        player.setGold(150);

        cout << "\nChoose your starter Beast companion:" << endl;
        cout << "1. " << TerminalHUD::getElementalBadge(0) << " Pyrosaur   (High Attack, Aggressive DPS)" << endl;
        cout << "2. " << TerminalHUD::getElementalBadge(1) << " Aquafang   (High HP, Stun Crowd Control)" << endl;
        cout << "3. " << TerminalHUD::getElementalBadge(2) << " Florataur  (Balanced Stats, Toxic Spores)" << endl;
        cout << "Select starter (1-3): ";
        int sChoice;
        cin >> sChoice;

        Beast starter;
        if (sChoice == 1) starter = Beast("Pyrosaur", 0, 55, 18, 12, 0, 1, 0);
        else if (sChoice == 2) starter = Beast("Aquafang", 1, 65, 14, 10, 0, 1, 0);
        else starter = Beast("Florataur", 2, 60, 15, 11, 0, 1, 0);

        player.catchBeast(starter, pcBox);
        dex.recordEncounter(starter.getName(), starter.getElementalType());
        dex.recordCapture(starter.getName());

        // Initial inventory backpack
        player.addItem(Item("HealthPotion", "HEAL", 25, "Restores 25 HP."));
        player.addItem(Item("AttackElixir", "ATTACK", 4, "Increases base attack by 4."));
        player.addItem(Item("Antidote", "CURE", 0, "Cures poison, burn, and stun ailments."));
        player.addItem(Item("UltraNet", "TRAP", 15, "Captures beasts with higher health."));
    }

    bool running = true;
    while (running) {
        WorldState ws;
        achievements.checkMilestones(player, pcBox);
        GameModeSettings::checkSpeedrunFail();

        cout << "\n==========================================================================" << endl;
        cout << "                        BEAST WEAVER: OVERWORLD HUB                       " << endl;
        cout << "==========================================================================" << endl;
        cout << " " << ws << " " << TerminalHUD::colorize(GameModeSettings::getStatusSummary(), TerminalHUD::YELLOW) << endl;
        cout << " Trainer: " << TerminalHUD::colorize(player.getName(), TerminalHUD::BOLD)
             << " | Gold: " << TerminalHUD::colorize(to_string(player.getGold()) + "G", TerminalHUD::YELLOW)
             << " | Active: " << player.getPartySize() << "/3"
             << " | PC: " << pcBox.getCount()
             << " | Badges: [" << AchievementSystem::getTotalUnlocked() << "/" << AchievementSystem::getTotalMilestones() << "]"
             << " | " << reputation << endl;
        cout << "--------------------------------------------------------------------------" << endl;
        cout << "  [1] 🗺️  2D Interactive Overworld Map   - Explore grid with WASD & Ambush" << endl;
        cout << "  [2] 🌲 🌋 🌊 Direct Zone Expedition  - Venture into Canopy, Crags, or Reef" << endl;
        cout << "  [3] 🏛️ Imperial Colosseum & Cup      - Wager Gold & Enter Tournament" << endl;
        cout << "  [4] 🏪 The Grand BeastMart           - Dynamic Economy & Night Peddler" << endl;
        cout << "  [5] 📜 Town Bounty Noticeboard       - Complete Contracts for Gold" << endl;
        cout << "  [6] 🏕️ Oakhaven Basecamp            - Rest, Heal Party & PC Storage" << endl;
        cout << "  [7] 🎫 View Trainer Passport Card    - Inspect Stats, Titles & Rating" << endl;
        cout << "  [8] 📖 Pokédex / Bestiary Catalog   - Species Lore & Discovery Records" << endl;
        cout << "  [9] 🏆 Trophy Case & Badges          - Milestones & Achievements" << endl;
        cout << "  [10] 🎒 Open Backpack                - Use Consumables & Inspect Gear" << endl;
        cout << "  [11] 💾 Multi-Slot Save Manager      - Save/Load Across 3 Save Slots" << endl;
        cout << "  [12] 🚪 Exit Game" << endl;
        cout << "--------------------------------------------------------------------------" << endl;
        cout << "Choose destination (1-12): ";

        int mainChoice;
        if (!(cin >> mainChoice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        switch (mainChoice) {
            case 1:
                mapNavigationLoop(player, pcBox, arena, worldMap, achievements, dex, reputation, bounties, market);
                break;
            case 2:
                exploreWildernessMenu(player, pcBox, arena, achievements, dex, reputation, bounties);
                break;
            case 3:
                npcChallengeMenu(player, pcBox, arena, achievements, dex, tourney, reputation);
                break;
            case 4:
                cout << "\n1. Visit Standard Market\n2. Seek Shady Night Peddler\nChoose: ";
                int mChoice;
                if (cin >> mChoice) {
                    if (mChoice == 1) market.visitMart(player);
                    else market.visitNightPeddler(player);
                }
                break;
            case 5:
                cout << bounties;
                cout << "\nClaim completed bounty reward (Enter slot #, or 0 to exit): ";
                int bPick;
                if (cin >> bPick && bPick > 0) {
                    bounties.claim(bPick - 1, player);
                }
                break;
            case 6:
                campMenu(player, pcBox);
                break;
            case 7: {
                TrainerCard tc(player, dex, achievements, ws, reputation);
                cout << tc;
                break;
            }
            case 8:
                cout << dex;
                break;
            case 9:
                cout << achievements;
                break;
            case 10:
                inventoryMenu(player);
                break;
            case 11: {
                cout << saveManager;
                cout << "\n1. Save to a Slot\n2. Load from a Slot\n3. Quick-Save to save.txt\nChoose: ";
                int sAct;
                if (cin >> sAct) {
                    if (sAct == 1) {
                        cout << "Select slot number (1-3): ";
                        int sNum;
                        if (cin >> sNum) saveManager.save(sNum, player, pcBox, achievements, dex);
                    } else if (sAct == 2) {
                        cout << "Select slot number (1-3): ";
                        int sNum;
                        if (cin >> sNum) saveManager.load(sNum, player, pcBox, achievements, dex);
                    } else if (sAct == 3) {
                        ofstream outFile("save.txt");
                        if (outFile.is_open()) {
                            outFile << player << pcBox << Trainer::getTotalCaught() << "\n" << ws << achievements << dex;
                            outFile.close();
                            TerminalHUD::flashMessage("Quick-Saved successfully to save.txt!", TerminalHUD::GREEN);
                        }
                    }
                }
                break;
            }
            case 12:
                cout << "\nThank you for exploring Beast Weaver! Farewell, Game Master!" << endl;
                running = false;
                break;
            default:
                cout << "Invalid destination! Please choose 1-12." << endl;
                break;
        }
    }

    return 0;
}

// Direction 4 Feature #2: Interactive 2D Overworld Map Navigation
void mapNavigationLoop(Trainer& player, StorageBox& pcBox, BattleArena& arena, WorldMap& map, AchievementSystem& achievements, Bestiary& dex, ReputationManager& rep, BountyBoard& bounties, Marketplace& market) {
    bool navigating = true;
    while (navigating) {
        cout << map;
        cout << "\n🧭 CONTROLS: [W] North | [S] South | [A] West | [D] East | [E] Enter Tile | [Q] Return to Hub" << endl;
        cout << "Enter movement command: ";
        char cmd;
        if (!(cin >> cmd)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        if (cmd == 'Q' || cmd == 'q') {
            navigating = false;
            break;
        } else if (cmd == 'E' || cmd == 'e') {
            char tile = map.getCurrentTile();
            if (tile == 'T') campMenu(player, pcBox);
            else if (tile == 'M') market.visitMart(player);
            else if (tile == 'C') {
                Tournament tourney("Grand Imperial Cup", 500);
                npcChallengeMenu(player, pcBox, arena, achievements, dex, tourney, rep);
            } else if (tile == 'F') {
                arena.startWildEncounter(player, pcBox, 1, &achievements, &dex);
                bounties.markComplete(2);
            } else if (tile == 'V') {
                arena.startWildEncounter(player, pcBox, 2, &achievements, &dex);
                bounties.markComplete(0);
            } else if (tile == 'O') {
                arena.startWildEncounter(player, pcBox, 3, &achievements, &dex);
                bounties.markComplete(1);
            } else {
                cout << "\nYou search the open plains but find only rustling wildflowers." << endl;
            }
        } else {
            char newTile = map.move(cmd);
            WorldState::advanceTime(1); // Walking advances world clock

            // Feature #4: Wilderness Event Dispatcher
            WorldState ws;
            EventDispatcher::triggerRandomEvent(player, ws, 25);

            // Chance of tall-grass random encounter if in wild tiles
            if ((newTile == 'F' || newTile == 'V' || newTile == 'O') && (rand() % 100 < 35)) {
                TerminalHUD::flashMessage("⚠️ TALL GRASS AMBUSH! A wild beast lunges at you!", TerminalHUD::RED);
                int z = (newTile == 'V') ? 2 : (newTile == 'O' ? 3 : 1);
                arena.startWildEncounter(player, pcBox, z, &achievements, &dex);
            }
        }
    }
}

// Direction 4: Direct Zone Expeditions
void exploreWildernessMenu(Trainer& player, StorageBox& pcBox, BattleArena& arena, AchievementSystem& achievements, Bestiary& dex, ReputationManager& rep, BountyBoard& bounties) {
    (void)rep;
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
        arena.startWildEncounter(player, pcBox, zoneChoice, &achievements, &dex);
        if (zoneChoice == 1) bounties.markComplete(2);
        else if (zoneChoice == 2) bounties.markComplete(0);
        else if (zoneChoice == 3) bounties.markComplete(1);
    }
}

// Direction 4: Colosseum Tournaments & Exhibitions
void npcChallengeMenu(Trainer& player, StorageBox& pcBox, BattleArena& arena, AchievementSystem& achievements, Bestiary& dex, Tournament& tourney, ReputationManager& rep) {
    cout << "\n==================================================" << endl;
    cout << "           COLOSSEUM NPC TOURNAMENT               " << endl;
    cout << "==================================================" << endl;
    cout << "1. Novice Scout Timmy     (Tier 1 | Wager: 60 Gold)" << endl;
    cout << "2. Specialist Hiker Brock (Tier 2 | Wager: 150 Gold)" << endl;
    cout << "3. Elite Champion Cynthia (Tier 3 | Wager: 350 Gold)" << endl;
    cout << "4. 🏆 ENTER THE GRAND IMPERIAL CUP TOURNAMENT (3-Round Bracket)" << endl;
    cout << "5. Back to Overworld Map" << endl;
    cout << "Choose option (1-5): ";

    int tierPick;
    if (!(cin >> tierPick) || tierPick < 1 || tierPick > 5) {
        cin.clear();
        cin.ignore(1000, '\n');
        return;
    }

    if (tierPick == 1) {
        Trainer npc("Scout Timmy", 1);
        arena.startMatch(player, npc, &achievements, &dex);
        rep.addFame(20).checkRank();
    } else if (tierPick == 2) {
        Trainer npc("Hiker Brock", 2);
        arena.startMatch(player, npc, &achievements, &dex);
        rep.addFame(50).checkRank();
    } else if (tierPick == 3) {
        Trainer npc("Champion Cynthia", 3);
        arena.startMatch(player, npc, &achievements, &dex);
        rep.addFame(120).checkRank();
    } else if (tierPick == 4) {
        if (tourney.enterTournament(player, pcBox, achievements, dex)) {
            rep.addFame(250).checkRank();
        }
    }
}

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
