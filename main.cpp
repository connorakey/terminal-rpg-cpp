#include <iostream>
#include <string>
#include <limits>
#include <random>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include "player.hpp"
#include "itemdatabase.hpp"
#include "enemydatabase.hpp"
#include "leveldatabase.hpp"
#include "chest.hpp"

// Function declarations
void triggerRandomEvent(Player& player);
void spawnChest(Player& player);
Enemy* spawnEnemy(Player& player);
void fightEnemy(Player& player, Enemy* enemy);
void spawnMerchant(Player& player);
void printCharacterInformation(const Player& player);
void printInventory(const std::vector<Item*>& inventory);
void removeItemFromInventory(std::vector<Item*>& inventory);
bool equipWeapon(Player& player); // Returns true if a weapon was equipped, false if cancelled
int generateRandomNumber(int min, int max);

int main() {
    std::cout << "-------- Terminal RPG --------" << '\n';

    // Initialize the databases
    ItemDatabase::getInstance().initialize();
    EnemyDatabase::getInstance().initialize();
    LevelDatabase::getInstance().initialize();

    std::cout << "Creating player..." << '\n';
    std::cout << "Enter your character's name: ";
    std::string playerName;
    std::getline(std::cin, playerName);
    Player player(playerName, 100, 100, 50, 50, 0, 0, 1, 0, 100, 100, 0);
    std::cout << "Welcome, " << playerName << "!" << '\n' << '\n';
    printCharacterInformation(player);

    while (true) {
        triggerRandomEvent(player);

        std::cout << "\nContinue adventuring? (y/n): ";
        char continueChoice;
        std::cin >> continueChoice;
        std::cin.ignore();

        if (continueChoice == 'n' || continueChoice == 'N') {
            std::cout << "Thanks for playing!" << std::endl;
            break;
        }
    }

    return 0;
}

void triggerRandomEvent(Player& player) {
    int enemyPercentage = 60;
    int chestPercentage = 30;
    int merchantPercentage = 10;
    int roll = generateRandomNumber(1, 100);

    if (roll <= enemyPercentage) {
        Enemy* enemy = spawnEnemy(player);
        if (enemy) {
            fightEnemy(player, enemy);
        }
    } else if (roll <= enemyPercentage + chestPercentage) {
        spawnChest(player);
    } else if (roll <= enemyPercentage + chestPercentage + merchantPercentage) {
        spawnMerchant(player);
    }
}

Enemy* spawnEnemy(Player& player) {
    std::cout << "An enemy appears!" << '\n';

    // Calculate level range for enemy spawning (player level ± 2, minimum level 1)
    int playerLevel = static_cast<int>(player.getLevel());
    int minEnemyLevel = std::max(1, playerLevel - 2);
    int maxEnemyLevel = playerLevel + 2;

    // Get a random enemy within the level range
    std::string enemyName = EnemyDatabase::getInstance().getRandomEnemyByLevel(minEnemyLevel, maxEnemyLevel);

    if (enemyName.empty()) {
        std::cout << "A goblin appears!" << '\n';
        enemyName = "Goblin"; // Fallback enemy
    }

    // Create the enemy
    Enemy* enemy = EnemyDatabase::getInstance().createEnemy(enemyName);
    if (!enemy) {
        std::cout << "Failed to spawn enemy!" << '\n';
        return nullptr;
    }

    std::cout << "A " << enemy->getName() << " (Level " << enemy->getLevel() << ") blocks your path!" << '\n';
    std::cout << enemy->getDescription() << '\n';
    std::cout << "Enemy Stats:" << '\n';
    std::cout << "  Health: " << enemy->getHealth() << '\n';
    std::cout << "  Attack: " << enemy->getMinAttack() << "-" << enemy->getMaxAttack() << '\n';
    std::cout << "  Defence: " << enemy->getDefence() << '\n';
    std::cout << "  Resistance: " << enemy->getResistance() << '\n';
    std::cout << "  Type: ";

    // Display enemy type
    switch (enemy->getType()) {
        case EnemyType::BEAST:
            std::cout << "Beast";
            break;
        case EnemyType::UNDEAD:
            std::cout << "Undead";
            break;
        case EnemyType::HUMANOID:
            std::cout << "Humanoid";
            break;
        case EnemyType::DRAGON:
            std::cout << "Dragon";
            break;
        case EnemyType::ELEMENTAL:
            std::cout << "Elemental";
            break;
        case EnemyType::DEMON:
            std::cout << "Demon";
            break;
        case EnemyType::GOBLINOID:
            std::cout << "Goblinoid";
            break;
    }

    std::cout << " | Rarity: ";

    // Display enemy rarity
    switch (enemy->getRarity()) {
        case EnemyRarity::COMMON:
            std::cout << "Common";
            break;
        case EnemyRarity::UNCOMMON:
            std::cout << "Uncommon";
            break;
        case EnemyRarity::RARE:
            std::cout << "Rare";
            break;
        case EnemyRarity::EPIC:
            std::cout << "Epic";
            break;
        case EnemyRarity::LEGENDARY:
            std::cout << "Legendary";
            break;
        case EnemyRarity::BOSS:
            std::cout << "Boss";
            break;
    }
    std::cout << '\n' << '\n';

    return enemy;
}

