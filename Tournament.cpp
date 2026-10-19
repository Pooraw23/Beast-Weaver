#include "Tournament.h"
#include <iostream>

using namespace std;

Tournament::Tournament(string name, int grandPrize) 
    : tournamentName(name), arena(name + " Arena"), championshipGrandPrize(grandPrize) {
}

Tournament::~Tournament() {
    // Destructor
}

bool Tournament::enterTournament(Trainer& player, StorageBox& pcBox, AchievementSystem& achievements, Bestiary& dex) {
    (void)pcBox;
    cout << "\n==========================================================================" << endl;
    cout << "             🏆  WELCOME TO THE " << tournamentName << "  🏆             " << endl;
    cout << "==========================================================================" << endl;
    cout << "Entry Requirements: Active conscious party." << endl;
    cout << "Format: 3-Round Single Elimination (Quarterfinal -> Semifinal -> Grand Final)" << endl;
    cout << "Grand Championship Purse: " << championshipGrandPrize << " Gold & Champion's Trophy!" << endl;
    cout << "==========================================================================" << endl;

    if (!player.hasUsableBeasts()) {
        cout << "\n❌ All your beasts are fainted! Heal your party before entering." << endl;
        return false;
    }

    // ROUND 1: Quarterfinals
    cout << "\n>>> [ROUND 1 OF 3: QUARTERFINALS] <<<" << endl;
    Trainer r1Opponent("Scout Timmy", 1);
    for (int i = 0; i < r1Opponent.getPartySize(); i++) {
        dex.recordEncounter(r1Opponent[i].getName(), r1Opponent[i].getElementalType());
    }
    arena.startMatch(player, r1Opponent);

    if (!player.hasUsableBeasts()) {
        cout << "\n💀 Knocked out in the Quarterfinals! Better luck next time." << endl;
        return false;
    }
    achievements.unlock("FIRST_BLOOD");

    cout << "\n🎉 Quarterfinal Victory! Resting party briefly before Round 2..." << endl;
    player.healParty();

    // ROUND 2: Semifinals
    cout << "\n>>> [ROUND 2 OF 3: SEMIFINALS] <<<" << endl;
    Trainer r2Opponent("Hiker Brock", 2);
    for (int i = 0; i < r2Opponent.getPartySize(); i++) {
        dex.recordEncounter(r2Opponent[i].getName(), r2Opponent[i].getElementalType());
    }
    arena.startMatch(player, r2Opponent);

    if (!player.hasUsableBeasts()) {
        cout << "\n💀 Knocked out in the Semifinals! Great run, Trainer." << endl;
        return false;
    }

    cout << "\n🎉 Semifinal Victory! Resting party briefly before the Grand Final..." << endl;
    player.healParty();

    // ROUND 3: Grand Finals
    cout << "\n>>> [ROUND 3 OF 3: THE GRAND FINALS] <<<" << endl;
    Trainer r3Opponent("Champion Cynthia", 3);
    for (int i = 0; i < r3Opponent.getPartySize(); i++) {
        dex.recordEncounter(r3Opponent[i].getName(), r3Opponent[i].getElementalType());
    }
    arena.startMatch(player, r3Opponent);

    if (!player.hasUsableBeasts()) {
        cout << "\n💀 Defeated in the Grand Finals! Champion Cynthia defended her title." << endl;
        return false;
    }

    // Grand Champion Victory!
    cout << "\n==========================================================================" << endl;
    cout << "           👑  CONGRATULATIONS! YOU ARE THE GRAND CHAMPION!  👑           " << endl;
    cout << "==========================================================================" << endl;
    cout << "Trainer " << player.getName() << " has conquered the " << tournamentName << "!" << endl;

    // Award grand prize via binary operator+= (Slide 06)
    player += championshipGrandPrize;
    achievements.unlock("CHAMPION_SLAYER");

    return true;
}
