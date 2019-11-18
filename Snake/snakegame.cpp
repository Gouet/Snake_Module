//#include <QRandomGenerator>
#include "snakegame.h"
#include <math.h>
#include <QDebug>

const std::unordered_map<SnakeGame::Direction, SnakeGame::Vector> SnakeGame::Directions = {
    { SnakeGame::Direction::UP, { 0, -1 } },
    { SnakeGame::Direction::DOWN, { 0, 1 } },
    { SnakeGame::Direction::LEFT, { -1, 0 } },
    { SnakeGame::Direction::RIGHT, { 1, 0 } }
};

SnakeGame::SnakeGame()
{
    m_reward = 0;
    this->m_sizeWall = 1;
}

void SnakeGame::initGame(size_t width, size_t height)
{
    //realloc map
    m_reward = 0;
    m_grid.clear();
    m_grid.assign(width * height, CellType::EMPTY);
    m_width = width;
    m_height = height;
    m_isOver = false;
    m_maxStep = false;
    m_timeStepAppleAte = 0;

    //write walls
    int idxReduced = this->m_sizeWall;

    for (int idx = 0; idx < idxReduced; idx++) {
        for (size_t x = 0; x < width - idx; x++) {
            setCellValue(x, idx, CellType::WALL);
            setCellValue(x, height - 1 - idx, CellType::WALL);
        }

        for (size_t y = 0; y < height; y++) {
            setCellValue(idx, y, CellType::WALL);
            setCellValue(width - 1 - idx, y, CellType::WALL);
        }
    }

    //setup snake
    m_snakeBody.clear();
    m_snakeSize = 3;
    setSnakeDirection(Directions.at(Direction::RIGHT));
    m_snakeDir = Direction::RIGHT;
    Position bodyPosition { (width - m_snakeSize) / 2, height / 2 };

    for (size_t i = 0; i < m_snakeSize; i++) {
        m_snakeBody.push_front(bodyPosition);
        bodyPosition.x += m_snakeDirection.x;
        bodyPosition.y += m_snakeDirection.y;
    }
    m_snakeHead = &m_snakeBody.front();
    m_snakeTail = &m_snakeBody.back();

    for (Position const &curr : m_snakeBody) {
        setCellValue(curr.x, curr.y, CellType::SNAKE);
    }

    randomizeApple();

    Position nextPos = getNextPos();
    double nextPosX = nextPos.x;
    double nextPosY = nextPos.y;
    double applePosX = m_applePos.x;
    double applePosY =  m_applePos.y;

    double dist = sqrt(pow(nextPosX - applePosX, 2) + pow(nextPosY - applePosY, 2));
    m_prevDist = dist;

    emit gameStarted();
}

double SnakeGame::getReward() {
    return m_reward;
}

void SnakeGame::updateGrid()
{
    Position nextPos = getNextPos();
    CellType nextCell = getCellValue(nextPos.x, nextPos.y);

    setCellValue(nextPos.x, nextPos.y, CellType::SNAKE);
    m_snakeBody.push_front(nextPos);
    m_snakeHead = &m_snakeBody.front();
    //m_reward = 0;
    double width = m_width;
    double height = m_height;
    double maxDist = sqrt(pow(0 - width, 2) + pow(0 - height, 2));
    bool head = true;

    switch (nextCell) {
    case CellType::APPLE:
        m_snakeSize++;
        randomizeApple();
        //m_timeStepAppleAte = 0;
        m_reward = 1;
        break;
    case CellType::SNAKE:
        //setCellValue(nextPos.x, nextPos.y, CellType::SNAKE);
        m_reward = -1;
        m_isOver = true;
        emit gameOver(m_snakeSize);
        break;
    case CellType::WALL:
        setCellValue(nextPos.x, nextPos.y, CellType::WALL);
        m_reward = -1;
        m_isOver = true;
        emit gameOver(m_snakeSize);
        break;
    default:
        double nextPosX = nextPos.x;
        double nextPosY = nextPos.y;
        double applePosX = m_applePos.x;
        double applePosY =  m_applePos.y;
        m_timeStepAppleAte++;

        //if (m_timeStepAppleAte > (m_width * m_height))
        //    m_maxStep = true;

        double dist = sqrt(pow(nextPosX - applePosX, 2) + pow(nextPosY - applePosY, 2));
        //dist = dist / maxDist;
        //dist = 1 - dist;
        //double isNear = dist - m_prevDist;
        //if (isNear > 0) {
        //    m_reward = -0.1;
        //} else {
        //    m_reward = 0.1;
        //}
        m_reward = 0;
        m_prevDist = dist;

        //m_reward = dist;

        head = true;
        for (Position const &curr : m_snakeBody) {
            if (!head)
                setCellValue(curr.x, curr.y, CellType::SNAKE);
            head = false;
        }
        setCellValue(m_snakeTail->x, m_snakeTail->y, CellType::EMPTY);
        m_snakeBody.pop_back();
        m_snakeTail = &m_snakeBody.back();
        break;
    }

    emit gridUpdated();
}

