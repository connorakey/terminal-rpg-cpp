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
    Player(const std::string& name,
           int health,
           unsigned int maxHealth,
           unsigned int stamina,
           unsigned int maxStamina,
           unsigned int defence,
           unsigned int resistance,
           unsigned int level,
           unsigned int experience,
           unsigned int gold,
           unsigned int nextLevelExp,
           unsigned int totalWeight,
           Item* equippedWeapon = nullptr,
           Item* equippedArmor = nullptr,
           std::vector<Item*> inventory = {});

    void takeDamage(unsigned int damage);

    void takeSpellDamage(unsigned int damage);

    void heal(unsigned int amount);

    void useStamina(unsigned int amount);

    void recoverStamina(unsigned int amount);

    void changeMaxHealth(signed int amount);

    void changeMaxStamina(signed int amount);

    void changeDefence(signed int amount);

    void changeResistance(signed int amount);

    void addGold(unsigned int amount);

    void removeGold(unsigned int amount);

    void addWeight(unsigned int amount);

    void removeWeight(unsigned int amount);

    void addItemToInventory(Item* item);

    bool pickupItem(Item* item);

    int getHealth() const;

    unsigned int getMaxHealth() const;

    unsigned int getStamina() const;

    unsigned int getMaxStamina() const;

    unsigned int getDefence() const;

    unsigned int getResistance() const;

    const std::string& getName() const;

    unsigned int getLevel() const;

    unsigned int getExperience() const;

    unsigned int getGold() const;

    unsigned int getNextLevelExp() const;

    const std::vector<Item*>& getInventory() const;

    Item* getEquippedWeapon() const;

    Item* getEquippedArmor() const;

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