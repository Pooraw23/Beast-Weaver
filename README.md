# Beast Weaver

A modular, turn-based C++ beast battle adventure game designed around core Object-Oriented Programming (OOP) principles.

---

## Architecture & OOP Design (Aligned with Course Lectures)

The project strictly follows the **3-file minimum architecture** (Interface `.h`, Implementation `.cpp`, and Test Driver):

- **[`Beast.h`](Beast.h) / [`Beast.cpp`](Beast.cpp)**
  - Encapsulates beast stats (HP, attack, speed, elemental type, status conditions).
  - Uses the **`this` pointer** for cascaded operations (`b.defend().heal();`).
  - Overloads compound assignment **`operator+=`** to consume `Item` objects.
  - Overloads unary prefix decrement **`operator--`** for end-of-turn status condition ticks.
  - Friend stream overloads for terminal output (`ostream& <<`) and file persistence (`ofstream& <<`, `ifstream& >>`).

- **[`Item.h`](Item.h) / [`Item.cpp`](Item.cpp)**
  - Represents consumables (Health Potions, Attack Boosts, Cure Antidotes, Capture Traps).
  - Overloads relational **`operator==`** and **`operator!=`** for searching and matching in inventory.
  - Overloads stream operators (`<<`, `>>`) for serialization.

- **[`StorageBox.h`](StorageBox.h) / [`StorageBox.cpp`](StorageBox.cpp)**
  - Implements the "PC Storage Box" managing excess beasts beyond the 3-beast active party limit.
  - Utilizes dynamic memory allocation (`new[]` / `delete[]`) with self-resizing.
  - Full **Rule of Three** implementation: Destructor, Copy Constructor (deep copy), and Copy Assignment Operator (`operator=`).
  - Overloads subscript **`operator[]`** with bounds checking.

- **[`Trainer.h`](Trainer.h) / [`Trainer.cpp`](Trainer.cpp)**
  - Manages active party (capped at 3 beasts) and inventory backpack.
  - Dynamic memory management for both party and inventory arrays with **Rule of Three** compliance.
  - Class-wide **`static` data member** and `static` function tracking total beasts caught.
  - Seamlessly routes captured beasts to party or PC Storage Box.
  - Stream serialization overloads (`ofstream& <<`, `ifstream& >>`).

- **[`Damage_Calculator.h`](Damage_Calculator.h) / [`Damage_Calculator.cpp`](Damage_Calculator.cpp)**
  - Friend class of `Beast` evaluating elemental advantages (Fire, Water, Grass) directly on private members.
  - Evaluates status conditions (Stun skip chance, Burn damage malus, and elemental status procs).

- **[`TestDriver.cpp`](TestDriver.cpp)**
  - Interactive terminal test driver and gameplay loop connecting all 4 modules.

---

## 4 Major Core Features

1. **Persistent Save/Load System**:
   - Saves complete game state (Trainer, Active Party, Inventory, PC Storage Box, Stats) into `save.txt`.
   - Automatically detects existing save files on startup and prompts to resume.
2. **Inventory & Consumables**:
   - Trainers carry usable items in a dynamic backpack.
   - Using items in combat or hub uses `beast += item` syntax.
   - Search inventory using `item == other`.
3. **Status Conditions**:
   - Burn (inflicts damage each turn + reduces attack power).
   - Poison (inflicts damage each turn).
   - Stun (50% chance to skip turn due to paralysis).
   - Automated end-of-turn execution via `--beast`.
4. **PC Storage Box & Party Limit**:
   - Active party capped at 3 beasts; subsequent captures are automatically stored in PC.
   - Players can deposit, withdraw, or swap beasts between active party and PC storage.

---

## How to Build and Run

Compile all modules using `g++` (or `clang++`):

```bash
g++ -std=c++17 -Wall Beast.cpp Item.cpp Trainer.cpp StorageBox.cpp Damage_Calculator.cpp TestDriver.cpp -o BeastWeaver
./BeastWeaver
```
