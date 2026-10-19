#include "Marketplace.h"
#include <iomanip>

int Marketplace::standardPricing(int basePrice, int weather, bool isNight) {
    (void)weather; (void)isNight;
    return basePrice;
}

int Marketplace::dynamicWeatherPricing(int basePrice, int weather, bool isNight) {
    float modifier = 1.0f;
    if (weather == 1) modifier += 0.15f; // Scorching heat increases supply stress (+15%)
    else if (weather == 2) modifier -= 0.10f; // Rainy discount (-10%)
    if (isNight) modifier += 0.20f; // Nighttime black-market surcharge (+20%)

    int finalPrice = static_cast<int>(basePrice * modifier);
    return (finalPrice > 5) ? finalPrice : 5;
}

Marketplace::Marketplace() {
    priceAlgorithm = &Marketplace::dynamicWeatherPricing;
}

void Marketplace::setPricingModel(PricingFormula formula) {
    priceAlgorithm = formula;
}

int Marketplace::calculatePrice(int basePrice, int weather, bool isNight) const {
    // Call through function pointer (Slide 03)
    return (*priceAlgorithm)(basePrice, weather, isNight);
}

void Marketplace::visitMart(Trainer& player) {
    int weather = WorldState::getWeather();
    bool isNight = WorldState::isNight();

    int pPotion = calculatePrice(30, weather, isNight);
    int pSuper  = calculatePrice(60, weather, isNight);
    int pElixir = calculatePrice(50, weather, isNight);
    int pAnti   = calculatePrice(25, weather, isNight);
    int pNet    = calculatePrice(75, weather, isNight);

    cout << "\n╔══════════════════════════════════════════════════════════════════════════════╗\n";
    cout << "║                     🏪  THE GRAND BEASTMART MARKETPLACE                      ║\n";
    cout << "║ Wallet: " << setw(5) << player.getGold() << " Gold | Weather Influence: " << left << setw(38) << WorldState::getWeatherString() << "║\n";
    cout << "╠══════════════════════════════════════════════════════════════════════════════╣\n";
    cout << "║ [1] Health Potion (+25 HP)        - Price: " << setw(3) << pPotion << " Gold                           ║\n";
    cout << "║ [2] Super Potion  (+45 HP)        - Price: " << setw(3) << pSuper  << " Gold                           ║\n";
    cout << "║ [3] Attack Elixir (+4 Base ATK)   - Price: " << setw(3) << pElixir << " Gold                           ║\n";
    cout << "║ [4] Antidote      (Cures Ails)    - Price: " << setw(3) << pAnti   << " Gold                           ║\n";
    cout << "║ [5] Ultra Net     (+15 Trap Boost)- Price: " << setw(3) << pNet    << " Gold                           ║\n";
    cout << "║ [6] Leave Shop                                                               ║\n";
    cout << "╚══════════════════════════════════════════════════════════════════════════════╝\n";
    cout << "Select item to buy: ";

    int choice;
    if (cin >> choice) {
        WorldState::advanceTime(1);
        if (choice == 1 && player.getGold() >= pPotion) {
            player -= pPotion;
            player.addItem(Item("HealthPotion", "HEAL", 25, "Restores 25 HP."));
        } else if (choice == 2 && player.getGold() >= pSuper) {
            player -= pSuper;
            player.addItem(Item("SuperPotion", "HEAL", 45, "Restores 45 HP."));
        } else if (choice == 3 && player.getGold() >= pElixir) {
            player -= pElixir;
            player.addItem(Item("AttackElixir", "ATTACK", 4, "Increases base attack by 4."));
        } else if (choice == 4 && player.getGold() >= pAnti) {
            player -= pAnti;
            player.addItem(Item("Antidote", "CURE", 0, "Cures poison, burn, and stun ailments."));
        } else if (choice == 5 && player.getGold() >= pNet) {
            player -= pNet;
            player.addItem(Item("UltraNet", "TRAP", 15, "Captures beasts with higher health."));
        } else if (choice != 6) {
            cout << "\n❌ Transaction failed: insufficient gold or invalid item!" << endl;
        }
    }
}

void Marketplace::visitNightPeddler(Trainer& player) {
    if (!WorldState::isNight()) {
        cout << "\n🌙 The Traveling Night Peddler only emerges in the shroud of Night!" << endl;
        return;
    }

    cout << "\n╔══════════════════════════════════════════════════════════════════════════════╗\n";
    cout << "║                 🏮  SHADY NIGHT PEDDLER'S BLACK MARKET  🏮                   ║\n";
    cout << "║ \"Psst... Looking for the rarest contraband in the realm?\"                     ║\n";
    cout << "╠══════════════════════════════════════════════════════════════════════════════╣\n";
    cout << "║ [1] MasterTrap   (+35 Guaranteed Capture Rate)  - 180 Gold                    ║\n";
    cout << "║ [2] DragonScale  (+8 Permanent Attack Base)     - 160 Gold                    ║\n";
    cout << "║ [3] ElixirOfLife (+60 HP Restore + Cures Ails)  - 140 Gold                    ║\n";
    cout << "║ [4] Slip Away into the Shadows                                               ║\n";
    cout << "╚══════════════════════════════════════════════════════════════════════════════╝\n";
    cout << "Choose black-market item: ";

    int pChoice;
    if (cin >> pChoice) {
        if (pChoice == 1 && player.getGold() >= 180) {
            player -= 180;
            player.addItem(Item("MasterTrap", "TRAP", 35, "Mastercrafted snare with overwhelming capture power."));
        } else if (pChoice == 2 && player.getGold() >= 160) {
            player -= 160;
            player.addItem(Item("DragonScale", "ATTACK", 8, "Ancient infused drake scale."));
        } else if (pChoice == 3 && player.getGold() >= 140) {
            player -= 140;
            player.addItem(Item("ElixirOfLife", "HEAL", 60, "Alchemical panacea of absolute vitality."));
        } else if (pChoice != 4) {
            cout << "\n❌ \"No gold, no wares! Come back when you have coin...\"" << endl;
        }
    }
}
