#ifndef PIECE_H
#define PIECE_H

#include <memory>
#include <string>
#include <utility>
#include "Player.h"
#include "Board.h"

class Player;
class ChessBoard;

// 定义棋子的类型
enum PieceType {
    QueenBee_id,
    Beetle_id,
    Spider_id,
    Ant_id,
    Grasshopper_id,
    Ladybug_id,
    Mosquito_id,
    Pillbug_id
};

class Piece {
public:
    Piece(int x, int y, int type, std::shared_ptr<Player> owner);
    
    void setPosition(int newX, int newY);
    int getPieceType() const;
    std::pair<int, int> getPosition() const;
    std::shared_ptr<Player> getOwner() const;
    void setOwner(std::shared_ptr<Player> newOwner);
    void setPieceType(int newPieceType);
    virtual std::string getName() const;
    
    // 验证棋子的移动
    virtual bool isValidMove(int toX, int toY, const ChessBoard& board) const = 0;

protected:
    int x, y;  // 棋子的位置
    int pieceType;  // 棋子的类型
    std::shared_ptr<Player> owner;  // 棋子的拥有者
};

// 各个棋子的类
class QueenBee : public Piece {
public:
    QueenBee(int x, int y, std::shared_ptr<Player> owner);
    bool isValidMove(int toX, int toY, const ChessBoard& board) const override;
    std::string getName() const override;
};

class Beetle : public Piece {
public:
    Beetle(int x, int y, std::shared_ptr<Player> owner);
    bool isValidMove(int toX, int toY, const ChessBoard& board) const override;
    std::string getName() const override;
};

class Grasshopper : public Piece {
public:
    Grasshopper(int x, int y, std::shared_ptr<Player> owner);
    bool isValidMove(int toX, int toY, const ChessBoard& board) const override;
    std::string getName() const override;
};

class Spider : public Piece {
public:
    Spider(int x, int y, std::shared_ptr<Player> owner);
    bool isValidMove(int toX, int toY, const ChessBoard& board) const override;
    std::string getName() const override;
};

class Ant : public Piece {
public:
    Ant(int x, int y, std::shared_ptr<Player> owner);
    bool isValidMove(int toX, int toY, const ChessBoard& board) const override;
    std::string getName() const override;
};

class Ladybug : public Piece {
public:
    Ladybug(int x, int y, std::shared_ptr<Player> owner);
    bool isValidMove(int toX, int toY, const ChessBoard& board) const override;
    std::string getName() const override;
};

class Mosquito : public Piece {
public:
    Mosquito(int x, int y, std::shared_ptr<Player> owner);
    bool isValidMove(int toX, int toY, const ChessBoard& board) const override;
    std::string getName() const override;
};

class Pillbug : public Piece {
public:
    Pillbug(int x, int y, std::shared_ptr<Player> owner);
    bool isValidMove(int toX, int toY, const ChessBoard& board) const override;
    std::string getName() const override;
};

#endif // PIECE_H
