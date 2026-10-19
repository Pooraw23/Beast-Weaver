#include "BountyBoard.h"
#include <iomanip>

int BountyBoard::totalBountiesCompleted = 0;

Bounty::Bounty() : title(""), targetDescription(""), rewardGold(0), rewardItem(Item()), completed(false), claimed(false) {}

Bounty::Bounty(string t, string desc, int gold, Item item)
    : title(t), targetDescription(desc), rewardGold(gold), rewardItem(item), completed(false), claimed(false) {}

BountyBoard::BountyBoard() {
    count = 4;
    board = new Bounty[count];

    board[0] = Bounty("Pyromaniac Pursuit", "Hunt or log any Fire-type species in the wild.", 80, Item("AttackElixir", "ATTACK", 4, "Increases base attack by 4."));
    board[1] = Bounty("Deep Sea Diver", "Explore Azure Reef and encounter a Water beast.", 80, Item("SuperPotion", "HEAL", 45, "Restores 45 HP."));
    board[2] = Bounty("Forest Warden", "Venture into Verdant Canopy and encounter a Grass beast.", 80, Item("UltraNet", "TRAP", 15, "Captures beasts with higher health."));
    board[3] = Bounty("Colosseum Champion", "Win any official trainer battle in the Arena.", 160, Item("HyperPotion", "HEAL", 80, "Restores 80 HP."));
}

BountyBoard::BountyBoard(const BountyBoard& other) {
    count = other.count;
    board = new Bounty[count];
    for (int i = 0; i < count; i++) {
        board[i] = other.board[i];
    }
}

BountyBoard& BountyBoard::operator=(const BountyBoard& other) {
    if (this != &other) {
        delete[] board;
        count = other.count;
        board = new Bounty[count];
        for (int i = 0; i < count; i++) {
            board[i] = other.board[i];
        }
    }
    return *this;
}

BountyBoard::~BountyBoard() {
    delete[] board;
}

void BountyBoard::markComplete(int index) {
    if (index >= 0 && index < count && !board[index].completed) {
        board[index].completed = true;
        cout << "\n📜 [BOUNTY COMPLETE] \"" << board[index].title << "\" is ready to claim at the Noticeboard!" << endl;
    }
}

bool BountyBoard::claim(int index, Trainer& player) {
    if (index < 0 || index >= count) return false;
    if (!board[index].completed) {
        cout << "\n❌ This bounty has not been completed yet!" << endl;
        return false;
    }
    if (board[index].claimed) {
        cout << "\n❌ You already claimed this bounty reward!" << endl;
        return false;
    }

    board[index].claimed = true;
    totalBountiesCompleted++;

    // Payout rewards
    player += board[index].rewardGold; // Operator+=
    player.addItem(board[index].rewardItem);

    cout << "\n🎉 [BOUNTY REWARD CLAIMED] Received " << board[index].rewardGold 
         << " Gold and " << board[index].rewardItem.getName() << "!" << endl;
    return true;
}

int BountyBoard::getCount() const { return count; }
int BountyBoard::getTotalCompleted() { return totalBountiesCompleted; }

ostream& operator<<(ostream& os, const BountyBoard& bb) {
    os << "\n╔══════════════════════════════════════════════════════════════════════════════╗\n";
    os << "║                     📜  OAKHAVEN TOWN BOUNTY BOARD  📜                      ║\n";
    os << "║               Contracts Claimed: [" << BountyBoard::totalBountiesCompleted << "/" << bb.count << "] Completed               ║\n";
    os << "╠══════════════════════════════════════════════════════════════════════════════╣\n";

    for (int i = 0; i < bb.count; i++) {
        string statusTag = "⏳ [IN PROGRESS]";
        if (bb.board[i].claimed) statusTag = "✅ [CLAIMED]    ";
        else if (bb.board[i].completed) statusTag = "⭐ [READY!]     ";

        os << "║ " << statusTag << " #" << (i + 1) << " " << left << setw(20) << bb.board[i].title 
           << " Reward: " << setw(4) << bb.board[i].rewardGold << "G + " << setw(14) << bb.board[i].rewardItem.getName() << " ║\n";
        os << "║      Task: " << left << setw(68) << bb.board[i].targetDescription << " ║\n";
        if (i < bb.count - 1) {
            os << "╟──────────────────────────────────────────────────────────────────────────────╢\n";
        }
    }
    os << "╚══════════════════════════════════════════════════════════════════════════════╝\n";
    return os;
}
