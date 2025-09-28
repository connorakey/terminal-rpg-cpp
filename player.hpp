//
// Created by Connor on 28/09/2025.
//

#ifndef TERMINAL_RPG_PLAYER_HPP
#define TERMINAL_RPG_PLAYER_HPP
#include <string>
#include <vector>
#include "item.hpp"
class Player {
public:
    Player(const std::string &name,
           int health,
           unsigned int maxHealth,
           unsigned int stamina,
           unsigned int maxStamina,
           unsigned int defence, // physical damage reduction
           unsigned int resistance, // magical damage reduction
           unsigned int level,
           unsigned int experience,
           unsigned int gold,
           unsigned int nextLevelExp,
           unsigned int totalWeight,
           Item* equippedWeapon = nullptr,
           Item* equippedArmor = nullptr,
           std::vector<Item*> &inventory = {})
        : name(name), health(health), maxHealth(maxHealth), stamina(stamina), maxStamina(maxStamina),
          defence(defence), resistance(resistance), level(level), experience(experience), gold(gold), nextLevelExp(nextLevelExp),
          equippedWeapon(nullptr), equippedArmor(nullptr) {}

    void takeDamage(unsigned int damage) {
        int actualDamage = static_cast<int>(damage) - static_cast<int>(defence);
        if (actualDamage < 0) actualDamage = 0;
        health -= actualDamage;
        if (health < 0) health = 0;
    }

    void takeSpellDamage(unsigned int damage) {
        int actualDamage = static_cast<int>(damage) - static_cast<int>(resistance);
        if (actualDamage < 0) actualDamage = 0;
        health -= actualDamage;
        if (health < 0) health = 0;
    }

    void heal(unsigned int amount) {
        health += static_cast<int>(amount);
        if (health > static_cast<int>(maxHealth)) health = static_cast<int>(maxHealth);
        if (health < 0) health = 0;
    }

    void useStamina(unsigned int amount) {
        if (amount > stamina) {
            stamina = 0;
        } else {
            stamina -= amount;
        }
    }

    void recoverStamina(unsigned int amount) {
        stamina += amount;
        if (stamina > maxStamina) stamina = maxStamina;
    }

    void changeMaxHealth(signed int amount) {
        maxHealth += amount;
        if (maxHealth < 1) maxHealth = 1;
        if (health > static_cast<int>(maxHealth)) health = static_cast<int>(maxHealth);
    }

    void changeMaxStamina(signed int amount) {
        maxStamina += amount;
        if (maxStamina < 1) maxStamina = 1;
        if (stamina > maxStamina) stamina = maxStamina;
    }

    void changeDefence(signed int amount) {
        defence += amount;
    }

    void changeResistance(signed int amount) {
        resistance += amount;
    }

    void addGold(unsigned int amount) {
        gold += amount;
    }

    void removeGold(unsigned int amount) {
        if (amount > gold) gold = 0;
        else gold -= amount;
    }

    void addWeight(unsigned int amount) {
        totalWeight += amount;
    }

    void removeWeight(unsigned int amount) {
        if (amount > totalWeight) totalWeight = 0;
        else totalWeight -= amount;
    }

    int getHealth() const { return health; }
    unsigned int getMaxHealth() const { return maxHealth; }
    unsigned int getStamina() const { return stamina; }
    unsigned int getMaxStamina() const { return maxStamina; }
    unsigned int getDefence() const { return defence; }
    unsigned int getResistance() const { return resistance; }
    const std::string &getName() const { return name; }
    unsigned int getLevel() const { return level; }
    unsigned int getExperience() const { return experience; }
    unsigned int getGold() const { return gold; }
    unsigned int getNextLevelExp() const { return nextLevelExp; }
    const std::vector<Item*> &getInventory() const { return inventory; }
    Item* getEquippedWeapon() const { return equippedWeapon; }
    Item* getEquippedArmor() const { return equippedArmor; }

private:
    const std::string name;
    int health;
    unsigned int maxHealth;
    unsigned int stamina;
    unsigned int maxStamina;
    unsigned int defence;
    unsigned int resistance;
    unsigned int level;
    unsigned int experience;
    unsigned int gold;
    unsigned int nextLevelExp;
    unsigned int totalWeight;
    std::vector<Item*> inventory;
    Item* equippedWeapon;
    Item* equippedArmor;
};
#endif //TERMINAL_RPG_PLAYER_HPP