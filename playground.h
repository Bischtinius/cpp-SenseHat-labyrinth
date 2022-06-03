#pragma once
#include "Cell.h"
#include "Player.h"
#include <iostream>

class playground
{
private:
    int nx = -1;
    int ny = -1;
    int ix = 0;
    int iy = 0;
    bool level_finished = false;
    Cell** map;
    Cell* taget = nullptr;
    Cell* position = nullptr;
    Player* player = new Player();

public:
    playground();
    playground(int nx, int ny);
    void setStart(int x, int y);
    Cell* newAdjacentCell(Cell* current_cell);
    Cell* newAdjacentField(Cell* current_cell);
    void generate_map();
    void show_map();
    void setTargetCell();
    void movePlayer();
    bool playerAlive();
    bool levelFinished();
    int PlayerPosX();
    int PlayerPosY();
    void clearBoard();
    bool resetGame();
    ~playground();
};
