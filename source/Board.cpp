#include "Board.h"
#include <string>
using namespace std;

Board::Board(sf::RenderWindow* _window) : window(_window) {
    this->window->setTitle("Chess");
    this->window->setFramerateLimit(60);
}

void Board::init() {
    finish = false;
    this->cells.resize(8);
    for (int row = 0; row < 8; row++) {
        this->cells[row].resize(8);
        for (int column = 0; column < 8; column++) {
            this->cells[row][column].rect.setSize(sf::Vector2f(100, 100));
            if ((row + column) % 2 == 0)
                this->cells[row][column].rect.setFillColor(sf::Color(192,192,192));
            else
                this->cells[row][column].rect.setFillColor(sf::Color(0,128,128));
            this->cells[row][column].rect.setPosition(sf::Vector2f((column*(100)), row*(100)));
        }
    }
    font.loadFromFile("resources/fonts/roboto.ttf");
    status_text.setFont(font);
    status_text.setCharacterSize(30);
    status_text.setStyle(sf::Text::Regular);
    status_text.setFillColor(sf::Color::Black);
    status_text.setPosition(810.f, 20.f);
    (turn == WHITE) ? status_text.setString("White's Turn") : status_text.setString("Black's Turn");
}

void Board::draw() {
    for (int row = 0; row < 8; row++) {
        for (int column = 0; column < 8; column++) {
            this->window->draw(this->cells[row][column].rect);
            if (square[row][column].getColor() != NONE)
                this->window->draw(square[row][column].sprite);
        }
    }
    window->draw(status_text);
}

void Board::run() {
    setBoard();
    init();
    window->display();
    while (window->isOpen()) {
        sf::Event event;

        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window->close();
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                mouse_clicked(sf::Mouse::getPosition(*window));
        }
        window->clear(sf::Color(90,90,90));
        draw();
        window->display();
        }
    }
}

void Board::mouse_clicked(const sf::Vector2i& position) {
    int row = position.y / 100;
    int col = position.x / 100;
    if (row <= 7 && col <= 7) {
        if (pieces_selected[0] == -1) {
            if (square[row][col].getColor() == turn) {
                pieces_selected[0] = row;
                pieces_selected[1] = col;
                cells[row][col].rect.setFillColor(sf::Color(173,216,230));
                cells[row][col].rect.setOutlineThickness(-1);
                cells[row][col].rect.setOutlineColor(sf::Color(0,128,128));
                legalMoves(row, col);
            }
        }
        else {
            if (square[pieces_selected[0]][pieces_selected[1]].getColor() == square[row][col].getColor()) {
                ((pieces_selected[0] + pieces_selected[1]) % 2 == 0) ?
                    cells[pieces_selected[0]][pieces_selected[1]].rect.setFillColor(sf::Color(192,192,192)) :
                    cells[pieces_selected[0]][pieces_selected[1]].rect.setFillColor(sf::Color(0,128,128));
                legalMovesBack(pieces_selected[0], pieces_selected[1]);
                pieces_selected[0] = row;
                pieces_selected[1] = col;
                cells[row][col].rect.setFillColor(sf::Color	(173,216,230));
                cells[row][col].rect.setOutlineThickness(-1);
                cells[row][col].rect.setOutlineColor(sf::Color(0,128,128));
                legalMoves(row, col);
            }
            else {
                if (isLegal(pieces_selected[1], pieces_selected[0], col, row)) {
                    Move(pieces_selected[1], pieces_selected[0], col, row);
                    ((pieces_selected[0] + pieces_selected[1]) % 2 == 0) ?
                        cells[pieces_selected[0]][pieces_selected[1]].rect.setFillColor(sf::Color(192,192,192)) :
                        cells[pieces_selected[0]][pieces_selected[1]].rect.setFillColor(sf::Color(0,128,128));
                    legalMovesBack(pieces_selected[0], pieces_selected[1]);
                    square[pieces_selected[0]][pieces_selected[1]].sprite.setPosition(sf::Vector2f(row, col));
                    pieces_selected[0] = -1;
                    pieces_selected[1] = -1;
                }
                else {
                    ((pieces_selected[0] + pieces_selected[1]) % 2 == 0) ?
                        cells[pieces_selected[0]][pieces_selected[1]].rect.setFillColor(sf::Color(192,192,192)) :
                        cells[pieces_selected[0]][pieces_selected[1]].rect.setFillColor(sf::Color(0,128,128));
                    legalMovesBack(pieces_selected[0], pieces_selected[1]);
                    pieces_selected[0] = -1;
                    pieces_selected[1] = -1;
                }
            }
        }
    }
}

