#ifndef BLOCK_H
#define BLOCK_H

#include "defines.h"

class Block
{
public:
    Block(int type);
    void init(int type);
    void move(Direction dir);
    void rotate(int dir);
    int getPx();
    int getPy();
    int getType();
    int getSquare(int y, int x);

private:
    int block[4][4];
    int type;
    int rotation;
    int px;
    int py;
};

#endif // BLOCK_H
