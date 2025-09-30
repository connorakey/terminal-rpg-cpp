//
// Created by Connor on 29/09/2025.
//

#include "itemdatabase.hpp"

// ItemTemplate constructor
ItemTemplate::ItemTemplate(const std::string& name, const std::string& description,
                          int value, int weight, ItemType type, Rarity rarity)
    : name(name), description(description), value(value), weight(weight),
      type(type), rarity(rarity) {}

// ItemDatabase implementation
ItemDatabase& ItemDatabase::getInstance() {
    static ItemDatabase instance;
    return instance;
}

void ItemDatabase::initialize() {
    createWeapons();
    createArmor();
    createPotions();
    createMiscItems();
}

const ItemTemplate* ItemDatabase::getItemTemplate(const std::string& itemName) const {
    auto it = itemTemplates.find(itemName);
    return (it != itemTemplates.end()) ? it->second.get() : nullptr;
}

Item* ItemDatabase::createItem(const std::string& itemName, int inventorySlotId) const {
    const ItemTemplate* template_ptr = getItemTemplate(itemName);
    if (!template_ptr) {
        return nullptr;
    }

    // Create new item with the inventory slot ID
    Item* item = new Item(inventorySlotId, template_ptr->name, template_ptr->description,
                         template_ptr->value, template_ptr->weight, template_ptr->type,
                         template_ptr->rarity);

    // Set type-specific data based on item type
    switch (template_ptr->type) {
        case WEAPON:
            item->setWeaponData(template_ptr->weaponData);
            break;
        case ARMOR:
            item->setArmorData(template_ptr->armorData);
            break;
        case POTION:
            item->setPotionData(template_ptr->potionData);
            break;
        case CURRENCY:
            // Currency items don't have special data - value is used for gold amount
            break;
        case MISC:
            // Misc items don't have special data
            break;
    }

    return item;
}

std::vector<std::string> ItemDatabase::getAllItemNames() const {
    std::vector<std::string> names;
    for (const auto& pair : itemTemplates) {
        names.push_back(pair.first);
    }
    return names;
}

std::vector<std::string> ItemDatabase::getItemsByType(ItemType type) const {
    std::vector<std::string> names;
    for (const auto& pair : itemTemplates) {
        if (pair.second->type == type) {
            names.push_back(pair.first);
        }
    }
    return names;
}

std::vector<std::string> ItemDatabase::getItemsByRarity(Rarity rarity) const {
    std::vector<std::string> names;
    for (const auto& pair : itemTemplates) {
        if (pair.second->rarity == rarity) {
            names.push_back(pair.first);
        }
    }
    return names;
}

void ItemDatabase::addItemTemplate(std::unique_ptr<ItemTemplate> itemTemplate) {
    itemTemplates[itemTemplate->name] = std::move(itemTemplate);
}

