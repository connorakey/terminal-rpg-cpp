//
// Created by Connor on 28/09/2025.
//

#ifndef TERMINAL_RPG_ITEM_H
#define TERMINAL_RPG_ITEM_H
#include <string>

class Item;

enum ItemType {
    WEAPON,
    ARMOR,
    POTION,
    CURRENCY, // e.g., gold, gems
    MISC // e.g., quest items, can't be equipped or used
};

enum PotionType {
    HEALING,
    POISON,
    DAMAGE,
    STAMINA,
    DEFENSE,
    RESISTENCE
};

enum Rarity {
    COMMON,
    UNCOMMON,
    RARE,
    EPIC,
    LEGENDARY,
    MYTHIC
};

enum WeaponType {
    SWORD,
    AXE,
    BOW,
    DAGGER,
    STAFF,
    MACE,
    SPEAR,
    CROSSBOW,
    HAMMER,
};

class WeaponData {
public:
    WeaponData();
    WeaponData(int minDamage, int maxDamage, int accuracy, int cooldown, WeaponType weaponType, int durability, int staminaCost);

    int getMinDamage() const;
    int getMaxDamage() const;
    int getAccuracy() const;
    int getCooldown() const;
    bool isOnCooldown() const;
    WeaponType getWeaponType() const;
    int getDurability() const;
    int getStaminaCost() const;

    void setMinDamage(int minDamage);
    void setMaxDamage(int maxDamage);
    void setAccuracy(int accuracy);
    void setCooldown(int cooldown);
    void setOnCooldown(bool onCooldown);
    void setWeaponType(WeaponType weaponType);
    void setDurability(int durability);
    void setStaminaCost(int staminaCost);

private:
    int minDamage;
    int maxDamage;
    int accuracy; // percentage
    int cooldown; // in milliseconds
    bool onCooldown;
    WeaponType weaponType;
    int durability; // amount of uses before breaking
    int staminaCost;
};

class ArmorData {
public:
    ArmorData();
    ArmorData(int armorValue, int durability);

    int getArmorValue() const;
    int getDurability() const;

    void setArmorValue(int armorValue);
    void setDurability(int durability);

private:
    int armorValue; // armour protection points
    int durability; // amount of hits before breaking
};

class PotionData {
public:
    PotionData();
    PotionData(PotionType potionType, int minPotency, int maxPotency);

    PotionType getPotionType() const;
    int getMinPotency() const;
    int getMaxPotency() const;

    void setPotionType(PotionType potionType);
    void setMinPotency(int minPotency);
    void setMaxPotency(int maxPotency);

private:
    PotionType potionType;
    int minPotency;
    int maxPotency;
};

class Item {
public:
    Item();
    Item(int id, const std::string& name, const std::string& description, int value, int weight, ItemType type, Rarity rarity);

    // Getters
    int getId() const;
    const std::string& getName() const;
    const std::string& getDescription() const;
    int getValue() const;
    int getWeight() const;
    ItemType getType() const;
    Rarity getRarity() const;
    bool isEquipped() const;

    // Data getters (for specific item types)
    const WeaponData& getWeaponData() const;
    const ArmorData& getArmorData() const;
    const PotionData& getPotionData() const;

    // Setters
    void setId(int id);
    void setName(const std::string& name);
    void setDescription(const std::string& description);
    void setValue(int value);
    void setWeight(int weight);
    void setType(ItemType type);
    void setRarity(Rarity rarity);
    void setEquipped(bool equipped);

    // Data setters (for specific item types)
    void setWeaponData(const WeaponData& weaponData);
    void setArmorData(const ArmorData& armorData);
    void setPotionData(const PotionData& potionData);

private:
    int id;
    std::string name;
    std::string description;
    int value;
    int weight;
    ItemType type;
    Rarity rarity;
    bool equipped;

    // Replace union with individual data members
    WeaponData weaponData;
    ArmorData armorData;
    PotionData potionData;
};

#endif //TERMINAL_RPG_ITEM_H