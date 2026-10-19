# Beast Weaver

A modular, turn-based C++ beast battle adventure game designed around core Object-Oriented Programming (OOP) principles.

---

## Architecture & OOP Design (Aligned with Course Lectures)

The project strictly follows the **3-file minimum architecture** (Interface `.h`, Implementation `.cpp`, and Test Driver), organizing features into four distinct architectural directions:

### Direction 4: The Game Master (UI, World State, Navigation & Game Flow)
*This is the user's primary direction, focusing on terminal UI rendering, world states, game loops, dynamic data structures, and player progression.*

1. **[`WorldMap.h`](WorldMap.h) / [`WorldMap.cpp`](WorldMap.cpp)**
   - **2D Dynamic Grid Allocation & Subscript Overloading (`operator()`)**: Allocates a 2D dynamic `char** grid` with deep copying (Rule of Three). Implements `char& operator()(int r, int c)` with boundary assertions for coordinate indexing.
   - **Interactive Overworld Exploration**: WASD tile movement across towns (`T`), markets (`M`), colosseums (`C`), forests (`F`), volcanos (`V`), and oceans (`O`).

2. **[`TerminalHUD.h`](TerminalHUD.h) / [`TerminalHUD.cpp`](TerminalHUD.cpp)**
   - **ANSI Color Terminal Rendering & Const Qualifications**: Employs `static const string` ANSI escape constants for bold styling, elemental badges, and dynamic color-coded health meters (`renderColorHpBar`).

3. **[`BattleLog.h`](BattleLog.h) / [`BattleLog.cpp`](BattleLog.cpp)**
   - **Dynamic String Array Buffer & Rule of Three**: Dynamically allocated event string buffer (`string* events`) with automatic self-resizing, deep copy constructor, deep copy assignment operator, and destructor.
   - Overloads `operator<<` for battle replay inspection and `exportToFile` for persistent combat logs.

