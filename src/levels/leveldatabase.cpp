//
// Created by Connor on 30/09/2025.
//

#include "leveldatabase.hpp"

#include <algorithm>
#include <cmath>
#include <sstream>

LevelTemplate::LevelTemplate(int level, unsigned int experienceRequired,
                             unsigned int totalExperienceRequired, unsigned int healthBonus,
                             unsigned int staminaBonus, unsigned int defenceBonus,
                             unsigned int resistanceBonus, const std::string& levelTitle,
                             double healthMultiplier, double staminaMultiplier,
                             double defenceMultiplier, double resistanceMultiplier)
    : level(level),
      experienceRequired(experienceRequired),
      totalExperienceRequired(totalExperienceRequired),
      healthBonus(healthBonus),
      staminaBonus(staminaBonus),
      defenceBonus(defenceBonus),
      resistanceBonus(resistanceBonus),
      levelTitle(levelTitle),
      healthMultiplier(healthMultiplier),
      staminaMultiplier(staminaMultiplier),
      defenceMultiplier(defenceMultiplier),
      resistanceMultiplier(resistanceMultiplier) {}

ExperienceReward::ExperienceReward(int baseExp, double levelMult, double rarityMult, int minExp,
                                   int maxExp)
    : baseExperience(baseExp),
      levelDifferenceMultiplier(levelMult),
      rarityMultiplier(rarityMult),
      minExperience(minExp),
      maxExperience(maxExp) {}

LevelDatabase& LevelDatabase::getInstance() {
    static LevelDatabase instance;
    return instance;
}

void LevelDatabase::initialize() {
    createEarlyLevels();
    createMidLevels();
    createHighLevels();
    createEndgameLevels();
}

const LevelTemplate* LevelDatabase::getLevelTemplate(int level) const {
    auto it = levelTemplates.find(level);
    return (it != levelTemplates.end()) ? it->second.get() : nullptr;
}

unsigned int LevelDatabase::getExperienceForLevel(int targetLevel) const {
    const LevelTemplate* template_ptr = getLevelTemplate(targetLevel);
    return template_ptr ? template_ptr->totalExperienceRequired : 0;
}

int LevelDatabase::getLevelFromExperience(unsigned int totalExperience) const {
    int resultLevel = 1;
    for (const auto& pair : levelTemplates) {
        if (totalExperience >= pair.second->totalExperienceRequired) {
            resultLevel = std::max(resultLevel, pair.first);
        }
    }
    return resultLevel;
}

unsigned int LevelDatabase::getExperienceForNextLevel(int currentLevel) const {
    if (currentLevel >= getMaxLevel()) {
        return 0;  // Already at max level
    }

    const LevelTemplate* nextLevel = getLevelTemplate(currentLevel + 1);
    return nextLevel ? nextLevel->totalExperienceRequired : 0;
}

int LevelDatabase::calculateExperienceReward(int playerLevel, int enemyLevel, bool isBoss) const {
    // Base experience calculation
    int baseExp = experienceRewards.baseExperience;

    double levelDiff = static_cast<double>(enemyLevel - playerLevel);
    double levelModifier = 1.0 + (levelDiff * 0.15);  // 15% per level difference
    levelModifier = std::max(0.25, levelModifier);    // Minimum 25% exp
    levelModifier = std::min(3.0, levelModifier);     // Maximum 300% exp

    double enemyLevelModifier = 1.0 + (enemyLevel * 0.1);

    double bossModifier = isBoss ? 5.0 : 1.0;

    int finalExp = static_cast<int>(baseExp * levelModifier * enemyLevelModifier * bossModifier);

    finalExp = std::max(experienceRewards.minExperience, finalExp);
    finalExp = std::min(experienceRewards.maxExperience, finalExp);

    return finalExp;
}

std::string LevelDatabase::getLevelProgressionInfo(int currentLevel,
                                                   unsigned int currentExp) const {
    std::ostringstream info;
    const LevelTemplate* currentTemplate = getLevelTemplate(currentLevel);

    if (!currentTemplate) {
        return "Invalid level data";
    }

    info << "Level " << currentLevel << ": " << currentTemplate->levelTitle << "\n";

    unsigned int nextLevelExp = getExperienceForNextLevel(currentLevel);
    if (nextLevelExp > 0) {
        unsigned int expNeeded = nextLevelExp - currentExp;
        info << "Experience: " << currentExp << " / " << nextLevelExp << "\n";
        info << "Experience needed for next level: " << expNeeded;
    } else {
        info << "Experience: " << currentExp << " (MAX LEVEL)";
    }

    return info.str();
}

