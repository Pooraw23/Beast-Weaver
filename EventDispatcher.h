#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H

#include <iostream>
#include <string>
#include "Trainer.h"
#include "WorldState.h"

using namespace std;

typedef void (*EventCallback)(Trainer& player, WorldState& ws);

class EventDispatcher {
private:
    static const int POOL_SIZE = 4;
    static EventCallback eventPool[POOL_SIZE];

    // Static event handlers
    static void shrineOfElements(Trainer& player, WorldState& ws);
    static void hiddenItemCache(Trainer& player, WorldState& ws);
    static void wanderingHermit(Trainer& player, WorldState& ws);
    static void mysticalSprings(Trainer& player, WorldState& ws);

public:
    static void initialize();
    static bool triggerRandomEvent(Trainer& player, WorldState& ws, int chancePercent = 25);
};

#endif
