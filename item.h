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

struct WeaponData {
    int minDamage;
    int maxDamage;
};

struct ArmorData {
    int armorValue;
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
    union {
        WeaponData weapon;
        ArmorData armor;
        PotionData potion;
    } data;
};
#endif //TERMINAL_RPG_ITEM_H