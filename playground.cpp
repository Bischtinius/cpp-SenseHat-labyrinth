#include "playground.h"
#include "joystickskrhabe010.h"
#include "ledmatrixled2472g.h"
#include <iostream>
#include <unistd.h>
#include <stdlib.h>

playground::playground(/* args */)
{
}

playground::playground(int nx, int ny)
{
    std::cout << "Playground Constructor" << std::endl;
    this->nx = nx;
    this->ny = ny;
    map = new Cell*[ny];
    for (int i = 0; i < nx; i++)
    {
        map[i] = new Cell[ny];
    }

    for (int i = 0; i < nx; ++i)
    {
        for (int j = 0; j < ny; ++j)
        {
            map[i][j] = Cell(i,j);
        }
    }
}

void playground::setStart(int x, int y)
{
    std::cout << "Playground setStart" << std::endl;

    this->ix = x;
    this->iy = y;
    this->player->setPos(x,y);
}

Cell *playground::newAdjacentCell(Cell *current_cell)
{
    int cx = current_cell->getPositionX();
    int cy = current_cell->getPositionY();
    int newx = 0;
    int newy = 0;
    bool outside_borders = true;
    bool not_new = true;
    int field_counter = 0;
    Cell* return_ptr = nullptr;

    srand ( time(NULL) );
    int direction = std::rand() % 4;     

    int rand_dir = std::rand() % 2;

    if (rand_dir == 0)
    {
        rand_dir = -1;
    }

    while (not_new && outside_borders && field_counter < 4)
    {


        if (direction == 0)
        {
            newx = cx;
            newy = cy - 1;
        }
        else if (direction == 1)
        {
            newx = cx + 1;
            newy = cy;
        }
        else if (direction == 2)
        {
            newx = cx;
            newy = cy + 1;
        }
        else if (direction == 3)
        {
            newx = cx - 1;
            newy = cy;
        }
        else
        {
            std::cout << "Error directions " << std::endl;
        }

        // std::cout << "current_pos " << cx << cy << std::endl;
        // std::cout << "new_pos " << newx << newy << std::endl;
        // std::cout << "direction " << direction << std::endl;

        // if (newx >= 0 && newx < 8 && newy >= 0 && newy < 8)
        if (newx >= 0 && newx < this->nx && newy >= 0 && newy < this->ny)
        {
            // std::cout << "inside Borders " << std::endl;
            if (this->map[newx][newy].getNew())
            {
                not_new = false;
                outside_borders = false;
                // std::cout << "NewField !!" << std::endl;
                return_ptr = map[newx][newy].getCurrent();
            }         
        }
        
        

        field_counter ++;
        direction += rand_dir;
        if (direction > 3)
        {
            direction = 0;
        }
        if (direction < 0)
        {
            direction = 3;
        }
        
        
    }

    return return_ptr;
}


Cell* playground::newAdjacentField(Cell* current_cell)
{
    Cell* return_ptr = this->newAdjacentCell(current_cell);
    if (return_ptr == nullptr)
    {
        return_ptr = current_cell->getPrevius();
    }
    std::cout << return_ptr << std::endl;
    return return_ptr;
    
}

void playground::generate_map()
{
    std::cout << "Playground generate_map" << std::endl;

    int wx = this->ix;
    int wy = this->iy;
    int newx = -1;
    int newy = -1;
    int wallx = -1;
    int wally = -1;

    int steps_from_start = 0;

    for (int i = 0; i < nx; ++i)
    {
        for (int j = 0; j < ny; ++j)
        {
            map[i][j].setNew();
        }
    }
    
    Cell* current_cell = map[wx][wy].getCurrent();
    Cell* previus_cell = nullptr;
    Cell* next_cell = nullptr;
    Cell* wall_cell = nullptr;
    current_cell->setField();

    int counter = 0;

    do
    {

        current_cell->setPrevius(previus_cell);
        current_cell->setStepsFromStart(steps_from_start);

        next_cell = this->newAdjacentField(current_cell->getCurrent());

        if (next_cell != nullptr)
        {
            newx = next_cell->getPositionX();
            newy = next_cell->getPositionY();

            map[newx][newy].setPrevius(current_cell);
            map[newx][newy].setStepsFromStart(steps_from_start+1);

            next_cell->setField();
        }
        

        wall_cell = this->newAdjacentCell(current_cell->getCurrent());
        // std::cout << "wall_cell " << wall_cell << std::endl;
        
        if (wall_cell != nullptr)
        {
            
            wallx = wall_cell->getPositionX();
            wally = wall_cell->getPositionY();

            map[wallx][wally].setStepsFromStart(-1);
            wall_cell->setWall();
        }

        

        previus_cell = current_cell;
        current_cell = next_cell;
        steps_from_start = current_cell->getSteps();

        // this->setTargetCell();
        this->show_map();

        float microsecond = 1000000;
        usleep(0.1 * microsecond);//sleeps for n second

        counter ++;
    //     std::cout << (counter < 100) << std::endl;
    } while (counter < 100);
    //     std::cout << (next_cell != nullptr) << std::endl;
    // } while ((next_cell != nullptr));
    this->level_finished = false;
    this->setTargetCell();
}


