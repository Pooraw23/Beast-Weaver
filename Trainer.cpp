#include "Trainer.h"
#include <iostream>

// Initialize static member; one classwide copy[cite: 47, 65]
int Trainer::totalBeastsCaught = 0;

Trainer::Trainer(string name) {
    trainerName = name;
    partySize = 0;
    party = nullptr; // Initialize pointer
}

Trainer::~Trainer() {
    // Reclaiming the space allocated[cite: 46, 59]
    delete[] party; 
}

void Trainer::catchBeast(const Beast& newBeast) {
    // Dynamically allocate a new array of size + 1[cite: 60]
    Beast* tempArray = new Beast[partySize + 1];

    // Copy existing beasts over[cite: 60]
    for (int i = 0; i < partySize; i++) {
        tempArray[i] = party[i];
    }

    // Add the new beast to the end
    tempArray[partySize] = newBeast;

    // Delete the old array to prevent memory leaks[cite: 60]
    delete[] party;

    // Point to the new array
    party = tempArray;
    partySize++;
    
    // Increment static class variable[cite: 47]
    ++totalBeastsCaught; 
    
    cout << trainerName << " caught " << newBeast.getName() << "!" << endl;
}

void Trainer::printParty() const {
    cout << "\n--- " << trainerName << "'s Party ---" << endl;
    for (int i = 0; i < partySize; i++) {
        cout << i + 1 << ". " << party[i] << endl;
    }
}

// Overloaded array index operator with bounds checking[cite: 61]
Beast& Trainer::operator[](int index) {
    if (index >= 0 && index < partySize) {
        return party[index];
    } else {
        cout << "Array out of bounds!" << endl;
        abort(); //[cite: 61]
    }
}

const Beast& Trainer::operator[](int index) const {
    if (index >= 0 && index < partySize) {
        return party[index];
    } else {
        cout << "Array out of bounds!" << endl;
        abort(); //[cite: 61]
    }
}

int Trainer::getTotalCaught() {
    return totalBeastsCaught;
}
