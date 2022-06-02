#pragma once

#include <iostream>
#include <string>
using namespace std;

enum Piece { KING, QUEEN, BISHOP, KNIGHT, ROOK, PAWN, EMPTY };
enum Color { WHITE, BLACK, NONE };

class Square {
    Piece piece;
	Color color;
	int x, y;
public:
    Square();
    void setSpace(Square*);
    void setEmpty();
    void setPieceAndColor(Piece, Color);
    Piece getPiece();
    Color getColor();
    void setX(int);
    void setY(int);
    int getX();
    int getY();
};