//
// Created by Connor on 30/09/2025.
//

#ifndef TERMINAL_RPG_LEVELDATABASE_HPP
#define TERMINAL_RPG_LEVELDATABASE_HPP

#include <vector>
#include <memory>
#include <map>
#include <string>

struct LevelTemplate {
    int level;
    unsigned int experienceRequired;
    unsigned int totalExperienceRequired; // Cumulative experience from level 1
    unsigned int healthBonus;
    unsigned int staminaBonus;
    unsigned int defenceBonus;
    unsigned int resistanceBonus;
    std::string levelTitle;

    // Stat multipliers for scaling
    double healthMultiplier;
    double staminaMultiplier;
    double defenceMultiplier;
    double resistanceMultiplier;

    LevelTemplate(int level, unsigned int experienceRequired, unsigned int totalExperienceRequired,
                 unsigned int healthBonus, unsigned int staminaBonus, unsigned int defenceBonus,
                 unsigned int resistanceBonus, const std::string& levelTitle,
                 double healthMultiplier = 1.0,
                 double staminaMultiplier = 1.0, double defenceMultiplier = 1.0,
                 double resistanceMultiplier = 1.0);
};

struct ExperienceReward {
    int baseExperience;
    double levelDifferenceMultiplier;
    double rarityMultiplier;
    int minExperience;
    int maxExperience;

    // Default constructor
    ExperienceReward() : baseExperience(20), levelDifferenceMultiplier(1.0),
                        rarityMultiplier(1.0), minExperience(1), maxExperience(1000) {}

    ExperienceReward(int baseExp, double levelMult = 1.0, double rarityMult = 1.0,
                    int minExp = 1, int maxExp = 1000);
};

class LevelDatabase {
public:
    static LevelDatabase& getInstance();

    void initialize();

    // Get level template by level number (returns nullptr if not found)
    const LevelTemplate* getLevelTemplate(int level) const;

    // Calculate experience required to reach a specific level
    unsigned int getExperienceForLevel(int targetLevel) const;

    // Calculate what level a player should be based on total experience
    int getLevelFromExperience(unsigned int totalExperience) const;

    // Calculate experience needed for next level
    unsigned int getExperienceForNextLevel(int currentLevel) const;

    // Calculate experience reward for defeating an enemy
    int calculateExperienceReward(int playerLevel, int enemyLevel, bool isBoss = false) const;

    // Get level progression info
    std::string getLevelProgressionInfo(int currentLevel, unsigned int currentExp) const;

    // Check if player can level up and return new level
    int checkLevelUp(unsigned int currentExperience, int currentLevel) const;

    // Get stat bonuses for a specific level
    void getStatBonuses(int level, unsigned int& healthBonus, unsigned int& staminaBonus,
                       unsigned int& defenceBonus, unsigned int& resistanceBonus) const;

    // Get level title for a specific level
    std::string getLevelTitle(int level) const;

    // Get maximum level
    int getMaxLevel() const;

private:
    LevelDatabase() = default;
    ~LevelDatabase() = default;
    LevelDatabase(const LevelDatabase&) = delete;
    LevelDatabase& operator=(const LevelDatabase&) = delete;

    std::map<int, std::unique_ptr<LevelTemplate>> levelTemplates;
    ExperienceReward experienceRewards;

    // Helper methods to create level data
    void createEarlyLevels();      // Levels 1-10
    void createMidLevels();        // Levels 11-25
    void createHighLevels();       // Levels 26-50
    void createEndgameLevels();    // Levels 51-100

    // Helper to add level template
    void addLevelTemplate(std::unique_ptr<LevelTemplate> levelTemplate);

    // Helper to calculate balanced experience requirements
    unsigned int calculateExperienceRequirement(int level) const;
};

#endif //TERMINAL_RPG_LEVELDATABASE_HPP
