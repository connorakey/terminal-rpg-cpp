# Terminal RPG

A text-based single player role-playing game written in C++ that runs in the terminal.

## Features

### Core Gameplay
- **Turn-based Combat System** - Engage enemies with normal attacks, power attacks, or defensive maneuvers
- **Enemy Encounters** - Face enemies that scale in difficulty with your level, from common goblins to legendary bosses
- **Inventory Management** - Collect weapons, armor, potions, and treasure
- **Merchant System** - Buy and sell items with wandering merchants
- **Chest System** - Discover chests that may be locked or trapped with various hazards
- **Leveling System** - Gain experience and level up to become stronger

### Combat Features
- **Weapon System** - Equip weapons with unique stats
- **Stamina** - Balance attack power with remaining stamina
- **Combat Actions**:
  - Normal Attack (costs stamina)
  - Power Attack (double damage, higher stamina cost)
  - Defend (recover stamina, reduce damage)
  - Use Items (potions for healing, buffs, or offensive effects)
  - Equip Weapons mid-combat
  - Attempt to Flee (20% chance)
- **Weapon Durability** - Weapons degrade with use and can break during combat
- **Critical Hits** - 10% chance to deal double damage

### Item Types
- **Weapons** - Various weapon types with damage ranges, accuracy, and durability
- **Armour** - Protective gear with armor value and durability
- **Potions** - Consumables for healing, stamina, damage buffs, defense, resistance, or poison
- **Currency** - Gold for trading with merchants
- **Miscellaneous** - Various collectible items

### Enemy System
- **Enemy Types**: Beast, Undead, Humanoid, Dragon, Elemental, Demon, Goblinoid
- **Rarity Levels**: Common, Uncommon, Rare, Epic, Legendary, Boss
- **Dynamic Spawning** - Enemies spawn within ±2 levels of the player
- **Boss Rewards** - Boss enemies grant 5x gold and experience

### Random Events
- **Enemy Encounters** (60% chance)
- **Treasure Chests** (30% chance)
- **Wandering Merchants** (10% chance)

### Trap System
Chests may contain traps:
- **Poison Dart** - Deals poison damage
- **Explosion** - Deals damage and may summon enemies
- **Alarm** - Alerts nearby enemies

## Building the Project

### Prerequisites
- CMake 3.15 or higher
- C++17 compatible compiler (GCC, Clang, or MSVC)
- Git (for cloning the repository)

### Build Instructions

#### Linux/macOS
```bash
# Clone the repository
git clone <repository-url>
cd terminal-rpg-cpp

# Run the build script
chmod +x build-release-unix.sh
./build-release-unix.sh
```

#### Windows (PowerShell)
```powershell
# Clone the repository
git clone <repository-url>
cd terminal-rpg-cpp

# Run the build script
.\build-release-windows.ps1
```

#### Windows (Command Prompt)
```cmd
# Clone the repository
git clone <repository-url>
cd terminal-rpg-cpp

# Run the build script
build-release-windows.cmd
```

#### Manual Build
```bash
# Create build directory
mkdir cmake-build-release
cd cmake-build-release

# Configure
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build . --config Release

# Run tests
ctest --output-on-failure
```

### Running the Game
After building, the binary will be located in the build directory:
```bash
# Linux/macOS
./cmake-build-release/terminal_rpg

# Windows
cmake-build-release\Release\terminal_rpg.exe
```

## Testing

The project uses [Catch2](https://github.com/catchorg/Catch2) testing framework. Tests are automatically run during the build process.

Run tests manually:
```bash
cd cmake-build-release
ctest --output-on-failure
```

### Test Coverage
- Chest system (traps, locks, loot)
- Player mechanics (damage, healing, stamina, leveling)
- Enemy system (combat, damage types)
- Item system (creation, properties)
- Item database (item retrieval, type filtering)
- Level database (level titles, experience)

## Project Structure

```
terminal-rpg-cpp/
├── src/
│   ├── main.cpp                 # Main game loop and logic
│   ├── chest/                   # Chest and trap system
│   │   ├── chest.cpp
│   │   └── chest.hpp
│   ├── enemies/                 # Enemy system
│   │   ├── enemy.cpp
│   │   ├── enemy.hpp
│   │   ├── enemydatabase.cpp
│   │   └── enemydatabase.hpp
│   ├── items/                   # Item system
│   │   ├── item.cpp
│   │   ├── item.hpp
│   │   ├── itemdatabase.cpp
│   │   └── itemdatabase.hpp
│   ├── levels/                  # Leveling system
│   │   ├── leveldatabase.cpp
│   │   └── leveldatabase.hpp
│   └── player/                  # Player character
│       ├── player.cpp
│       └── player.hpp
├── tests/                       # Unit tests
│   ├── test_chest.cpp
│   ├── test_enemy.cpp
│   ├── test_item.cpp
│   ├── test_itemdatabase.cpp
│   ├── test_leveldatabase.cpp
│   └── test_player.cpp
├── .github/workflows/           # CI/CD pipelines
│   ├── auto-format.yml          # Auto-formatting
│   ├── build-release-binaries.yml
│   └── cmake-multi-platform.yml # Multi-platform testing
├── CMakeLists.txt               # CMake configuration
├── build-release-unix.sh        # Unix build script
├── build-release-windows.ps1    # Windows PowerShell build script
├── build-release-windows.cmd    # Windows batch build script
└── README.md                    # This file
```

## Game Mechanics

### Player Stats
- **Health**: Hit points - when reduced to 0, the game ends
- **Stamina**: Required for attacks, slowly regenerates each turn
- **Defense**: Reduces physical damage taken
- **Resistance**: Reduces magical damage taken
- **Gold**: Currency for trading
- **Level**: Character progression
- **Experience**: Progress toward next level

### Combat 
1. Enemy appears based on player level
2. Choose action: Attack, Defend, Power Attack, Use Item, Equip Weapon, or Flee
3. Stamina management affects action availability
4. Enemy counterattacks if player acts
5. Natural stamina regeneration each turn
6. Victory grants gold and experience

### Weapon Information 
- Each weapon has unique damage range, accuracy, and stamina cost
- Weapons have durability that decreases with each hit
- Power attacks deal double damage but cost triple stamina and cause double durability loss
- Broken weapons are automatically removed from inventory
- Unarmed combat is available when no weapon is equipped

## CI/CD

The project includes GitHub Actions workflows for:
- **Multi-platform Testing**: Builds and tests on Ubuntu and Windows with GCC, Clang, and MSVC
- **Auto-formatting**: Automatically formats C++ code using clang-format
- **Release Binaries**: Builds release binaries for distribution for Windows, Linux and Mac systems.

## Version

Current Version: 1.0.0

## Credits

Built with:
- C++ 17
- CMake
- Catch2 Testing Framework

Made By: Connor Akey