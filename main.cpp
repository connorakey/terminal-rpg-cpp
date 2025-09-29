#include <iostream>
#include <string>
#include <limits>
#include <random>
#include <vector>
#include "player.hpp"
#include "itemdatabase.hpp"
#include "chest.hpp"

// Function declarations
void triggerRandomEvent(Player& player);
void spawnChest(Player& player);
void spawnMerchant(Player& player);
void printCharacterInformation(const Player& player);
void printInventory(const std::vector<Item*>& inventory);
void removeItemFromInventory(std::vector<Item*>& inventory);
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
        spawnChest(player);
    } else if (roll <= enemyPercentage + chestPercentage + merchantPercentage) {
        spawnMerchant(player);
    }
}

void spawnChest(Player& player) {
    std::cout << "You found a chest!" << '\n';
    std::cout << "Do you want to open it? (y/n): ";
    char choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 'y' || choice == 'Y') {
        // Generate random chest properties
        bool isLocked = generateRandomNumber(1, 100) > 90; // 10% chance locked

        // Determine trap type
        trap trapType = NONE;
        int trapRoll = generateRandomNumber(1, 100);
        if (trapRoll <= 20) { // 20% chance trapped
            int trapTypeRoll = generateRandomNumber(1, 3);
            switch (trapTypeRoll) {
                case 1:
                    trapType = POISON_DART;
                    break;
                case 2:
                    trapType = EXPLOSION;
                    break;
                case 3:
                    trapType = ALARM;
                    break;
                default:
                    trapType = POISON_DART;
            }
        }

        // Generate a random item for the chest
        Item* chestItem = nullptr;
        int itemTypeRoll = generateRandomNumber(1, 100);
        ItemType selectedType;
        if (itemTypeRoll <= 40) {
            selectedType = CURRENCY; // 40% chance for currency
        } else if (itemTypeRoll <= 65) {
            selectedType = POTION; // 25% chance for potions
        } else if (itemTypeRoll <= 85) {
            selectedType = WEAPON; // 20% chance for weapons
        } else if (itemTypeRoll <= 95) {
            selectedType = ARMOR; // 10% chance for armor
        } else {
            selectedType = MISC; // 5% chance for misc items
        }

        // Get all items of the selected type
        std::vector<std::string> itemsOfType = ItemDatabase::getInstance().getItemsByType(selectedType);

        if (!itemsOfType.empty()) {
            // Select a random item from the type
            int randomIndex = generateRandomNumber(0, itemsOfType.size() - 1);
            std::string selectedItemName = itemsOfType[randomIndex];

            // Create the item
            chestItem = ItemDatabase::getInstance().createItem(selectedItemName, 1);
        }

        // Create the chest with generated properties
        Chest chest(isLocked, trapType, chestItem);

        // Attempt to open the chest
        OpenChestResult result = chest.open();

        // Handle the result
        switch (result.result) {
            case SUCCESS:
                std::cout << "You successfully opened the chest!" << '\n';
                if (result.item) {
                    std::cout << "You found: " << result.item->getName() << "!" << '\n';
                    std::cout << result.item->getDescription() << '\n';

                    // Handle the item based on its type
                    if (result.item->getType() == CURRENCY) {
                        player.pickupItem(result.item);
                    } else {
                        player.addItemToInventory(result.item);
                    }
                }
                break;

            case LOCKED:
                std::cout << "The chest is locked!" << '\n';
                // Clean up the item since player couldn't get it
                if (chestItem) delete chestItem;
                break;

            case TRAPPED:
                std::cout << "The chest was trapped!" << '\n';

                // Handle trap effects
                switch (result.trapInfo.type) {
                    case POISON_DART:
                        std::cout << "A poison dart shoots out and hits you!" << '\n';
                        std::cout << "You take " << result.trapInfo.damage << " poison damage!" << '\n';
                        player.takeDamage(result.trapInfo.damage);
                        break;

                    case EXPLOSION:
                        std::cout << "The chest explodes!" << '\n';
                        std::cout << "You take " << result.trapInfo.damage << " explosive damage!" << '\n';
                        player.takeDamage(result.trapInfo.damage);
                        if (result.trapInfo.summonsEnemies) {
                            std::cout << "The explosion attracts nearby enemies!" << '\n';
                            // TODO: Implement enemy summoning logic
                        }
                        break;

                    case ALARM:
                        std::cout << "An alarm goes off, alerting nearby creatures!" << '\n';
                        if (result.trapInfo.summonsEnemies) {
                            std::cout << "Enemies are approaching!" << '\n';
                            // TODO: Implement enemy summoning logic
                        }
                        break;

                    case NONE:
                        break;
                }

                // Player still gets the item if they survive the trap
                if (result.item) {
                    std::cout << "Despite the trap, you manage to retrieve the item!" << '\n';
                    std::cout << "You found: " << result.item->getName() << "!" << '\n';
                    std::cout << result.item->getDescription() << '\n';

                    // Handle the item based on its type
                    if (result.item->getType() == CURRENCY) {
                        player.pickupItem(result.item);
                    } else {
                        player.addItemToInventory(result.item);
                    }
                }
                break;

            case EMPTY:
                std::cout << "You opened the chest, but it's empty." << '\n';
                break;
        }
    } else {
        std::cout << "You decided to leave the chest alone." << '\n';
    }
}

