#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QObject>
#include <QKeyEvent>
#include "defines.h"
#include "block.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_btnStart_clicked();

    void on_spinLevel_valueChanged(int arg1);
    void onTimer();
    bool eventFilter(QObject* watched, QEvent* event);

private:
    Ui::Widget *ui;
    QPainter *painter;
    QVector<QVector<int>> vec;
    QTimer timer;
    int tmp;
    int gameDelay;
    int board[boardHeight][boardWidth];
    bool playGame;
    bool initialStatus;
    Block *currentBlock;
    int score;

    void init();
    void game();
    void paintEvent(QPaintEvent *e);
    void drawBoard(QPainter &p);
    void drawBlock(QPainter &p);
    void drawSquare(QPainter &p, int y, int x, int type);
    void drawGameOver(QPainter &p);
    void boardInit();
    void addBlockToBoard();
    void breakBlocks();
    void moveAboveColumns(int column);
    bool isBlockAbleToMove(Direction dir);
    bool isBlockDrawable();
    bool isBlockAbleToRotate();
    bool isGameOver();
    void gameStart();
    void gameOver();

    Qt::GlobalColor typeToColor(int type);
};
#endif // WIDGET_H
