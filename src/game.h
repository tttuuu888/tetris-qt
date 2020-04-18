#ifndef GAME_H
#define GAME_H

#include "defines.h"
#include "block.h"

class Game
{
public:
    Game(int boardWidth, int boardHeight, int maxDelay=1200, int minDelay=100);
    ~Game();
    void init();
    void setLevel(int level);
    int getLevel();
    int getScore();
    int getBoardWidth();
    int getBoardHeight();
    int getBoardValue(int x, int y);
    int getGameDelay();
    int getBlockPositionX();
    int getBlockPositionY();
    int getBlockSquareValue(int x, int y);
    void gameStart();
    void gameOver();
    void gamePlayTurn();
    GameStatus getGameStatus();
    void moveBlock(Direction dir);
    void rotateBlock();

private:
    int *board;
    int boardWidth;
    int boardHeight;
    int minGameDelay;
    int maxGameDelay;
    int gameDelay;
    int level;
    int score;
    GameStatus gameStatus;
    Block *currentBlock;

    void addCurrentwBlockToBoard();
    void breakBlocks();
    void moveAboveColumnsDown(int column);
    void newBlock();
    void updateGameLevel();
    void updateGameDelay();
    bool isBlockAbleToMove(Direction dir);
    bool isBlockDrawable();
    bool isBlockAbleToRotate();
    bool isBlockAndBoardOverlap();
    bool isGameOver();
};

#endif // GAME_H