void fightEnemy(Player& player, Enemy* enemy) {
    std::cout << '\n' << "Combat begins!" << '\n';
    std::cout << "--------------------------------" << '\n';

    while (enemy->isAlive() && player.getHealth() > 0) {
        std::cout << '\n' << "=== COMBAT STATUS ===" << '\n';
        std::cout << "Your Health: " << player.getHealth() << "/" << player.getMaxHealth() << '\n';
        std::cout << "Your Stamina: " << player.getStamina() << "/" << player.getMaxStamina() << '\n';
        std::cout << enemy->getName() << " Health: " << enemy->getHealth() << '\n';
        std::cout << "--------------------------------" << '\n';

        std::cout << "Choose your action:" << '\n';
        std::cout << "1. Attack (costs stamina)" << '\n';
        std::cout << "2. Defend (recover stamina, reduce incoming damage)" << '\n';
        std::cout << "3. Power Attack (double damage, costs more stamina)" << '\n';
        std::cout << "4. Use Item" << '\n';
        std::cout << "5. Equip Weapon (takes a turn)" << '\n';
        std::cout << "6. Try to flee" << '\n';
        std::cout << "Enter your choice (1-6): ";

        int combatChoice = 0;
        std::cin >> combatChoice;
        if (std::cin.fail() || combatChoice < 1 || combatChoice > 6) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid choice! You fumble and lose your turn!" << '\n';
            combatChoice = 0; // Skip player turn
        }

        bool playerDefending = false;
        bool playerActed = true;

        switch (combatChoice) {
            case 1: { // Normal Attack
                if (player.getStamina() < 5) {
                    std::cout << "You're too tired to attack effectively! (Need 5 stamina)" << '\n';
                    playerActed = false;
                    break;
                }

                std::cout << "You attack the " << enemy->getName() << "!" << '\n';
                player.useStamina(5);

                // Calculate damage based on equipped weapon
                int playerDamage;
                Item* equippedWeapon = player.getEquippedWeapon();
                if (equippedWeapon && equippedWeapon->getType() == WEAPON) {
                    const WeaponData& weaponData = equippedWeapon->getWeaponData();

                    // Check accuracy - roll to see if attack hits
                    int accuracyRoll = generateRandomNumber(1, 100);
                    if (accuracyRoll > weaponData.getAccuracy()) {
                        std::cout << "You miss with your " << equippedWeapon->getName() << "!" << '\n';
                        playerDamage = 0;
                    } else {
                        playerDamage = generateRandomNumber(weaponData.getMinDamage(), weaponData.getMaxDamage());
                        std::cout << "You strike with your " << equippedWeapon->getName() << "!" << '\n';

                        // Reduce weapon durability on successful hit
                        WeaponData& mutableWeaponData = const_cast<WeaponData&>(weaponData);
                        int currentDurability = mutableWeaponData.getDurability();
                        if (currentDurability > 0) {
                            mutableWeaponData.setDurability(currentDurability - 1);
                            if (currentDurability - 1 <= 0) {
                                std::cout << "Your " << equippedWeapon->getName() << " breaks from overuse!" << '\n';
                                equippedWeapon->setEquipped(false);
                                // Remove broken weapon from inventory
                                auto& inv = const_cast<std::vector<Item*>&>(player.getInventory());
                                auto it = std::find(inv.begin(), inv.end(), equippedWeapon);
                                if (it != inv.end()) {
                                    inv.erase(it);
                                    delete equippedWeapon;
                                }
                            } else if (currentDurability - 1 <= 5) {
                                std::cout << "Your " << equippedWeapon->getName() << " is getting worn out! (Durability: " << (currentDurability - 1) << ")" << '\n';
                            }
                        }
                    }
                } else {
                    // Unarmed combat - always hits but lower damage
                    playerDamage = generateRandomNumber(3, 8);
                    std::cout << "You attack with your bare hands!" << '\n';
                }

                if (playerDamage > 0) {
                    // Check for critical hit (10% chance)
                    int critRoll = generateRandomNumber(1, 100);
                    if (critRoll <= 10) {
                        playerDamage *= 2;
                        std::cout << "Critical hit! ";
                    }

                    enemy->takeDamage(playerDamage);
                    std::cout << "You deal " << playerDamage << " damage!" << '\n';
                } else if (equippedWeapon) {
                    std::cout << "No damage dealt!" << '\n';
                }
                break;
            }

            case 2: { // Defend
                std::cout << "You take a defensive stance!" << '\n';
                playerDefending = true;
                // Recover 30% of max stamina while defending
                unsigned int staminaRecovery = static_cast<unsigned int>(player.getMaxStamina() * 0.3);
                player.recoverStamina(staminaRecovery);
                std::cout << "You recover stamina and prepare to block incoming attacks!" << '\n';
                break;
            }

            case 3: { // Power Attack (double damage, triple stamina, lower accuracy, double durability loss)
                Item* equippedWeapon = player.getEquippedWeapon();
                int requiredStamina = 15; // Default for unarmed

                if (equippedWeapon && equippedWeapon->getType() == WEAPON) {
                    const WeaponData& weaponData = equippedWeapon->getWeaponData();
                    requiredStamina = weaponData.getStaminaCost() * 3; // Power attack costs 3x weapon stamina
                }

                if (player.getStamina() < requiredStamina) {
                    std::cout << "You don't have enough stamina for a power attack! (Need " << requiredStamina << " stamina)" << '\n';
                    playerActed = false;
                    break;
                }

                std::cout << "You charge up a powerful attack!" << '\n';
                player.useStamina(requiredStamina);

                int playerDamage;
                if (equippedWeapon && equippedWeapon->getType() == WEAPON) {
                    const WeaponData& weaponData = equippedWeapon->getWeaponData();

                    // Check accuracy for power attack (slightly reduced accuracy due to wild swing)
                    int accuracyRoll = generateRandomNumber(1, 100);
                    int powerAttackAccuracy = weaponData.getAccuracy() - 10; // 10% accuracy penalty for power attacks
                    if (accuracyRoll > powerAttackAccuracy) {
                        std::cout << "You swing wildly and miss with your " << equippedWeapon->getName() << "!" << '\n';
                        playerDamage = 0;
                    } else {
                        playerDamage = generateRandomNumber(weaponData.getMinDamage(), weaponData.getMaxDamage()) * 2;
                        std::cout << "You unleash a devastating blow with your " << equippedWeapon->getName() << "!" << '\n';

                        // Power attacks cause more durability damage (2 points instead of 1)
                        WeaponData& mutableWeaponData = const_cast<WeaponData&>(weaponData);
                        int currentDurability = mutableWeaponData.getDurability();
                        if (currentDurability > 0) {
                            int durabilityLoss = std::min(2, currentDurability); // Lose 2 durability, but not more than current
                            mutableWeaponData.setDurability(currentDurability - durabilityLoss);

                            if (currentDurability - durabilityLoss <= 0) {
                                std::cout << "Your " << equippedWeapon->getName() << " breaks from the intense power attack!" << '\n';
                                equippedWeapon->setEquipped(false);
                                // Remove broken weapon from inventory
                                auto& inv = const_cast<std::vector<Item*>&>(player.getInventory());
                                auto it = std::find(inv.begin(), inv.end(), equippedWeapon);
                                if (it != inv.end()) {
                                    inv.erase(it);
                                    delete equippedWeapon;
                                }
                            } else if (currentDurability - durabilityLoss <= 5) {
                                std::cout << "Your " << equippedWeapon->getName() << " is severely damaged from the power attack! (Durability: " << (currentDurability - durabilityLoss) << ")" << '\n';
                            }
                        }
                    }
                } else {
                    // Unarmed power attack always hits low damage
                    playerDamage = generateRandomNumber(6, 16); // Double base damage
                    std::cout << "You put all your strength into a crushing blow!" << '\n';
                }

                if (playerDamage > 0) {
                    enemy->takeDamage(playerDamage);
                    std::cout << "You deal " << playerDamage << " damage!" << '\n';
                } else if (equippedWeapon) {
                    std::cout << "No damage dealt!" << '\n';
                }
                break;
            }

            case 4: { // Use Item
                const std::vector<Item*>& inventory = player.getInventory();
                std::vector<Item*> usableItems;

                // Find usable items (potions)
                for (Item* item : inventory) {
                    if (item->getType() == POTION) {
                        usableItems.push_back(item);
                    }
                }

                if (usableItems.empty()) {
                    std::cout << "You have no usable items in combat!" << '\n';
                    playerActed = false;
                    break;
                }

                std::cout << "Choose an item to use:" << '\n';
                for (size_t i = 0; i < usableItems.size(); ++i) {
                    const PotionData& potionData = usableItems[i]->getPotionData();
                    std::cout << i + 1 << ". " << usableItems[i]->getName()
                              << " (Restores " << potionData.getMinPotency() << "-" << potionData.getMaxPotency() << " HP)" << '\n';
                }
                std::cout << "0. Cancel" << '\n';

                int itemChoice;
                std::cin >> itemChoice;
                if (itemChoice == 0 || itemChoice < 0 || itemChoice > static_cast<int>(usableItems.size())) {
                    std::cout << "Cancelled item use." << '\n';
                    playerActed = false;
                    break;
                }

                Item* chosenItem = usableItems[itemChoice - 1];
                const PotionData& potionData = chosenItem->getPotionData();

                if (potionData.getPotionType() == HEALING) {
                    int healAmount = generateRandomNumber(potionData.getMinPotency(), potionData.getMaxPotency());
                    player.heal(healAmount);
                    std::cout << "You use " << chosenItem->getName() << " and recover " << healAmount << " health!" << '\n';
                } else {
                    std::cout << "This item can't be used in combat!" << '\n';
                    playerActed = false;
                    break;
                }

                // Remove used item from inventory
                auto& inv = const_cast<std::vector<Item*>&>(player.getInventory());
                auto it = std::find(inv.begin(), inv.end(), chosenItem);
                if (it != inv.end()) {
                    inv.erase(it);
                    delete chosenItem;
                }
                break;
            }

            case 5: { // Equip Weapon
                if (equipWeapon(player)) {
                    std::cout << "Weapon equipped!" << '\n';
                } else {
                    std::cout << "Weapon equip cancelled." << '\n';
                    playerActed = false;
                }
                break;
            }

            case 6: { // Try to flee
                int fleeChance = generateRandomNumber(1, 100);
                if (fleeChance <= 20) { // 20% chance to flee successfully mid-fight
                    std::cout << "You successfully escape from the " << enemy->getName() << "!" << '\n';
                    delete enemy;
                    return;
                } else {
                    std::cout << "You failed to escape! The " << enemy->getName() << " blocks your path!" << '\n';
                }
                break;
            }

            case 0: // Skip turn (invalid input)
                std::cout << "You stand there confused, losing your chance to act!" << '\n';
                playerActed = false;
                break;
        }

        // Check if enemy is defeated
        if (!enemy->isAlive()) {
            std::cout << '\n' << "The " << enemy->getName() << " has been defeated!" << '\n';
            std::cout << "Victory!" << '\n';

            // Reward system with boss multiplier
            int baseGoldReward = enemy->getLevel() * generateRandomNumber(5, 15);
            int baseExpReward = enemy->getLevel() * generateRandomNumber(10, 20);

            // 5x rewards for boss enemies
            if (enemy->getRarity() == EnemyRarity::BOSS) {
                baseGoldReward *= 5;
                baseExpReward *= 5;
                std::cout << "Boss defeated! Bonus rewards granted!" << '\n';
            }

            std::cout << "You gained " << baseExpReward << " experience and " << baseGoldReward << " gold!" << '\n';
            player.addGold(baseGoldReward);

            break;
        }

        // Enemy's turn
        if (playerActed) {
            std::cout << '\n' << "--- Enemy Turn ---" << '\n';
            std::cout << "The " << enemy->getName() << " attacks you!" << '\n';

            int enemyDamage = enemy->attack();

            // Apply defense bonus if player was defending
            if (playerDefending) {
                enemyDamage = enemyDamage / 2; // Reduce damage by half when defending
                std::cout << "You block some of the damage!" << '\n';
            }

            player.takeDamage(enemyDamage);
            std::cout << "The " << enemy->getName() << " deals " << enemyDamage << " damage to you!" << '\n';
        }

        // Natural stamina recovery (percentage-based each turn)
        if (!playerDefending) {
            // Recover 6% of max stamina each turn (scales with level)
            unsigned int naturalRecovery = static_cast<unsigned int>(player.getMaxStamina() * 0.06);
            player.recoverStamina(naturalRecovery);
        }

        std::cout << "--------------------------------" << '\n';

        if (player.getHealth() <= 0) {
            std::cout << '\n' << "You have been defeated!" << '\n';
            std::cout << "Game Over!" << '\n';
            delete enemy;
            exit(0);
        }

        // Add small pause for readability
        std::cout << "Press Enter to continue...";
        std::cin.ignore();
        std::cin.get();
    }

    // Clean up enemy
    delete enemy;
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
                                Enemy* enemy = spawnEnemy(player);
                                if (enemy) {
                                    fightEnemy(player, enemy);
                                }
                        }
                        break;

                    case ALARM:
                        std::cout << "An alarm goes off, alerting nearby creatures!" << '\n';
                        if (result.trapInfo.summonsEnemies) {
                            std::cout << "Enemies are approaching!" << '\n';
                            Enemy* enemy = spawnEnemy(player);
                            if (enemy) {
                                fightEnemy(player, enemy);
                            }
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
    std::cout << "Level: " << player.getLevel() << " (" << LevelDatabase::getInstance().getLevelTitle(player.getLevel()) << ")" << '\n';
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

bool equipWeapon(Player& player) {
    const std::vector<Item*>& inventory = player.getInventory();
    std::vector<Item*> availableWeapons;

    // Find all weapons in inventory
    for (Item* item : inventory) {
        if (item->getType() == WEAPON) {
            availableWeapons.push_back(item);
        }
    }

    if (availableWeapons.empty()) {
        std::cout << "You have no weapons in your inventory to equip!" << '\n';
        return false;
    }

    // Show currently equipped weapon
    Item* currentWeapon = player.getEquippedWeapon();
    if (currentWeapon) {
        std::cout << "Currently equipped: " << currentWeapon->getName() << '\n';
        const WeaponData& wd = currentWeapon->getWeaponData();
        std::cout << "  Damage: " << wd.getMinDamage() << "-" << wd.getMaxDamage()
                  << " | Stamina Cost: " << wd.getStaminaCost() << '\n';
    } else {
        std::cout << "Currently equipped: None (fighting unarmed)" << '\n';
    }

    std::cout << '\n' << "Available weapons to equip:" << '\n';
    for (size_t i = 0; i < availableWeapons.size(); ++i) {
        Item* weapon = availableWeapons[i];
        const WeaponData& wd = weapon->getWeaponData();
        std::cout << i + 1 << ". " << weapon->getName() << '\n';
        std::cout << "   Damage: " << wd.getMinDamage() << "-" << wd.getMaxDamage()
                  << " | Stamina Cost: " << wd.getStaminaCost() << '\n';
        std::cout << "   " << weapon->getDescription() << '\n';
    }
    std::cout << "0. Cancel" << '\n';

    std::cout << "Choose a weapon to equip: ";
    int choice;
    std::cin >> choice;

    if (choice == 0 || choice < 0 || choice > static_cast<int>(availableWeapons.size())) {
        return false; // Cancelled
    }

    Item* chosenWeapon = availableWeapons[choice - 1];

    // If there's a currently equipped weapon, unequip it first
    if (currentWeapon) {
        currentWeapon->setEquipped(false);
        std::cout << "Unequipped " << currentWeapon->getName() << '\n';
    }

    // Equip the new weapon
    chosenWeapon->setEquipped(true);
    std::cout << "Equipped " << chosenWeapon->getName() << "!" << '\n';

    return true;
}

int generateRandomNumber(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(min, max);
    return distr(gen);
}
