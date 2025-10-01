#include <catch2/catch_test_macros.hpp>
#include "../src/levels/leveldatabase.hpp"
#include <climits>

TEST_CASE("LevelDatabase singleton pattern works correctly", "[LevelDatabase]") {
    LevelDatabase& db1 = LevelDatabase::getInstance();
    LevelDatabase& db2 = LevelDatabase::getInstance();

    REQUIRE(&db1 == &db2); // Same instance
}

TEST_CASE("LevelDatabase initializes and retrieves level templates correctly", "[LevelDatabase]") {
    LevelDatabase& db = LevelDatabase::getInstance();
    db.initialize();

    // Test early levels
    const LevelTemplate* level1 = db.getLevelTemplate(1);
    REQUIRE(level1 != nullptr);
    REQUIRE(level1->level == 1);
    REQUIRE(level1->experienceRequired == 0);
    REQUIRE(level1->totalExperienceRequired == 0);
    REQUIRE(level1->healthBonus == 0);
    REQUIRE(level1->staminaBonus == 0);
    REQUIRE(level1->defenceBonus == 0);
    REQUIRE(level1->resistanceBonus == 0);
    REQUIRE(level1->levelTitle == "Novice Adventurer");

    const LevelTemplate* level2 = db.getLevelTemplate(2);
    REQUIRE(level2 != nullptr);
    REQUIRE(level2->level == 2);
    REQUIRE(level2->experienceRequired == 100);
    REQUIRE(level2->totalExperienceRequired == 100);
    REQUIRE(level2->healthBonus == 15);
    REQUIRE(level2->staminaBonus == 5);
    REQUIRE(level2->defenceBonus == 1);
    REQUIRE(level2->resistanceBonus == 0);
    REQUIRE(level2->levelTitle == "Apprentice");

    const LevelTemplate* level10 = db.getLevelTemplate(10);
    REQUIRE(level10 != nullptr);
    REQUIRE(level10->level == 10);
    REQUIRE(level10->experienceRequired == 1000);
    REQUIRE(level10->totalExperienceRequired == 4100);
    REQUIRE(level10->healthBonus == 60);
    REQUIRE(level10->staminaBonus == 30);
    REQUIRE(level10->defenceBonus == 5);
    REQUIRE(level10->resistanceBonus == 4);
    REQUIRE(level10->levelTitle == "Elite Warrior");

    // Test mid levels (generated)
    const LevelTemplate* level15 = db.getLevelTemplate(15);
    REQUIRE(level15 != nullptr);
    REQUIRE(level15->level == 15);
    REQUIRE(level15->levelTitle == "Champion");

    const LevelTemplate* level20 = db.getLevelTemplate(20);
    REQUIRE(level20 != nullptr);
    REQUIRE(level20->level == 20);
    REQUIRE(level20->levelTitle == "Hero");

    const LevelTemplate* level25 = db.getLevelTemplate(25);
    REQUIRE(level25 != nullptr);
    REQUIRE(level25->level == 25);
    REQUIRE(level25->levelTitle == "Legendary Hero");

    // Test high levels
    const LevelTemplate* level35 = db.getLevelTemplate(35);
    REQUIRE(level35 != nullptr);
    REQUIRE(level35->level == 35);
    REQUIRE(level35->levelTitle == "Paragon");

    const LevelTemplate* level45 = db.getLevelTemplate(45);
    REQUIRE(level45 != nullptr);
    REQUIRE(level45->level == 45);
    REQUIRE(level45->levelTitle == "Ascendant");

    // Test endgame levels
    const LevelTemplate* level70 = db.getLevelTemplate(70);
    REQUIRE(level70 != nullptr);
    REQUIRE(level70->level == 70);
    REQUIRE(level70->levelTitle == "Beyond God");

    const LevelTemplate* level90 = db.getLevelTemplate(90);
    REQUIRE(level90 != nullptr);
    REQUIRE(level90->level == 90);
    REQUIRE(level90->levelTitle == "Godslayer");

    const LevelTemplate* level99 = db.getLevelTemplate(99);
    REQUIRE(level99 != nullptr);
    REQUIRE(level99->level == 99);
    REQUIRE(level99->levelTitle == "Supreme Being");

    const LevelTemplate* level100 = db.getLevelTemplate(100);
    REQUIRE(level100 != nullptr);
    REQUIRE(level100->level == 100);
    REQUIRE(level100->levelTitle == "The One");
}

