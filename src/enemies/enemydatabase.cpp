//
// Created by Connor on 30/09/2025.
//

#include "enemydatabase.hpp"

#include <algorithm>
#include <random>

// EnemyTemplate constructor
EnemyTemplate::EnemyTemplate(const std::string& name, const std::string& description, int level,
                             int health, int minAttack, int maxAttack, int defence, int resistance,
                             EnemyType type, EnemyRarity rarity)
    : name(name),
      description(description),
      level(level),
      health(health),
      minAttack(minAttack),
      maxAttack(maxAttack),
      defence(defence),
      resistance(resistance),
      type(type),
      rarity(rarity) {}

// EnemyDatabase implementation
EnemyDatabase& EnemyDatabase::getInstance() {
    static EnemyDatabase instance;
    return instance;
}

void EnemyDatabase::initialize() {
    createBeasts();
    createUndead();
    createHumanoids();
    createDragons();
    createElementals();
    createDemons();
    createGoblinoids();
    createBosses();
}

const EnemyTemplate* EnemyDatabase::getEnemyTemplate(const std::string& enemyName) const {
    auto it = enemyTemplates.find(enemyName);
    return (it != enemyTemplates.end()) ? it->second.get() : nullptr;
}

Enemy* EnemyDatabase::createEnemy(const std::string& enemyName) const {
    const EnemyTemplate* template_ptr = getEnemyTemplate(enemyName);
    if (!template_ptr) {
        return nullptr;
    }

    return new Enemy(template_ptr->name, template_ptr->description, template_ptr->level,
                     template_ptr->health, template_ptr->minAttack, template_ptr->maxAttack,
                     template_ptr->defence, template_ptr->resistance, template_ptr->type,
                     template_ptr->rarity);
}

std::vector<std::string> EnemyDatabase::getAllEnemyNames() const {
    std::vector<std::string> names;
    for (const auto& pair : enemyTemplates) {
        names.push_back(pair.first);
    }
    return names;
}

std::vector<std::string> EnemyDatabase::getEnemiesByType(EnemyType type) const {
    std::vector<std::string> names;
    for (const auto& pair : enemyTemplates) {
        if (pair.second->type == type) {
            names.push_back(pair.first);
        }
    }
    return names;
}

std::vector<std::string> EnemyDatabase::getEnemiesByRarity(EnemyRarity rarity) const {
    std::vector<std::string> names;
    for (const auto& pair : enemyTemplates) {
        if (pair.second->rarity == rarity) {
            names.push_back(pair.first);
        }
    }
    return names;
}

std::vector<std::string> EnemyDatabase::getEnemiesByLevelRange(int minLevel, int maxLevel) const {
    std::vector<std::string> names;
    for (const auto& pair : enemyTemplates) {
        if (pair.second->level >= minLevel && pair.second->level <= maxLevel) {
            names.push_back(pair.first);
        }
    }
    return names;
}

std::string EnemyDatabase::getRandomEnemyByLevel(int minLevel, int maxLevel) const {
    std::vector<std::string> eligibleEnemies = getEnemiesByLevelRange(minLevel, maxLevel);

    if (eligibleEnemies.empty()) {
        return "";
    }

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, eligibleEnemies.size() - 1);

    return eligibleEnemies[dist(gen)];
}

void EnemyDatabase::addEnemyTemplate(std::unique_ptr<EnemyTemplate> enemyTemplate) {
    enemyTemplates[enemyTemplate->name] = std::move(enemyTemplate);
}

void EnemyDatabase::createBeasts() {
    // Forest Wolf
    auto forestWolf = std::make_unique<EnemyTemplate>(
        "Forest Wolf", "A fierce wolf with sharp fangs and keen senses", 2, 30, 5, 8, 1, 0,
        EnemyType::BEAST, EnemyRarity::COMMON);
    addEnemyTemplate(std::move(forestWolf));

    // Giant Bear
    auto giantBear = std::make_unique<EnemyTemplate>(
        "Giant Bear", "A massive bear with powerful claws and thick fur", 5, 80, 10, 15, 3, 1,
        EnemyType::BEAST, EnemyRarity::UNCOMMON);
    addEnemyTemplate(std::move(giantBear));

    // Dire Wolf
    auto direWolf = std::make_unique<EnemyTemplate>(
        "Dire Wolf", "An unnaturally large wolf with glowing red eyes", 7, 100, 12, 18, 2, 2,
        EnemyType::BEAST, EnemyRarity::RARE);
    addEnemyTemplate(std::move(direWolf));

    // Giant Spider
    auto giantSpider =
        std::make_unique<EnemyTemplate>("Giant Spider", "A venomous spider the size of a horse", 4,
                                        50, 8, 12, 2, 1, EnemyType::BEAST, EnemyRarity::UNCOMMON);
    addEnemyTemplate(std::move(giantSpider));

    // Cave Bat
    auto caveBat = std::make_unique<EnemyTemplate>(
        "Cave Bat", "A small but aggressive bat with razor sharp teeth", 1, 15, 3, 6, 0, 0,
        EnemyType::BEAST, EnemyRarity::COMMON);
    addEnemyTemplate(std::move(caveBat));
}

