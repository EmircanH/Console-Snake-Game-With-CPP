
#include "../Inc/Snake.hpp"

Snake::Snake() {
    snakeLength = 5;
    type = 'O';
    maxSnakeLenght = 500;
    cells = new Cell * [snakeLength];
    for (int i = 0; i < snakeLength; i++)
        cells[i] = new Cell(10 - i, 10, Cell_Direction::RIGHT, 'O');
}
Snake::Snake(const int& _x, const int& _y, const int& length, const int& maxLength) {
    type = 'O';
    snakeLength = length;
    maxSnakeLenght = maxLength;
    cells = new Cell * [snakeLength];
    for (int i = 0; i < snakeLength; i++)
        cells[i] = new Cell(_x - i, _y, Cell_Direction::RIGHT, type);
}
void Snake::addTail() {
    if (snakeLength != 0) {
        Cell** temp = new Cell * [snakeLength];
        for (int i = 0; i < snakeLength; i++)
            temp[i] = cells[i];
        delete[] cells;

        cells = new Cell * [snakeLength + 1];
        for (int i = 0; i < snakeLength; i++)
            cells[i] = temp[i];

        cells[snakeLength] = new Cell(*cells[snakeLength - 1]);
        if (snakeLength == 1) {
            if (cells[snakeLength]->getType() == 'O') {
                cells[snakeLength]->setType('o');
            }
            else if (cells[snakeLength]->getType() == (char)219) {
                cells[snakeLength]->setType((char)176);
            }
        }

        switch (cells[snakeLength - 1]->getDirection()) {
            int x, y;
        case Cell_Direction::RIGHT:
            x = cells[snakeLength]->getX();
            cells[snakeLength]->setX(--x);
            break;
        case Cell_Direction::LEFT:
            x = cells[snakeLength]->getX();
            cells[snakeLength]->setX(++x);
            break;
        case Cell_Direction::DOWN:
            y = cells[snakeLength]->getY();
            cells[snakeLength]->setY(--y);
            break;
        case Cell_Direction::UP:
            y = cells[snakeLength]->getY();
            cells[snakeLength]->setY(++y);
            break;
        default:
            break;
        }
        delete[] temp;
        snakeLength++;
    }
    else {
        cells = new Cell * [1];
        cells[0] = new Cell(1, 1, Cell_Direction::RIGHT, type);
        snakeLength++;
    }
}
void Snake::removeTail() {
    try {
        if (snakeLength == 1) throw 404;
        Cell** temp = new Cell * [snakeLength - 1];
        for (int i = 0; i < snakeLength - 1; i++)
            temp[i] = cells[i];
        delete[] cells[snakeLength - 1];
        delete[] cells;

        cells = new Cell * [snakeLength - 1];
        for (int i = 0; i < snakeLength - 1; i++)
            cells[i] = temp[i];
        delete[] temp;
        snakeLength--;
    }
    catch (int) {

    }
}
bool Snake::move() {
    for (int i = snakeLength - 1; i > 0; i--) {
        cells[i]->setX(cells[i - 1]->getX());
        cells[i]->setY(cells[i - 1]->getY());
    }
    cells[0]->bodyMove();

    for (int i = snakeLength - 1; i > 0; i--) {
        cells[i]->setDirection(cells[i - 1]->getDirection());
    }
    return true;
}
void Snake::setType(const char& _type) {
    type = _type;
    for (int i = 0; i < snakeLength; i++) {
        cells[i]->setType(type);
    }
    if (type == 'O' || type == 'o')
        cells[0]->setType('O');
    if (type == (char)176)
        cells[0]->setType((char)219);
}
void Snake::setMaxLength(const int& length) {
    maxSnakeLenght = length;
}
int Snake::checkBody() {
    int x = cells[0]->getX();
    int y = cells[0]->getY();
    for (int i = snakeLength - 1; i > 0; i--) {
        if ((x == cells[i]->getX()) && (y == cells[i]->getY()))
            return i;
    }
    return 0;
}
int Snake::getSnakeLength() {
    return snakeLength;
}
int Snake::getMaxSnakeLength() {
    return maxSnakeLenght;
}
void Snake::build(char** scene) {
    for (int i = 0; i < snakeLength; i++) {
        cells[i]->build(scene);
    }
}