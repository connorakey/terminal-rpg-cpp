//
// Created by Connor on 28/09/2025.
//

#ifndef TERMINAL_RPG_ENEMY_H
#define TERMINAL_RPG_ENEMY_H
#include <string>
class Enemy {
public:
    Enemy(const std::string &name, const std::string &description, int health, int minAttack, int maxAttack, int defence)
        : name(name), description(description), health(health), minAttack(minAttack), maxAttack(maxAttack), defence(defence) {}

    bool isAlive() const { return health > 0; }

    void takeDamage(int damage) {
        int actualDamage = damage - defence;
        if (actualDamage < 0) actualDamage = 0;
        health -= actualDamage;
        if (health < 0) health = 0;
    }

    int attack() const {
        return minAttack + (rand() % (maxAttack - minAttack + 1));
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
};
#endif //TERMINAL_RPG_ENEMY_H