void ItemDatabase::createWeapons() {
    // Swords
    auto ironSword = std::make_unique<ItemTemplate>(
        "Iron Sword", "A sturdy iron sword for basic combat", 50, 5, WEAPON, COMMON);
    ironSword->weaponData = WeaponData(8, 12, 85, 1000, SWORD, 100, 10);
    addItemTemplate(std::move(ironSword));

    auto steelSword = std::make_unique<ItemTemplate>(
        "Steel Sword", "A well-crafted steel blade", 120, 6, WEAPON, UNCOMMON);
    steelSword->weaponData = WeaponData(12, 18, 88, 1200, SWORD, 150, 12);
    addItemTemplate(std::move(steelSword));

    auto mysticSword = std::make_unique<ItemTemplate>(
        "Mystic Blade", "A sword infused with magical energy", 500, 4, WEAPON, EPIC);
    mysticSword->weaponData = WeaponData(20, 28, 92, 800, SWORD, 200, 15);
    addItemTemplate(std::move(mysticSword));

    // Bows
    auto woodenBow = std::make_unique<ItemTemplate>(
        "Wooden Bow", "A simple bow made from oak wood", 35, 3, WEAPON, COMMON);
    woodenBow->weaponData = WeaponData(6, 10, 75, 1500, BOW, 80, 8);
    addItemTemplate(std::move(woodenBow));

    auto elfBow = std::make_unique<ItemTemplate>(
        "Longbow", "An elegant longbow forged from the tops of the grandest mountain ranges.", 300, 2, WEAPON, RARE);
    elfBow->weaponData = WeaponData(15, 22, 90, 1000, BOW, 180, 10);
    addItemTemplate(std::move(elfBow));

    // Daggers
    auto rustyDagger = std::make_unique<ItemTemplate>(
        "Rusty Dagger", "An old, worn dagger", 15, 1, WEAPON, COMMON);
    rustyDagger->weaponData = WeaponData(4, 8, 90, 500, DAGGER, 50, 5);
    addItemTemplate(std::move(rustyDagger));

    auto poisonDagger = std::make_unique<ItemTemplate>(
        "Poison Dagger", "A dagger coated with deadly poison", 200, 1, WEAPON, RARE);
    poisonDagger->weaponData = WeaponData(8, 14, 95, 600, DAGGER, 120, 6);
    addItemTemplate(std::move(poisonDagger));

    // Axes
    auto battleAxe = std::make_unique<ItemTemplate>(
        "Battle Axe", "A heavy dual-handed axe", 80, 8, WEAPON, UNCOMMON);
    battleAxe->weaponData = WeaponData(15, 25, 70, 2000, AXE, 120, 20);
    addItemTemplate(std::move(battleAxe));

    // Staffs
    auto wizardStaff = std::make_unique<ItemTemplate>(
        "Wizard Staff", "A staff humming with magical power", 250, 3, WEAPON, RARE);
    wizardStaff->weaponData = WeaponData(10, 20, 80, 1800, STAFF, 100, 25);
    addItemTemplate(std::move(wizardStaff));
}

void ItemDatabase::createArmor() {
    // Leather Armor
    auto leatherArmor = std::make_unique<ItemTemplate>(
        "Leather Armor", "Basic protection made from animal hide", 40, 8, ARMOR, COMMON);
    leatherArmor->armorData = ArmorData(2, 100);
    addItemTemplate(std::move(leatherArmor));

    // Chain Mail
    auto chainMail = std::make_unique<ItemTemplate>(
        "Chain Mail", "Interlocked metal rings provide decent protection", 100, 15, ARMOR, UNCOMMON);
    chainMail->armorData = ArmorData(3, 150);
    addItemTemplate(std::move(chainMail));

    // Steel Armor
    auto steelArmor = std::make_unique<ItemTemplate>(
        "Steel Armor", "Heavy steel plates offering excellent protection", 300, 25, ARMOR, RARE);
    steelArmor->armorData = ArmorData(4, 200);
    addItemTemplate(std::move(steelArmor));

    // Dragon Scale Armor
    auto dragonArmor = std::make_unique<ItemTemplate>(
        "Dragon Scale Armor", "Armor crafted from ancient dragon scales", 1000, 20, ARMOR, LEGENDARY);
    dragonArmor->armorData = ArmorData(5, 300);
    addItemTemplate(std::move(dragonArmor));

    // Cloth Robes
    auto mageRobes = std::make_unique<ItemTemplate>(
        "Mage Robes", "Light robes from the local witchery", 75, 3, ARMOR, UNCOMMON);
    mageRobes->armorData = ArmorData(1, 80);
    addItemTemplate(std::move(mageRobes));
}

