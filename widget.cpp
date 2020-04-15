#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QTimer>
#include <QTime>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    init();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::init()
{
    ui->setupUi(this);
    this->initialStatus = true;
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    this->installEventFilter(this);

    this->boardInit();

    connect(&timer, SIGNAL(timeout()), this, SLOT(onTimer()));

    this->playGame = false;
    this->gameDelay = 1200;
    this->currentBlock = new Block(2);
}

void Widget::onTimer()
{
    qDebug("tick");
    if(this->isBlockAbleToMove(Down))
    {
        this->currentBlock->move(Down);
    }
    else
    {
        this->addBlockToBoard();
        this->breakBlocks();
        this->adjustGameLevel();
        this->currentBlock->init(qrand()%7 + 2);
        if(!this->isBlockDrawable())
            this->gameOver();
    }

    update();
}

void Widget::paintEvent(QPaintEvent *e)
{
    QPainter p(this);

    this->drawBoard(p);

    if(this->isGameOver())
        this->drawGameOver(p);
    else
        this->drawBlock(p);

    p.end();
}

void Widget::drawBoard(QPainter &p)
{
    for(int i=0; i<boardHeight; i++) {
        for(int j=0; j<boardWidth; j++) {
            this->drawSquare(p, i,j,this->board[i][j]);
        }
    }
}

void Widget::drawBlock(QPainter &p)
{
    if(this->initialStatus)
        return;

    int sy = this->currentBlock->getPy();
    int sx = this->currentBlock->getPx();

    for(int i=sy; i<sy+4; i++)
        for(int j=sx; j<sx+4; j++) {
            if(this->board[i][j] == 0)
                this->drawSquare(p, i,j, this->currentBlock->getSquare(i-sy, j-sx));
        }

    ui->editLevel->setText(QString::number(this->level));
    ui->editScore->setText(QString::number(this->score));
}

void Widget::addBlockToBoard()
{
    int sy = this->currentBlock->getPy();
    int sx = this->currentBlock->getPx();

    for(int i=sy; i<sy+4; i++)
        for(int j=sx; j<sx+4; j++)
            if(this->board[i][j] == 0)
                this->board[i][j] = this->currentBlock->getSquare(i-sy, j-sx);
}

void Widget::breakBlocks()
{
    for(int i=1; i<boardHeight-1; i++) {
        int emptySquares = 0;

        for(int j=1; j<boardWidth-1; j++) {
           if(this->board[i][j] == 0)
               emptySquares++;
        }

        if(emptySquares == 0) {
            this->moveAboveColumns(i);
            this->score += 100;
        }
    }
}

void Widget::moveAboveColumns(int column)
{
    for(int i=column; i>1; i--)
        for(int j=1; j<boardWidth-1; j++)
            this->board[i][j] = this->board[i-1][j];
}

void Widget::drawSquare(QPainter &p, int y, int x, int type)
{
    const int startY = 40;
    const int startX = 20;

    int xx = startX + (x * blockSize);
    int yy = startY + (y * blockSize);

    if(xx + blockSize > startX + boardWidth * blockSize || yy + blockSize > startY + boardHeight * blockSize)
        return;

    QPen pen(this->typeToColor(type));
    p.setBrush(QBrush(this->typeToColor(type), Qt::SolidPattern));
    p.drawRect(QRect(xx,yy,blockSize,blockSize));
}

bool Widget::isGameOver()
{
    if(this->initialStatus)
        return false;

    return !this->playGame;
}

void Widget::adjustGameLevel()
{
    if(this->score / 1000 < this->level)
        return;

    this->level++;

    qDebug("Level : %d", this->level);
    int newDealy = this->gameDelay = 1200 - (this->level * 100);

    this->gameDelay = newDealy > 100 ? newDealy : 100;
    this->timer.setInterval(this->gameDelay);
}

