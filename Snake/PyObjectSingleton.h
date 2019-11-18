#ifndef PYOBJECT_SINGLETON_H_
# define PYOBJECT_SINGLETON_H_

#include "widget.h"
#include <map>
#include <list>

class PyObjectWrapper {
public:
    PyObjectWrapper();

public:
    void step(SnakeWidget::Action);
    void reset();
    void setWallSize(int);
    bool done();
    void show();
    double getReward();
    void setShow(bool);
    std::vector<SnakeGame::CellType> const &getObservation();
    QImage const &getSnakeObsevation();
    int getHeight();
    int getWidth();

    SnakeGame::Direction getCurrentDirection();

private:
    SnakeWidget *widget;
    int width;
    int height;
};

class PyObjectSingleton {
public:
    PyObjectSingleton();
    static PyObjectSingleton *Shared();

    PyObjectWrapper *createWrapperFrom(void*);
    PyObjectWrapper *getWrapper(void*);
    std::map<void*, PyObjectWrapper *> const &getWrapperMap();
    std::list<void *> const &getWrapperToCreate();
    void addWrapperInPendingList(void*);
    void resetPendingWrapper();

private:
    std::map<void*, PyObjectWrapper *> m_conv;
    std::list<void *> m_wrappersPending;
};

#endif
