#pragma once

#include "Square.h"
#include <iostream>
using namespace std;

class Board {
    Square square[8][8];
	Color turn = WHITE;
    bool moveKing(Square*, Square*);
    bool moveQueen(Square*, Square*);
    bool moveBishop(Square*, Square*);
    bool moveKnight(Square*, Square*);
    bool moveRook(Square*, Square*);
    bool movePawn(Square*, Square*);
    bool isLegal(int, int, int, int);
    bool isInCheck(Color);
    bool checkMate(Color);
    void printBoard();
public:
    Square* getSquare(int, int);
    void setSquare(Square*, int, int);
    bool Move();
    void setBoard();
    bool playGame();
};