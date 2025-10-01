#include <catch2/catch_test_macros.hpp>
#include "../src/player/player.hpp"
#include "../src/levels/leveldatabase.hpp"

TEST_CASE("Player takes damage correctly", "[Player]") {
    Player player("Test Player", 100, 100, 50, 50, 5, 3, 1, 0, 100, 100, 0);

    player.takeDamage(20);
    REQUIRE(player.getHealth() == 85); // 20 - 5 defence = 15 damage, 100 - 15 = 85

    player.takeDamage(10);
    REQUIRE(player.getHealth() == 80); // 10 - 5 defence = 5 damage, 85 - 5 = 80

    player.takeDamage(100);
    REQUIRE(player.getHealth() == 0); // Health should not go below 0
}

TEST_CASE("Player takes spell damage correctly", "[Player]") {
    Player player("Test Player", 100, 100, 50, 50, 5, 3, 1, 0, 100, 100, 0);
    player.takeSpellDamage(20);
    REQUIRE(player.getHealth() == 83); // 20 - 3 resistance = 17 damage
    player.takeSpellDamage(10);
    REQUIRE(player.getHealth() == 76); // 10 - 3 resistance = 7 damage
    player.takeSpellDamage(100);
    REQUIRE(player.getHealth() == 0); // Health should not go below 0
}

TEST_CASE("Player heals correctly", "[Player]") {
    Player player("Test Player", 100, 100, 50, 50, 5, 3, 1, 0, 100, 100, 0);
    player.takeDamage(50);
    REQUIRE(player.getHealth() == 55); // 50 - 5 defence = 45 damage, 100 - 45 = 55
    player.heal(20);
    REQUIRE(player.getHealth() == 75); // Healed by 20, 55 + 20 = 75
    player.heal(50);
    REQUIRE(player.getHealth() == 100); // Healed by 50 but should not exceed max health
}

TEST_CASE("Player uses and recovers stamina correctly", "[Player]") {
    Player player("Test Player", 100, 100, 50, 50, 5, 3, 1, 0, 100, 100, 0);
    player.useStamina(20);
    REQUIRE(player.getStamina() == 30); // Used 20 stamina
    player.useStamina(40);
    REQUIRE(player.getStamina() == 0); // Used more than available stamina, should go to 0
    player.recoverStamina(25);
    REQUIRE(player.getStamina() == 25); // Recovered 25 stamina
    player.recoverStamina(50);
    REQUIRE(player.getStamina() == 50); // Recovered more than max stamina, should not exceed max
}

TEST_CASE("Player changes max health and stamina correctly", "[Player]") {
    Player player("Test Player", 100, 100, 50, 50, 5, 3, 1, 0, 100, 100, 0);
    player.changeMaxHealth(20);
    REQUIRE(player.getMaxHealth() == 120); // Max health increased by 20
    player.changeMaxHealth(-50);
    REQUIRE(player.getMaxHealth() == 70); // Max health decreased by 50
    player.changeMaxHealth(-100);
    REQUIRE(player.getMaxHealth() == 1); // Max health should not go below 1

    player.changeMaxStamina(15);
    REQUIRE(player.getMaxStamina() == 65); // Max stamina increased by 15
    player.changeMaxStamina(-30);
    REQUIRE(player.getMaxStamina() == 35); // Max stamina decreased by 30
    player.changeMaxStamina(-50);
    REQUIRE(player.getMaxStamina() == 1); // Max stamina should not go below 1
}

TEST_CASE("Player changes defence and resistance correctly", "[Player]") {
    Player player("Test Player", 100, 100, 50, 50, 5, 3, 1, 0, 100, 100, 0);
    player.changeDefence(3);
    REQUIRE(player.getDefence() == 8); // Defence increased by 3
    player.changeDefence(-10);
    REQUIRE(player.getDefence() == -2); // Defence decreased by 10

    player.changeResistance(4);
    REQUIRE(player.getResistance() == 7); // Resistance increased by 4
    player.changeResistance(-10);
    REQUIRE(player.getResistance() == -3); // Resistance decreased by 10
}

