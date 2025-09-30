//
// Created by Connor on 29/09/2025.
//

#ifndef TERMINAL_RPG_ITEMDATABASE_HPP
#define TERMINAL_RPG_ITEMDATABASE_HPP

#include <vector>
#include <memory>
#include <map>
#include <string>
#include "item.hpp"

struct ItemTemplate {
    std::string name;
    std::string description;
    int value;
    int weight;
    ItemType type;
    Rarity rarity;

    // Type data
    WeaponData weaponData;
    ArmorData armorData;
    PotionData potionData;

    ItemTemplate(const std::string& name, const std::string& description,
                int value, int weight, ItemType type, Rarity rarity);
};

class ItemDatabase {
public:
    static ItemDatabase& getInstance();

    // Initialize the database with all predefined items
    void initialize();

    // Get item template by name (returns nullptr if not found)
    const ItemTemplate* getItemTemplate(const std::string& itemName) const;

    // Create a new Item instance from template (with unique inventory ID)
    Item* createItem(const std::string& itemName, int inventorySlotId) const;

    std::vector<std::string> getAllItemNames() const;

    std::vector<std::string> getItemsByType(ItemType type) const;

    std::vector<std::string> getItemsByRarity(Rarity rarity) const;

private:
    ItemDatabase() = default;
    ~ItemDatabase() = default;
    ItemDatabase(const ItemDatabase&) = delete;
    ItemDatabase& operator=(const ItemDatabase&) = delete;

    std::map<std::string, std::unique_ptr<ItemTemplate>> itemTemplates;

    // Helper methods to create specific item types
    void createWeapons();
    void createArmor();
    void createPotions();
    void createMiscItems();

    // Helper to add item template
    void addItemTemplate(std::unique_ptr<ItemTemplate> itemTemplate);
};

#endif //TERMINAL_RPG_ITEMDATABASE_HPP