void EnemyDatabase::createUndead() {
    // Skeleton Warrior
    auto skeletonWarrior = std::make_unique<EnemyTemplate>(
        "Skeleton Warrior",
        "A skeleton wielding a rusty sword and a craving for the taste of blood", 3, 40, 6, 10, 2,
        3, EnemyType::UNDEAD, EnemyRarity::COMMON);
    addEnemyTemplate(std::move(skeletonWarrior));

    // Zombie
    auto zombie =
        std::make_unique<EnemyTemplate>("Zombie", "A shambling corpse with an insatiable hunger", 2,
                                        50, 4, 8, 1, 2, EnemyType::UNDEAD, EnemyRarity::COMMON);
    addEnemyTemplate(std::move(zombie));

    // Wraith
    auto wraith = std::make_unique<EnemyTemplate>(
        "Wraith", "A ghostly figure that drains the life from its victims", 8, 70, 15, 20, 1, 8,
        EnemyType::UNDEAD, EnemyRarity::RARE);
    addEnemyTemplate(std::move(wraith));

    // Lich
    auto lich =
        std::make_unique<EnemyTemplate>("Lich", "An undead sorcerer of immense magical power", 15,
                                        200, 25, 35, 5, 15, EnemyType::UNDEAD, EnemyRarity::EPIC);
    addEnemyTemplate(std::move(lich));
}

void EnemyDatabase::createHumanoids() {
    // Bandit
    auto bandit =
        std::make_unique<EnemyTemplate>("Bandit", "A desperate outlaw seeking easy prey", 3, 35, 5,
                                        9, 2, 0, EnemyType::HUMANOID, EnemyRarity::COMMON);
    addEnemyTemplate(std::move(bandit));

    // Orc Warrior
    auto orcWarrior = std::make_unique<EnemyTemplate>(
        "Orc Warrior", "A brutish orc armed with crude weapons", 5, 60, 8, 14, 3, 1,
        EnemyType::HUMANOID, EnemyRarity::UNCOMMON);
    addEnemyTemplate(std::move(orcWarrior));

    // Dark Elf Assassin
    auto darkElfAssassin = std::make_unique<EnemyTemplate>(
        "Dark Elf Assassin", "A swift and deadly assassin from the underground", 10, 80, 18, 25, 2,
        4, EnemyType::HUMANOID, EnemyRarity::RARE);
    addEnemyTemplate(std::move(darkElfAssassin));

    // Cultist
    auto cultist =
        std::make_unique<EnemyTemplate>("Cultist", "A fanatical worshipper of dark powers", 4, 40,
                                        6, 11, 1, 5, EnemyType::HUMANOID, EnemyRarity::COMMON);
    addEnemyTemplate(std::move(cultist));
}

void EnemyDatabase::createGoblinoids() {
    // Goblin
    auto goblin =
        std::make_unique<EnemyTemplate>("Goblin", "A small, cunning creature with sharp claws", 1,
                                        20, 3, 6, 1, 0, EnemyType::GOBLINOID, EnemyRarity::COMMON);
    addEnemyTemplate(std::move(goblin));

    // Hobgoblin
    auto hobgoblin = std::make_unique<EnemyTemplate>(
        "Hobgoblin", "A larger, more organized cousin of the goblin", 4, 55, 7, 12, 3, 1,
        EnemyType::GOBLINOID, EnemyRarity::UNCOMMON);
    addEnemyTemplate(std::move(hobgoblin));

    // Goblin Shaman
    auto goblinShaman = std::make_unique<EnemyTemplate>(
        "Goblin Shaman", "A goblin wielding primitive but effective magic", 6, 45, 10, 16, 1, 6,
        EnemyType::GOBLINOID, EnemyRarity::UNCOMMON);
    addEnemyTemplate(std::move(goblinShaman));
}

