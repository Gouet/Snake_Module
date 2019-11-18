#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QImage>
#include <QHBoxLayout>
#include <QLabel>
#include <QMutex>
#include <QSemaphore>
#include "snakegame.h"

class SnakeWidget : public QWidget
{
    Q_OBJECT

public:
    enum Action {
        UP,
        LEFT,
        DOWN,
        RIGHT
//        NONE
    };

public:
    SnakeWidget(int height, int width, QWidget *parent = 0);
    ~SnakeWidget();

public:
    void init();
    void step(Action action);
    double getReward();

    void setWallSize(int);
    const SnakeGame::SnakeGrid &getObservation();
    QImage const &getSnakeImageRot() const;
    bool isSnakeGameOver();
    void setRender(bool);
    QColor getColorFrom(SnakeGame::CellType);
    SnakeGame::Direction getCurrentDirection();

    virtual void paintEvent(QPaintEvent*);

signals:
    void tryToRepaint();

public slots:
    void repaint();

private:
    void keyPressEvent(QKeyEvent *event);

private:
    void drawSnakeView();

public:
    QLabel m_label;
    QLabel m_labelSnakeView;

private:
    int gameWidth;
    int gameHeight;
    QHBoxLayout m_layout;

    QImage m_image;
    QImage m_snakeView;
    QImage m_snakeRotView;
    SnakeGame   m_game;
    bool m_isRenderMode;
    QSemaphore m_sem;
    QMutex m_mutex;
    bool m_update;
};

#endif // WIDGET_H