TEST_CASE("LevelDatabase handles invalid level requests", "[LevelDatabase]") {
    LevelDatabase& db = LevelDatabase::getInstance();
    db.initialize();

    // Test invalid levels
    REQUIRE(db.getLevelTemplate(0) == nullptr);
    REQUIRE(db.getLevelTemplate(-1) == nullptr);
    REQUIRE(db.getLevelTemplate(101) == nullptr);
    REQUIRE(db.getLevelTemplate(999) == nullptr);
}

TEST_CASE("LevelDatabase getExperienceForLevel works correctly", "[LevelDatabase]") {
    LevelDatabase& db = LevelDatabase::getInstance();
    db.initialize();

    REQUIRE(db.getExperienceForLevel(1) == 0);
    REQUIRE(db.getExperienceForLevel(2) == 100);
    REQUIRE(db.getExperienceForLevel(10) == 4100);
    REQUIRE(db.getExperienceForLevel(0) == 0); // Invalid level
    REQUIRE(db.getExperienceForLevel(101) == 0); // Invalid level
}

TEST_CASE("LevelDatabase getLevelFromExperience works correctly", "[LevelDatabase]") {
    LevelDatabase& db = LevelDatabase::getInstance();
    db.initialize();

    REQUIRE(db.getLevelFromExperience(0) == 1);
    REQUIRE(db.getLevelFromExperience(50) == 1);
    REQUIRE(db.getLevelFromExperience(100) == 2);
    REQUIRE(db.getLevelFromExperience(250) == 3);
    REQUIRE(db.getLevelFromExperience(4100) == 10);
    // The experience requirements grow exponentially
    REQUIRE(db.getLevelFromExperience(10000000) >= 85); // Should reach a high level (actually reaches 87)
    REQUIRE(db.getLevelFromExperience(UINT_MAX) == 100); // Maximum possible experience should reach max level
}

TEST_CASE("LevelDatabase getExperienceForNextLevel works correctly", "[LevelDatabase]") {
    LevelDatabase& db = LevelDatabase::getInstance();
    db.initialize();

    REQUIRE(db.getExperienceForNextLevel(1) == 100);
    REQUIRE(db.getExperienceForNextLevel(2) == 250);
    REQUIRE(db.getExperienceForNextLevel(9) == 4100);
    REQUIRE(db.getExperienceForNextLevel(100) == 0); // Max level
    REQUIRE(db.getExperienceForNextLevel(101) == 0); // Above max level
}

TEST_CASE("LevelDatabase calculateExperienceReward works correctly", "[LevelDatabase]") {
    LevelDatabase& db = LevelDatabase::getInstance();
    db.initialize();

    // Base case: same level enemy
    int baseReward = db.calculateExperienceReward(5, 5, false);
    REQUIRE(baseReward > 0);

    // Higher level enemy should give more experience
    int higherReward = db.calculateExperienceReward(5, 10, false);
    REQUIRE(higherReward > baseReward);

    // Lower level enemy should give less experience
    int lowerReward = db.calculateExperienceReward(10, 5, false);
    REQUIRE(lowerReward < baseReward);

    // Boss should give 5x more experience
    int bossReward = db.calculateExperienceReward(5, 5, true);
    REQUIRE(bossReward > baseReward * 4); // Should be roughly 5x

    // Test extreme cases
    int veryLowReward = db.calculateExperienceReward(50, 1, false);
    REQUIRE(veryLowReward >= 1); // Should be at minimum

    int veryHighReward = db.calculateExperienceReward(1, 50, false);
    REQUIRE(veryHighReward <= 1000); // Should be capped at maximum
}

