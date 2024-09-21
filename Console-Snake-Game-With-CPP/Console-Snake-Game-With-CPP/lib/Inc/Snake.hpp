#pragma once
#ifndef SNAKE_HPP
#define SNAKE_HPP


#include "../Inc/Cell.hpp"


class Snake {
private:
    int snakeLength;
    int maxSnakeLenght;
    char type;
public:
    Cell** cells;
    Snake();
    Snake(const int& _x, const int& _y, const int& length, const int& maxLength);

    ~Snake() {
        for (int i = 0; i < snakeLength; i++)
            delete[] cells[i];
        delete[] cells;
    }
    void    addTail();
    void    removeTail();
    bool    move();
    void    setType(const char& _type);
    void    setMaxLength(const int& length);
    int     checkBody();
    int     getSnakeLength();
    int     getMaxSnakeLength();
    void build(char **scene);
};

#endif // !SNAKE_HPP