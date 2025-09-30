//
// Created by Connor on 30/09/2025.
//

#ifndef TERMINAL_RPG_ENEMYDATABASE_HPP
#define TERMINAL_RPG_ENEMYDATABASE_HPP

#include <vector>
#include <memory>
#include <map>
#include <string>
#include "enemy.hpp"

struct EnemyTemplate {
    std::string name;
    std::string description;
    int level;
    int health;
    int minAttack;
    int maxAttack;
    int defence;
    int resistance;
    EnemyType type;
    EnemyRarity rarity;

    EnemyTemplate(const std::string& name, const std::string& description,
                 int level, int health, int minAttack, int maxAttack,
                 int defence, int resistance, EnemyType type, EnemyRarity rarity);
};

class EnemyDatabase {
public:
    static EnemyDatabase& getInstance();

    // Initialize the database with all predefined enemies
    void initialize();

    // Get enemy template by name (returns nullptr if not found)
    const EnemyTemplate* getEnemyTemplate(const std::string& enemyName) const;

    // Create a new Enemy instance from template
    Enemy* createEnemy(const std::string& enemyName) const;

    // Get all enemy names
    std::vector<std::string> getAllEnemyNames() const;

    // Get enemies by type
    std::vector<std::string> getEnemiesByType(EnemyType type) const;

    // Get enemies by rarity
    std::vector<std::string> getEnemiesByRarity(EnemyRarity rarity) const;

    // Get enemies by level range
    std::vector<std::string> getEnemiesByLevelRange(int minLevel, int maxLevel) const;

    // Get random enemy by level range
    std::string getRandomEnemyByLevel(int minLevel, int maxLevel) const;

private:
    EnemyDatabase() = default;
    ~EnemyDatabase() = default;
    EnemyDatabase(const EnemyDatabase&) = delete;
    EnemyDatabase& operator=(const EnemyDatabase&) = delete;

    std::map<std::string, std::unique_ptr<EnemyTemplate>> enemyTemplates;

    // Helper methods to create specific enemy types
    void createBeasts();
    void createUndead();
    void createHumanoids();
    void createDragons();
    void createElementals();
    void createDemons();
    void createGoblinoids();
    void createBosses();

    // Helper to add enemy template
    void addEnemyTemplate(std::unique_ptr<EnemyTemplate> enemyTemplate);
};

#endif //TERMINAL_RPG_ENEMYDATABASE_HPP
