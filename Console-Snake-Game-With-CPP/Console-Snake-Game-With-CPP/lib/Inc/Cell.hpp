#pragma once

#ifndef CELL_HPP
#define CELL_HPP


enum class Cell_Direction {
    RIGHT,
    LEFT,
    UP,
    DOWN
};

class Cell {
    private:
        int X;
        int Y;
        char type;
        Cell_Direction Direction;
    public:
        Cell();
        Cell(const int& _x, const int& _y, Cell_Direction _Direction, const char& _Type);
        Cell(const int& _x, const int& _y, const char& _Type);
        Cell(Cell& _cell);


        void setX(const int& _x);
        void setY(const int& _y);
        void setDirection(Cell_Direction _Direction);
        void setType(const char& _type);
        void bodyMove();
        int getX();
        int getY();
        Cell_Direction getDirection();
        void build(char **scene);

        char getType();
}; 
#endif // CELL_HPP