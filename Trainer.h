#ifndef TRAINER_H
#define TRAINER_H

#include <string>
#include "Beast.h" // Object Composition

using namespace std;

class Trainer {
private:
    string trainerName;
    Beast* party;  // Pointer for dynamic memory allocation of the Beast array
    int partySize;
    
    // Static data member to track global progress across all Trainer instances
    static int totalBeastsCaught; 

public:
    // Constructors and Destructors
    Trainer(string name);
    ~Trainer(); // Crucial: Will contain delete[] party; to prevent memory leaks

    // Core functionality
    void catchBeast(const Beast& newBeast); // Handles the new/delete dynamic resizing
    void printParty() const;

    // Operator overloading for array indexing
    Beast& operator[](int index); 
    const Beast& operator[](int index) const; 

    // Static function to read the static variable
    static int getTotalCaught(); 
};

#endif
