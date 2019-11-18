#include <QObject>
#include <QPainter>
#include <QKeyEvent>
#include <QDebug>
#include <QMutexLocker>

#include "widget.h"
#include <iostream>
#include <QTransform>

SnakeWidget::SnakeWidget(int height, int width, QWidget *parent)
    : QWidget(parent)
{
    this->gameWidth = width;
    this->gameHeight = height;
    m_isRenderMode = false;
    m_update = false;
}

SnakeWidget::~SnakeWidget()
{

}

void SnakeWidget::init()
{
    m_image = QImage(840, 840, QImage::Format::Format_RGB32);
    m_snakeView = QImage(this->gameWidth * 2 + 1, this->gameHeight * 2 + 1, QImage::Format::Format_RGB32);
    m_image.fill(QColor("#000000"));
    m_snakeView.fill(QColor("#000000"));
    m_snakeRotView = m_snakeView;
    m_snakeRotView.fill(QColor("#000000"));
    m_layout.addWidget(&m_label);
    m_layout.addWidget(&m_labelSnakeView);
    setLayout(&m_layout);

    m_game.initGame(this->gameWidth, this->gameHeight);
    drawSnakeView();
}

void SnakeWidget::step(Action action)
{
    m_game.setSnakeDirection(static_cast<SnakeGame::Direction>(action));

    m_game.updateGrid();
    if (m_game.isMaxStep()) {
        init();
    }
    drawSnakeView();
    if (m_isRenderMode) {
        m_update = true;
        update();
    }
}

void SnakeWidget::setWallSize(int size) {
    m_game.setWallSize(size);
}

double SnakeWidget::getReward() {
    return m_game.getReward();
}

QColor SnakeWidget::getColorFrom(SnakeGame::CellType cell) {
    QColor color("#000000");

    switch (cell) {
    case SnakeGame::CellType::WALL:
        color = QColor("#f4773d");
        break;
    case SnakeGame::CellType::APPLE:
        color = QColor("#f43d3d");
        break;
    case SnakeGame::CellType::EMPTY:
        color = QColor("#000000");
        break;
    case SnakeGame::CellType::SNAKE:
        color = QColor("#58f43d");
        break;
    }
    return color;
}

SnakeGame::Direction SnakeWidget::getCurrentDirection() {
    return m_game.getDirection();
}

void SnakeWidget::drawSnakeView() {
    SnakeGame::Position *position = m_game.getSnakeHead();

    if (!position) {
        return;
    }

    for (int y = 0; y < m_snakeView.height(); y++) {
        for (int x = 0; x < m_snakeView.width(); x++) {
            int realPosX = position->x - (m_snakeView.width() / 2) + x;
            int realPosY = position->y - (m_snakeView.height() / 2) + y;
            QColor color = QColor("#f4773d");
            if (m_game.isInsideBorder(realPosX, realPosY)) {
                SnakeGame::CellType cell = m_game.getCellValue(realPosX, realPosY);
                color = getColorFrom(cell);
            }
            m_snakeView.setPixelColor(x, y, color);
        }
    }

    QTransform myTransform;
    myTransform.rotate(m_game.getDirectionDegree());
    m_snakeRotView = m_snakeView.transformed(myTransform);
}

void SnakeWidget::paintEvent(QPaintEvent*) {
    if (!m_update)
        return;
    QPainter painter(&m_image);
    size_t i = 0;
    size_t boxWidth = m_image.width() / m_game.getWidth();
    size_t boxHeight = m_image.height() / m_game.getHeight();

    for (SnakeGame::CellType cell : m_game.getGrid()) {
        size_t x = i % m_game.getWidth();
        size_t y = i / m_game.getHeight();

        switch (cell) {
        case SnakeGame::CellType::WALL:
            painter.setBrush(QBrush(QColor("#f4773d")));
            break;
        case SnakeGame::CellType::APPLE:
            painter.setBrush(QBrush(QColor("#f43d3d")));
            break;
        case SnakeGame::CellType::EMPTY:
            painter.setBrush(QBrush(QColor("#000000")));
            break;
        case SnakeGame::CellType::SNAKE:
            painter.setBrush(QBrush(QColor("#58f43d")));
            break;
        }

        painter.drawRect(QRect(x * boxWidth, y * boxHeight, boxWidth, boxHeight));
        i++;
    }

    //drawSnakeView();

    //m_labelSnakeView.setPixmap(QPixmap::fromImage(m_snakeRotView.scaled(m_snakeRotView.width() * 10, m_snakeRotView.height() * 10)));

    m_label.setPixmap(QPixmap::fromImage(m_image));
    m_update = false;
    m_sem.release();
}

void SnakeWidget::repaint()
{
//QMutexLocker _mut(&m_mutex);

    qDebug() << "REPAINT done.";
}

void SnakeWidget::setRender(bool render) {
    m_isRenderMode = render;
}

void SnakeWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
        step(Action::UP);
        break;
    case Qt::Key_Down:
        step(Action::DOWN);
        break;
    case Qt::Key_Left:
        step(Action::LEFT);
        break;
    case Qt::Key_Right:
        step(Action::RIGHT);
        break;
    }
}

const SnakeGame::SnakeGrid &SnakeWidget::getObservation() {
    return m_game.getGrid();
}

QImage const &SnakeWidget::getSnakeImageRot() const {
    return m_snakeRotView;
}

bool SnakeWidget::isSnakeGameOver() {
    return m_game.isOver();
}
