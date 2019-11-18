#include <cmath>
# define _hypot hypot
#include <Python.h>
#include "PyObjectSingleton.h"
#include <QDebug>
#include <windows.h>
#include <iostream>
#include <QApplication>
#include <QTimer>
#include <QSemaphore>
#include <QObject>

PyMODINIT_FUNC PyInit_pySnakeEnv(void);

QSemaphore g_sem;
QApplication *a = nullptr;
//SnakeWidget *w = nullptr;

DWORD WINAPI myThread(LPVOID lpParameter)
{
    /*
    int argc =0;
    char **argv = NULL;
    QApplication a(argc, argv);
    QTimer timer;

    QObject::connect(&timer, &QTimer::timeout, [=]() {
        for (auto conv : PyObjectSingleton::Shared()->getWrapperToCreate()) {
             PyObjectWrapper *wrapper = PyObjectSingleton::Shared()->createWrapperFrom(conv);
             wrapper->show();
             g_sem.release(1);
        }

        PyObjectSingleton::Shared()->resetPendingWrapper();

    }
    );

    timer.start(1000);

    return a.exec();
    */
}

PyObject *createEnv(PyObjectWrapper *wrapper) {
    //std::vector<SnakeGame::CellType> const &cells = wrapper->getObservation();
    QImage snakeView = wrapper->getSnakeObsevation().convertToFormat(QImage::Format_Grayscale8);

    PyObject *sample = PyTuple_New(1);
    PyObject *channels = PyTuple_New(1);
    PyObject *heightList = PyTuple_New(snakeView.height());

    for (int y = 0; y < snakeView.height(); y++)
     {
        PyObject *widthList = PyTuple_New(snakeView.width());
        for (int x = 0; x < snakeView.width(); x++) {
            PyTuple_SetItem(widthList, x, PyFloat_FromDouble(snakeView.pixelColor(x, y).value() / 256.0 ));
        }
        PyTuple_SetItem(heightList, y, widthList);
    }
    PyTuple_SetItem(channels, 0, heightList);
    PyTuple_SetItem(sample, 0, channels);
    return sample;
}

static PyObject *step(PyObject *self, PyObject *args)
{
    double action = PyFloat_AsDouble(args);
    PyObjectWrapper * wrapper = PyObjectSingleton::Shared()->getWrapper(self);
    SnakeWidget::Action actionConv = SnakeWidget::UP;
//    if (action == 0)
//        actionConv = SnakeWidget::NONE;

   SnakeGame::Direction dir = wrapper->getCurrentDirection();
   if (action == 0) {
        actionConv = static_cast<SnakeWidget::Action>(dir);
   }
   else if (action == 1) {
       if (dir == SnakeGame::Direction::RIGHT) {
            actionConv = SnakeWidget::UP;
       } else if (dir == SnakeGame::UP) {
            actionConv = SnakeWidget::LEFT;
        } else if (dir == SnakeGame::LEFT) {
           actionConv = SnakeWidget::DOWN;
       } else if (dir == SnakeGame::DOWN) {
           actionConv = SnakeWidget::RIGHT;
       }
   } else if (action == 2) {
       if (dir == SnakeGame::Direction::UP) {
            actionConv = SnakeWidget::RIGHT;
       } else if (dir == SnakeGame::RIGHT) {
            actionConv = SnakeWidget::DOWN;
        } else if (dir == SnakeGame::DOWN) {
           actionConv = SnakeWidget::LEFT;
       } else if (dir == SnakeGame::LEFT) {
           actionConv = SnakeWidget::UP;
       }
   }

//actionConv = static_cast<SnakeWidget::Action>(dir);

  /*  if (action == 0)
        actionConv = SnakeWidget::UP;
    else if (action == 1)
        actionConv = SnakeWidget::DOWN;
    else if (action == 2)
        actionConv = SnakeWidget::RIGHT;
    else if (action == 3)
        actionConv = SnakeWidget::LEFT; */

    wrapper->step(actionConv);
    a->processEvents();

    PyObject *list = PyTuple_New(3);
    PyTuple_SetItem(list, 0, createEnv(wrapper));
    PyTuple_SetItem(list, 1, PyFloat_FromDouble(wrapper->getReward()));
    PyTuple_SetItem(list, 2, PyBool_FromLong(wrapper->done() == true ? 1 : 0));
    return list;
}

static PyObject *make(PyObject *self, PyObject *args)
{
    static bool isThreadCreated = false;
    if (!isThreadCreated) {
       // CreateThread(0, 0, myThread,NULL, 0, NULL);
        int argc =0;
        char **argv = NULL;
        a = new QApplication(argc, argv);

        isThreadCreated = true;
    }

    PyObject *newEnv = PyInit_pySnakeEnv();

    PyObjectWrapper *wrapper = PyObjectSingleton::Shared()->createWrapperFrom(newEnv);
    wrapper->show();
    //PyObjectSingleton::Shared()->addWrapperInPendingList(newEnv);

    //g_sem.acquire(1);

    return newEnv;
}

static PyObject *render(PyObject *self, PyObject *args)
{
    bool render = PyObject_IsTrue(args) == 1;

    for (auto conv : PyObjectSingleton::Shared()->getWrapperMap()) {
        conv.second->setShow(render);
    }

    Py_RETURN_NONE;
}

static PyObject *reset(PyObject *self, PyObject *args)
{
    PyObjectWrapper * wrapper = PyObjectSingleton::Shared()->getWrapper(self);

    wrapper->reset();
    return createEnv(wrapper);
}

static PyObject *reduce_wall(PyObject *self, PyObject *args)
{
   /* PyObjectWrapper * wrapper = PyObjectSingleton::Shared()->getWrapper(self);

    wrapper->reset();*/
    long wallSize = PyLong_AsLong(args);
    std::map<void*, PyObjectWrapper *> const &wrappers = PyObjectSingleton::Shared()->getWrapperMap();

    for (auto &it : wrappers) {
        it.second->setWallSize(wallSize);
    }

    Py_RETURN_NONE;
}

static PyMethodDef SpamMethods[] = {
    {"make",  make, METH_NOARGS, "Execute a shell command."},
    {"render",  render, METH_O, "Execute a shell command."},
    {"reduce_wall",  reduce_wall, METH_O, "Execute a shell command."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static PyMethodDef SnakeEnvMethods[] = {
    {"step",  step, METH_O, "Execute a shell command."},
    {"reset",  reset, METH_NOARGS, "Execute a shell command."},
//    {"make",  make, METH_O, "Execute a shell command."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};


static struct PyModuleDef spammodule = {
    PyModuleDef_HEAD_INIT,
    "pySnake",   /* name of module */
    "", /* module documentation, may be NULL */
    -1,       /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
    SpamMethods
};

static struct PyModuleDef snakeEnvModule = {
    PyModuleDef_HEAD_INIT,
    "pyEnvSnake",   /* name of module */
    "", /* module documentation, may be NULL */
    -1,       /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
    SnakeEnvMethods
};

PyMODINIT_FUNC
PyInit_pySnake(void)
{
    return PyModule_Create(&spammodule);
}

PyMODINIT_FUNC
PyInit_pySnakeEnv(void)
{
    return PyModule_Create(&snakeEnvModule);
}


//int main(int argc, char *argv[])
//{

//}
