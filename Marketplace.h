#ifndef MARKETPLACE_H
#define MARKETPLACE_H

#include <iostream>
#include <string>
#include "Trainer.h"
#include "WorldState.h"

using namespace std;

// Function pointer for pricing calculations (Slide 03)
typedef int (*PricingFormula)(int basePrice, int weather, bool isNight);

class Marketplace {
private:
    PricingFormula priceAlgorithm; // Function pointer member

    // Static pricing formulas
    static int standardPricing(int basePrice, int weather, bool isNight);
    static int dynamicWeatherPricing(int basePrice, int weather, bool isNight);

public:
    Marketplace();
    void setPricingModel(PricingFormula formula);

    int calculatePrice(int basePrice, int weather, bool isNight) const;

    // Interactive shop terminals
    void visitMart(Trainer& player);
    void visitNightPeddler(Trainer& player);
};

#endif