void playground::show_map()
{
    // std::cout << "Playground show_map" << std::endl;

    init_LED_LED2472G();
    clear_allLED_LED2472G();
    // set_singleLED_LED2472G(10,0,0xF,0x3,0x10);
    bool current_field_is_wall = false;
    bool current_field_is_field = false;
    bool current_field_is_target = false;
    bool current_field_is_position = false;
    Cell* current_field;
    int colorR = 0;
    int colorG = 0;
    int colorB = 0;
    // std::cout << "Playground show_map Here" << std::endl;
    for (int x = 0; x < this->nx; x++)
    {
        for (int y = 0; y < this->ny; y++)
        {
            current_field = map[y][x].getCurrent();
            current_field_is_target = current_field->getTarget();
            current_field_is_field = current_field->getField();
            current_field_is_wall = current_field->getWall();

            if (y == this->player->getPosX() && x == this->player->getPosY())
            {
                current_field_is_position = true;
            }else
            {
                current_field_is_position = false;
            }
            
            
            if (current_field_is_wall)
            {
                colorR = 70;
                colorG = 0;
                colorB = 0;
            }else if (current_field_is_target)
            {
                colorR = 204;
                colorG = 0;
                colorB = 204;
            }else if (current_field_is_field)
            {
                colorR = 0;
                colorG = 0;
                colorB = 0;
            }else 
            {
                colorR = 0;
                colorG = 0;
                colorB = 150;
            }
            
            if (current_field_is_position)
            {
                int factor = this->player->getLifes();
                colorR = 30 * factor*-1;
                colorG = 30 * factor*-1;
                colorB = 30 * factor*-1;
            }
            
            set_singleLED_LED2472G(x,y,colorR,colorG,colorB);
            
        }
    }
    
    close_LED_LED2472G(); 
}


void playground::setTargetCell()
{
    Cell* target_cell;
    int max_steps = 0;

    int delta_x = 0;
    int delta_y = 0;
    int delta = 0;
    int max_delta = 0;

    for (int i = 0; i < nx; ++i)
    {
        for (int j = 0; j < ny; ++j)
        {
            // delta_x = abs(map[i][j].getPositionX() - this->ix);
            // delta_y = abs(map[i][j].getPositionY() - this->iy);

            // delta = delta_x+delta_y;
            // if((delta > max_delta) && map[i][j].getField())
            // {
            //     max_delta = delta;
            //     target_cell = map[i][j].getCurrent();

            // }

            if (map[i][j].getSteps() > max_steps)
            {
                max_steps = map[i][j].getSteps();
                target_cell = map[i][j].getCurrent();
                // std::cout << max_steps << std::endl;
            }
        }
    }
    target_cell->setTarget();
    this->taget = target_cell;
    std::cout << "TargetCell " << target_cell->getPositionX() << target_cell->getPositionY() << " Steps " << target_cell->getSteps() << " " << target_cell << std::endl;
}


void playground::movePlayer()
{
    init_JoyStick_SKRHABE010();
    int cx = this->player->getPosX();
    int cy = this->player->getPosY();

    int newx = -1;
    int newy = -1;

    int direction = read_Joystick_SKRHABE010();
    std::cout << "Joystick" << direction << std::endl;

    if (direction == 105)
    {
        newx = cx;
        newy = cy-1;
    }else if (direction == 106)
    {
        newx = cx;
        newy = cy+1;
    }else if (direction == 103)
    {
        newx = cx-1;
        newy = cy;
    }else if (direction == 108)
    {
        newx = cx+1;
        newy = cy;
    }else
    {
        std::cout << "no Direction" << std::endl;
        newx = cx;
        newy = cy;
    }

    if (newx < 0 || newx >= 8 || newy < 0 || newy >= 8)
    {
        std::cout << "outside" << std::endl;
        this->player->demage();
        newx = cx;
        newy = cy;
    }
    

    if (map[newx][newy].getWall())
    {
        this->player->demage();
        newx = cx;
        newy = cy;
    }

    this->player->setPos(newx,newy);

    if (map[newx][newy].getTarget())
    {
        this->level_finished = true;
    }
    
}

void playground::clearBoard()
{
    init_LED_LED2472G();
	set_allLED_LED2472G(0xF,0x3,0x1);

    float microsecond = 1000000;
    usleep(0.1 * microsecond);//sleeps for n second

	clear_allLED_LED2472G();
	close_LED_LED2472G();
}

bool playground::resetGame()
{
    init_JoyStick_SKRHABE010();
    int direction = read_Joystick_SKRHABE010();

    if (direction != 0)
    {
        return true;
    }else
    {
        return false;
    }
    
    

}

bool playground::playerAlive()
{
    return this->player->getAlive();
}

bool playground::levelFinished()
{
    return this->level_finished;
}

int playground::PlayerPosX()
{
    return this->player->getPosX();
}

int playground::PlayerPosY()
{
    return this->player->getPosY();
}

playground::~playground()
{
}