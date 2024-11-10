#include "Piece.h"

Piece::Piece(int x, int y, int type, std::shared_ptr<Player> owner)
    : x(x), y(y), pieceType(type), owner(owner) {}


void Piece::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
}

int Piece::getPieceType() const {
    return pieceType;
}

std::pair<int, int> Piece::getPosition() const {
    return std::make_pair(x, y);
}

std::shared_ptr<Player> Piece::getOwner() const {
    return owner;
}

void Piece::setOwner(std::shared_ptr<Player> newOwner) {
    owner = newOwner;
}

void Piece::setPieceType(int newPieceType) {
    pieceType = newPieceType;
}

std::string Piece::getName() const {
    return "Unknown Piece";
}

bool Piece::isValidMove(int toX, int toY, const ChessBoard& board) const {
    // 默认的移动验证逻辑
    return toX >= 0 && toY >= 0 && toX < board.getWidth() && toY < board.getHeight();
}

// 各个子类的实现...

// QueenBee类的实现
QueenBee::QueenBee(int x, int y, std::shared_ptr<Player> owner)
    : Piece(x, y, PieceType::QueenBee_id, owner) {}

bool QueenBee::isValidMove(int toX, int toY, const ChessBoard& board) const {
    int dx = abs(toX - x);
    int dy = abs(toY - y);
    return (dx + dy == 1 || (dx == 1 && dy == 1)) && board.hasAdjacentPiece(toX, toY);
}

std::string QueenBee::getName() const {
    return "QueenBee";
}

// Beetle类的实现
Beetle::Beetle(int x, int y, std::shared_ptr<Player> owner)
    : Piece(x, y, PieceType::Beetle_id, owner) {}

bool Beetle::isValidMove(int toX, int toY, const ChessBoard& board) const {
    return (abs(toX - x) <= 1 && abs(toY - y) <= 1) && board.hasAdjacentPiece(toX, toY);
}

std::string Beetle::getName() const {
    return "Beetle";
}

// Grasshopper类的实现
Grasshopper::Grasshopper(int x, int y, std::shared_ptr<Player> owner)
    : Piece(x, y, PieceType::Grasshopper_id, owner) {}

bool Grasshopper::isValidMove(int toX, int toY, const ChessBoard& board) const {
    if (!board.hasAdjacentPiece(toX, toY)) return false;

    if (x == toX) {
        int step = (toY > y) ? 1 : -1;
        for (int j = y + step; j != toY; j += step) {
            if (board.getBoard()[x][j].empty()) return false;
        }
    } else if (y == toY) {
        int step = (toX > x) ? 1 : -1;
        for (int i = x + step; i != toX; i += step) {
            if (board.getBoard()[i][y].empty()) return false;
        }
    } else {
        return false;
    }
    return true;
}

std::string Grasshopper::getName() const {
    return "Grasshopper";
}

// Spider类的实现
Spider::Spider(int x, int y, std::shared_ptr<Player> owner)
    : Piece(x, y, PieceType::Spider_id, owner) {}

bool Spider::isValidMove(int toX, int toY, const ChessBoard& board) const {
    int distance = abs(toX - x) + abs(toY - y);
    return distance == 4 && board.hasAdjacentPiece(toX, toY);
}

std::string Spider::getName() const {
    return "Spider";
}

// Ant类的实现
Ant::Ant(int x, int y, std::shared_ptr<Player> owner)
    : Piece(x, y, PieceType::Ant_id, owner) {}

bool Ant::isValidMove(int toX, int toY, const ChessBoard& board) const {
    return board.hasAdjacentPiece(toX, toY);
}

std::string Ant::getName() const {
    return "Ant";
}

// Ladybug类的实现
Ladybug::Ladybug(int x, int y, std::shared_ptr<Player> owner)
    : Piece(x, y, PieceType::Ladybug_id, owner) {}

bool Ladybug::isValidMove(int toX, int toY, const ChessBoard& board) const {
    int distance = abs(toX - x) + abs(toY - y);
    return distance == 4 && board.hasAdjacentPiece(toX, toY);
}

std::string Ladybug::getName() const {
    return "Ladybug";
}

// Mosquito类的实现
Mosquito::Mosquito(int x, int y, std::shared_ptr<Player> owner)
    : Piece(x, y, PieceType::Mosquito_id, owner) {}

bool Mosquito::isValidMove(int toX, int toY, const ChessBoard& board) const {
    return board.hasAdjacentPiece(toX, toY);
}

std::string Mosquito::getName() const {
    return "Mosquito";
}

// Pillbug类的实现
Pillbug::Pillbug(int x, int y, std::shared_ptr<Player> owner)
    : Piece(x, y, PieceType::Pillbug_id, owner) {}

bool Pillbug::isValidMove(int toX, int toY, const ChessBoard& board) const {
    return (abs(toX - x) <= 1 && abs(toY - y) <= 1) && board.hasAdjacentPiece(toX, toY);
}
std::string Pillbug::getName() const {
    return "Pillbug";
}

