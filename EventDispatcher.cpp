#include "EventDispatcher.h"
#include <cstdlib>

EventCallback EventDispatcher::eventPool[EventDispatcher::POOL_SIZE] = { nullptr, nullptr, nullptr, nullptr };

void EventDispatcher::initialize() {
    eventPool[0] = &EventDispatcher::shrineOfElements;
    eventPool[1] = &EventDispatcher::hiddenItemCache;
    eventPool[2] = &EventDispatcher::wanderingHermit;
    eventPool[3] = &EventDispatcher::mysticalSprings;
}

void EventDispatcher::shrineOfElements(Trainer& player, WorldState& ws) {
    (void)ws;
    cout << "\n✨ [EVENT: ANCIENT ELEMENTAL SHRINE]" << endl;
    cout << "You stumble upon an ancient monolith inscribed with prehistoric runes." << endl;
    if (player.getPartySize() > 0) {
        Beast& lead = player[0];
        lead.heal(20);
        // Grant an attack elixir permanently
        player.addItem(Item("ShrineBlessing", "ATTACK", 2, "Essence of the ancient monolith."));
        player.useItem(player.getInventoryCount() - 1, lead);
        cout << lead.getName() << " was blessed by the shrine! (+2 Permanent Attack Base)" << endl;
    }
}

void EventDispatcher::hiddenItemCache(Trainer& player, WorldState& ws) {
    (void)ws;
    cout << "\n📦 [EVENT: HIDDEN SMUGGLER'S CACHE]" << endl;
    cout << "Half-buried in the soil, you discover a leather travel sack!" << endl;
    player += 45; // Currency operator+=
    player.addItem(Item("UltraNet", "TRAP", 15, "Captures beasts with higher health."));
    cout << "Acquired 45 Gold and an UltraNet!" << endl;
}

void EventDispatcher::wanderingHermit(Trainer& player, WorldState& ws) {
    (void)ws;
    cout << "\n🧙 [EVENT: WANDERING HERMIT ALCHEMIST]" << endl;
    cout << "An elderly traveler sits by a small campfire brewing herbal teas." << endl;
    for (int i = 0; i < player.getPartySize(); i++) {
        player[i].setStatus(0); // Cleanse all negative conditions
    }
    player.addItem(Item("SuperPotion", "HEAL", 45, "Restores 45 HP."));
    cout << "The hermit cleansed all status ailments from your team and gifted a SuperPotion!" << endl;
}

void EventDispatcher::mysticalSprings(Trainer& player, WorldState& ws) {
    (void)ws;
    cout << "\n💧 [EVENT: MYSTICAL FAIRY SPRINGS]" << endl;
    cout << "A glowing azure geyser gently bubbles with rejuvenating restorative water." << endl;
    player.healParty();
    cout << "All beasts in your active party have been completely healed to full HP!" << endl;
}

bool EventDispatcher::triggerRandomEvent(Trainer& player, WorldState& ws, int chancePercent) {
    if (eventPool[0] == nullptr) {
        initialize();
    }

    if (rand() % 100 < chancePercent) {
        int idx = rand() % POOL_SIZE;
        // Invoke through function pointer (Slide 03)
        (*eventPool[idx])(player, ws);
        return true;
    }
    return false;
}
