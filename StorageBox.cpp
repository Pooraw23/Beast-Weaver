#include "StorageBox.h"
#include <cstdlib>

// Default and parameterized constructor
StorageBox::StorageBox(int initialCapacity) {
    capacity = (initialCapacity > 0) ? initialCapacity : 10;
    count = 0;
    boxArray = new Beast[capacity];
}

// Copy constructor for deep copy (Slide 07)
StorageBox::StorageBox(const StorageBox& other) {
    capacity = other.capacity;
    count = other.count;
    boxArray = new Beast[capacity];
    for (int i = 0; i < count; i++) {
        boxArray[i] = other.boxArray[i];
    }
}

// Overloaded assignment operator with self-assignment check (Slide 05 & 07)
StorageBox& StorageBox::operator=(const StorageBox& other) {
    if (this != &other) {
        delete[] boxArray; // Reclaim old memory
        capacity = other.capacity;
        count = other.count;
        boxArray = new Beast[capacity];
        for (int i = 0; i < count; i++) {
            boxArray[i] = other.boxArray[i];
        }
    }
    return *this;
}

// Destructor
StorageBox::~StorageBox() {
    delete[] boxArray;
}

// Private helper for dynamic array resizing
void StorageBox::resize(int newCapacity) {
    Beast* temp = new Beast[newCapacity];
    for (int i = 0; i < count; i++) {
        temp[i] = boxArray[i];
    }
    delete[] boxArray;
    boxArray = temp;
    capacity = newCapacity;
}

void StorageBox::addBeast(const Beast& beast) {
    if (count >= capacity) {
        resize(capacity * 2);
    }
    boxArray[count] = beast;
    count++;
    cout << "Stored " << beast.getName() << " in PC Storage Box (Slot " << count << ")." << endl;
}

Beast StorageBox::removeBeast(int index) {
    if (index < 0 || index >= count) {
        cout << "StorageBox Error: Invalid index " << index << endl;
        return Beast();
    }
    Beast retrieved = boxArray[index];
    // Shift elements down
    for (int i = index; i < count - 1; i++) {
        boxArray[i] = boxArray[i + 1];
    }
    count--;
    return retrieved;
}

void StorageBox::printBox() const {
    cout << "\n==========================================" << endl;
    cout << "             PC STORAGE BOX               " << endl;
    cout << "==========================================" << endl;
    if (count == 0) {
        cout << "  (Storage Box is currently empty)" << endl;
    } else {
        for (int i = 0; i < count; i++) {
            cout << "  [" << (i + 1) << "] " << boxArray[i] << endl;
        }
    }
    cout << "==========================================" << endl;
}

int StorageBox::getCount() const {
    return count;
}

bool StorageBox::hasUsableBeasts() const {
    for (int i = 0; i < count; i++) {
        if (!boxArray[i].isFainted()) return true;
    }
    return false;
}

int StorageBox::getFirstUsableIndex() const {
    for (int i = 0; i < count; i++) {
        if (!boxArray[i].isFainted()) return i;
    }
    return -1;
}

void StorageBox::healAll() {
    for (int i = 0; i < count; i++) {
        boxArray[i].restoreHealth();
    }
}

Beast& StorageBox::operator[](int index) {
    if (index >= 0 && index < count) {
        return boxArray[index];
    } else {
        cout << "StorageBox: Index out of bounds!" << endl;
        abort();
    }
}

const Beast& StorageBox::operator[](int index) const {
    if (index >= 0 && index < count) {
        return boxArray[index];
    } else {
        cout << "StorageBox: Index out of bounds!" << endl;
        abort();
    }
}

ofstream& operator<<(ofstream& out, const StorageBox& box) {
    out << box.count << "\n";
    for (int i = 0; i < box.count; i++) {
        out << box.boxArray[i]; // Uses Beast's ofstream overload
    }
    return out;
}

ifstream& operator>>(ifstream& in, StorageBox& box) {
    int loadedCount = 0;
    if (in >> loadedCount) {
        // Clear existing array
        delete[] box.boxArray;
        box.count = 0;
        box.capacity = (loadedCount > 10) ? loadedCount + 5 : 10;
        box.boxArray = new Beast[box.capacity];

        for (int i = 0; i < loadedCount; i++) {
            Beast b;
            in >> b; // Uses Beast's ifstream overload
            box.boxArray[box.count++] = b;
        }
    }
    return in;
}