void spawnMerchant(Player& player) {
    std::cout << "A wandering merchant appears!" << '\n';
    int rand = generateRandomNumber(1, 6);
    switch (rand) {
        case 1:
            std::cout << "Merchant: Well hello there young traveler! Would you be intresting in selling me your wares?" << '\n';
            break;
        case 2:
            std::cout << "Merchant: How are ye doing on this fine day? Care to trade?" << '\n';
            break;
        case 3:
            std::cout << "Merchant: I have the finest goods in all the land! Care to take a look?" << '\n';
            break;
        case 4:
            std::cout << "Merchant: I've heard tales of your adventures. Care to trade?" << '\n';
            break;
        case 5:
            std::cout << "Merchant: I remember when I was your age, full of dreams and ambitions. Care to trade?" << '\n';
            break;
        case 6:
            std::cout << "Merchant: The road is dangerous, but my goods can make it safer. Care to trade?" << '\n';
            break;
        default:
            std::cout << "Merchant: Greetings! Care to trade?" << '\n';
            break;
    }
    std::cout << "------ Merchant Inventory ------" << '\n';
    rand = generateRandomNumber(1, 5); // Random number generator for the items types (as of now only 5 types)
    std::vector<std::string> itemNames;
    switch (rand) {
        case 1:
            itemNames = ItemDatabase::getInstance().getItemsByType(WEAPON);
            break;
        case 2:
            itemNames = ItemDatabase::getInstance().getItemsByType(ARMOR);
            break;
        case 3:
            itemNames = ItemDatabase::getInstance().getItemsByType(POTION);
            break;
        case 4:
            itemNames = ItemDatabase::getInstance().getItemsByType(CURRENCY);
            break;
        case 5:
            itemNames = ItemDatabase::getInstance().getItemsByType(MISC);
            break;
        default:
            itemNames = std::vector<std::string>(); // Empty vector
            break;
    }
    // Build merchant inventory
    std::vector<Item*> merchantInventory;
    int slotId = 1;
    for (const std::string& itemName : itemNames) {
        Item* item = ItemDatabase::getInstance().createItem(itemName, slotId++);
        if (item) merchantInventory.push_back(item);
    }
    // Print merchant inventory
    for (size_t i = 0; i < merchantInventory.size(); ++i) {
        Item* item = merchantInventory[i];
        std::cout << i + 1 << ". " << item->getName() << " (" << item->getValue() << " gold)" << '\n';
        std::cout << "    " << item->getDescription() << '\n';
        std::cout << "    Type: " << item->getType() << ", Rarity: " << item->getRarity() << '\n';
        // Type-specific data
        if (item->getType() == WEAPON) {
            const WeaponData& wd = item->getWeaponData();
            std::cout << "    Weapon Data:" << '\n';
            std::cout << "      Min Damage: " << wd.getMinDamage() << '\n';
            std::cout << "      Max Damage: " << wd.getMaxDamage() << '\n';
            std::cout << "      Accuracy: " << wd.getAccuracy() << " %" << '\n';
            std::cout << "      Cooldown: " << wd.getCooldown() << " ms" << '\n';
            std::cout << "      Weapon Type: " << wd.getWeaponType() << '\n';
            std::cout << "      Durability: " << wd.getDurability() << '\n';
            std::cout << "      Stamina Cost: " << wd.getStaminaCost() << '\n';
        } else if (item->getType() == ARMOR) {
            const ArmorData& ad = item->getArmorData();
            std::cout << "    Armor Data:" << '\n';
            std::cout << "      Armor Value: " << ad.getArmorValue() << '\n';
            std::cout << "      Durability: " << ad.getDurability() << '\n';
        } else if (item->getType() == POTION) {
            const PotionData& pd = item->getPotionData();
            std::cout << "    Potion Data:" << '\n';
            std::cout << "      Potion Type: " << pd.getPotionType() << '\n';
            std::cout << "      Min Potency: " << pd.getMinPotency() << '\n';
            std::cout << "      Max Potency: " << pd.getMaxPotency() << '\n';
        } else if (item->getType() == CURRENCY) {
            std::cout << "    Currency Data:" << '\n';
            std::cout << "      Gold Value: " << item->getValue() << '\n';
        }
        std::cout << "--------------------------" << '\n';
    }
    std::cout << "Merchant: If you see anything you like, just let me know!" << '\n';
    int choice = 0;
    while (choice != 3) {
        std::cout << "1. Buy Item" << '\n';
        std::cout << "2. Sell Item" << '\n';
        std::cout << "3. Leave" << '\n';
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        if (std::cin.fail() || choice < 1 || choice > 3) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid choice. Please enter a number between 1 and 3." << '\n';
            continue;
        }
        switch (choice) {
            case 1: {
                // Buy Item
                std::cout << "Which item would you like to buy? Enter the number: ";
                int buyChoice = 0;
                std::cin >> buyChoice;
                if (std::cin.fail() || buyChoice < 1 || buyChoice > (int)merchantInventory.size()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid item number." << '\n';
                    break;
                }
                Item* itemToBuy = merchantInventory[buyChoice - 1];
                if (player.getGold() < itemToBuy->getValue()) {
                    std::cout << "You don't have enough gold!" << '\n';
                } else {
                    player.removeGold(itemToBuy->getValue());
                    player.addItemToInventory(itemToBuy);
                    std::cout << "You bought " << itemToBuy->getName() << " for " << itemToBuy->getValue() << " gold." << '\n';
                    merchantInventory.erase(merchantInventory.begin() + (buyChoice - 1));
                }
                break;
            }
            case 2: {
                // Sell Item
                if (player.getInventory().empty()) {
                    std::cout << "You have no items to sell." << '\n';
                    break;
                }
                std::cout << "Your inventory:" << '\n';
                printInventory(player.getInventory());
                std::cout << "Enter the number of the item to sell: ";
                int sellChoice = 0;
                std::cin >> sellChoice;
                const std::vector<Item*>& inv = player.getInventory();
                if (std::cin.fail() || sellChoice < 1 || sellChoice > (int)inv.size()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid item number." << '\n';
                    break;
                }
                Item* itemToSell = inv[sellChoice - 1];
                player.addGold(itemToSell->getValue());
                std::cout << "You sold " << itemToSell->getName() << " for " << itemToSell->getValue() << " gold." << '\n';
                removeItemFromInventory(const_cast<std::vector<Item*>&>(player.getInventory()));
                break;
            }
            case 3:
                std::cout << "Merchant: Safe travels, adventurer!" << '\n';
                break;
        }
    }
    // Clean up remaining merchant items
    for (Item* item : merchantInventory) {
        delete item;
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
