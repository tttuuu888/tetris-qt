#include "widget.h"
#include "defines.h"
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

    this->installEventFilter(this);
    this->game = new Game(12, 20);

    connect(&timer, SIGNAL(timeout()), this, SLOT(onTimer()));
}

void Widget::onTimer()
{
    // qDebug("tick");
    this->game->gamePlayTurn();

    update();
}

void Widget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter p(this);

    this->drawBoard(p);

    switch(this->game->getGameStatus())
    {
    case GamePlaying:
        this->drawBlock(p);
        break;
    case GameOver:
        this->gameOver();
        this->drawGameOver(p);
        break;
    default:
        break;
    }

    ui->editLevel->setText(QString::number(this->game->getLevel()));
    ui->editScore->setText(QString::number(this->game->getScore()));

    if(this->timer.interval() != this->game->getGameDelay()) {
        // qDebug("delay %d to %d", this->timer.interval(), this->game->getGameDelay());
        this->timer.setInterval(this->game->getGameDelay());
    }

    p.end();
}

void Widget::drawBoard(QPainter &p)
{
    for(int i=0; i<this->game->getBoardHeight(); i++) {
        for(int j=0; j<this->game->getBoardWidth(); j++) {
            this->drawSquare(p, i,j, this->game->getBoardValue(j,i));
        }
    }
}

void Widget::drawBlock(QPainter &p)
{
    int sy = this->game->getBlockPositionY();
    int sx = this->game->getBlockPositionX();

    for(int i=sy; i<sy+4; i++)
        for(int j=sx; j<sx+4; j++) {
            if(this->game->getBoardValue(j,i) == 0)
                this->drawSquare(p, i,j, this->game->getBlockSquareValue(j-sx, i-sy));
        }
}

void Widget::drawSquare(QPainter &p, int y, int x, int type)
{
    const int startX = boardStartX;
    const int startY = boardStartY;

    int xx = startX + (x * blockSize);
    int yy = startY + (y * blockSize);

    if(xx + blockSize > startX + this->game->getBoardWidth() * blockSize ||
       yy + blockSize > startY + this->game->getBoardHeight() * blockSize)
        return;

    QPen pen(this->typeToColor(type));
    p.setBrush(QBrush(this->typeToColor(type), Qt::SolidPattern));
    p.drawRect(QRect(xx,yy,blockSize,blockSize));
}

void Widget::drawGameOver(QPainter &p)
{
    // qDebug("game over draw");
    ui->btnStart->setText("Start");

    const int fontSize = 25;
    const int sx = boardStartX + blockSize;
    const int sy = (this->game->getBoardHeight() * blockSize + boardStartY) / 2 - fontSize;
    const int width = blockSize * (this->game->getBoardWidth()-2);
    const int height = 100;

    p.fillRect(QRect(sx,sy,width,height), QBrush(QColor(110,0,255,170)));
    p.setFont(QFont("Arial", fontSize));
    p.setPen(Qt::red);
    p.drawText(QRect(sx, sy, width, height-fontSize-3), Qt::AlignCenter, "Game Over");

    QString result = QString::asprintf("Score : %d", this->game->getScore());
    p.drawText(QRect(sx, sy, width, height+fontSize+3), Qt::AlignCenter, result);
}

void Widget::gameStart()
{
    // qDebug("game start");
    this->game->gameStart();
    this->timer.start(this->game->getGameDelay());
    ui->btnStart->setText("Stop");

    this->setFocus();
}

void Widget::gameOver()
{
    // qDebug("game over");
    this->timer.stop();
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
    switch(this->game->getGameStatus())
    {
    case GameInitialStatus:
    case GameOver:
        this->gameStart();
        break;
    case GamePlaying:
        this->game->gameOver();
        this->gameOver();
        break;
    }

    update();
}


bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    if( event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        switch(keyEvent->key()) {
        case Qt::Key_Left:
            this->game->moveBlock(Left);
            break;
        case Qt::Key_Right:
            this->game->moveBlock(Right);
            break;
        case Qt::Key_Up:
            this->game->rotateBlock();
            break;
        case Qt::Key_Down:
            this->game->moveBlock(Down);
            break;
        case Qt::Key_Space:
            this->game->moveBlock(ToBottom);
            break;
        }
        update();

        return true;
    }
    else
    {
        return QObject::eventFilter(watched, event);
    }
}
