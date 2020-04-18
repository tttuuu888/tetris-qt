#ifndef BLOCK_H
#define BLOCK_H

#include "defines.h"

class Block
{
public:
    Block(int type, int px, int py);
    void init(int type);
    void move(Direction dir);
    void rotate(int dir);
    int getPx();
    int getPy();
    int getType();
    int getSquare(int x, int y);

private:
    int block[16];
    int type;
    int rotation;
    int initialPx;
    int initialPy;
    int px;
    int py;
};

#endif // BLOCK_H
