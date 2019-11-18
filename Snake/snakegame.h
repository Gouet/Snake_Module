#ifndef SNAKEGAME_H
#define SNAKEGAME_H

#include <QObject>

#include <vector>
#include <list>
#include <unordered_map>
#include <memory>

class SnakeGame : public QObject
{
    Q_OBJECT

public:
    enum CellType {
        EMPTY,
        WALL,
        SNAKE,
        APPLE
    };

    enum Direction {
        UP = 0,
        LEFT,
        DOWN,
        RIGHT
    };

    using SnakeGrid = std::vector<CellType>;

public:
    struct Position {
        int x;
        int y;
    };

    struct Vector {
        long x;
        long y;
    };

    static const std::unordered_map<Direction, Vector> Directions;

public:
    SnakeGame();

public:
    void initGame(size_t width, size_t height);
    void updateGrid();
    double getReward();
    void setSnakeDirection(Direction direction);
    int getDirectionDegree() const;
    Direction getDirection() const;
    void setWallSize(int);

public:
    const SnakeGrid &getGrid() const;
    size_t getWidth() const;
    size_t getHeight() const;
    bool isOver() const;
    Position *getSnakeHead() const;
    CellType getCellValue(int x, int y) const;
    bool isInsideBorder(int x, int y) const;
    bool isMaxStep() const;

private:
    void setCellValue(size_t x, size_t y, CellType value);
    void setCellValue(size_t i, CellType value);
    void setSnakeDirection(Vector const &direction);
    Position getNextPos() const;
    void randomizeApple();
    std::list<std::unique_ptr<Vector> > getEmptyTiles() const;


signals:
    void gridUpdated();
    void gameStarted();
    void gameOver(size_t score);

private:
    SnakeGrid    m_grid;
    size_t  m_width;
    size_t  m_height;
    bool m_isOver;
    bool m_maxStep;
    double m_reward;

private:
    Position *m_snakeHead;
    Position *m_snakeTail;
    Position m_applePos;
    std::list<Position> m_snakeBody;
    size_t m_snakeSize;
    Vector m_snakeDirection;
    Direction m_snakeDir;
    int     m_timeStepAppleAte;
    double m_prevDist;
    int m_sizeWall;
};

#endif // SNAKEGAME_H
