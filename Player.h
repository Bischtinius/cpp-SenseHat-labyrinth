#pragma onece

class Player
{
private:
    int x = -1;
    int y = -1;
    bool isAlive = true;
    int lifes = 3;
public:
    Player();
    int getPosX();
    int getPosY();
    int getLifes();
    void setPos(int x, int y);
    void demage();
    bool getAlive();

    ~Player();
};
