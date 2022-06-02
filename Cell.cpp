#include "Cell.h"
#include <iostream>

Cell::Cell(/* args */)
{
}

Cell::Cell(int x, int y)
{
    this->x = x;
    this->y = y;
    this->setNew();
}

void Cell::setWall()
{
    if (this->isWall == false)
    {
        std::cout << "setWall " << x << y << std::endl;
        this->isWall = true;
        this->isField = false;
        this->isNew = false;
    }
    
}

void Cell::setField()
{
    if (this->isField == false)
    {
        std::cout << "setField " << x << y << " Steps " << steps_from_start << " " << previus << std::endl;
        this->isField = true;
        this->isWall = false;
        this->isNew = false;
    }
    
}

void Cell::setNew()
{
    this->isField = false;
    this->isWall = false;
    this->isNew = true;
    this->isTarget = false;
    this->steps_from_start = -1;
    this->previus = nullptr;
}

bool Cell::getWall()
{
    return this->isWall;
}

bool Cell::getField()
{
    return this->isField;
}

bool Cell::getNew()
{
    return this->isNew;
}


int Cell::getPositionX()
{
    return this->x;
}

int Cell::getPositionY()
{
    return this->y;
}

int Cell::getSteps()
{
    return this->steps_from_start;
}

Cell *Cell::getCurrent()
{
    return this;
}


Cell *Cell::getPrevius()
{
    return this->previus;
}


void Cell::setStepsFromStart(int steps)
{
    if (this->steps_from_start < 0)
    {
        this->steps_from_start = steps;
    }
    
}

void Cell::setTarget()
{
    this->isTarget = true;
}

bool Cell::getTarget()
{
    return this->isTarget;
}

void Cell::setPrevius(Cell* previus)
{
    if (this->previus == nullptr)
    {
        this->previus=previus;
    }
    
}

Cell::~Cell()
{
}
