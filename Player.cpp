#include "Player.h"

Player::Player()
{
}

int Player::getPosX()
{
    return this->x;
}

int Player::getPosY()
{
    return this->y;
}

int Player::getLifes()
{
    return this->lifes;
}

void Player::setPos(int x, int y)
{
    this->x = x;
    this->y = y;
}

void Player::demage()
{
    this->lifes--;
    if (this->lifes == 0)
    {
        this->isAlive = false;
    }
    
}

bool Player::getAlive()
{
    return this->isAlive;
}

Player::~Player()
{
}
