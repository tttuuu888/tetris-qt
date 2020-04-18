#include <QTime>
#include "game.h"

Game::Game(int boardWidth, int boardHeight, int maxDelay, int minDelay)
{
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    this->boardWidth = boardWidth;
    this->boardHeight = boardHeight;
    this->maxGameDelay = maxDelay;
    this->minGameDelay = minDelay;
    this->gameDelay = maxDelay;
    this->gameStatus = GameInitialStatus;

    this->board = new int[boardWidth * boardHeight];
    this->currentBlock = new Block(2);

    this->init();
}


Game::~Game()
{
    delete this->board;
    delete this->currentBlock;
}

void Game::init()
{
    this->level = 1;
    this->score = 0;

    this->currentBlock->init(qrand()%7 + 2);

    for(int i=0; i<this->boardHeight; i++)
        for(int j=0; j<this->boardWidth; j++)
            if(i==0 || i == this->boardHeight - 1 || j==0 || j== this->boardWidth -1)
                this->board[i*this->boardWidth + j] = 1;
            else
                this->board[i*this->boardWidth + j] = 0;
}

void Game::setLevel(int level)
{
    this->level = level;
}

int Game::getLevel()
{
    return this->level;
}

int Game::getScore()
{
    return this->score;
}

int Game::getBoardWidth()
{
    return this->boardWidth;
}

int Game::getBoardHeight()
{
    return this->boardHeight;
}

int Game::getBoardValue(int x, int y)
{
    return this->board[y*this->boardWidth + x];
}

int Game::getGameDelay()
{
    return this->gameDelay;
}

int Game::getBlockPositionX()
{
    return this->currentBlock->getPx();
}

int Game::getBlockPositionY()
{
    return this->currentBlock->getPy();
}

int Game::getBlockSquareValue(int x, int y)
{
    return this->currentBlock->getSquare(y, x);
}

void Game::addCurrentwBlockToBoard()
{
    int sy = this->currentBlock->getPy();
    int sx = this->currentBlock->getPx();

    for(int i=sy; i<sy+4; i++)
        for(int j=sx; j<sx+4; j++)
            if(this->board[i*this->boardWidth + j] == 0)
                this->board[i*this->boardWidth + j] = this->currentBlock->getSquare(i-sy, j-sx);
}

void Game::breakBlocks()
{
    for(int i=1; i<this->boardHeight-1; i++) {
        int emptySquares = 0;

        for(int j=1; j<this->boardWidth-1; j++) {
            if(this->board[i*this->boardWidth + j] == 0)
                emptySquares++;
        }

        if(emptySquares == 0) {
            this->moveAboveColumnsDown(i);
            this->score += 100;
        }
    }

}

void Game::moveAboveColumnsDown(int column)
{
    for(int i=column; i>1; i--)
        for(int j=1; j<this->boardWidth-1; j++)
            this->board[i*this->boardWidth + j] = this->board[(i-1)*this->boardWidth + j];
}


void Game::updateGameLevel()
{
    if(this->score / 1000 < this->level)
        return;

    this->level++;

    qDebug("Level : %d", this->level);

    this->updateGameDelay();
}


void Game::updateGameDelay()
{
    int newDealy = this->gameDelay = this->maxGameDelay - (this->level * 100);
    this->gameDelay = newDealy > this->minGameDelay ? newDealy : this->minGameDelay;
}

void Game::newBlock()
{
    this->addCurrentwBlockToBoard();
    this->breakBlocks();
    this->updateGameLevel();
    this->currentBlock->init(qrand()%7 + 2);
}

bool Game::isGameOver()
{
    return this->gameStatus == GameOver;
}

bool Game::isBlockAbleToMove(Direction dir)
{
    int sy = this->currentBlock->getPy();
    int sx = this->currentBlock->getPx();

    switch(dir){
    case Left:
        sx -=1;
        break;
    case Right:
        sx +=1;
        break;
    case Down:
        sy += 1;
        break;
    default:
        break;
    }

    if(sx < 0 || sx >= this->boardWidth || sy < 0 || sy >= this->boardHeight)
        return false;

    for(int i=sy; i<sy+4; i++)
        for(int j=sx; j<sx+4; j++)
            if(this->board[i*this->boardWidth + j] != 0 && this->currentBlock->getSquare(i-sy, j-sx) != 0)
                return false;

    return true;
}

bool Game::isBlockDrawable()
{
    return this->isBlockAndBoardOverlap() == false;
}

bool Game::isBlockAbleToRotate()
{
    this->currentBlock->rotate(1);

    bool result = this->isBlockAndBoardOverlap() == false;

    this->currentBlock->rotate(-1);

    return result;
}

bool Game::isBlockAndBoardOverlap()
{
    int sy = this->currentBlock->getPy();
    int sx = this->currentBlock->getPx();

    for(int i=sy; i<sy+4; i++)
        for(int j=sx; j<sx+4; j++)
            if(this->board[i*this->boardWidth + j] != 0 && this->currentBlock->getSquare(i-sy, j-sx) != 0)
                return true;

    return false;
}

void Game::gameStart()
{
    qDebug("game start");
    this->init();
    this->gameStatus = GamePlaying;
}

void Game::gameOver()
{
    qDebug("game over");
    this->gameStatus = GameOver;
}

void Game::gamePlayTurn()
{
    if(this->isBlockAbleToMove(Down))
        this->currentBlock->move(Down);
    else
        this->newBlock();

    if(!this->isBlockDrawable())
        this->gameOver();
}

GameStatus Game::getGameStatus()
{
    return this->gameStatus;
}

void Game::moveBlock(Direction dir)
{
    if(dir == ToBottom)
    {
        while(this->isBlockAbleToMove(Down)) {
            this->currentBlock->move(Down);
        }

        this->newBlock();
    }
    else
    {
        if(this->isBlockAbleToMove(dir))
            this->currentBlock->move(dir);
    }
}
void Game::rotateBlock()
{
    if(this->isBlockAbleToRotate())
        this->currentBlock->rotate(1);
}
