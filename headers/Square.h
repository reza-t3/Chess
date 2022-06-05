#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

enum Piece { KING, QUEEN, BISHOP, KNIGHT, ROOK, PAWN, EMPTY };
enum myColor { WHITE, BLACK, NONE };

class Square {
public:
    Piece piece;
	myColor color;
	int x, y;
    sf::Sprite sprite;
    Square();
    void setSpace(Square*);
    void setEmpty();
    void setPieceAndColor(Piece, myColor);
    Piece getPiece();
    myColor getColor();
    void setX(int);
    void setY(int);
    int getX();
    int getY();
};