TEST_CASE("LevelDatabase getLevelProgressionInfo works correctly", "[LevelDatabase]") {
    LevelDatabase& db = LevelDatabase::getInstance();
    db.initialize();

    // Test normal level progression
    std::string info = db.getLevelProgressionInfo(1, 50);
    REQUIRE(info.find("Level 1: Novice Adventurer") != std::string::npos);
    REQUIRE(info.find("Experience: 50 / 100") != std::string::npos);
    REQUIRE(info.find("Experience needed for next level: 50") != std::string::npos);

    // Test max level
    std::string maxInfo = db.getLevelProgressionInfo(100, 999999);
    REQUIRE(maxInfo.find("Level 100: The One") != std::string::npos);
    REQUIRE(maxInfo.find("(MAX LEVEL)") != std::string::npos);

    // Test invalid level
    std::string invalidInfo = db.getLevelProgressionInfo(0, 0);
    REQUIRE(invalidInfo == "Invalid level data");
}

TEST_CASE("LevelDatabase checkLevelUp works correctly", "[LevelDatabase]") {
    LevelDatabase& db = LevelDatabase::getInstance();
    db.initialize();

    // No level up
    REQUIRE(db.checkLevelUp(50, 1) == 1);

    // Level up from 1 to 2
    REQUIRE(db.checkLevelUp(100, 1) == 2);

    // Level up from 1 to 3
    REQUIRE(db.checkLevelUp(250, 1) == 3);

    // Multiple level ups
    REQUIRE(db.checkLevelUp(4100, 1) == 10);

    // Never level down
    REQUIRE(db.checkLevelUp(0, 5) == 5);
}

TEST_CASE("LevelDatabase getStatBonuses works correctly", "[LevelDatabase]") {
    LevelDatabase& db = LevelDatabase::getInstance();
    db.initialize();

    unsigned int health, stamina, defence, resistance;

    // Level 1 should have no bonuses
    db.getStatBonuses(1, health, stamina, defence, resistance);
    REQUIRE(health == 0);
    REQUIRE(stamina == 0);
    REQUIRE(defence == 0);
    REQUIRE(resistance == 0);

    // Level 2 should have level 2 bonuses only
    db.getStatBonuses(2, health, stamina, defence, resistance);
    REQUIRE(health == 15);
    REQUIRE(stamina == 5);
    REQUIRE(defence == 1);
    REQUIRE(resistance == 0);

    // Level 3 should have cumulative bonuses from levels 2 and 3
    db.getStatBonuses(3, health, stamina, defence, resistance);
    REQUIRE(health == 35); // 15 + 20
    REQUIRE(stamina == 13); // 5 + 8
    REQUIRE(defence == 2); // 1 + 1
    REQUIRE(resistance == 1); // 0 + 1

    // Test invalid level
    db.getStatBonuses(0, health, stamina, defence, resistance);
    REQUIRE(health == 0);
    REQUIRE(stamina == 0);
    REQUIRE(defence == 0);
    REQUIRE(resistance == 0);
}

TEST_CASE("LevelDatabase getLevelTitle works correctly", "[LevelDatabase]") {
    LevelDatabase& db = LevelDatabase::getInstance();
    db.initialize();

    REQUIRE(db.getLevelTitle(1) == "Novice Adventurer");
    REQUIRE(db.getLevelTitle(2) == "Apprentice");
    REQUIRE(db.getLevelTitle(10) == "Elite Warrior");
    REQUIRE(db.getLevelTitle(15) == "Champion");
    REQUIRE(db.getLevelTitle(20) == "Hero");
    REQUIRE(db.getLevelTitle(25) == "Legendary Hero");
    REQUIRE(db.getLevelTitle(35) == "Paragon");
    REQUIRE(db.getLevelTitle(45) == "Ascendant");
    REQUIRE(db.getLevelTitle(70) == "Beyond God");
    REQUIRE(db.getLevelTitle(90) == "Godslayer");
    REQUIRE(db.getLevelTitle(99) == "Supreme Being");
    REQUIRE(db.getLevelTitle(100) == "The One");
    REQUIRE(db.getLevelTitle(0) == "Unknown");
    REQUIRE(db.getLevelTitle(101) == "Unknown");
}

TEST_CASE("LevelDatabase getMaxLevel works correctly", "[LevelDatabase]") {
    LevelDatabase& db = LevelDatabase::getInstance();
    db.initialize();

    REQUIRE(db.getMaxLevel() == 100);
}

