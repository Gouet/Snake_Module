#include <iostream>
#include <QThread>
#include "PyObjectSingleton.h"

PyObjectWrapper::PyObjectWrapper() {
    this->height = 20;
    this->width = 20;
    widget = new SnakeWidget(height, width);
    widget->setVisible(false);
//    widget->show();
}

void PyObjectWrapper::step(SnakeWidget::Action actionConv) {
    widget->step(actionConv);
}

SnakeGame::Direction PyObjectWrapper::getCurrentDirection() {
    return widget->getCurrentDirection();
}

void PyObjectWrapper::reset() {
    widget->init();
}

void PyObjectWrapper::setWallSize(int size) {
    widget->setWallSize(size);
}

double PyObjectWrapper::getReward() {
    return widget->getReward();
}

std::vector<SnakeGame::CellType> const &PyObjectWrapper::getObservation() {
    return widget->getObservation();
}

QImage const &PyObjectWrapper::getSnakeObsevation() {
    return widget->getSnakeImageRot();
}

bool PyObjectWrapper::done() {
    return widget->isSnakeGameOver();
}

void PyObjectWrapper::show() {
    if (!widget->isVisible()) {
        widget->show();
        widget->setVisible(false);
    }
}

void PyObjectWrapper::setShow(bool render) {
    if (render) {
        widget->show();
    }
    widget->setVisible(render);
    widget->setRender(render);
}

int PyObjectWrapper::getHeight() {
    return this->height;
}

int PyObjectWrapper::getWidth() {
    return this->width;
}

PyObjectSingleton::PyObjectSingleton() {

}

PyObjectSingleton *PyObjectSingleton::Shared() {
    static PyObjectSingleton *shared = new PyObjectSingleton();

    return shared;
}

PyObjectWrapper *PyObjectSingleton::createWrapperFrom(void*object) {
    PyObjectWrapper *wrapper = new PyObjectWrapper();
    m_conv[object] = wrapper;
    return wrapper;
}

PyObjectWrapper *PyObjectSingleton::getWrapper(void*object) {
    return m_conv[object];
}

std::map<void*, PyObjectWrapper *> const &PyObjectSingleton::getWrapperMap() {
    return m_conv;
}

std::list<void *> const &PyObjectSingleton::getWrapperToCreate() {
    return m_wrappersPending;
}

void PyObjectSingleton::addWrapperInPendingList(void*data) {
    m_wrappersPending.push_back(data);
}

void PyObjectSingleton::resetPendingWrapper() {
    m_wrappersPending.clear();
}
