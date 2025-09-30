//
// Created by Connor on 28/09/2025.
//

#ifndef TERMINAL_RPG_CHEST_HPP
#define TERMINAL_RPG_CHEST_HPP
#include "../items/item.hpp"
#include <random>

enum openResult {
    SUCCESS,
    LOCKED,
    TRAPPED,
    EMPTY,
};

enum trap {
    NONE, // no trap
    POISON_DART, // damage player over time
    EXPLOSION, // damage player, high damage can summon enemies
    ALARM, // summon enemies
};

struct trapResult {
    trap type;
    int damage; // 0 if no damage
    bool summonsEnemies; // true if summons enemies
};

struct OpenChestResult {
    openResult result;
    Item* item; // nullptr if no item
    trapResult trapInfo; // trap details if trapped, else type = NONE
};

class Chest {
public:
    Chest(bool isLocked, trap trapped, Item* item);
    OpenChestResult open();
    Item* getItem() const;
private:
    bool isLocked;
    trap trapped;
    Item* item;
    int randomInt(int min, int max);
    bool randomChance(int threshold);
};

#endif //TERMINAL_RPG_CHEST_HPP