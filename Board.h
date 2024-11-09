#ifndef BOARD_H
#define BOARD_H

#include <memory>
#include <vector>
#include "Piece.h"

class Player;
class Piece;

class ChessBoard {
private:
    static const int size = 10;
    std::vector<std::vector<std::vector<std::shared_ptr<Piece>>>> board;

    // 静态成员变量，存储唯一实例
    static std::shared_ptr<ChessBoard> instance;

    // 私有化构造函数，防止外部创建实例
    ChessBoard();

    // 私有化拷贝构造函数和赋值操作符，防止拷贝和赋值
    ChessBoard(const ChessBoard&) = delete;
    ChessBoard& operator=(const ChessBoard&) = delete;



public:
    // 提供静态方法获取唯一实例
    static std::shared_ptr<ChessBoard> getInstance();

    // 辅助函数，用于检查某个位置是否在棋盘范围内
    bool isWithinBounds(int x, int y) const;


    int getWidth()const{return size;}
    int getHeight()const{return size;}

    std::shared_ptr<Piece> getPieceAt(int x, int y) const;

    std::vector<std::shared_ptr<Piece>> getAdjacentPieces(int x, int y) const;
    
    // 放置棋子
    void placePiece(int x, int y, uint8_t pieceType, std::shared_ptr<Player> player);
    
    // 移动棋子
    void movePiece(int fromX, int fromY, int toX, int toY, const std::string& pieceType, std::shared_ptr<Player> player);
    
    // 显示棋盘
    void displayBoard() const;
    
    // 检查某个位置是否有相邻棋子
    bool hasAdjacentPiece(int x, int y) const;
    
    // 检查位置是否被包围
    bool isPositionSurrounded(int x, int y, bool considerEdge = false) const;

    // 清空棋盘
    void clearBoard();

    const std::vector<std::vector<std::vector<std::shared_ptr<Piece>>>>& getBoard() const { return board; }
};

#endif // BOARD_H
