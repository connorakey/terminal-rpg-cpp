#include <catch2/catch_test_macros.hpp>
#include "../src/enemies/enemy.hpp"

TEST_CASE("Enemy takes damage correctly", "[Enemy]") {
    Enemy enemy("Test Goblin", "A weak goblin.", 1, 50, 5, 10, 2, 1, EnemyType::GOBLINOID, EnemyRarity::COMMON);

    enemy.takeDamage(10);
    REQUIRE(enemy.getHealth() == 42); // 10 - 2 defence = 8 damage

    enemy.takeDamage(5);
    REQUIRE(enemy.getHealth() == 39); // 5 - 2 defence = 3 damage

    enemy.takeDamage(50);
    REQUIRE(enemy.getHealth() == 0); // Health should not go below 0
}

TEST_CASE("Enemy takes spell damage correctly", "[Enemy]") {
    Enemy enemy("Test Skeleton", "A bony skeleton.", 1, 40, 4, 8, 1, 3, EnemyType::UNDEAD, EnemyRarity::COMMON);
    enemy.takeSpellDamage(10);
    REQUIRE(enemy.getHealth() == 33); // 10 - 3 resistance = 7 damage
    enemy.takeSpellDamage(5);
    REQUIRE(enemy.getHealth() == 31); // 5 - 3 resistance = 2 damage
    enemy.takeSpellDamage(50);
    REQUIRE(enemy.getHealth() == 0); // Health should not go below 0
}

TEST_CASE("Enemy heals correctly", "[Enemy]") {
    Enemy enemy("Test Orc", "A strong orc.", 1, 60, 8, 14, 3, 1, EnemyType::HUMANOID, EnemyRarity::UNCOMMON);
    enemy.takeDamage(30);
    REQUIRE(enemy.getHealth() == 33); // 30 - 3 defence = 27 damage
    enemy.healEnemy(10);
    REQUIRE(enemy.getHealth() == 43); // Healed by 10
    enemy.healEnemy(20);
    REQUIRE(enemy.getHealth() == 63); // Healed by 20
}

TEST_CASE("Enemy attack returns value within range", "[Enemy]") {
    Enemy enemy("Test Dragon", "A fierce dragon.", 1, 200, 20, 40, 10, 5, EnemyType::DRAGON, EnemyRarity::EPIC);
    for (int i = 0; i < 100; ++i) {
        int damage = enemy.attack();
        REQUIRE(damage >= 20);
        REQUIRE(damage <= 40);
    }
}

TEST_CASE("Enemy isAlive works correctly", "[Enemy]") {
    Enemy enemy("Test Demon", "A terrifying demon.", 1, 80, 15, 25, 5, 10, EnemyType::DEMON, EnemyRarity::RARE);
    REQUIRE(enemy.isAlive() == true);
    enemy.takeDamage(85); // 85 - 5 defence = 80 damage, health goes to 0
    REQUIRE(enemy.isAlive() == false);
}

TEST_CASE("Enemy getters return correct values", "[Enemy]") {
    Enemy enemy("Test Elemental", "A mystical elemental.", 1, 100, 10, 20, 4, 8, EnemyType::ELEMENTAL, EnemyRarity::RARE);
    REQUIRE(enemy.getName() == "Test Elemental");
    REQUIRE(enemy.getDescription() == "A mystical elemental.");
    REQUIRE(enemy.getLevel() == 1);
    REQUIRE(enemy.getHealth() == 100);
    REQUIRE(enemy.getMinAttack() == 10);
    REQUIRE(enemy.getMaxAttack() == 20);
    REQUIRE(enemy.getDefence() == 4);
    REQUIRE(enemy.getResistance() == 8);
    REQUIRE(enemy.getType() == EnemyType::ELEMENTAL);
    REQUIRE(enemy.getRarity() == EnemyRarity::RARE);
}