void Board::legalMoves(int row, int col) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (isLegal(col, row, j, i)) {
                cells[i][j].rect.setFillColor(sf::Color	(240,230,140));
                cells[i][j].rect.setOutlineThickness(-1);
                cells[i][j].rect.setOutlineColor(sf::Color(0,128,128));
            }
        }
    }
}

void Board::legalMovesBack(int row, int col) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (isLegal(col, row, j, i)) {
                ((i + j) % 2 == 0) ?
                cells[i][j].rect.setFillColor(sf::Color(192,192,192)) :
                cells[i][j].rect.setFillColor(sf::Color(0,128,128));
            }
        }
    }
}

void Board::Move(int x1, int x2, int y1, int y2) {
    myColor tempC;
    myPiece tempP;
    Square* begin = getSquare(x1, y1);
    Square* end = getSquare(x2, y2);
    if (getSquare(x1, y1)->getColor() == turn) {
        tempP = end->getPiece();
        tempC = end->getColor();
        end->setSpace(begin);
        begin->setEmpty();
        if (isInCheck(turn)) {
            begin->setSpace(end);
            end->setPieceAndColor(tempP, tempC);
            return;
        }
    }
    if (turn == BLACK)
        turn = WHITE;
    else
        turn = BLACK;
}

void Board::setBoard() {
    cout << "set your board:" << endl;
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            string p;
            cin >> p;
            if (p == "PB") {
                square[row][col].setPieceAndColor(PAWN, BLACK);
                square[row][col].sprite.setPosition(sf::Vector2f(col * 100, row * 100));
                textures[row][col].loadFromFile("resources/images2/BlackPawn.png");
                square[row][col].sprite.setTexture(textures[row][col]);
            }
            else if (p == "KB") {
                square[row][col].setPieceAndColor(KING, BLACK);
                square[row][col].sprite.setPosition(sf::Vector2f(col * 100, row * 100));
                textures[row][col].loadFromFile("resources/images2/BlackKing.png");
                square[row][col].sprite.setTexture(textures[row][col]);
            }
            else if (p == "RB") {
                square[row][col].setPieceAndColor(ROOK, BLACK);
                square[row][col].sprite.setPosition(sf::Vector2f(col * 100, row * 100));
                textures[row][col].loadFromFile("resources/images2/BlackRook.png");
                square[row][col].sprite.setTexture(textures[row][col]);
            }
            else if (p == "NB") {
                square[row][col].setPieceAndColor(KNIGHT, BLACK);
                square[row][col].sprite.setPosition(sf::Vector2f(col * 100, row * 100));
                textures[row][col].loadFromFile("resources/images2/BlackKnight.png");
                square[row][col].sprite.setTexture(textures[row][col]);
            }
            else if (p == "BB") {
                square[row][col].setPieceAndColor(BISHOP, BLACK);
                square[row][col].sprite.setPosition(sf::Vector2f(col * 100, row * 100));
                textures[row][col].loadFromFile("resources/images2/BlackBishop.png");
                square[row][col].sprite.setTexture(textures[row][col]);
            }
            else if (p == "QB") {
                square[row][col].setPieceAndColor(QUEEN, BLACK);
                square[row][col].sprite.setPosition(sf::Vector2f(col * 100, row * 100));
                textures[row][col].loadFromFile("resources/images2/BlackQueen.png");
                square[row][col].sprite.setTexture(textures[row][col]);
            }
            else if (p == "PW") {
                square[row][col].setPieceAndColor(PAWN, WHITE);
                square[row][col].sprite.setPosition(sf::Vector2f(col * 100, row * 100));
                textures[row][col].loadFromFile("resources/images2/WhitePawn.png");
                square[row][col].sprite.setTexture(textures[row][col]);
            }
            else if (p == "KW") {
                square[row][col].setPieceAndColor(KING, WHITE);
                square[row][col].sprite.setPosition(sf::Vector2f(col * 100, row * 100));
                textures[row][col].loadFromFile("resources/images2/WhiteKing.png");
                square[row][col].sprite.setTexture(textures[row][col]);
            }
            else if (p == "RW") {
                square[row][col].setPieceAndColor(ROOK, WHITE);
                square[row][col].sprite.setPosition(sf::Vector2f(col * 100, row * 100));
                textures[row][col].loadFromFile("resources/images2/WhiteRook.png");
                square[row][col].sprite.setTexture(textures[row][col]);
            }
            else if (p == "NW") {
                square[row][col].setPieceAndColor(KNIGHT, WHITE);
                square[row][col].sprite.setPosition(sf::Vector2f(col * 100, row * 100));
                textures[row][col].loadFromFile("resources/images2/WhiteKnight.png");
                square[row][col].sprite.setTexture(textures[row][col]);
            }
            else if (p == "BW") {
                square[row][col].setPieceAndColor(BISHOP, WHITE);
                square[row][col].sprite.setPosition(sf::Vector2f(col * 100, row * 100));
                textures[row][col].loadFromFile("resources/images2/WhiteBishop.png");
                square[row][col].sprite.setTexture(textures[row][col]);
            }
            else if (p == "QW") {
                square[row][col].setPieceAndColor(QUEEN, WHITE);
                square[row][col].sprite.setPosition(sf::Vector2f(col * 100, row * 100));
                textures[row][col].loadFromFile("resources/images2/WhiteQueen.png");
                square[row][col].sprite.setTexture(textures[row][col]);
            }
            else
                square[row][col].setPieceAndColor(EMPTY, NONE);
                
        }
    }
    cout << "which color starts? (w for white, b for black)" << endl;
    string c;
    while (true) {
        cin >> c;
        if (c == "w") {
            turn = WHITE;
            break;
        }
        else if (c == "b") {
            turn = BLACK;
            break;
        }
        else
            cout << "wrong input!try again" << endl;
    }
}