int LevelDatabase::checkLevelUp(unsigned int currentExperience, int currentLevel) const {
    int newLevel = getLevelFromExperience(currentExperience);
    return std::max(newLevel, currentLevel);  // Never level down
}

void LevelDatabase::getStatBonuses(int level, unsigned int& healthBonus, unsigned int& staminaBonus,
                                   unsigned int& defenceBonus,
                                   unsigned int& resistanceBonus) const {
    healthBonus = 0;
    staminaBonus = 0;
    defenceBonus = 0;
    resistanceBonus = 0;

    // Collect bonuses from level 1 to current level
    for (int i = 1; i <= level; ++i) {
        const LevelTemplate* template_ptr = getLevelTemplate(i);
        if (template_ptr) {
            healthBonus += template_ptr->healthBonus;
            staminaBonus += template_ptr->staminaBonus;
            defenceBonus += template_ptr->defenceBonus;
            resistanceBonus += template_ptr->resistanceBonus;
        }
    }
}

std::string LevelDatabase::getLevelTitle(int level) const {
    const LevelTemplate* template_ptr = getLevelTemplate(level);
    return template_ptr ? template_ptr->levelTitle : "Unknown";
}

int LevelDatabase::getMaxLevel() const {
    int maxLevel = 1;
    for (const auto& pair : levelTemplates) {
        maxLevel = std::max(maxLevel, pair.first);
    }
    return maxLevel;
}

void LevelDatabase::addLevelTemplate(std::unique_ptr<LevelTemplate> levelTemplate) {
    levelTemplates[levelTemplate->level] = std::move(levelTemplate);
}

unsigned int LevelDatabase::calculateExperienceRequirement(int level) const {
    if (level <= 1) return 0;

    // Formula: base * (level^1.8) + (level * 50)
    double base = 100.0;
    double exponential = std::pow(static_cast<double>(level), 1.8);
    double linear = static_cast<double>(level) * 50.0;

    return static_cast<unsigned int>(base * exponential + linear);
}

void LevelDatabase::createEarlyLevels() {
    auto level1 = std::make_unique<LevelTemplate>(1, 0, 0, 0, 0, 0, 0, "Novice Adventurer", 1.0,
                                                  1.0, 1.0, 1.0);
    addLevelTemplate(std::move(level1));

    auto level2 =
        std::make_unique<LevelTemplate>(2, 100, 100, 15, 5, 1, 0, "Apprentice", 1.0, 1.0, 1.0, 1.0);
    addLevelTemplate(std::move(level2));

    auto level3 =
        std::make_unique<LevelTemplate>(3, 150, 250, 20, 8, 1, 1, "Trainee", 1.0, 1.0, 1.0, 1.0);
    addLevelTemplate(std::move(level3));

    auto level4 =
        std::make_unique<LevelTemplate>(4, 200, 450, 25, 10, 2, 1, "Scout", 1.0, 1.0, 1.0, 1.0);
    addLevelTemplate(std::move(level4));

    auto level5 =
        std::make_unique<LevelTemplate>(5, 300, 750, 30, 12, 2, 1, "Warrior", 1.1, 1.0, 1.0, 1.0);
    addLevelTemplate(std::move(level5));

    auto level6 =
        std::make_unique<LevelTemplate>(6, 400, 1150, 35, 15, 3, 2, "Veteran", 1.1, 1.0, 1.0, 1.0);
    addLevelTemplate(std::move(level6));

    auto level7 = std::make_unique<LevelTemplate>(7, 500, 1650, 40, 18, 3, 2, "Skilled Fighter",
                                                  1.1, 1.1, 1.0, 1.0);
    addLevelTemplate(std::move(level7));

    auto level8 = std::make_unique<LevelTemplate>(8, 650, 2300, 45, 20, 4, 3, "Seasoned Warrior",
                                                  1.1, 1.1, 1.0, 1.0);
    addLevelTemplate(std::move(level8));

    auto level9 = std::make_unique<LevelTemplate>(9, 800, 3100, 50, 25, 4, 3, "Veteran Warrior",
                                                  1.1, 1.1, 1.1, 1.0);
    addLevelTemplate(std::move(level9));

    auto level10 = std::make_unique<LevelTemplate>(10, 1000, 4100, 60, 30, 5, 4, "Elite Warrior",
                                                   1.2, 1.1, 1.1, 1.1);
    addLevelTemplate(std::move(level10));
}

