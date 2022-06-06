#include "Square.h"

Square::Square() {
            piece = EMPTY;
            color = NONE;
        }
        
void Square::setSpace(Square* space) {
        color = space->getColor();
	    piece = space->getPiece();
    }

void Square::setEmpty() {
        color = NONE;
	    piece = EMPTY;
    }

void Square::setPieceAndColor(Piece p, myColor c) {
        piece = p;
	    color = c;
    }

Piece Square::getPiece() {
        return piece;
    }

myColor Square::getColor() {
        return color;
    }

void Square::setX(int xx) {
        x = xx;
    }

void Square::setY(int yy) {
    y = yy;
}

int Square::getX() {
    return x;
}

int Square::getY() {
    return y;
}

