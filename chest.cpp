#include "chest.hpp"
#include <random>

Chest::Chest(bool isLocked, trap trapped, Item* item)
    : isLocked(isLocked), trapped(trapped), item(item) {}

OpenChestResult Chest::open() {
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

Item* Chest::getItem() const {
    return item;
}

int Chest::randomInt(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(min, max);
    return dist(gen);
}

bool Chest::randomChance(int threshold) {
    return randomInt(1, 100) > threshold;
}