TEST_CASE("Player adds and removes gold correctly", "[Player]") {
    Player player("Test Player", 100, 100, 50, 50, 5, 3, 1, 0, 0, 100, 0);
    player.addGold(50);
    REQUIRE(player.getGold() == 50); // Added 50 gold
    player.addGold(100);
    REQUIRE(player.getGold() == 150); // Added another 100 gold
    player.removeGold(30);
    REQUIRE(player.getGold() == 120); // Removed 30 gold
    player.removeGold(200);
    REQUIRE(player.getGold() == 0); // Removed more than available gold, should go to 0
}

TEST_CASE("Player gains experience and levels up correctly", "[Player]") {
    // Initialize the LevelDatabase for this test
    LevelDatabase::getInstance().initialize();

    Player player("Test Player", 100, 100, 50, 50, 5, 3, 1, 0, 100, 100, 0);
    player.gainExperience(50);
    REQUIRE(player.getExperience() == 50); // Gained 50 experience
    REQUIRE(player.getLevel() == 1); // Still level 1
    REQUIRE(player.getNextLevelExp() == 100); // Next level at 100 experience

    player.gainExperience(60);
    REQUIRE(player.getExperience() == 110); // Gained another 60 experience
    REQUIRE(player.getLevel() == 2); // Should have leveled up to level 2
    REQUIRE(player.getNextLevelExp() == 250); // Next level at 250 experience

    player.gainExperience(200);
    REQUIRE(player.getExperience() == 310); // Gained another 200 experience
    REQUIRE(player.getLevel() == 3); // Should have leveled up to level 3
    REQUIRE(player.getNextLevelExp() == 450); // Next level at 450 experience
}

TEST_CASE("Player inventory management works correctly", "[Player]") {
    Player player("Test Player", 100, 100, 50, 50, 5, 3, 1, 0, 100, 100, 0);
    Item* sword = new Item(1, "Sword", "A sharp blade.", 100, 5, WEAPON, COMMON);
    Item* shield = new Item(2, "Shield", "A sturdy shield.", 150, 8, ARMOR, UNCOMMON);

    REQUIRE(player.pickupItem(sword) == true); // Should be able to pick up sword
    REQUIRE(player.getInventory().size() == 1);
    REQUIRE(player.getInventory()[0] == sword);

    REQUIRE(player.pickupItem(shield) == true); // Should be able to pick up shield
    REQUIRE(player.getInventory().size() == 2);
    REQUIRE(player.getInventory()[1] == shield);

    // Simulate weight limit by creating a heavy item
    Item* heavyItem = new Item(3, "Heavy Item", "Too heavy to carry.", 200, 200, MISC, RARE);
    REQUIRE(player.pickupItem(heavyItem) == false); // Should not be able to pick up heavy item
    REQUIRE(player.getInventory().size() == 2); // Inventory size should remain the same

    delete sword;
    delete shield;
    delete heavyItem;
}

TEST_CASE("Player equips and unequips items correctly", "[Player]") {
    Player player("Test Player", 100, 100, 50, 50, 5, 3, 1, 0, 100, 100, 0);
    Item* sword = new Item(1, "Sword", "A sharp blade.", 100, 5, WEAPON, COMMON);
    Item* shield = new Item(2, "Shield", "A sturdy shield.", 150, 8, ARMOR, UNCOMMON);

    player.setEquippedWeapon(sword);
    REQUIRE(player.getEquippedWeapon() == sword); // Sword should be equipped

    player.setEquippedArmor(shield);
    REQUIRE(player.getEquippedArmor() == shield); // Shield should be equipped

    player.setEquippedWeapon(nullptr);
    REQUIRE(player.getEquippedWeapon() == nullptr); // No weapon should be equipped

    player.setEquippedArmor(nullptr);
    REQUIRE(player.getEquippedArmor() == nullptr); // No armor should be equipped

    delete sword;
    delete shield;
}