#include "block.h"

const int allBlocks[7][4][16] = {
    {{2, 2, 2, 2,
      0, 0, 0, 0,
      0, 0, 0, 0,
      0, 0, 0, 0},
     {0, 2, 0, 0,
      0, 2, 0, 0,
      0, 2, 0, 0,
      0, 2, 0, 0},
     {2, 2, 2, 2,
      0, 0, 0, 0,
      0, 0, 0, 0,
      0, 0, 0, 0},
     {0, 2, 0, 0,
      0, 2, 0, 0,
      0, 2, 0, 0,
      0, 2, 0, 0}},

    {{3, 0, 0, 0,
      3, 3, 3, 0,
      0, 0, 0, 0,
      0, 0, 0, 0},
     {3, 3, 0, 0,
      3, 0, 0, 0,
      3, 0, 0, 0,
      0, 0, 0, 0},
     {3, 3, 3, 0,
      0, 0, 3, 0,
      0, 0, 0, 0,
      0, 0, 0, 0},
     {0, 3, 0, 0,
      0, 3, 0, 0,
      3, 3, 0, 0,
      0, 0, 0, 0}},

    {{0, 0, 4, 0,
      4, 4, 4, 0,
      0, 0, 0, 0,
      0, 0, 0, 0},
     {4, 0, 0, 0,
      4, 0, 0, 0,
      4, 4, 0, 0,
      0, 0, 0, 0},
     {4, 4, 4, 0,
      4, 0, 0, 0,
      0, 0, 0, 0,
      0, 0, 0, 0},
     {4, 4, 0, 0,
      0, 4, 0, 0,
      0, 4, 0, 0,
      0, 0, 0, 0}},

    {{5, 5, 0, 0,
      5, 5, 0, 0,
      0, 0, 0, 0,
      0, 0, 0, 0},
     {5, 5, 0, 0,
      5, 5, 0, 0,
      0, 0, 0, 0,
      0, 0, 0, 0},
     {5, 5, 0, 0,
      5, 5, 0, 0,
      0, 0, 0, 0,
      0, 0, 0, 0},
     {5, 5, 0, 0,
      5, 5, 0, 0,
      0, 0, 0, 0,
      0, 0, 0, 0}},

    {{0, 6, 6, 0,
      6, 6, 0, 0,
      0, 0, 0, 0,
      0, 0, 0, 0},
     {6, 0, 0, 0,
      6, 6, 0, 0,
      0, 6, 0, 0,
      0, 0, 0, 0},
     {0, 6, 6, 0,
      6, 6, 0, 0,
      0, 0, 0, 0,
      0, 0, 0, 0},
     {6, 0, 0, 0,
      6, 6, 0, 0,
      0, 6, 0, 0,
      0, 0, 0, 0}},

    {{0, 7, 0, 0,
      7, 7, 7, 0,
      0, 0, 0, 0,
      0, 0, 0, 0},
     {7, 0, 0, 0,
      7, 7, 0, 0,
      7, 0, 0, 0,
      0, 0, 0, 0},
     {7, 7, 7, 0,
      0, 7, 0, 0,
      0, 0, 0, 0,
      0, 0, 0, 0},
     {0, 7, 0, 0,
      7, 7, 0, 0,
      0, 7, 0, 0,
      0, 0, 0, 0}},

    {{8, 8, 0, 0,
      0, 8, 8, 0,
      0, 0, 0, 0,
      0, 0, 0, 0},
     {0, 8, 0, 0,
      8, 8, 0, 0,
      8, 0, 0, 0,
      0, 0, 0, 0},
     {8, 8, 0, 0,
      0, 8, 8, 0,
      0, 0, 0, 0,
      0, 0, 0, 0},
     {0, 8, 0, 0,
      8, 8, 0, 0,
      8, 0, 0, 0,
      0, 0, 0, 0}},
};

Block::Block(int type, int px, int py)
{
    this->initialPx = px;
    this->initialPy = py;

    this->init(type);
}

void Block::init(int type)
{
    this->rotation = 0;
    this->type = type;

    this->px = this->initialPx;
    this->py = this->initialPy;

    for (int i = 0; i < 16; i++)
        this->block[i] = allBlocks[type - 2][this->rotation][i];
}

void Block::move(Direction dir)
{
    switch (dir) {
    case Left:
        this->px--;
        break;
    case Right:
        this->px++;
        break;
    case Down:
        this->py++;
        break;
    default:
        break;
    }
}

void Block::rotate(int dir)
{
    if (dir > 0) {
        this->rotation++;
        if (this->rotation > 3)
            this->rotation = 0;
    } else {
        this->rotation--;
        if (this->rotation < 0)
            this->rotation = 3;
    }

    for (int i = 0; i < 16; i++)
        this->block[i] = allBlocks[this->type - 2][this->rotation][i];
}

int Block::getPx()
{
    return this->px;
}

int Block::getPy()
{
    return this->py;
}

int Block::getType()
{
    return this->type;
}

int Block::getSquare(int x, int y)
{
    return this->block[y * 4 + x];
}
