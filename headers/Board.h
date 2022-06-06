#pragma once

#include <SFML/Graphics.hpp>
#include "Square.h"
#include "Cell.h"

class Board {
public:
    Board(sf::RenderWindow* _window);
    sf::RenderWindow* window;
    Square square[8][8];
    sf::Texture textures[8][8];
	myColor turn;
    sf::Font font;
    bool finish;
    sf::Text status_text;
    Cells cells;
    int pieces_selected[2] = {-1, -1};
    void run();
    void init();
    void draw();
    void mouse_clicked(const sf::Vector2i& position);
    void legalMoves(int row, int col);
    void legalMovesBack(int row, int col);
    void setBoard();
    bool moveKing(Square*, Square*);
    bool moveQueen(Square*, Square*);
    bool moveBishop(Square*, Square*);
    bool moveKnight(Square*, Square*);
    bool moveRook(Square*, Square*);
    bool movePawn(Square*, Square*);
    bool isLegal(int, int, int, int);
    bool isInCheck(myColor);
    bool checkMate(myColor);
    void printBoard();
    Square* getSquare(int, int);
    void Move(int x1, int x2, int y1, int y2);
};