void ItemDatabase::createPotions() {
    // Healing Potions
    auto minorHealing = std::make_unique<ItemTemplate>(
        "Minor Healing Potion", "Restores a small amount of health", 25, 1, POTION, COMMON);
    minorHealing->potionData = PotionData(HEALING, 15, 25);
    addItemTemplate(std::move(minorHealing));

    auto healingPotion = std::make_unique<ItemTemplate>(
        "Healing Potion", "Restores a moderate amount of health", 50, 1, POTION, UNCOMMON);
    healingPotion->potionData = PotionData(HEALING, 30, 45);
    addItemTemplate(std::move(healingPotion));

    auto majorHealing = std::make_unique<ItemTemplate>(
        "Major Healing Potion", "Restores a large amount of health", 100, 1, POTION, RARE);
    majorHealing->potionData = PotionData(HEALING, 60, 80);
    addItemTemplate(std::move(majorHealing));

    // Stamina Potions
    auto staminaPotion = std::make_unique<ItemTemplate>(
        "Stamina Potion", "Restores stamina for continued fighting", 30, 1, POTION, COMMON);
    staminaPotion->potionData = PotionData(STAMINA, 20, 35);
    addItemTemplate(std::move(staminaPotion));

    // Damage Potions
    auto strengthPotion = std::make_unique<ItemTemplate>(
        "Strength Potion", "Temporarily increases damage output", 75, 1, POTION, UNCOMMON);
    strengthPotion->potionData = PotionData(DAMAGE, 5, 10);
    addItemTemplate(std::move(strengthPotion));

    // Defense Potions
    auto defensePotion = std::make_unique<ItemTemplate>(
        "Defense Potion", "Temporarily increases defensive capabilities", 60, 1, POTION, UNCOMMON);
    defensePotion->potionData = PotionData(DEFENSE, 3, 7);
    addItemTemplate(std::move(defensePotion));

    // Poison Potions
    auto poisonVial = std::make_unique<ItemTemplate>(
        "Poison Vial", "A deadly toxin that damages over time", 40, 1, POTION, RARE);
    poisonVial->potionData = PotionData(POISON, 10, 20);
    addItemTemplate(std::move(poisonVial));
}

void ItemDatabase::createMiscItems() {
    // Currency Items
    auto goldCoin = std::make_unique<ItemTemplate>(
        "Gold Coin", "A shiny gold coin", 1, 0, CURRENCY, COMMON);
    addItemTemplate(std::move(goldCoin));

    auto silverCoin = std::make_unique<ItemTemplate>(
        "Silver Coin", "A silver coin of moderate value", 1, 0, CURRENCY, COMMON);
    addItemTemplate(std::move(silverCoin));

    auto goldPouch = std::make_unique<ItemTemplate>(
        "Gold Pouch", "A small pouch containing gold coins", 5, 1, CURRENCY, UNCOMMON);
    addItemTemplate(std::move(goldPouch));

    auto treasureChest = std::make_unique<ItemTemplate>(
        "Treasure Chest", "A chest filled with valuable coins", 50, 5, CURRENCY, RARE);
    addItemTemplate(std::move(treasureChest));

    auto gem = std::make_unique<ItemTemplate>(
        "Precious Gem", "A valuable gemstone", 200, 1, CURRENCY, RARE);
    addItemTemplate(std::move(gem));

    // Regular Misc Items
    // Currency/Valuable Items
    auto bread = std::make_unique<ItemTemplate>(
        "Bread", "A simple loaf of bread", 3, 1, MISC, COMMON);
    addItemTemplate(std::move(bread));

    auto torch = std::make_unique<ItemTemplate>(
        "Torch", "A wooden torch for lighting dark areas", 5, 2, MISC, COMMON);
    addItemTemplate(std::move(torch));

    // Quest Items
    auto ancientKey = std::make_unique<ItemTemplate>(
        "Ancient Key", "An old key covered in mysterious runes", 0, 1, MISC, EPIC);
    addItemTemplate(std::move(ancientKey));

    auto dragonEgg = std::make_unique<ItemTemplate>(
        "Dragon Egg", "A rare dragon egg pulsing with inner fire", 5000, 10, MISC, MYTHIC);
    addItemTemplate(std::move(dragonEgg));
}
