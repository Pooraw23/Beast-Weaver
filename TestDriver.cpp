#include <iostream>
#include "Beast.h"
#include "Trainer.h"
#include "Damage_Calculator.h"

using namespace std;

int main() {
    // Instantiate objects[cite: 31]
    Trainer player("Pratyush");
    
    // Create base beasts (Name, Type, HP, Attack, Speed)
    Beast starter("Pyrosaur", 0, 50, 15, 10);
    Beast wild("Aquafang", 1, 40, 12, 8);
    
    player.catchBeast(starter);
    
    bool playing = true;
    int choice;
    
    cout << "\n--- WILD BATTLE START ---" << endl;
    
    while (playing && !wild.isFainted() && !player[0].isFainted()) {
        cout << "\n=================================" << endl;
        cout << "Enemy: " << wild << endl; // Uses overloaded <<[cite: 57]
        cout << "You:   " << player[0] << endl; // Uses overloaded [] and <<[cite: 57, 61]
        cout << "=================================" << endl;
        
        cout << "1. Attack\n2. Defend & Heal\n3. Capture\n4. Run\nChoose action: ";
        cin >> choice; // Taking input from user[cite: 26]
        
        if (choice == 1) {
            DamageCalculator::resolveAttack(player[0], wild);
            if (!wild.isFainted()) {
                DamageCalculator::resolveAttack(wild, player[0]);
            }
        } 
        else if (choice == 2) {
            // Cascaded member function call using 'this' pointer[cite: 44, 45]
            player[0].defend().heal(); 
            DamageCalculator::resolveAttack(wild, player[0]);
        }
        else if (choice == 3) {
            if (wild.getHp() < 15) {
                player.catchBeast(wild);
                playing = false;
            } else {
                cout << "The beast's HP is too high to capture!" << endl;
                DamageCalculator::resolveAttack(wild, player[0]);
            }
        }
        else if (choice == 4) {
            cout << "Got away safely!" << endl;
            playing = false;
        }
    }
    
    if (wild.isFainted()) {
        cout << wild.getName() << " fainted! You win!" << endl;
    } else if (player[0].isFainted()) {
        cout << player[0].getName() << " fainted! You blacked out!" << endl;
    }
    
    player.printParty();
    
    // Checking static member[cite: 47]
    cout << "\nTotal beasts captured across game: " << Trainer::getTotalCaught() << endl;
    
    return 0;
}
