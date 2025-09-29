#include <iostream>
#include <string>
#include <limits>
#include <random>
#include <vector>
#include "player.hpp"
#include "itemdatabase.hpp"

// Function declarations
void printCharacterInformation(const Player& player);
void printInventory(const std::vector<Item*>& inventory);
void removeItemFromInventory(std::vector<Item*>& inventory);
void demonstrateCurrencyPickup(Player& player);
int generateRandomNumber(int min, int max);

int main() {
    std::cout << "------ Terminal RPG ------" << '\n';

    // Initialize the item database
    ItemDatabase::getInstance().initialize();

    std::cout << "Creating player..." << '\n';
    std::cout << "Enter your character's name: ";
    std::string playerName;
    std::getline(std::cin, playerName);
    Player player(playerName, 100, 100, 50, 50, 10, 5, 1, 0, 100, 100, 0);
    std::cout << "Welcome, " << playerName << "!" << '\n' << '\n';
    printCharacterInformation(player);
    return 0;
}

void triggerRandomEvent(Player& player) {
    int enemyPercentage = 60;
    int chestPercentage = 30;
    int merchantPercentage = 10;
    int roll = generateRandomNumber(1, 100);

    if (roll <= enemyPercentage) {
        // Trigger enemy encounter
    } else if (roll <= enemyPercentage + chestPercentage) {
        // Spawn a chest
    } else if (roll <= enemyPercentage + chestPercentage + merchantPercentage) {
        // Spawn a merchant
    }
}

void printCharacterInformation(const Player& player) {
    std::cout << "Character Information:" << '\n';
    std::cout << "Name: " << player.getName() << '\n';
    std::cout << "Health: " << player.getHealth() << "/" << player.getMaxHealth() << '\n';
    std::cout << "Stamina: " << player.getStamina() << "/" << player.getMaxStamina() << '\n';
    std::cout << "Defence: " << player.getDefence() << '\n';
    std::cout << "Resistance: " << player.getResistance() << '\n';
    std::cout << "Gold: " << player.getGold() << '\n';
    std::cout << "Level: " << player.getLevel() << '\n';
    std::cout << "Experience: " << player.getExperience() << '\n';
    std::cout << "Next Level Exp: " << player.getNextLevelExp() << '\n';
}

void printInventory(const std::vector<Item*>& inventory) {
    if (inventory.empty()) {
        std::cout << "Inventory is empty." << '\n';
        return;
    }
    std::cout << "Inventory:" << '\n';
    for (size_t i = 0; i < inventory.size(); ++i) {
        const Item* item = inventory[i];
        std::cout << i + 1 << ".\n";
        std::cout << "  ID: " << item->getId() << '\n';
        std::cout << "  Name: " << item->getName() << '\n';
        std::cout << "  Description: " << item->getDescription() << '\n';
        std::cout << "  Value: " << item->getValue() << '\n';
        std::cout << "  Weight: " << item->getWeight() << '\n';
        std::cout << "  Type: " << item->getType() << '\n';
        std::cout << "  Rarity: " << item->getRarity() << '\n';
        std::cout << "  Equipped: " << (item->isEquipped() ? "Yes" : "No") << '\n';
        // Print type-specific data
        if (item->getType() == WEAPON) {
            const WeaponData& wd = item->getWeaponData();
            std::cout << "  Weapon Data:" << '\n';
            std::cout << "    Min Damage: " << wd.getMinDamage() << '\n';
            std::cout << "    Max Damage: " << wd.getMaxDamage() << '\n';
            std::cout << "    Accuracy: " << wd.getAccuracy() << " %" << '\n';
            std::cout << "    Cooldown: " << wd.getCooldown() << " ms" << '\n';
            std::cout << "    Weapon Type: " << wd.getWeaponType() << '\n';
            std::cout << "    Durability: " << wd.getDurability() << '\n';
            std::cout << "    Stamina Cost: " << wd.getStaminaCost() << '\n';
        } else if (item->getType() == ARMOR) {
            const ArmorData& ad = item->getArmorData();
            std::cout << "  Armor Data:" << '\n';
            std::cout << "    Armor Value: " << ad.getArmorValue() << '\n';
            std::cout << "    Durability: " << ad.getDurability() << '\n';
        } else if (item->getType() == POTION) {
            const PotionData& pd = item->getPotionData();
            std::cout << "  Potion Data:" << '\n';
            std::cout << "    Potion Type: " << pd.getPotionType() << '\n';
            std::cout << "    Min Potency: " << pd.getMinPotency() << '\n';
            std::cout << "    Max Potency: " << pd.getMaxPotency() << '\n';
        } else if (item->getType() == CURRENCY) {
            std::cout << "  Currency Data:" << '\n';
            std::cout << "    Gold Value: " << item->getValue() << '\n';
        }
        std::cout << "--------------------------" << '\n';
    }
}

void removeItemFromInventory(std::vector<Item*>& inventory) {
    if (inventory.empty()) {
        std::cout << "Inventory is empty. Nothing to remove." << '\n';
        return;
    }
    printInventory(inventory);
    std::cout << "Enter the number of the item to remove: ";
    int choice = 0;
    while (true) {
        std::cin >> choice;
        if (std::cin.fail() || choice < 1 || choice > static_cast<int>(inventory.size())) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid choice. Please enter a valid item number: ";
        } else {
            break;
        }
    }
    Item* removed = inventory[choice - 1];
    inventory.erase(inventory.begin() + (choice - 1));
    std::cout << "Removed item: " << removed->getName() << " from inventory." << '\n';
}


int generateRandomNumber(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(min, max);
    return dist(gen);
}
