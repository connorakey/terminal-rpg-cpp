//
// Created by Connor on 28/09/2025.
//

#ifndef TERMINAL_RPG_CHEST_HPP
#define TERMINAL_RPG_CHEST_HPP
#include "item.hpp"
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
    Chest(bool isLocked, trap trapped, Item* item)
        : isLocked(isLocked), trapped(trapped), item(item) {}

    OpenChestResult open() {
        if (isLocked) {
            return {LOCKED, nullptr, {NONE, 0, false}};
        }
        if (trapped == NONE) {
            if (item == nullptr) {
                return {EMPTY, nullptr, {NONE, 0, false}};
            }
            return {SUCCESS, item, {NONE, 0, false}};
        }
        if (trapped == POISON_DART) {
            int damage = randomInt(10, 60);
            return {TRAPPED, item, {POISON_DART, damage, false}};
        }
        if (trapped == EXPLOSION) {
            int damage = randomInt(30, 70);
            bool summonsEnemies = randomChance(30); // >30 out of 100 summons enemies
            return {TRAPPED, item, {EXPLOSION, damage, summonsEnemies}};
        }
        if (trapped == ALARM) {
            return {TRAPPED, item, {ALARM, 0, true}};
        }
        // Fallback if chest is empty, or something unexpected occurs.
        return {EMPTY, nullptr, {NONE, 0, false}};
    }

    Item* getItem() const { return item; }
private:
    bool isLocked;
    trap trapped;
    Item* item;

    // Helper to generate random integer in [min, max]
    int randomInt(int min, int max) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(min, max);
        return dist(gen);
    }
    // Helper to return true if random number in [1,100] is greater than threshold
    bool randomChance(int threshold) {
        return randomInt(1, 100) > threshold;
    }
};

#endif //TERMINAL_RPG_CHEST_HPP