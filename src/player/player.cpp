#include "player.hpp"

#include <algorithm>
#include <iostream>

#include "../levels/leveldatabase.hpp"

Player::Player(const std::string& name, int health, unsigned int maxHealth, unsigned int stamina,
               unsigned int maxStamina, unsigned int defence, unsigned int resistance,
               unsigned int level, unsigned int experience, unsigned int gold,
               unsigned int nextLevelExp, unsigned int totalWeight, Item* equippedWeapon,
               Item* equippedArmor, std::vector<Item*> inventory)
    : name(name),
      health(health),
      maxHealth(maxHealth),
      stamina(stamina),
      maxStamina(maxStamina),
      defence(defence),
      resistance(resistance),
      level(level),
      experience(experience),
      gold(gold),
      nextLevelExp(nextLevelExp),
      totalWeight(totalWeight),
      inventory(inventory),
      equippedWeapon(equippedWeapon),
      equippedArmor(equippedArmor),
      baseMaxHealth(maxHealth),
      baseMaxStamina(maxStamina),
      baseDefence(defence),
      baseResistance(resistance) {}

void Player::takeDamage(unsigned int damage) {
    int actualDamage = static_cast<int>(damage) - static_cast<int>(defence);
    if (actualDamage < 0) actualDamage = 0;
    health -= actualDamage;
    if (health < 0) health = 0;
}

void Player::takeSpellDamage(unsigned int damage) {
    int actualDamage = static_cast<int>(damage) - static_cast<int>(resistance);
    if (actualDamage < 0) actualDamage = 0;
    health -= actualDamage;
    if (health < 0) health = 0;
}

void Player::heal(unsigned int amount) {
    health += static_cast<int>(amount);
    if (health > static_cast<int>(maxHealth)) health = static_cast<int>(maxHealth);
    if (health < 0) health = 0;
}

void Player::useStamina(unsigned int amount) {
    if (amount > stamina) {
        stamina = 0;
    } else {
        stamina -= amount;
    }
}

void Player::recoverStamina(unsigned int amount) {
    stamina += amount;
    if (stamina > maxStamina) stamina = maxStamina;
}

void Player::changeMaxHealth(signed int amount) {
    maxHealth = std::max(1, static_cast<int>(maxHealth) + amount);
    if (health > static_cast<int>(maxHealth)) health = static_cast<int>(maxHealth);
}

void Player::changeMaxStamina(signed int amount) {
    maxStamina = std::max(1, static_cast<int>(maxStamina) + amount);
    if (stamina > maxStamina) stamina = maxStamina;
}

void Player::changeDefence(signed int amount) { defence += amount; }

void Player::changeResistance(signed int amount) { resistance += amount; }

void Player::addGold(unsigned int amount) { gold += amount; }

void Player::removeGold(unsigned int amount) {
    if (amount > gold)
        gold = 0;
    else
        gold -= amount;
}

void Player::addWeight(unsigned int amount) { totalWeight += amount; }

void Player::removeWeight(unsigned int amount) {
    if (amount > totalWeight)
        totalWeight = 0;
    else
        totalWeight -= amount;
}

bool Player::pickupItem(Item* item) {
    if (!item) return false;

    // Special handling for currency items
    if (item->getType() == CURRENCY) {
        // Add gold value to player's gold
        addGold(item->getValue());
        std::cout << "Picked up " << item->getName() << " and gained " << item->getValue()
                  << " gold!" << std::endl;
        std::cout << "Current gold: " << gold << std::endl;

        // Currency items are used up immediately, so we delete the item.
        delete item;
        return true;
    } else {
        // Weight limit check (assume 100 for now, adjust as needed)
        unsigned int maxWeight = 100;
        if (totalWeight + item->getWeight() > maxWeight) {
            std::cout << "Cannot pick up " << item->getName() << ", too heavy!" << std::endl;
            return false;
        }
        // Regular items are added to inventory
        addItemToInventory(item);
        std::cout << "Picked up " << item->getName() << " and added it to inventory." << std::endl;
        return true;
    }
}

void Player::addItemToInventory(Item* item) {
    if (!item) return;

    // Add weight to player
    addWeight(item->getWeight());

    // Add item to inventory
    inventory.push_back(item);
}

void Player::gainExperience(unsigned int amount) {
    experience += amount;
    std::cout << "Gained " << amount << " experience!" << std::endl;

    // Check for level up
    if (checkAndLevelUp()) {
        std::cout << "Level up! You are now level " << level << "!" << std::endl;
        updateStatsForLevel();

        // Show level progression info
        std::cout << LevelDatabase::getInstance().getLevelProgressionInfo(level, experience)
                  << std::endl;
    }

    // Update next level experience
    nextLevelExp = LevelDatabase::getInstance().getExperienceForNextLevel(level);
}

bool Player::checkAndLevelUp() {
    int newLevel = LevelDatabase::getInstance().checkLevelUp(experience, level);
    if (newLevel > static_cast<int>(level)) {
        level = newLevel;
        return true;
    }
    return false;
}

void Player::updateStatsForLevel() {
    // Get stat bonuses from level database
    unsigned int healthBonus, staminaBonus, defenceBonus, resistanceBonus;
    LevelDatabase::getInstance().getStatBonuses(level, healthBonus, staminaBonus, defenceBonus,
                                                resistanceBonus);

    // Update max stats with bonuses
    unsigned int oldMaxHealth = maxHealth;
    unsigned int oldMaxStamina = maxStamina;

    maxHealth = baseMaxHealth + healthBonus;
    maxStamina = baseMaxStamina + staminaBonus;
    defence = baseDefence + defenceBonus;
    resistance = baseResistance + resistanceBonus;

    // Heal proportionally when max health/stamina increases
    if (maxHealth > oldMaxHealth) {
        heal(maxHealth - oldMaxHealth);
    }
    if (maxStamina > oldMaxStamina) {
        recoverStamina(maxStamina - oldMaxStamina);
    }

    // Update next level experience requirement
    nextLevelExp = LevelDatabase::getInstance().getExperienceForNextLevel(level);
}

void Player::setEquippedWeapon(Item* weapon) { equippedWeapon = weapon; }

void Player::setEquippedArmor(Item* armor) { equippedArmor = armor; }

int Player::getHealth() const { return health; }
unsigned int Player::getMaxHealth() const { return maxHealth; }
unsigned int Player::getStamina() const { return stamina; }
unsigned int Player::getMaxStamina() const { return maxStamina; }
unsigned int Player::getDefence() const { return defence; }
unsigned int Player::getResistance() const { return resistance; }
const std::string& Player::getName() const { return name; }
unsigned int Player::getLevel() const { return level; }
unsigned int Player::getExperience() const { return experience; }
unsigned int Player::getGold() const { return gold; }
unsigned int Player::getNextLevelExp() const { return nextLevelExp; }
const std::vector<Item*>& Player::getInventory() const { return inventory; }
Item* Player::getEquippedWeapon() const { return equippedWeapon; }
Item* Player::getEquippedArmor() const { return equippedArmor; }
