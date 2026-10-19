# Beast Weaver

A modular, turn-based C++ beast battle adventure game designed around core Object-Oriented Programming (OOP) principles.

---

## Architecture & OOP Design (Aligned with Course Lectures)

The project strictly follows the **3-file minimum architecture** (Interface `.h`, Implementation `.cpp`, and Test Driver):

- **[`Beast.h`](Beast.h) / [`Beast.cpp`](Beast.cpp)** *(Direction 2)*
  - Encapsulates beast stats (HP, attack, speed, elemental type, status conditions, level, exp).
  - Uses the **`this` pointer** for cascaded operations (`b.defend().heal();`).
  - Overloads compound assignment **`operator+=`** to consume `Item` objects.
  - Overloads unary prefix decrement **`operator--`** for end-of-turn status condition damage ticks.
  - Overloads unary prefix increment **`operator++`** for Level Up & stat progression.
  - Friend stream overloads for terminal output (`ostream& <<`) and file persistence (`ofstream& <<`, `ifstream& >>`).

- **[`Item.h`](Item.h) / [`Item.cpp`](Item.cpp)** *(Direction 2)*
  - Represents consumables (Health Potions, Attack Boosts, Cure Antidotes, Capture Traps).
  - Overloads relational **`operator==`** and **`operator!=`** for searching and matching in inventory.
  - Overloads stream operators (`<<`, `>>`) for serialization.

- **[`BattleArena.h`](BattleArena.h) / [`BattleArena.cpp`](BattleArena.cpp)** *(Direction 3 & 4)*
  - Dedicated arena management class handling turn-based combat between two `Trainer` objects passed **by reference** (`startMatch(Trainer&, Trainer&)`).
  - Visual **ASCII Battle HUD**: Dual-card battle layout with real-time graphical HP bars (`renderHpBar`), elemental tags, and status ailment badges.
  - Forces trainers to select the next valid conscious beast when the active beast faints (`hasUsableBeasts()`).
  - Incorporates NPC tactical AI executing cascaded member function calls via the **`this` pointer** (`npc[idx].defend().heal()`).
  - Automatically awards experience/level-ups (`++player[idx]`) and wager prize money (`player += npc.getPrizeMoney()`).

- **[`WorldState.h`](WorldState.h) / [`WorldState.cpp`](WorldState.cpp)** *(Direction 4: The Game Master)*
  - Global world clock managing **Day / Night cycles** (Morning, Afternoon, Dusk, Night) and total turn tracking.
  - Implements **class-wide `static` variables** and static methods (`advanceTime`, `restUntilMorning`, `isNight`).
  - Stream operator overloading (`operator<<`) for the Overworld HUD banner and file serialization.

- **[`StorageBox.h`](StorageBox.h) / [`StorageBox.cpp`](StorageBox.cpp)** *(Direction 1)*
  - Implements the "PC Storage Box" managing excess beasts beyond the 3-beast active party limit.
  - Utilizes dynamic memory allocation (`new[]` / `delete[]`) with self-resizing.
  - Full **Rule of Three** implementation: Destructor, Copy Constructor (deep copy), and Copy Assignment Operator (`operator=`).
  - Overloads subscript **`operator[]`** with bounds checking.

- **[`Trainer.h`](Trainer.h) / [`Trainer.cpp`](Trainer.cpp)** *(Direction 1 & 2)*
  - Manages active party (capped at 3 beasts) and inventory backpack with dynamic memory and **Rule of Three**.
  - Custom **Parameterized NPC Constructor** (`Trainer(name, tier)`) pre-filling dynamic party arrays and prize wagers based on difficulty.
  - Evaluates party health via `hasUsableBeasts()`.
  - Overloads binary **`operator+=`** and **`operator-=`** for wallet gold management (prize earnings & shop purchases).
  - Class-wide **`static` data member** and `static` function tracking total beasts caught.
  - Stream serialization overloads (`ofstream& <<`, `ifstream& >>`).

- **[`Damage_Calculator.h`](Damage_Calculator.h) / [`Damage_Calculator.cpp`](Damage_Calculator.cpp)** *(Direction 3: Combat Engineer)*
  - Friend class of `Beast` evaluating elemental advantages (Fire, Water, Grass) directly on private members.
  - Evaluates status conditions (Stun skip chance, Burn damage malus, and elemental status procs).

- **[`TestDriver.cpp`](TestDriver.cpp)** *(Direction 4: The Game Master)*
  - Interactive Overworld Map navigation hub connecting wilderness zones (Verdant Canopy, Obsidian Crags, Azure Reef), Colosseum arena, BeastMart, and basecamp.

---

## How to Build and Run

Compile all modules using `g++` (or `clang++`):

```bash
g++ -std=c++17 -Wall Beast.cpp Item.cpp Trainer.cpp StorageBox.cpp Damage_Calculator.cpp BattleArena.cpp WorldState.cpp TestDriver.cpp -o BeastWeaver
./BeastWeaver
```
