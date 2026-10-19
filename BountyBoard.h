#ifndef BOUNTYBOARD_H
#define BOUNTYBOARD_H

#include <iostream>
#include <string>
#include "Trainer.h"
#include "Bestiary.h"

using namespace std;

class Bounty {
public:
    string title;
    string targetDescription;
    int rewardGold;
    Item rewardItem;
    bool completed;
    bool claimed;

    Bounty();
    Bounty(string t, string desc, int gold, Item item);
};

class BountyBoard {
private:
    Bounty* board;     // Dynamic array of active bounties (Slide 05 & 07)
    int count;

    static int totalBountiesCompleted; // Static variable (Slide 05)

public:
    // Rule of Three (Slide 07)
    BountyBoard();
    BountyBoard(const BountyBoard& other);
    BountyBoard& operator=(const BountyBoard& other);
    ~BountyBoard();

    void markComplete(int index);
    bool claim(int index, Trainer& player);
    int getCount() const;
    static int getTotalCompleted();

    // Stream operator for ASCII Noticeboard UI (Slide 06)
    friend ostream& operator<<(ostream& os, const BountyBoard& bb);
};

#endif
