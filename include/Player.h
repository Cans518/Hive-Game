#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include "Board.h"

class ChessBoard;

class Player : public std::enable_shared_from_this<Player> {
public:
    static const std::map<std::string, int> DLC_pieceCount;
    static const std::map<std::string, int> noDLC_pieceCount;

    Player(const std::string& name, const std::map<std::string, int>& pieceCount);
    Player(const std::string& name);

    std::string getName() const;
    void placePiece(int pieceType, int x, int y, ChessBoard& board);
    void movePiece(int fromX, int fromY, int toX, int toY, const std::string& pieceType, ChessBoard& board);
    
    // 统一的特殊能力函数入口
    void useSpecialAbility(const std::string& abilityName, int x, int y, ChessBoard& board, int targetX = -1, int targetY = -1);

    void displayPieceCount() const;
    std::pair<int, int> getQueenBeePosition() const;

private:
    std::string name;
    std::map<std::string, int> pieceCount;
    bool isQueenBeePlaced;
    std::pair<int, int> queenBeePosition;

    void validatePiecePlacement(const std::string& pieceName) const;
    void useMosquitoAbility(int mosquitoX, int mosquitoY, ChessBoard& board);
    void useTidewormAbility(int tidewormX, int tidewormY, int targetX, int targetY, ChessBoard& board);
    void moveLadybug(int fromX, int fromY, int toX, int toY, ChessBoard& board);
};

#endif // PLAYER_H