TEST_CASE("LevelTemplate constructor works correctly", "[LevelTemplate]") {
    LevelTemplate template1(5, 300, 750, 30, 12, 2, 1, "Warrior");

    REQUIRE(template1.level == 5);
    REQUIRE(template1.experienceRequired == 300);
    REQUIRE(template1.totalExperienceRequired == 750);
    REQUIRE(template1.healthBonus == 30);
    REQUIRE(template1.staminaBonus == 12);
    REQUIRE(template1.defenceBonus == 2);
    REQUIRE(template1.resistanceBonus == 1);
    REQUIRE(template1.levelTitle == "Warrior");
    REQUIRE(template1.healthMultiplier == 1.0);
    REQUIRE(template1.staminaMultiplier == 1.0);
    REQUIRE(template1.defenceMultiplier == 1.0);
    REQUIRE(template1.resistanceMultiplier == 1.0);

    // Test with custom multipliers
    LevelTemplate template2(10, 1000, 4100, 60, 30, 5, 4, "Elite Warrior",
                           1.2, 1.1, 1.1, 1.1);

    REQUIRE(template2.healthMultiplier == 1.2);
    REQUIRE(template2.staminaMultiplier == 1.1);
    REQUIRE(template2.defenceMultiplier == 1.1);
    REQUIRE(template2.resistanceMultiplier == 1.1);
}

TEST_CASE("ExperienceReward constructors work correctly", "[ExperienceReward]") {
    // Test default constructor
    ExperienceReward defaultReward;
    REQUIRE(defaultReward.baseExperience == 20);
    REQUIRE(defaultReward.levelDifferenceMultiplier == 1.0);
    REQUIRE(defaultReward.rarityMultiplier == 1.0);
    REQUIRE(defaultReward.minExperience == 1);
    REQUIRE(defaultReward.maxExperience == 1000);

    // Test parameterized constructor
    ExperienceReward customReward(50, 1.5, 2.0, 5, 2000);
    REQUIRE(customReward.baseExperience == 50);
    REQUIRE(customReward.levelDifferenceMultiplier == 1.5);
    REQUIRE(customReward.rarityMultiplier == 2.0);
    REQUIRE(customReward.minExperience == 5);
    REQUIRE(customReward.maxExperience == 2000);

    // Test parameterized constructor with defaults
    ExperienceReward partialReward(30);
    REQUIRE(partialReward.baseExperience == 30);
    REQUIRE(partialReward.levelDifferenceMultiplier == 1.0);
    REQUIRE(partialReward.rarityMultiplier == 1.0);
    REQUIRE(partialReward.minExperience == 1);
    REQUIRE(partialReward.maxExperience == 1000);
}

TEST_CASE("LevelDatabase experience calculation boundary conditions", "[LevelDatabase]") {
    LevelDatabase& db = LevelDatabase::getInstance();
    db.initialize();

    // Test experience reward minimum and maximum bounds
    int minReward = db.calculateExperienceReward(100, 1, false);
    REQUIRE(minReward >= 1); // Should be at minimum

    int maxReward = db.calculateExperienceReward(1, 100, true);
    REQUIRE(maxReward <= 1000); // Should be at maximum

    // Test level difference multiplier extremes
    int negativeDiff = db.calculateExperienceReward(50, 10, false);
    REQUIRE(negativeDiff > 0); // Should still be positive

    int positiveDiff = db.calculateExperienceReward(10, 50, false);
    REQUIRE(positiveDiff > negativeDiff); // Higher level enemy gives more exp
}

TEST_CASE("LevelDatabase handles edge cases correctly", "[LevelDatabase]") {
    LevelDatabase& db = LevelDatabase::getInstance();
    db.initialize();

    // Test very high experience values
    int levelFromHighExp = db.getLevelFromExperience(UINT_MAX);
    REQUIRE(levelFromHighExp == 100); // Should cap at max level

    // Test stat bonuses for high levels
    unsigned int health, stamina, defence, resistance;
    db.getStatBonuses(100, health, stamina, defence, resistance);
    REQUIRE(health > 0);
    REQUIRE(stamina > 0);
    REQUIRE(defence > 0);
    REQUIRE(resistance > 0);

    // Test progression info for edge levels
    std::string progressInfo = db.getLevelProgressionInfo(99, 999999);
    REQUIRE(progressInfo.find("Level 99") != std::string::npos);
    REQUIRE(progressInfo.find("Supreme Being") != std::string::npos);
}