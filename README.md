# Beast Weaver

A turn-based C++ beast battle game.

## Project Structure

- `Beast.h` / `Beast.cpp` - Beast class representing creatures, their stats, and battle actions.
- `Trainer.h` / `Trainer.cpp` - Trainer class managing team beasts and inventory.
- `Damage_Calculator.h` / `Damage_Calculator.cpp` - Damage and combat resolution mechanics.
- `TestDriver.cpp` - Main entry point and interactive game loop.

## How to Build and Run

Compile all files using `g++` (or `clang++`):

```bash
g++ -std=c++17 Beast.cpp Trainer.cpp Damage_Calculator.cpp TestDriver.cpp -o BeastWeaver
./BeastWeaver
```