bool Board::moveKing(Square* currentPlace, Square* nextPlace) {
        if (abs(nextPlace->getX() - currentPlace->getX()) <= 1) {
            if (abs(nextPlace->getY() - currentPlace->getY()) <= 1) {
                if (abs(nextPlace->getX() - currentPlace->getX()) == 0 && abs(nextPlace->getY() - currentPlace->getY()) == 0)
                    return false;
                else
                    return true;
            }
            else
                return false;
        }
        else
            return false;
    }

bool Board::moveQueen(Square* currentPlace, Square* nextPlace) {
        int currentX = currentPlace->getX();
        int currentY = currentPlace->getY();
        int nextX = nextPlace->getX();
        int nextY = nextPlace->getY();
        if (currentX != nextX || currentY != nextY) {
            if (currentX == nextX) {
                int yShift = (nextY - currentY) / (abs(nextY - currentY));
                for (int i = currentY + yShift; i != nextY; i += yShift) {
                    if (getSquare(nextX,i)->getColor() != NONE)
                        return false;
                }
                return true;
            }
            else if (currentY == nextY) {
                int xShift = (nextX - currentX) / (abs(nextX - currentX));
                for (int i = currentX + xShift; i != nextX; i += xShift) {
                    if (getSquare(i,nextY)->getColor() != NONE)
                        return false;
                }
                return true;
            }
            else {
                if (abs(currentX - nextX) == abs(currentY - nextY)) {
                    int xShift = (nextX - currentX) / (abs(nextX - currentX));
                    int yShift = (nextY - currentY) / (abs(nextY - currentY));
                    for (int i = 1; i < abs(currentX - nextX); i++) {
                        if (getSquare(currentX + xShift*i,currentY + yShift*i)->getColor() != NONE)
                            return false;
                    }
                    return true;
                }
                else
                    return false;
            }
        }
        else
            return false;
    }