void SnakeGame::setSnakeDirection(SnakeGame::Direction direction)
{
    const Vector &targetDirection = SnakeGame::Directions.at(direction);

    if (m_snakeDirection.x + targetDirection.x == 0
        || m_snakeDirection.y + targetDirection.y == 0)
        return;

    m_snakeDir = direction;
    setSnakeDirection(targetDirection);
}

int SnakeGame::getDirectionDegree() const {
    if (m_snakeDir == Direction::UP) {
        return 0;
    } else if (m_snakeDir == Direction::DOWN) {
        return -180;
    } else if (m_snakeDir == Direction::LEFT) {
        return -270;
    } else if (m_snakeDir == Direction::RIGHT) {
        return -90;
    }
    return 0;
}

SnakeGame::Direction SnakeGame::getDirection() const {
    return m_snakeDir;
}

void SnakeGame::setWallSize(int size) {
    this->m_sizeWall = size;
}

const SnakeGame::SnakeGrid &SnakeGame::getGrid() const
{
    return m_grid;
}

size_t SnakeGame::getWidth() const
{
    return m_width;
}

size_t SnakeGame::getHeight() const
{
    return m_height;
}

bool SnakeGame::isOver() const
{
    return m_isOver;
}

void SnakeGame::setCellValue(size_t x, size_t y, SnakeGame::CellType value)
{
    setCellValue(x + y * m_width, value);
}

void SnakeGame::setCellValue(size_t i, SnakeGame::CellType value)
{
    m_grid[i] = value;
}

SnakeGame::CellType SnakeGame::getCellValue(int x, int y) const
{
    return m_grid[x + y * m_width];
}

bool SnakeGame::isInsideBorder(int x, int y) const
{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
        return false;
    }
    return true;
}

bool SnakeGame::isMaxStep() const {
    return m_maxStep;
}

void SnakeGame::setSnakeDirection(const SnakeGame::Vector &direction)
{
    m_snakeDirection.x = direction.x;
    m_snakeDirection.y = direction.y;
}

SnakeGame::Position SnakeGame::getNextPos() const
{
    return Position {
        m_snakeHead->x + m_snakeDirection.x,
        m_snakeHead->y + m_snakeDirection.y
    };

}

std::list<std::unique_ptr<SnakeGame::Vector> > SnakeGame::getEmptyTiles() const {
    std::list<std::unique_ptr<Vector> > list;
    size_t i = 0;

    for (SnakeGame::CellType cell : m_grid) {
        size_t x = i % m_height;
        size_t y = i / m_width;

        if (cell == CellType::EMPTY) {
            Vector *vec = new Vector();
            vec->x = x;
            vec->y = y;
            list.emplace_back(vec);
        }
        ++i;
    }
    return list;
}

SnakeGame::Position *SnakeGame::getSnakeHead() const {
    return m_snakeHead;
}

void SnakeGame::randomizeApple()
{
    std::list<std::unique_ptr<Vector> > const &list = getEmptyTiles();
    if (list.size() == 0) {
        m_isOver = true;
        return;
    }
    size_t value = rand() % (list.size() - 1);
    int idx = 0;
    for (auto it = list.begin(); it != list.end() ; it++) {
        if (idx == value) {
            m_applePos.x = it->get()->x;
            m_applePos.y = it->get()->y;
            setCellValue(it->get()->x, it->get()->y, CellType::APPLE);
            return;
        }
        ++idx;
    }
/*
    setCellValue(i, CellType::APPLE);

    bool reset = false;
        size_t i = rand() % (m_width * m_height - 1);

        while (m_grid[i] != CellType::EMPTY) {
            ++i;
            if (i > (m_width * m_height) - 1) {
                i = 0;
                if (reset) {
                    m_isOver = true;
                    return;
                }
                reset = true;
            }
        }
        setCellValue(i, CellType::APPLE);
        */
}
