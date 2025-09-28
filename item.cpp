#include "item.hpp"

// WeaponData implementations
WeaponData::WeaponData()
    : minDamage(0), maxDamage(0), accuracy(0), cooldown(0), onCooldown(false),
      weaponType(SWORD), durability(0), staminaCost(0) {}

WeaponData::WeaponData(int minDamage, int maxDamage, int accuracy, int cooldown, WeaponType weaponType, int durability, int staminaCost)
    : minDamage(minDamage), maxDamage(maxDamage), accuracy(accuracy), cooldown(cooldown),
      onCooldown(false), weaponType(weaponType), durability(durability), staminaCost(staminaCost) {}

int WeaponData::getMinDamage() const { return minDamage; }
int WeaponData::getMaxDamage() const { return maxDamage; }
int WeaponData::getAccuracy() const { return accuracy; }
int WeaponData::getCooldown() const { return cooldown; }
bool WeaponData::isOnCooldown() const { return onCooldown; }
WeaponType WeaponData::getWeaponType() const { return weaponType; }
int WeaponData::getDurability() const { return durability; }
int WeaponData::getStaminaCost() const { return staminaCost; }

void WeaponData::setMinDamage(int minDamage) { this->minDamage = minDamage; }
void WeaponData::setMaxDamage(int maxDamage) { this->maxDamage = maxDamage; }
void WeaponData::setAccuracy(int accuracy) { this->accuracy = accuracy; }
void WeaponData::setCooldown(int cooldown) { this->cooldown = cooldown; }
void WeaponData::setOnCooldown(bool onCooldown) { this->onCooldown = onCooldown; }
void WeaponData::setWeaponType(WeaponType weaponType) { this->weaponType = weaponType; }
void WeaponData::setDurability(int durability) { this->durability = durability; }
void WeaponData::setStaminaCost(int staminaCost) { this->staminaCost = staminaCost; }

// ArmorData implementations
ArmorData::ArmorData() : armorValue(0), durability(0) {}

ArmorData::ArmorData(int armorValue, int durability) : armorValue(armorValue), durability(durability) {}

int ArmorData::getArmorValue() const { return armorValue; }
int ArmorData::getDurability() const { return durability; }

void ArmorData::setArmorValue(int armorValue) { this->armorValue = armorValue; }
void ArmorData::setDurability(int durability) { this->durability = durability; }

// PotionData implementations
PotionData::PotionData() : potionType(HEALING), minPotency(0), maxPotency(0) {}

PotionData::PotionData(PotionType potionType, int minPotency, int maxPotency)
    : potionType(potionType), minPotency(minPotency), maxPotency(maxPotency) {}

PotionType PotionData::getPotionType() const { return potionType; }
int PotionData::getMinPotency() const { return minPotency; }
int PotionData::getMaxPotency() const { return maxPotency; }

void PotionData::setPotionType(PotionType potionType) { this->potionType = potionType; }
void PotionData::setMinPotency(int minPotency) { this->minPotency = minPotency; }
void PotionData::setMaxPotency(int maxPotency) { this->maxPotency = maxPotency; }

// Item implementations
Item::Item() : id(0), name(""), description(""), value(0), weight(0), type(MISC), rarity(COMMON), equipped(false) {
    // Data members are automatically initialized with their default constructors
}

Item::Item(int id, const std::string& name, const std::string& description, int value, int weight, ItemType type, Rarity rarity)
    : id(id), name(name), description(description), value(value), weight(weight), type(type), rarity(rarity), equipped(false) {
    // Data members are automatically initialized with their default constructors
}

int Item::getId() const { return id; }
const std::string &Item::getName() const { return name; }
const std::string &Item::getDescription() const { return description; }
int Item::getValue() const { return value; }
int Item::getWeight() const { return weight; }
ItemType Item::getType() const { return type; }
Rarity Item::getRarity() const { return rarity; }
bool Item::isEquipped() const { return equipped; }

const WeaponData& Item::getWeaponData() const { return weaponData; }
const ArmorData& Item::getArmorData() const { return armorData; }
const PotionData& Item::getPotionData() const { return potionData; }

void Item::setId(int id) { this->id = id; }
void Item::setName(const std::string& name) { this->name = name; }
void Item::setDescription(const std::string &description) { this->description = description; }
void Item::setValue(int value) { this->value = value; }
void Item::setWeight(int weight) { this->weight = weight; }
void Item::setType(ItemType type) { this->type = type; }
void Item::setRarity(Rarity rarity) { this->rarity = rarity; }
void Item::setEquipped(bool equipped) { this->equipped = equipped; }

void Item::setWeaponData(const WeaponData& weaponData) { this->weaponData = weaponData; }
void Item::setArmorData(const ArmorData& armorData) { this->armorData = armorData; }
void Item::setPotionData(const PotionData& potionData) { this->potionData = potionData; }
