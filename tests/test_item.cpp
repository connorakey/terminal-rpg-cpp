#include <catch2/catch_test_macros.hpp>
#include "../src/items/item.hpp"

TEST_CASE("Item default constructor initializes members correctly", "[Item]") {
    Item item;

    REQUIRE(item.getId() == 0);
    REQUIRE(item.getName() == "");
    REQUIRE(item.getDescription() == "");
    REQUIRE(item.getValue() == 0);
    REQUIRE(item.getWeight() == 0);
    REQUIRE(item.getType() == MISC);
    REQUIRE(item.getRarity() == COMMON);
    REQUIRE(item.isEquipped() == false);

    // Check that type specific data members are initialized correctly
    WeaponData weaponData = item.getWeaponData();
    REQUIRE(weaponData.getMinDamage() == 0);
    REQUIRE(weaponData.getMaxDamage() == 0);
    REQUIRE(weaponData.getAccuracy() == 0);
    REQUIRE(weaponData.getCooldown() == 0);
    REQUIRE(weaponData.isOnCooldown() == false);
    REQUIRE(weaponData.getWeaponType() == SWORD);
    REQUIRE(weaponData.getDurability() == 0);
    REQUIRE(weaponData.getStaminaCost() == 0);

    ArmorData armorData = item.getArmorData();
    REQUIRE(armorData.getArmorValue() == 0);
    REQUIRE(armorData.getDurability() == 0);

    PotionData potionData = item.getPotionData();
    REQUIRE(potionData.getPotionType() == HEALING);
    REQUIRE(potionData.getMinPotency() == 0);
    REQUIRE(potionData.getMaxPotency() == 0);
}

TEST_CASE("Item parameterized constructor initializes members correctly", "[Item]") {
    Item item(1, "Sword of Testing", "A sword used for testing.", 100, 5, WEAPON, RARE);

    REQUIRE(item.getId() == 1);
    REQUIRE(item.getName() == "Sword of Testing");
    REQUIRE(item.getDescription() == "A sword used for testing.");
    REQUIRE(item.getValue() == 100);
    REQUIRE(item.getWeight() == 5);
    REQUIRE(item.getType() == WEAPON);
    REQUIRE(item.getRarity() == RARE);
    REQUIRE(item.isEquipped() == false);

    // Check that type-specific data members are initialized correctly
    WeaponData weaponData = item.getWeaponData();
    REQUIRE(weaponData.getMinDamage() == 0);
    REQUIRE(weaponData.getMaxDamage() == 0);
    REQUIRE(weaponData.getAccuracy() == 0);
    REQUIRE(weaponData.getCooldown() == 0);
    REQUIRE(weaponData.isOnCooldown() == false);
    REQUIRE(weaponData.getWeaponType() == SWORD);
    REQUIRE(weaponData.getDurability() == 0);
    REQUIRE(weaponData.getStaminaCost() == 0);

    ArmorData armorData = item.getArmorData();
    REQUIRE(armorData.getArmorValue() == 0);
    REQUIRE(armorData.getDurability() == 0);

    PotionData potionData = item.getPotionData();
    REQUIRE(potionData.getPotionType() == HEALING);
    REQUIRE(potionData.getMinPotency() == 0);
    REQUIRE(potionData.getMaxPotency() == 0);
}

TEST_CASE("Item setters update members correctly", "[Item]") {
    Item item;

    item.setId(2);
    item.setName("Shield of Testing");
    item.setDescription("A shield used for testing.");
    item.setValue(150);
    item.setWeight(10);
    item.setType(ARMOR);
    item.setRarity(EPIC);
    item.setEquipped(true);

    REQUIRE(item.getId() == 2);
    REQUIRE(item.getName() == "Shield of Testing");
    REQUIRE(item.getDescription() == "A shield used for testing.");
    REQUIRE(item.getValue() == 150);
    REQUIRE(item.getWeight() == 10);
    REQUIRE(item.getType() == ARMOR);
    REQUIRE(item.getRarity() == EPIC);
    REQUIRE(item.isEquipped() == true);
}

TEST_CASE("Item type specific data setters and getters work correctly", "[Item]") {
    Item item;

    WeaponData wd(10, 20, 90, 1000, SWORD, 50, 5);
    item.setWeaponData(wd);
    WeaponData retrievedWd = item.getWeaponData();
    REQUIRE(retrievedWd.getMinDamage() == 10);
    REQUIRE(retrievedWd.getMaxDamage() == 20);
    REQUIRE(retrievedWd.getAccuracy() == 90);
    REQUIRE(retrievedWd.getCooldown() == 1000);
    REQUIRE(retrievedWd.getWeaponType() == SWORD);
    REQUIRE(retrievedWd.getDurability() == 50);
    REQUIRE(retrievedWd.getStaminaCost() == 5);

    ArmorData ad(15, 40);
    item.setArmorData(ad);
    ArmorData retrievedAd = item.getArmorData();
    REQUIRE(retrievedAd.getArmorValue() == 15);
    REQUIRE(retrievedAd.getDurability() == 40);

    PotionData pd(POISON, 20, 50);
    item.setPotionData(pd);
    PotionData retrievedPd = item.getPotionData();
    REQUIRE(retrievedPd.getPotionType() == POISON);
    REQUIRE(retrievedPd.getMinPotency() == 20);
    REQUIRE(retrievedPd.getMaxPotency() == 50);
}

TEST_CASE("Item equipped state toggles correctly", "[Item]") {
    Item item;

    REQUIRE(item.isEquipped() == false);
    item.setEquipped(true);
    REQUIRE(item.isEquipped() == true);
    item.setEquipped(false);
    REQUIRE(item.isEquipped() == false);
}