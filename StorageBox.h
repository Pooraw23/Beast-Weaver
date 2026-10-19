#ifndef STORAGEBOX_H
#define STORAGEBOX_H

#include <iostream>
#include <fstream>
#include "Beast.h"

using namespace std;

class StorageBox {
private:
    Beast* boxArray;     // Dynamically allocated array for storage
    int count;           // Number of beasts currently stored
    int capacity;        // Current capacity of the dynamic array

    void resize(int newCapacity);

public:
    // Constructors and Destructor (Rule of Three)
    explicit StorageBox(int initialCapacity = 10);
    StorageBox(const StorageBox& other);
    StorageBox& operator=(const StorageBox& other);
    ~StorageBox();

    // Storage operations
    void addBeast(const Beast& beast);
    Beast removeBeast(int index);
    void printBox() const;
    int getCount() const;
    bool hasUsableBeasts() const;
    int getFirstUsableIndex() const;
    void healAll();

    // Subscript operator overloading with bounds checking
    Beast& operator[](int index);
    const Beast& operator[](int index) const;

    // File stream operators for persistent save/load
    friend ofstream& operator<<(ofstream& out, const StorageBox& box);
    friend ifstream& operator>>(ifstream& in, StorageBox& box);
};

#endif
