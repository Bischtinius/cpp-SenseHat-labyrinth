#pragma once

class Cell
{
private:
    int x = -1;
    int y = -1;
    bool isWall = false;
    bool isField = false;
    bool isNew = true;
    bool isTarget = false;
    Cell* previus = nullptr;
    int steps_from_start = -1;

public:
    Cell();
    Cell(int x, int y);
    void setWall();
    void setField();
    void setNew();
    void setTarget();
    void setPrevius(Cell* previus);
    void setStepsFromStart(int steps);
    bool getWall();
    bool getField();
    bool getNew();
    bool getTarget();
    int getPositionX();
    int getPositionY();
    int getSteps();
    Cell *getPrevius();
    Cell *getCurrent();
    virtual ~Cell();
};
