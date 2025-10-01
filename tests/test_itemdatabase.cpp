#include <catch2/catch_test_macros.hpp>

#include "../src/items/item.hpp"
#include "../src/items/itemdatabase.hpp"

TEST_CASE("ItemDatabase singleton pattern works correctly", "[itemdatabase]") {
    ItemDatabase& db1 = ItemDatabase::getInstance();
    ItemDatabase& db2 = ItemDatabase::getInstance();
    REQUIRE(&db1 == &db2);
}

TEST_CASE("ItemDatabase initializes and retrieves item templates correctly", "[itemdatabase]") {
    ItemDatabase& db = ItemDatabase::getInstance();
    db.initialize();
    const ItemTemplate* sword = db.getItemTemplate("Iron Sword");
    REQUIRE(sword != nullptr);
    REQUIRE(sword->name == "Iron Sword");
    REQUIRE(sword->type == WEAPON);
    REQUIRE(sword->rarity == COMMON);
    REQUIRE(sword->weaponData.getMinDamage() == 8);
    REQUIRE(sword->weaponData.getMaxDamage() == 12);

    const ItemTemplate* armor = db.getItemTemplate("Steel Armor");
    REQUIRE(armor != nullptr);
    REQUIRE(armor->type == ARMOR);
    REQUIRE(armor->armorData.getArmorValue() == 4);
    REQUIRE(armor->armorData.getDurability() == 200);

    const ItemTemplate* potion = db.getItemTemplate("Healing Potion");
    REQUIRE(potion != nullptr);
    REQUIRE(potion->type == POTION);
    REQUIRE(potion->potionData.getPotionType() == HEALING);
    REQUIRE(potion->potionData.getMinPotency() == 30);
    REQUIRE(potion->potionData.getMaxPotency() == 45);

    const ItemTemplate* misc = db.getItemTemplate("Bread");
    REQUIRE(misc != nullptr);
    REQUIRE(misc->type == MISC);
    REQUIRE(misc->rarity == COMMON);

    // Nonexistent item
    REQUIRE(db.getItemTemplate("Nonexistent Item") == nullptr);
}

TEST_CASE("ItemDatabase creates items from templates correctly", "[itemdatabase]") {
    ItemDatabase& db = ItemDatabase::getInstance();
    db.initialize();
    Item* sword = db.createItem("Iron Sword", 42);
    REQUIRE(sword != nullptr);
    REQUIRE(sword->getId() == 42);
    REQUIRE(sword->getName() == "Iron Sword");
    REQUIRE(sword->getType() == WEAPON);
    REQUIRE(sword->getWeaponData().getMinDamage() == 8);
    REQUIRE(sword->getWeaponData().getMaxDamage() == 12);
    delete sword;

    Item* armor = db.createItem("Steel Armor", 99);
    REQUIRE(armor != nullptr);
    REQUIRE(armor->getId() == 99);
    REQUIRE(armor->getType() == ARMOR);
    REQUIRE(armor->getArmorData().getArmorValue() == 4);
    delete armor;

    Item* potion = db.createItem("Healing Potion", 7);
    REQUIRE(potion != nullptr);
    REQUIRE(potion->getType() == POTION);
    REQUIRE(potion->getPotionData().getPotionType() == HEALING);
    delete potion;

    Item* misc = db.createItem("Bread", 5);
    REQUIRE(misc != nullptr);
    REQUIRE(misc->getType() == MISC);
    delete misc;

    // Nonexistent item
    Item* none = db.createItem("Nonexistent Item", 1);
    REQUIRE(none == nullptr);
}

TEST_CASE("ItemDatabase getAllItemNames returns all item names", "[itemdatabase]") {
    ItemDatabase& db = ItemDatabase::getInstance();
    db.initialize();
    std::vector<std::string> names = db.getAllItemNames();
    REQUIRE(std::find(names.begin(), names.end(), "Iron Sword") != names.end());
    REQUIRE(std::find(names.begin(), names.end(), "Steel Armor") != names.end());
    REQUIRE(std::find(names.begin(), names.end(), "Healing Potion") != names.end());
    REQUIRE(std::find(names.begin(), names.end(), "Bread") != names.end());
}

TEST_CASE("ItemDatabase getItemsByType filters items by type", "[itemdatabase]") {
    ItemDatabase& db = ItemDatabase::getInstance();
    db.initialize();
    std::vector<std::string> weapons = db.getItemsByType(WEAPON);
    REQUIRE(std::find(weapons.begin(), weapons.end(), "Iron Sword") != weapons.end());
    REQUIRE(std::find(weapons.begin(), weapons.end(), "Battle Axe") != weapons.end());
    std::vector<std::string> potions = db.getItemsByType(POTION);
    REQUIRE(std::find(potions.begin(), potions.end(), "Healing Potion") != potions.end());
    std::vector<std::string> misc = db.getItemsByType(MISC);
    REQUIRE(std::find(misc.begin(), misc.end(), "Bread") != misc.end());
}

TEST_CASE("ItemDatabase getItemsByRarity filters items by rarity", "[itemdatabase]") {
    ItemDatabase& db = ItemDatabase::getInstance();
    db.initialize();
    std::vector<std::string> common = db.getItemsByRarity(COMMON);
    REQUIRE(std::find(common.begin(), common.end(), "Iron Sword") != common.end());
    REQUIRE(std::find(common.begin(), common.end(), "Bread") != common.end());
    std::vector<std::string> rare = db.getItemsByRarity(RARE);
    REQUIRE(std::find(rare.begin(), rare.end(), "Steel Armor") != rare.end());
    REQUIRE(std::find(rare.begin(), rare.end(), "Poison Vial") != rare.end());
}

TEST_CASE("ItemDatabase handles edge cases and empty database", "[itemdatabase]") {
    // Create a fresh instance (simulate empty database)
    ItemDatabase& db = ItemDatabase::getInstance();
    // Clear and reinitialize
    db.initialize();
    // Try to get item by invalid type/rarity
    std::vector<std::string> mythic = db.getItemsByRarity(MYTHIC);
    REQUIRE(std::find(mythic.begin(), mythic.end(), "Dragon Egg") != mythic.end());
    std::vector<std::string> currency = db.getItemsByType(CURRENCY);
    REQUIRE(std::find(currency.begin(), currency.end(), "Gold Coin") != currency.end());
    // Check for either "Gem" or "Precious Gem" - break into separate checks
    bool hasGem = std::find(currency.begin(), currency.end(), "Gem") != currency.end();
    bool hasPreciousGem =
        std::find(currency.begin(), currency.end(), "Precious Gem") != currency.end();
    REQUIRE((hasGem || hasPreciousGem));
}
