#include "TrainerCard.h"
#include <iomanip>

TrainerCard::TrainerCard(const Trainer& p, const Bestiary& d, const AchievementSystem& a, const WorldState& w, const ReputationManager& r)
    : player(p), dex(d), achievements(a), world(w), reputation(r) {}

ostream& operator<<(ostream& os, const TrainerCard& tc) {
    float dexPct = (static_cast<float>(tc.dex.getTotalCaught()) / tc.dex.getCount()) * 100.0f;
    float badgePct = (static_cast<float>(tc.achievements.getTotalUnlocked()) / tc.achievements.getTotalMilestones()) * 100.0f;

    os << "\n╔══════════════════════════════════════════════════════════════════════════════╗\n";
    os << "║                 🎫  OFFICIAL BEAST WEAVER TRAINER PASSPORT  🎫                ║\n";
    os << "╠══════════════════════════════════════════════════════════════════════════════╣\n";
    os << "║ TRAINER NAME : " << left << setw(28) << tc.player.getName() 
       << " TRAINER ID : #BW-2026-0819     ║\n";
    os << "║ TITLE / RANK : " << left << setw(58) << tc.reputation.getTitle() << "║\n";
    os << "╟──────────────────────────────────────────────────────────────────────────────╢\n";
    os << "║ POKÉDEX DISCOVERY : " << setw(2) << tc.dex.getTotalSeen() << "/" << setw(2) << tc.dex.getCount() 
       << " Seen | " << setw(2) << tc.dex.getTotalCaught() << "/" << setw(2) << tc.dex.getCount() 
       << " Caught (" << fixed << setprecision(1) << dexPct << "%)" 
       << setw(16) << "" << "║\n";
    os << "║ BADGE TROPHIES    : " << setw(2) << tc.achievements.getTotalUnlocked() << "/" << setw(2) << tc.achievements.getTotalMilestones() 
       << " Milestones (" << fixed << setprecision(1) << badgePct << "%)"
       << setw(29) << "" << "║\n";
    os << "║ CURRENT WALLET    : " << setw(5) << tc.player.getGold() << " Gold | TOTAL CAUGHT: " 
       << setw(3) << Trainer::getTotalCaught() << " Beasts" 
       << setw(22) << "" << "║\n";
    os << "║ EXPEDITION DAY    : Day " << setw(3) << tc.world.getDay() << " (" 
       << setw(10) << tc.world.getTimeOfDayString() << ") | Total Turns: " << setw(4) << tc.world.getTotalTurns() 
       << setw(15) << "" << "║\n";
    os << "╚══════════════════════════════════════════════════════════════════════════════╝\n";
    return os;
}