void Widget::gameStart()
{
    qDebug("game start");
    this->timer.start(this->gameDelay);
    ui->btnStart->setText("Stop");
    this->currentBlock->init(qrand()%7 + 2);
    this->boardInit();
    this->playGame = true;

    this->setFocus();
}

void Widget::gameOver()
{
    qDebug("game over");
    this->timer.stop();
    this->playGame = false;
}

void Widget::drawGameOver(QPainter &p)
{
    qDebug("game over draw");
    ui->btnStart->setText("Start");

    p.fillRect(QRect(40,200,200,100), QBrush(QColor(155,0,255,128)));
    p.setFont(QFont("Arial", 25));
    p.setPen(Qt::red);
    p.drawText(QRect(30, 140, 220, 180), Qt::AlignCenter, "Game Over");

    QString result = QString::asprintf("Score : %d", this->score);
    p.drawText(QRect(30, 170, 220, 180), Qt::AlignCenter, result);
}

void Widget::boardInit()
{
    this->level = 1;
    this->score = 0;

    for(int i=0; i<boardHeight; i++) {
        for(int j=0; j<boardWidth; j++) {
            if(i==0 || i == boardHeight - 1 || j==0 || j== boardWidth -1)
                this->board[i][j] = 1;
            else
                this->board[i][j] = 0;
        }
    }
}

bool Widget::isBlockAbleToMove(Direction dir)
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
    }

    if(sx < 0 || sx >= boardWidth || sy < 0 || sy >= boardHeight)
        return false;

    for(int i=sy; i<sy+4; i++)
        for(int j=sx; j<sx+4; j++)
            if(this->board[i][j] != 0 && this->currentBlock->getSquare(i-sy, j-sx) != 0)
                return false;

    return true;
}

bool Widget::isBlockDrawable()
{
    return !(this->isBlockAndBoardOverlap());
}

bool Widget::isBlockAbleToRotate()
{
    this->currentBlock->rotate(1);

    bool result = !(this->isBlockAndBoardOverlap());

    this->currentBlock->rotate(-1);

    return result;
}

bool Widget::isBlockAndBoardOverlap()
{
    int sy = this->currentBlock->getPy();
    int sx = this->currentBlock->getPx();

    for(int i=sy; i<sy+4; i++)
        for(int j=sx; j<sx+4; j++)
            if(this->board[i][j] != 0 && this->currentBlock->getSquare(i-sy, j-sx) != 0)
                return true;

    return false;
}

Qt::GlobalColor Widget::typeToColor(int type)
{
    switch (type) {
    case 1:
        return Qt::gray;
    case 2:
        return Qt::cyan;
    case 3:
        return Qt::blue;
    case 4:
        return Qt::magenta;
    case 5:
        return Qt::yellow;
    case 6:
        return Qt::green;
    case 7:
        return Qt::darkRed;
    case 8:
        return Qt::red;
    default:
        return Qt::black;
    }
}


void Widget::on_btnStart_clicked()
{
    this->initialStatus = false;
    if(this->playGame == false)
        this->gameStart();
    else
        this->gameOver();

    update();
}


bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    if( event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        switch(keyEvent->key()) {
        case Qt::Key_Left:
            if(this->isBlockAbleToMove(Left)) {
                this->currentBlock->move(Left);
                update();
            }
            break;
        case Qt::Key_Right:
            if(this->isBlockAbleToMove(Right)) {
                this->currentBlock->move(Right);
                update();
            }
            break;
        case Qt::Key_Up:
            if(this->isBlockAbleToRotate()) {
                this->currentBlock->rotate(1);
                update();
            }
            break;
        case Qt::Key_Down:
            if(this->isBlockAbleToMove(Down)) {
                this->currentBlock->move(Down);
                update();
            }
            break;
        case Qt::Key_Space:
            while(this->isBlockAbleToMove(Down)) {
                this->currentBlock->move(Down);
            }
            update();
            break;
        }
    }
}
