
#include "../Inc/Cell.hpp"

Cell::Cell() {
    X = 10;
    Y = 10;
    Direction = Cell_Direction::RIGHT;
    type = 'O';
}
Cell::Cell(const int& _x, const int& _y, Cell_Direction _Direction, const char& _Type) {
    X = _x;
    Y = _y;
    Direction = _Direction;
    type = _Type;
}
Cell::Cell(const int& _x, const int& _y, const char& _Type) {
    X = _x;
    Y = _y;
    Direction = Cell_Direction::RIGHT;
    type = _Type;
}
Cell::Cell(Cell& _cell) {
    X = _cell.X;
    Y = _cell.Y;
    Direction = _cell.Direction;
    type = _cell.type;
}

void Cell::setX(const int& _x) {
    X = _x;
}
void Cell::setY(const int& _y) {
    Y = _y;
}
void Cell::setDirection(Cell_Direction _Direction) {
    Direction = _Direction;
}
void Cell::setType(const char& _type) {
    type = _type;
}
int Cell::getX() {
    return X;
}
int Cell::getY() {
    return Y;
}
Cell_Direction Cell::getDirection() {
    return Direction;
}
char Cell::getType() {
    return type;
}
void Cell::bodyMove() {
    switch (Direction) {
    case Cell_Direction::RIGHT:
        X++;
        break;
    case Cell_Direction::LEFT:
        X--;
        break;
    case Cell_Direction::UP:
        Y--;
        break;
    case Cell_Direction::DOWN:
        Y++;
        break;
    default:
        break;
    }
}
void Cell::build(char** scene) {
    scene[Y][X] = type;
}