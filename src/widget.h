#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QObject>
#include <QKeyEvent>
#include "defines.h"
#include "game.h"
#include "block.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

const int blockSize = 20;
const int boardStartX = 20;
const int boardStartY = 40;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_btnStart_clicked();

    void onTimer();
    bool eventFilter(QObject* watched, QEvent* event);

private:
    Ui::Widget *ui;
    Game *game;
    QTimer timer;

    void init();
    void paintEvent(QPaintEvent *e);
    void drawBoard(QPainter &p);
    void drawBlock(QPainter &p);
    void drawSquare(QPainter &p, int y, int x, int type);
    void drawGameOver(QPainter &p);
    void gameStart();
    void gameOver();

    Qt::GlobalColor typeToColor(int type);
};
#endif // WIDGET_H
