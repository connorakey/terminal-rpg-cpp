//
// Created by Connor on 28/09/2025.
//

#ifndef TERMINAL_RPG_ENEMY_H
#define TERMINAL_RPG_ENEMY_H
#include <string>
#include <random>

class Enemy {
public:
    Enemy(const std::string &name, const std::string &description, int level, int health, int minAttack, int maxAttack, int defence, int resistance);

    bool isAlive() const;
    void takeDamage(int damage);
    void takeSpellDamage(int damage);
    void healEnemy(int amount);
    int attack() const;

    int getHealth() const;
    std::string getName() const;
    std::string getDescription() const;
    int getMinAttack() const;
    int getMaxAttack() const;
    int getDefence() const;
    int getResistance() const;
    int getLevel() const;

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