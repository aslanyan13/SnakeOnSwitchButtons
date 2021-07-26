#include "widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent)
{
    srand(time(NULL));

    int windowW = 20 + MAP_WIDTH * 40;
    int windowH = 20 + MAP_HEIGHT * 25;
    this->resize(windowW, windowH);
    this->setWindowTitle("Snake game. Score: 0");

    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_WIDTH; j++) {
            switches[i][j] = new Switch(this);
            switches[i][j]->move(10 + j * 40, 10 + i * 25);
            switches[i][j]->show();
            map[i][j] = false;
        }
    }

    headPos = QPoint(5, 5);
    bodyPos.push_back(QPoint(4, 5));

    generateFood();
    map[foodPos.y()][foodPos.x()] = true;

    for (int i = 0; i < bodyPos.size(); i++)
        map[bodyPos[i].y()][bodyPos[i].x()] = true;

    map[headPos.y()][headPos.x()] = true;
    redrawMap();

    timer = new QTimer(this);
    timer->setInterval(1000 / 10);
    timer->start();
    connect(timer, &QTimer::timeout, this, &Widget::gameLoop);

    QShortcut * upKey = new QShortcut(QKeySequence("Up"), this, nullptr, nullptr, Qt::ApplicationShortcut);
    QShortcut * downKey = new QShortcut(QKeySequence("Down"), this, nullptr, nullptr, Qt::ApplicationShortcut);
    QShortcut * leftKey = new QShortcut(QKeySequence("Left"), this, nullptr, nullptr, Qt::ApplicationShortcut);
    QShortcut * rightKey = new QShortcut(QKeySequence("Right"), this, nullptr, nullptr, Qt::ApplicationShortcut);
    QShortcut * pause = new QShortcut(QKeySequence("P"), this, nullptr, nullptr, Qt::ApplicationShortcut);

    connect (upKey, &QShortcut::activated, [&]()    { if (headPos.x() != bodyPos[0].x()) this->dir = '^'; });
    connect (downKey, &QShortcut::activated, [&]()  { if (headPos.x() != bodyPos[0].x()) this->dir = 'v'; });
    connect (leftKey, &QShortcut::activated, [&]()  { if (headPos.y() != bodyPos[0].y()) this->dir = '<'; });
    connect (rightKey, &QShortcut::activated, [&]() { if (headPos.y() != bodyPos[0].y()) this->dir = '>'; });
    connect (pause, &QShortcut::activated, [&]() {
        if(timer->isActive()) timer->stop();
        else timer->start();
    });
}

Widget::~Widget()
{
}

void Widget::redrawMap() {
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_WIDTH; j++) {
            switches[i][j]->setActive(map[i][j]);
        }
    }
}
void Widget::gameLoop() {
    int oldHeadX = headPos.x();
    int oldHeadY = headPos.y();
    map[oldHeadY][oldHeadX] = false;

    if (dir == '>') headPos.setX(oldHeadX + 1);
    if (dir == '<') headPos.setX(oldHeadX - 1);
    if (dir == '^') headPos.setY(oldHeadY - 1);
    if (dir == 'v') headPos.setY(oldHeadY + 1);

    if (headPos.x() >= MAP_WIDTH) headPos.setX(0);
    if (headPos.x() < 0) headPos.setX(MAP_WIDTH - 1);
    if (headPos.y() >= MAP_HEIGHT) headPos.setY(0);
    if (headPos.y() < 0) headPos.setY(MAP_HEIGHT - 1);

    // If snake head position equal to food position, then generate new food and increase skake length
    if (headPos == foodPos)
    {
        scores++;
        this->setWindowTitle("Snake game. Score: " + QString::number(scores));
        bodyPos.push_back(bodyPos[bodyPos.size() - 1]); // Add new position to body, which is equal to last position
        generateFood();
    }
    else if (map[headPos.y()][headPos.x()] == true)
    {
        timer->stop();
        int ret = QMessageBox::information(this, "Game over!", "Scores: " + QString::number(scores) + ". Wanna play again?", QMessageBox::Yes | QMessageBox::No);

        // Reset game
        if (ret == QMessageBox::Yes) {
            for (int i = 0; i < MAP_HEIGHT; i++)
                for (int j = 0; j < MAP_WIDTH; j++)
                    map[i][j] = false;

            bodyPos = vector<QPoint>();
            headPos = QPoint(5, 5);
            bodyPos.push_back(QPoint(4, 5));

            generateFood();
            map[foodPos.y()][foodPos.x()] = true;
            dir = '>';
            timer->start();
            scores = 0;
            this->setWindowTitle("Snake game. Score: " + QString::number(scores));
        }
        if (ret == QMessageBox::No) this->close();
    }

    for (int i = 0; i < bodyPos.size(); i++)
        map[bodyPos[i].y()][bodyPos[i].x()] = false;

    int tempX, tempY;
    for (int i = 0; i < bodyPos.size(); i++) {
        if (i == 0) {
            tempX = bodyPos[i].x();
            tempY = bodyPos[i].y();
            bodyPos[i].setX(oldHeadX);
            bodyPos[i].setY(oldHeadY);
        } else {
            int tempX2 = bodyPos[i].x();
            int tempY2 = bodyPos[i].y();
            bodyPos[i].setX(tempX);
            bodyPos[i].setY(tempY);
            tempX = tempX2;
            tempY = tempY2;
        }
    }

    map[foodPos.y()][foodPos.x()] = true;

    for (int i = 0; i < bodyPos.size(); i++)
        map[bodyPos[i].y()][bodyPos[i].x()] = true;
    map[headPos.y()][headPos.x()] = true;

    redrawMap();
}
