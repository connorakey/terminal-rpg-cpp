#include <catch2/catch_test_macros.hpp>
#include "../src/chest/chest.hpp"
#include "../src/items/item.hpp"

TEST_CASE("Chest getItem returns correct item", "[Chest]") {
    Item* testItem = new Item(0, "Test Item", "An item for testing.", 50, 2, MISC, COMMON);
    Chest chest(false, NONE, testItem);

    REQUIRE(chest.getItem() == testItem);

    delete testItem;
}

TEST_CASE("Chest opens correctly when not locked and not trapped", "[Chest]") {
    Item* testItem = new Item(1, "Test Item", "An item for testing.", 100, 5, WEAPON, COMMON);
    Chest chest(false, NONE, testItem);

    OpenChestResult result = chest.open();

    REQUIRE(result.result == SUCCESS);
    REQUIRE(result.item == testItem);
    REQUIRE(result.trapInfo.type == NONE);
    REQUIRE(result.trapInfo.damage == 0);
    REQUIRE(result.trapInfo.summonsEnemies == false);

    delete testItem;
}

TEST_CASE("Chest fails to open when locked", "[Chest]") {
    Item* testItem = new Item(2, "Test Item", "An item for testing.", 150, 3, ARMOR, UNCOMMON);
    Chest chest(true, NONE, testItem);

    OpenChestResult result = chest.open();

    REQUIRE(result.result == LOCKED);
    REQUIRE(result.item == nullptr);
    REQUIRE(result.trapInfo.type == NONE);
    REQUIRE(result.trapInfo.damage == 0);
    REQUIRE(result.trapInfo.summonsEnemies == false);

    delete testItem;
}

TEST_CASE("Chest triggers poison dart trap", "[Chest]") {
    Item* testItem = new Item(3, "Poisoned Item", "An item for testing.", 200, 4, POTION, RARE);
    Chest chest(false, POISON_DART, testItem);

    OpenChestResult result = chest.open();

    REQUIRE(result.result == TRAPPED);
    REQUIRE(result.item == testItem);
    REQUIRE(result.trapInfo.type == POISON_DART);
    REQUIRE(result.trapInfo.damage >= 10);
    REQUIRE(result.trapInfo.damage <= 60);
    REQUIRE(result.trapInfo.summonsEnemies == false);

    delete testItem;
}

TEST_CASE("Chest triggers explosion trap", "[Chest]") {
    Item* testItem = new Item(4, "Test Item", "An item for testing.", 250, 2, MISC, EPIC);
    Chest chest(false, EXPLOSION, testItem);

    OpenChestResult result = chest.open();

    REQUIRE(result.result == TRAPPED);
    REQUIRE(result.item == testItem);
    REQUIRE(result.trapInfo.type == EXPLOSION);
    REQUIRE(result.trapInfo.damage >= 30);
    REQUIRE(result.trapInfo.damage <= 70);
    REQUIRE((result.trapInfo.summonsEnemies == true || result.trapInfo.summonsEnemies == false));

    delete testItem;
}

TEST_CASE("Chest triggers alarm trap", "[Chest]") {
    Item* testItem = new Item(5, "Test Item", "An item for testing.", 300, 1, CURRENCY, LEGENDARY);
    Chest chest(false, ALARM, testItem);

    OpenChestResult result = chest.open();

    REQUIRE(result.result == TRAPPED);
    REQUIRE(result.item == testItem);
    REQUIRE(result.trapInfo.type == ALARM);
    REQUIRE(result.trapInfo.damage == 0);
    REQUIRE(result.trapInfo.summonsEnemies == true);

    delete testItem;
}

TEST_CASE("Chest opens empty when no item present", "[Chest]") {
    Chest chest(false, NONE, nullptr);

    OpenChestResult result = chest.open();

    REQUIRE(result.result == EMPTY);
    REQUIRE(result.item == nullptr);
    REQUIRE(result.trapInfo.type == NONE);
    REQUIRE(result.trapInfo.damage == 0);
    REQUIRE(result.trapInfo.summonsEnemies == false);
}

TEST_CASE("Chest with trap but no item still triggers trap", "[Chest]") {
    Chest chest(false, POISON_DART, nullptr);

    OpenChestResult result = chest.open();

    REQUIRE(result.result == TRAPPED);
    REQUIRE(result.item == nullptr);
    REQUIRE(result.trapInfo.type == POISON_DART);
    REQUIRE(result.trapInfo.damage >= 10);
    REQUIRE(result.trapInfo.damage <= 60);
    REQUIRE(result.trapInfo.summonsEnemies == false);
}

TEST_CASE("Chest with explosion trap but no item still triggers trap", "[Chest]") {
    Chest chest(false, EXPLOSION, nullptr);

    OpenChestResult result = chest.open();

    REQUIRE(result.result == TRAPPED);
    REQUIRE(result.item == nullptr);
    REQUIRE(result.trapInfo.type == EXPLOSION);
    REQUIRE(result.trapInfo.damage >= 30);
    REQUIRE(result.trapInfo.damage <= 70);
    REQUIRE((result.trapInfo.summonsEnemies == true || result.trapInfo.summonsEnemies == false));
}

TEST_CASE("Chest with alarm trap but no item still triggers trap", "[Chest]") {
    Chest chest(false, ALARM, nullptr);

    OpenChestResult result = chest.open();

    REQUIRE(result.result == TRAPPED);
    REQUIRE(result.item == nullptr);
    REQUIRE(result.trapInfo.type == ALARM);
    REQUIRE(result.trapInfo.damage == 0);
    REQUIRE(result.trapInfo.summonsEnemies == true);
}

TEST_CASE("Chest with locked and trapped states", "[Chest]") {
    Item* testItem = new Item(6, "Test Item", "An item for testing.", 350, 6, WEAPON, LEGENDARY);
    Chest chest(true, POISON_DART, testItem);

    OpenChestResult result = chest.open();

    REQUIRE(result.result == LOCKED);
    REQUIRE(result.item == nullptr);
    REQUIRE(result.trapInfo.type == NONE);
    REQUIRE(result.trapInfo.damage == 0);
    REQUIRE(result.trapInfo.summonsEnemies == false);

    delete testItem;
}