bool Board::moveBishop(Square* currentPlace, Square* nextPlace) {
    int currentX = currentPlace->getX();
    int currentY = currentPlace->getY();
    int nextX = nextPlace->getX();
    int nextY = nextPlace->getY();
    if (abs(currentX - nextX) == abs(currentY - nextY)) {
        int xShift = (nextX - currentX) / (abs(nextX - currentX));
        int yShift = (nextY - currentY) / (abs(nextY - currentY));
        for (int i = 1; i < abs(currentX - nextX); i++) {
            if (getSquare(currentX + xShift*i,currentY + yShift*i)->getColor() != NONE)
                return false;
        }
        return true;
    }
    else
        return false;
}

bool Board::moveKnight(Square* currentPlace, Square* nextPlace) {
    int currentX = currentPlace->getX();
    int currentY = currentPlace->getY();
    int nextX = nextPlace->getX();
    int nextY = nextPlace->getY();
    if ((abs(currentX - nextX) == 2 && abs(currentY - nextY) == 1) || (abs(currentX - nextX) == 1 && abs(currentY - nextY) == 2))
        return true;
    else
        return false;
}

bool Board::moveRook(Square* currentPlace, Square* nextPlace) {
    int currentX = currentPlace->getX();
    int currentY = currentPlace->getY();
    int nextX = nextPlace->getX();
    int nextY = nextPlace->getY();
    if (currentX != nextX || currentY != nextY) {
        if (currentX == nextX) {
            int yShift = (nextY - currentY) / (abs(nextY - currentY));
            for (int i = currentY + yShift; i != nextY; i += yShift) {
                if (getSquare(nextX,i)->getColor() != NONE)
                    return false;
            }
            if (getSquare(nextX,nextY)->getColor() != NONE)
                return true;
            else
                return true;
        }
        else {
            if (currentY == nextY) {
                int xShift = (nextX - currentX) / (abs(nextX - currentX));
                for (int i = currentX + xShift; i != nextX; i += xShift) {
                    if (getSquare(i,nextY)->getColor() != NONE)
                        return false;
                }
                return true;
            }
            else
                return false;
        }
    }
    else
        return false;
}

bool Board::movePawn(Square* currentPlace, Square* nextPlace) {
    int currentX = currentPlace->getX();
    int currentY = currentPlace->getY();
    int nextX = nextPlace->getX();
    int nextY = nextPlace->getY();
    if (currentPlace->getColor() == WHITE) {
        if (currentX == nextX && nextY == currentY - 1 && nextPlace->getColor() == NONE)
            return true;
        else if (currentX == nextX && nextY == currentY - 2 && currentY == 6 &&
        getSquare(currentX, currentY-1)->getColor() == NONE && nextPlace->getColor() == NONE)
            return true;
        else {
            if (abs(nextX - currentX) == 1 && currentY - 1 == nextY) {
                if (getSquare(nextX, nextY)->getColor() == BLACK)
                    return true;
                else
                    return false;
            }
            else
                return false;
        }
    }
    else {
        if (currentX == nextX && nextY == currentY + 1 && nextPlace->getColor() == NONE)
            return true;
        else if (currentX == nextX && nextY == currentY + 2 && currentY == 1 &&
        getSquare(currentX, currentY+1)->getColor() == NONE && nextPlace->getColor() == NONE)
            return true;
        else {
            if (abs(nextX - currentX) == 1 && currentY + 1 == nextY) {
                if (getSquare(nextX, nextY)->getColor() == WHITE)
                    return true;
                else
                    return false;
            }
            else                        
                return false;
        }
    }
}