void EnemyDatabase::createElementals() {
    // Fire Elemental
    auto fireElemental =
        std::make_unique<EnemyTemplate>("Fire Elemental", "A being of pure flame and fury", 8, 90,
                                        15, 22, 2, 10, EnemyType::ELEMENTAL, EnemyRarity::RARE);
    addEnemyTemplate(std::move(fireElemental));

    // Ice Elemental
    auto iceElemental = std::make_unique<EnemyTemplate>(
        "Ice Elemental", "A crystalline creature that radiates freezing cold", 8, 100, 12, 18, 4, 8,
        EnemyType::ELEMENTAL, EnemyRarity::RARE);
    addEnemyTemplate(std::move(iceElemental));

    // Earth Elemental
    auto earthElemental = std::make_unique<EnemyTemplate>(
        "Earth Elemental", "A massive creature made of stone and soil", 10, 150, 14, 20, 8, 5,
        EnemyType::ELEMENTAL, EnemyRarity::RARE);
    addEnemyTemplate(std::move(earthElemental));
}

void EnemyDatabase::createDemons() {
    // Imp
    auto imp = std::make_unique<EnemyTemplate>(
        "Imp", "A small demon with a mischievous and malicious nature", 3, 30, 5, 9, 1, 4,
        EnemyType::DEMON, EnemyRarity::UNCOMMON);
    addEnemyTemplate(std::move(imp));

    // Lesser Demon
    auto lesserDemon = std::make_unique<EnemyTemplate>(
        "Lesser Demon", "A fiendish creature from the depths of hell", 7, 85, 12, 18, 3, 6,
        EnemyType::DEMON, EnemyRarity::RARE);
    addEnemyTemplate(std::move(lesserDemon));

    // Demon Lord
    auto demonLord = std::make_unique<EnemyTemplate>(
        "Demon Lord", "A powerful demon commanding legions of the damned", 18, 300, 30, 45, 8, 20,
        EnemyType::DEMON, EnemyRarity::LEGENDARY);
    addEnemyTemplate(std::move(demonLord));
}

void EnemyDatabase::createDragons() {
    // Young Dragon
    auto youngDragon = std::make_unique<EnemyTemplate>(
        "Young Dragon", "A juvenile dragon with developing powers", 12, 180, 20, 30, 6, 8,
        EnemyType::DRAGON, EnemyRarity::EPIC);
    addEnemyTemplate(std::move(youngDragon));

    // Ancient Dragon
    auto ancientDragon = std::make_unique<EnemyTemplate>(
        "Ancient Dragon", "An ancient beast of immense power and wisdom", 25, 500, 40, 60, 12, 25,
        EnemyType::DRAGON, EnemyRarity::LEGENDARY);
    addEnemyTemplate(std::move(ancientDragon));

    // Dragonling
    auto dragonling = std::make_unique<EnemyTemplate>(
        "Dragonling", "A small dragon like creature with limited abilities", 6, 70, 10, 16, 3, 4,
        EnemyType::DRAGON, EnemyRarity::UNCOMMON);
    addEnemyTemplate(std::move(dragonling));
}

void EnemyDatabase::createBosses() {
    // The Shadow King
    auto shadowKing = std::make_unique<EnemyTemplate>(
        "The Shadow King", "A legendary ruler of darkness and despair", 20, 400, 35, 50, 10, 18,
        EnemyType::UNDEAD, EnemyRarity::BOSS);
    addEnemyTemplate(std::move(shadowKing));

    // Inferno Beast
    auto infernoBeast = std::make_unique<EnemyTemplate>(
        "Inferno Beast", "A colossal creature wreathed in eternal flames", 22, 450, 38, 55, 8, 22,
        EnemyType::ELEMENTAL, EnemyRarity::BOSS);
    addEnemyTemplate(std::move(infernoBeast));

    // Goblin King
    auto goblinKing = std::make_unique<EnemyTemplate>(
        "Goblin King", "The supreme ruler of all goblinoid tribes", 15, 250, 25, 35, 6, 8,
        EnemyType::GOBLINOID, EnemyRarity::BOSS);
    addEnemyTemplate(std::move(goblinKing));

    // Void Reaper
    auto voidReaper = std::make_unique<EnemyTemplate>(
        "Void Reaper", "An entity from the space between worlds", 30, 600, 45, 70, 15, 30,
        EnemyType::DEMON, EnemyRarity::BOSS);
    addEnemyTemplate(std::move(voidReaper));
}
