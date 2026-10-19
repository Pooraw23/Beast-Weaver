# Beast Weaver

A modular, turn-based C++ beast battle adventure game designed around core Object-Oriented Programming (OOP) principles.

---

## Architecture & OOP Design (Aligned with Course Lectures)

The project strictly follows the **3-file minimum architecture** (Interface `.h`, Implementation `.cpp`, and Test Driver):

- **[`Beast.h`](Beast.h) / [`Beast.cpp`](Beast.cpp)**
  - Encapsulates beast stats (HP, attack, speed, elemental type, status conditions, level, exp).
  - Uses the **`this` pointer** for cascaded operations (`b.defend().heal();`).
  - Overloads compound assignment **`operator+=`** to consume `Item` objects.
  - Overloads unary prefix decrement **`operator--`** for end-of-turn status condition damage ticks.
  - Overloads unary prefix increment **`operator++`** for Level Up & stat progression.
  - Friend stream overloads for terminal output (`ostream& <<`) and file persistence (`ofstream& <<`, `ifstream& >>`).

- **[`Item.h`](Item.h) / [`Item.cpp`](Item.cpp)**
  - Represents consumables (Health Potions, Attack Boosts, Cure Antidotes, Capture Traps).
  - Overloads relational **`operator==`** and **`operator!=`** for searching and matching in inventory.
  - Overloads stream operators (`<<`, `>>`) for serialization.

- **[`BattleArena.h`](BattleArena.h) / [`BattleArena.cpp`](BattleArena.cpp)**
  - Dedicated arena management class handling turn-based combat between two `Trainer` objects passed **by reference** (`startMatch(Trainer&, Trainer&)`).
  - Forces trainers to select the next valid conscious beast when the active beast faints (`hasUsableBeasts()`).
  - Incorporates NPC tactical AI executing cascaded member function calls via the **`this` pointer** (`npc[idx].defend().heal()`).
  - Automatically awards experience/level-ups (`++player[idx]`) and wager prize money (`player += npc.getPrizeMoney()`).

- **[`StorageBox.h`](StorageBox.h) / [`StorageBox.cpp`](StorageBox.cpp)**
  - Implements the "PC Storage Box" managing excess beasts beyond the 3-beast active party limit.
  - Utilizes dynamic memory allocation (`new[]` / `delete[]`) with self-resizing.
  - Full **Rule of Three** implementation: Destructor, Copy Constructor (deep copy), and Copy Assignment Operator (`operator=`).
  - Overloads subscript **`operator[]`** with bounds checking.

- **[`Trainer.h`](Trainer.h) / [`Trainer.cpp`](Trainer.cpp)**
  - Manages active party (capped at 3 beasts) and inventory backpack with dynamic memory and **Rule of Three**.
  - Custom **Parameterized NPC Constructor** (`Trainer(name, tier)`) pre-filling dynamic party arrays and prize wagers based on difficulty.
  - Evaluates party health via `hasUsableBeasts()`.
  - Overloads binary **`operator+=`** and **`operator-=`** for wallet gold management (prize earnings & shop purchases).
  - Class-wide **`static` data member** and `static` function tracking total beasts caught.
  - Stream serialization overloads (`ofstream& <<`, `ifstream& >>`).

- **[`Damage_Calculator.h`](Damage_Calculator.h) / [`Damage_Calculator.cpp`](Damage_Calculator.cpp)**
  - Friend class of `Beast` evaluating elemental advantages (Fire, Water, Grass) directly on private members.
  - Evaluates status conditions (Stun skip chance, Burn damage malus, and elemental status procs).

- **[`TestDriver.cpp`](TestDriver.cpp)**
  - Interactive terminal test driver and gameplay loop connecting all modules, shops, arenas, and storage.

---

## Core Systems & Mechanics

1. **Dedicated BattleArena (Composition & References)**:
   - Trainers battle in a dedicated arena class passed by reference until one trainer's entire party faints.
2. **Experience Points & Leveling (Unary Operator `++`)**:
   - Defeating opponent beasts grants EXP; upon leveling up, `++beast` increments level, boosts Max HP, Attack, Speed, and restores health.
3. **Prize Money & The Beast Mart (Binary Operators `+=` & `-=`)**:
   - Wagers earned from NPC victories (`player += prizeMoney`).
   - Spend gold at the Beast Mart for consumables (`player -= itemPrice`).
4. **NPC AI & Cascaded Commands (`this` Pointer)**:
   - NPCs analyze battlefield conditions; if HP falls below 25%, executes `npc[idx].defend().heal()` in a single cascaded expression.
5. **Custom NPC Constructors (Parameterized Instantiation)**:
   - `Trainer npc(name, tier)` automatically sets up themed parties for Novice Scouts, Specialty Hikers, and Elite Champions.
6. **Persistent Save/Load System**:
   - Full serialization of party, gold, storage, and items via file stream operators into `save.txt`.
7. **PC Storage Box (Rule of Three)**:
   - 3-beast active party cap with automatic transfer to PC storage and full swap/deposit/withdraw management.

---

## How to Build and Run

Compile all modules using `g++` (or `clang++`):

```bash
g++ -std=c++17 -Wall Beast.cpp Item.cpp Trainer.cpp StorageBox.cpp Damage_Calculator.cpp BattleArena.cpp TestDriver.cpp -o BeastWeaver
./BeastWeaver
```