void LevelDatabase::createMidLevels() {
    for (int level = 11; level <= 25; ++level) {
        unsigned int expRequired = calculateExperienceRequirement(level);
        unsigned int totalExp =
            (level > 1) ? getLevelTemplate(level - 1)->totalExperienceRequired + expRequired
                        : expRequired;

        unsigned int healthBonus = 40 + (level - 10) * 8;
        unsigned int staminaBonus = 20 + (level - 10) * 4;
        unsigned int defenceBonus = 3 + (level - 10) / 3;
        unsigned int resistanceBonus = 2 + (level - 10) / 4;

        std::string title;
        if (level <= 15) {
            title = "Champion";
        } else if (level <= 20) {
            title = "Hero";
        } else {
            title = "Legendary Hero";
        }

        double healthMult = 1.1 + (level - 10) * 0.02;
        double staminaMult = 1.1 + (level - 10) * 0.015;
        double defenceMult = 1.1 + (level - 10) * 0.01;
        double resistanceMult = 1.1 + (level - 10) * 0.01;

        auto levelTemplate = std::make_unique<LevelTemplate>(
            level, expRequired, totalExp, healthBonus, staminaBonus, defenceBonus, resistanceBonus,
            title, healthMult, staminaMult, defenceMult, resistanceMult);
        addLevelTemplate(std::move(levelTemplate));
    }
}

void LevelDatabase::createHighLevels() {
    for (int level = 26; level <= 50; ++level) {
        unsigned int expRequired = calculateExperienceRequirement(level);
        unsigned int totalExp = getLevelTemplate(level - 1)->totalExperienceRequired + expRequired;

        // Higher stat bonuses for high levels
        unsigned int healthBonus = 60 + (level - 25) * 12;
        unsigned int staminaBonus = 30 + (level - 25) * 6;
        unsigned int defenceBonus = 5 + (level - 25) / 2;
        unsigned int resistanceBonus = 4 + (level - 25) / 3;

        std::string title;
        if (level <= 35) {
            title = "Paragon";
        } else if (level <= 45) {
            title = "Ascendant";
        } else {
            title = "Demigod";
        }

        double healthMult = 1.3 + (level - 25) * 0.025;
        double staminaMult = 1.2 + (level - 25) * 0.02;
        double defenceMult = 1.2 + (level - 25) * 0.015;
        double resistanceMult = 1.2 + (level - 25) * 0.015;

        auto levelTemplate = std::make_unique<LevelTemplate>(
            level, expRequired, totalExp, healthBonus, staminaBonus, defenceBonus, resistanceBonus,
            title, healthMult, staminaMult, defenceMult, resistanceMult);
        addLevelTemplate(std::move(levelTemplate));
    }
}

void LevelDatabase::createEndgameLevels() {
    for (int level = 51; level <= 100; ++level) {
        unsigned int expRequired = calculateExperienceRequirement(level);
        unsigned int totalExp = getLevelTemplate(level - 1)->totalExperienceRequired + expRequired;

        // Massive stat bonuses for endgame
        unsigned int healthBonus = 100 + (level - 50) * 20;
        unsigned int staminaBonus = 50 + (level - 50) * 10;
        unsigned int defenceBonus = 8 + (level - 50) / 2;
        unsigned int resistanceBonus = 6 + (level - 50) / 2;

        std::string title;
        if (level <= 70) {
            title = "Beyond God";
        } else if (level <= 90) {
            title = "Godslayer";
        } else if (level < 100) {
            title = "Supreme Being";
        } else {
            title = "The One";
        }

        double healthMult = 1.5 + (level - 50) * 0.03;
        double staminaMult = 1.4 + (level - 50) * 0.025;
        double defenceMult = 1.3 + (level - 50) * 0.02;
        double resistanceMult = 1.3 + (level - 50) * 0.02;

        auto levelTemplate = std::make_unique<LevelTemplate>(
            level, expRequired, totalExp, healthBonus, staminaBonus, defenceBonus, resistanceBonus,
            title, healthMult, staminaMult, defenceMult, resistanceMult);
        addLevelTemplate(std::move(levelTemplate));
    }
}
