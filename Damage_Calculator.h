#ifndef DAMAGECALCULATOR_H
#define DAMAGECALCULATOR_H

#include "Beast.h"

class DamageCalculator {
public:
    // Static function so we don't need to instantiate a DamageCalculator object to use it
    static void resolveAttack(Beast& attacker, Beast& defender);
};

#endif
