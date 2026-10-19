#ifndef DAMAGECALCULATOR_H
#define DAMAGECALCULATOR_H

#include "Beast.h"

class DamageCalculator {
public:
    // Resolves attack between beasts, checks status ailments (stun/burn), and applies status effects
    static bool resolveAttack(Beast& attacker, Beast& defender);
};

#endif
