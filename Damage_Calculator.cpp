#include "Damage_Calculator.h"
#include <iostream>

using namespace std;

void DamageCalculator::resolveAttack(Beast& attacker, Beast& defender) {
    cout << "\n" << attacker.getName() << " attacks " << defender.getName() << "!" << endl;
    
    float multiplier = 1.0;
    
    // Rock-Paper-Scissors Logic (0: Fire, 1: Water, 2: Grass)
    // Accessing private members directly due to friend function status[cite: 48]
    if (attacker.elementalType == 0 && defender.elementalType == 2) multiplier = 2.0;
    else if (attacker.elementalType == 1 && defender.elementalType == 0) multiplier = 2.0;
    else if (attacker.elementalType == 2 && defender.elementalType == 1) multiplier = 2.0;
    else if (attacker.elementalType == 2 && defender.elementalType == 0) multiplier = 0.5;
    else if (attacker.elementalType == 0 && defender.elementalType == 1) multiplier = 0.5;
    else if (attacker.elementalType == 1 && defender.elementalType == 2) multiplier = 0.5;

    int damage = attacker.attackBase * multiplier;
    
    if (multiplier > 1.0) cout << "It's super effective!" << endl;
    else if (multiplier < 1.0) cout << "It's not very effective..." << endl;
    
    // Deducting health directly from private member[cite: 48]
    defender.hp -= damage;
    if (defender.hp < 0) defender.hp = 0;
    
    cout << defender.getName() << " took " << damage << " damage." << endl;
}
