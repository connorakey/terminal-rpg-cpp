#include "enemy.hpp"
#include <random>

Enemy::Enemy(const std::string &name, const std::string &description, int health, int minAttack, int maxAttack, int defence, int resistance)
    : name(name), description(description), health(health), minAttack(minAttack), maxAttack(maxAttack), defence(defence), resistance(resistance) {}

bool Enemy::isAlive() const {
    return health > 0;
}

void Enemy::takeDamage(int damage) {
    int actualDamage = damage - defence;
    if (actualDamage < 0) actualDamage = 0;
    health -= actualDamage;
    if (health < 0) health = 0;
}

void Enemy::takeSpellDamage(int damage) {
    health -= (damage - resistance);
    if (health < 0) health = 0;
}

void Enemy::healEnemy(int amount) {
    health += amount;
}

int Enemy::attack() const {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(minAttack, maxAttack);
    return dist(gen);
}

int Enemy::getHealth() const {
    return health;
}

std::string Enemy::getName() const {
    return name;
}

std::string Enemy::getDescription() const {
    return description;
}

int Enemy::getMinAttack() const {
    return minAttack;
}

int Enemy::getMaxAttack() const {
    return maxAttack;
}

int Enemy::getDefence() const {
    return defence;
}

int Enemy::getResistance() const {
    return resistance;
}

int Enemy::getLevel() const {
    return level;
}