bool Board::isLegal(int x1, int y1, int x2, int y2) {
    if (x1 < 0 || x1 > 7 || y1 < 0 || y1 > 7 || x2 < 0 || x2 > 7 || y2 < 0 || y2 > 7)
        return false;
    // myColor tempC;
    // Piece tempP;
    Square* source = getSquare(x1, y1);
    Square* destination = getSquare(x2, y2);
    source->setX(x1);
    source->setY(y1);
    destination->setX(x2);
    destination->setY(y2);
    if (source->getColor() == destination->getColor())
        return false;
    // tempP = destination->getPiece();
    // tempC = destination->getColor();
    // destination->setSpace(source);
    // source->setEmpty();
    // if (isInCheck(turn)) {
    //     source->setSpace(destination);
    //     destination->setPieceAndColor(tempP, tempC);
    //     return false;
    // }
    // source->setSpace(destination);
    // destination->setPieceAndColor(tempP, tempC);
    myPiece p = source->getPiece();
    if (p == KING)
        return moveKing(source, destination);
    else if (p == QUEEN)
        return moveQueen(source, destination);
    else if (p == BISHOP)
        return moveBishop(source, destination);
    else if (p == KNIGHT)
        return moveKnight(source, destination);
    else if (p == ROOK)
        return moveRook(source, destination);
    else if (p == PAWN)
        return movePawn(source, destination);
    else if (p == EMPTY)
        return false;
    else
        return false;
}

bool Board::isInCheck(myColor c) {
    int xKIng, yKing;
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (getSquare(col,row)->getColor() == c) {
                if (getSquare(col,row)->getPiece() == KING) {
                    xKIng = col;
                    yKing = row;
                }
            }
        }
    }
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (getSquare(col,row)->getColor() != NONE) {
                if (c != getSquare(col,row)->getColor()) {
                    if (isLegal(col, row, xKIng, yKing))
                        return true;
                }
            }
        }
    }
    return false;
}

bool Board::checkMate(myColor c) {
    myColor tempC;
    myPiece tempP;
    int xKIng, yKing;
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (getSquare(col,row)->getColor() == c) {
                if (getSquare(col,row)->getPiece() == KING) {
                    xKIng = col;
                    yKing = row;
                }
            }
        }
    }
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (getSquare(col,row)->getColor() != c) {
                if (isLegal(xKIng, yKing, col, row)) {
                    tempP = square[row][col].getPiece();
                    tempC = square[row][col].getColor();
                    getSquare(col,row)->setSpace(getSquare(xKIng,yKing));
                    getSquare(xKIng,yKing)->setEmpty();
                    if (!isInCheck(c)) {
                        getSquare(xKIng, yKing)->setSpace(getSquare(col, row));
                        getSquare(col, row)->setPieceAndColor(tempP, tempC);
                        return false;
                    }
                    else {
                        getSquare(xKIng, yKing)->setSpace(getSquare(col, row));
                        getSquare(col, row)->setPieceAndColor(tempP, tempC);
                    }
                }
            }
        }
    }
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (getSquare(col,row)->getColor() == c) {
                for (int i = 0; i < 8; i++) {
                    for (int j = 0; j < 8; j++) {
                        if (isLegal(col,row,j,i)) {
                            tempP = square[i][j].getPiece();
                            tempC = square[i][j].getColor();
                            getSquare(j,i)->setSpace(getSquare(col,row));
                            getSquare(col, row)->setEmpty();
                            if (!isInCheck(c)) {
                                getSquare(col, row)->setSpace(getSquare(j, i));
                                getSquare(j, i)->setPieceAndColor(tempP, tempC);
                                return false;
                            }
                            else {
                                getSquare(col, row)->setSpace(getSquare(j, i));
                                getSquare(j, i)->setPieceAndColor(tempP, tempC);
                            }
                        }
                    }
                } 
            }           
        }
    }
    return true;
}

//printBoard() is unnecessary
void Board::printBoard() {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            myPiece p = square[row][col].getPiece();
            myColor c = square[row][col].getColor();
            if (p == KING)
                (c == WHITE) ? cout << "KW " : cout << "KB ";
            else if (p == QUEEN)
                (c == WHITE) ? cout << "QW " : cout << "QB ";
            else if(p == BISHOP)
                (c == WHITE) ? cout << "BW " : cout << "BB ";
            else if (p == KNIGHT)
                (c == WHITE) ? cout << "NW " : cout << "NB ";
            else if(p == ROOK)
                (c == WHITE) ? cout << "RW " : cout << "RB ";
            else if (p == PAWN)
                (c == WHITE) ? cout << "PW " : cout << "PB ";
            else if (p == EMPTY)
                cout << "-- ";
        }
        cout << endl;
    }
}

Square* Board::getSquare(int x, int y) {
		return &square[y][x];
	}

