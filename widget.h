#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QPushButton>
#include <QDebug>
#include <QShortcut>
#include <QKeySequence>
#include <QMessageBox>

#include <vector>

#include "switch.h"

const int MAP_WIDTH = 20;
const int MAP_HEIGHT = 15;

using namespace std;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    bool map[MAP_HEIGHT][MAP_WIDTH];
    Switch * switches[MAP_HEIGHT][MAP_WIDTH];

    void redrawMap();
    void gameLoop();
    // Generating new food, which is not overlap snake body and head
    void generateFood() {
        bool isFree;
        int x, y;
        do {
            isFree = true;
            x = rand() % MAP_WIDTH;
            y = rand() % MAP_HEIGHT;
            for (int i = 0; i < bodyPos.size(); i++)
                if (x == bodyPos[i].x() && y == bodyPos[i].y()) isFree = false;
            if (x == headPos.x() && y == headPos.y()) isFree = false;
        } while(!isFree);
        foodPos = QPoint(x, y);
    }

    QPoint headPos;
    QPoint foodPos;
    vector <QPoint> bodyPos;
    char dir = '>';
    int scores = 0;

    QTimer * timer;
};
#endif // WIDGET_H
