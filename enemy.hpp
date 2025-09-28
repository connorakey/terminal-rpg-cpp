//
// Created by Connor on 28/09/2025.
//

#ifndef TERMINAL_RPG_ENEMY_H
#define TERMINAL_RPG_ENEMY_H
#include <string>
#include <random>

class Enemy {
public:
    Enemy(const std::string &name, const std::string &description, int health, int minAttack, int maxAttack, int defence, int resistance)
        : name(name), description(description), health(health), minAttack(minAttack), maxAttack(maxAttack), defence(defence), resistance(resistance) {}

    bool isAlive() const { return health > 0; }

    void takeDamage(int damage) {
        int actualDamage = damage - defence;
        if (actualDamage < 0) actualDamage = 0;
        health -= actualDamage;
        if (health < 0) health = 0;
    }

    void takeSpellDamage(int damage) {
        health -= (damage - resistance);
        if (health < 0) health = 0;
    }

    void healEnemy(int amount) {
        health += amount;
    }

    int attack() const {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(minAttack, maxAttack);
        return dist(gen);
    }

    int getHealth() const { return health; }
    std::string getName() const { return name; }
    std::string getDescription() const { return description; }
    int getMinAttack() const { return minAttack; }
    int getMaxAttack() const { return maxAttack; }
    int getDefence() const { return defence; }

private:
    std::string name;
    std::string description;
    int health;
    int minAttack;
    int maxAttack;
    int defence;
    int resistance;
};
#endif //TERMINAL_RPG_ENEMY_H