4. **[`EventDispatcher.h`](EventDispatcher.h) / [`EventDispatcher.cpp`](EventDispatcher.cpp)**
   - **Wilderness Random Encounters & Function Pointer Table**: Uses `typedef void (*EventCallback)(Trainer&, WorldState&)` to store an array of event callbacks (Shrine of Elements, Smuggler's Cache, Wandering Hermit, Mystical Springs). Dispatches random wilderness anomalies upon step completion.

5. **[`BountyBoard.h`](BountyBoard.h) / [`BountyBoard.cpp`](BountyBoard.cpp)**
   - **Noticeboard Contracts & Static Completion Tracking**: Dynamic array of `Bounty` objects with Rule of Three memory management. Tracks global quest completion via `static int totalBountiesCompleted`.
   - Overloads `operator<<` for a terminal bounty board UI with completion stamps.

6. **[`Marketplace.h`](Marketplace.h) / [`Marketplace.cpp`](Marketplace.cpp)**
   - **Dynamic Market Economy & Function Pointer Pricing**: Employs `typedef int (*PricingFormula)(int basePrice, int weather, bool isNight)` allowing interchangeable pricing strategies (Standard Day Rates, Shady Night Peddler markup, Weather surge pricing).

7. **[`ReputationManager.h`](ReputationManager.h) / [`ReputationManager.cpp`](ReputationManager.cpp)**
   - **Trainer Renown & Cascaded Member Functions (`this` Pointer)**: Static renown tracking (`static int globalFamePoints`). Uses `return *this` to support cascaded method chaining: `rep.addFame(150).checkRank();`.
   - Overloads `operator<<` for fame badge inspection.

8. **[`TrainerCard.h`](TrainerCard.h) / [`TrainerCard.cpp`](TrainerCard.cpp)**
   - **Class Composition via References**: Composes references (`Trainer&`, `Bestiary&`, `AchievementSystem&`, `WorldState&`, `ReputationManager&`) into a single passport object without unnecessary copying.
   - Overloads `operator<<` to render a double-bordered trainer passport card.

9. **[`GameModeSettings.h`](GameModeSettings.h) / [`GameModeSettings.cpp`](GameModeSettings.cpp)**
   - **Challenge Game Modes & Global Flags**: Static configuration switches for **Nuzlocke Permadeath** (fainted beasts cannot be revived) and **Speedrun Turn Countdown** (strict total-turn countdown clock).

10. **[`SaveManager.h`](SaveManager.h) / [`SaveManager.cpp`](SaveManager.cpp)**
    - **Multi-Save Slot System & Profile Serialization**: Dynamically allocated array of profile slots with Rule of Three. Serializes and deserializes Trainer party, PC storage box, unlocked achievements, Pokédex discovery, and world clock state to individual slot files (`save_slot1.txt`, `save_slot2.txt`, `save_slot3.txt`).

11. **[`WorldState.h`](WorldState.h) / [`WorldState.cpp`](WorldState.cpp)**
    - Global world clock managing **Day / Night cycles** (Morning, Afternoon, Dusk, Night) and **Dynamic Weather** (Clear Skies, Scorching Sun, Torrential Rain, Mystic Fog) with `static` variables.

12. **[`Achievement.h`](Achievement.h) / [`Achievement.cpp`](Achievement.cpp)**
    - Tracks player milestones and achievements with `static` counters, dynamic memory, and `operator<<` trophy case rendering.

13. **[`Bestiary.h`](Bestiary.h) / [`Bestiary.cpp`](Bestiary.cpp)**
    - Pokédex / Beast Encyclopedia logging 12 elemental species with Rule of Three and subscript `operator[]`.

14. **[`Tournament.h`](Tournament.h) / [`Tournament.cpp`](Tournament.cpp)**
    - 3-round single elimination championship (Quarterfinals -> Semifinals -> Grand Finals) composing `BattleArena`.

---

### Direction 1, 2 & 3 Support Modules

- **[`BattleArena.h`](BattleArena.h) / [`BattleArena.cpp`](BattleArena.cpp)** *(Direction 3 & 4)*: Turn-based arena loop passing `Trainer&` references, dual-card ASCII HUD, NPC strategy using `this` pointer cascading (`npc[idx].defend().heal()`), and EXP/Prize payouts.
- **[`Beast.h`](Beast.h) / [`Beast.cpp`](Beast.cpp)** *(Direction 2)*: Encapsulates stats, cascaded commands, `operator+=` for items, unary `--` for status damage, and unary `++` for level-ups.
- **[`Item.h`](Item.h) / [`Item.cpp`](Item.cpp)** *(Direction 2)*: Usable consumables with relational `operator==` and stream operators.
- **[`StorageBox.h`](StorageBox.h) / [`StorageBox.cpp`](StorageBox.cpp)** *(Direction 1)*: PC Storage Box dynamic array with Rule of Three and subscript `operator[]`.
- **[`Trainer.h`](Trainer.h) / [`Trainer.cpp`](Trainer.cpp)** *(Direction 1 & 2)*: Active party management, parameterized difficulty constructors, wallet operators (`+=`, `-=`), and `static` catch counters.
- **[`Damage_Calculator.h`](Damage_Calculator.h) / [`Damage_Calculator.cpp`](Damage_Calculator.cpp)** *(Direction 3)*: Friend class evaluating rock-paper-scissors affinities, status conditions, and weather boosts.

---

## How to Build and Run

Compile all modules using `g++` (C++17):

```bash
g++ -std=c++17 -Wall \
  Beast.cpp Item.cpp Trainer.cpp StorageBox.cpp Damage_Calculator.cpp \
  BattleArena.cpp WorldState.cpp Achievement.cpp Bestiary.cpp Tournament.cpp \
  TerminalHUD.cpp BattleLog.cpp WorldMap.cpp EventDispatcher.cpp \
  BountyBoard.cpp Marketplace.cpp ReputationManager.cpp TrainerCard.cpp \
  GameModeSettings.cpp SaveManager.cpp TestDriver.cpp \
  -o BeastWeaver
```

Run the game:
```bash
./BeastWeaver
```

---

## Unit Testing

To run the automated Direction 4 verification suite:
```bash
g++ -std=c++17 -Wall -I. \
  Beast.cpp Item.cpp Trainer.cpp StorageBox.cpp Damage_Calculator.cpp \
  BattleArena.cpp WorldState.cpp Achievement.cpp Bestiary.cpp Tournament.cpp \
  TerminalHUD.cpp BattleLog.cpp WorldMap.cpp EventDispatcher.cpp \
  BountyBoard.cpp Marketplace.cpp ReputationManager.cpp TrainerCard.cpp \
  GameModeSettings.cpp SaveManager.cpp \
  ~/.gemini/antigravity/brain/4e29e35c-e199-4974-9d7f-08bee0c19819/scratch/unit_tests_5.cpp \
  -o unit_tests_5
./unit_tests_5
```
