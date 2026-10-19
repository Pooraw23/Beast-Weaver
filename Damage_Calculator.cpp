#include "Damage_Calculator.h"
#include "WorldState.h"
#include <iostream>
#include <cstdlib>

using namespace std;

bool DamageCalculator::resolveAttack(Beast& attacker, Beast& defender) {
    // 1. Evaluate Attacker Status: Stun Check
    // Directly accessing private member statusState as a friend class (Slide 05)
    if (attacker.statusState == 3) {
        if (rand() % 100 < 50) {
            cout << "\n⚡ [STUNNED] " << attacker.name << " is paralyzed by stun and cannot attack!" << endl;
            return false;
        }
    }

    cout << "\n>>> " << attacker.name << " attacks " << defender.name << "!" << endl;

    float multiplier = 1.0f;

    // 2. Rock-Paper-Scissors Elemental Logic (0: Fire, 1: Water, 2: Grass)
    if (attacker.elementalType == 0 && defender.elementalType == 2) multiplier = 2.0f;
    else if (attacker.elementalType == 1 && defender.elementalType == 0) multiplier = 2.0f;
    else if (attacker.elementalType == 2 && defender.elementalType == 1) multiplier = 2.0f;
    else if (attacker.elementalType == 2 && defender.elementalType == 0) multiplier = 0.5f;
    else if (attacker.elementalType == 0 && defender.elementalType == 1) multiplier = 0.5f;
    else if (attacker.elementalType == 1 && defender.elementalType == 2) multiplier = 0.5f;

    // 3. Direction 4: Dynamic Weather Affinity Modifier
    int weather = WorldState::getWeather();
    if (attacker.elementalType == 0 && weather == 1) { // Fire under Scorching Sun
        multiplier *= 1.25f;
        cout << "☀️🔥 [WEATHER BOOST] Scorching Sun intensifies " << attacker.name << "'s flames!" << endl;
    } else if (attacker.elementalType == 1 && weather == 2) { // Water under Torrential Rain
        multiplier *= 1.25f;
        cout << "🌧️💧 [WEATHER BOOST] Torrential Rain strengthens " << attacker.name << "'s tidal force!" << endl;
    } else if (attacker.elementalType == 2 && weather == 3) { // Grass under Mystic Fog
        multiplier *= 1.25f;
        cout << "🌫️🌿 [WEATHER BOOST] Mystic Fog shrouds and empowers " << attacker.name << "'s vines!" << endl;
    }

    // 4. Evaluate Attacker Status: Burn weakens attack output
    if (attacker.statusState == 2) {
        multiplier *= 0.75f;
        cout << "🔥 [BURN MALUS] " << attacker.name << "'s burn weakened its attack output!" << endl;
    }

    int damage = static_cast<int>(attacker.attackBase * multiplier);
    if (damage < 1) damage = 1;

    if (multiplier > 1.0f) cout << "🌟 It's super effective!" << endl;
    else if (multiplier < 1.0f) cout << "🛡️ It's not very effective..." << endl;

    // Deducting health directly from private member hp
    defender.hp -= damage;
    if (defender.hp < 0) defender.hp = 0;

    cout << "💥 " << defender.name << " took " << damage << " damage. (HP: " 
         << defender.hp << "/" << defender.maxHp << ")" << endl;

    // 5. Inflict Status Conditions (if defender is alive and not already afflicted)
    if (!defender.isFainted() && defender.statusState == 0) {
        int chance = rand() % 100;
        if (attacker.elementalType == 0 && chance < 35) { // 35% Burn from Fire
            defender.statusState = 2;
            cout << "🔥 [BURN INFLICTED] " << defender.name << " was badly burned!" << endl;
        } else if (attacker.elementalType == 1 && chance < 30) { // 30% Stun from Water
            defender.statusState = 3;
            cout << "⚡ [STUN INFLICTED] " << defender.name << " is paralyzed by the sudden impact!" << endl;
        } else if (attacker.elementalType == 2 && chance < 40) { // 40% Poison from Grass
            defender.statusState = 1;
            cout << "☠️ [POISON INFLICTED] " << defender.name << " was poisoned by toxic spores!" << endl;
        }
    }

    return true;
}
