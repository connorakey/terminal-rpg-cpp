//
// Created by Connor on 28/09/2025.
//

#ifndef TERMINAL_RPG_ITEM_H
#define TERMINAL_RPG_ITEM_H
#include <string>

struct Item;

enum ItemType {
    WEAPON,
    ARMOR,
    POTION,
    MISC
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

struct WeaponData {
    int minDamage;
    int maxDamage;
    int accuracy; // percentage
    int cooldown; // in milliseconds
    bool onCooldown;
    WeaponType weaponType;
    int durability; // amount of uses before breaking
    int staminaCost;
};

struct ArmorData {
    int armorValue;
    int durability; // amount of hits before breaking
};

struct PotionData {
    PotionType potionType;
    int minPotency;
    int maxPotency;
};

struct Item {
    int id;
    std::string name;
    std::string description;
    int value;
    int weight;
    ItemType type;
    Rarity rarity;
    union {
        WeaponData weapon;
        ArmorData armor;
        PotionData potion;
    } data;
    bool equipped;
};

#endif //TERMINAL_RPG_ITEM_H