#include "Player.h"
#include "Board.h"
#include "Piece.h"

// 初始化 DLC 和非 DLC 的棋子数量
const std::map<std::string, int> Player::DLC_pieceCount = {
    {"QueenBee", 1}, {"Beetle", 2}, {"Spider", 2}, {"Ant", 3},
    {"Grasshopper", 3}, {"Ladybug", 1}, {"Mosquito", 1}, {"Pillbug", 1}
};

const std::map<std::string, int> Player::noDLC_pieceCount = {
    {"QueenBee", 1}, {"Beetle", 2}, {"Spider", 2}, {"Ant", 3}, {"Grasshopper", 3}
};

Player::Player(const std::string& name, const std::map<std::string, int>& pieceCount)
    : name(name), pieceCount(pieceCount), isQueenBeePlaced(false), queenBeePosition(-1, -1) {}

Player::Player(const std::string& name)
    : name(name), pieceCount(noDLC_pieceCount), isQueenBeePlaced(false), queenBeePosition(-1, -1) {}

std::string Player::getName() const {
    return name;
}

// 验证棋子是否可以放置
void Player::validatePiecePlacement(const std::string& pieceName) const {
    auto it = pieceCount.find(pieceName);
    if (it == pieceCount.end() || it->second <= 0) {
        throw std::runtime_error("No " + pieceName + " pieces left to place.");
    }
}

void Player::placePiece(int pieceType, int x, int y, ChessBoard& board) {
    std::string pieceName;

    // 根据 pieceType 获取棋子名称
    switch (pieceType) {
        case PieceType::QueenBee_id:
            pieceName = "QueenBee";
            break;
        case PieceType::Beetle_id:
            pieceName = "Beetle";
            break;
        case PieceType::Spider_id:
            pieceName = "Spider";
            break;
        case PieceType::Ant_id:
            pieceName = "Ant";
            break;
        case PieceType::Grasshopper_id:
            pieceName = "Grasshopper";
            break;
        case PieceType::Ladybug_id:
            pieceName = "Ladybug";
            break;
        case PieceType::Mosquito_id:
            pieceName = "Mosquito";
            break;
        case PieceType::Pillbug_id:
            pieceName = "Pillbug";
            break;
        default:
            throw std::invalid_argument("Invalid piece type.");
    }

    // 检查棋子是否可以放置
    validatePiecePlacement(pieceName);

    // 在棋盘上放置棋子
    board.placePiece(x, y, pieceType, shared_from_this());

    // 更新棋子信息
    pieceCount[pieceName]--;
    if (pieceName == "QueenBee") {
        isQueenBeePlaced = true;
        queenBeePosition = {x, y};
    }
}

void Player::movePiece(int fromX, int fromY, int toX, int toY, const std::string& pieceType, ChessBoard& board) {
    if (!isQueenBeePlaced) {
        throw std::runtime_error("You must place the QueenBee before moving other pieces.");
    }

    board.movePiece(fromX, fromY, toX, toY, pieceType, shared_from_this());

    if (pieceType == "QueenBee") {
        queenBeePosition = {toX, toY};
    }
}

void Player::useSpecialAbility(const std::string& abilityName, int x, int y, ChessBoard& board, int targetX, int targetY) {
    if (abilityName == "Mosquito") {
        useMosquitoAbility(x, y, board);
    } else if (abilityName == "Tideworm") {
        useTidewormAbility(x, y, targetX, targetY, board);
    } else if (abilityName == "Ladybug") {
        moveLadybug(x, y, targetX, targetY, board);
    } else {
        throw std::invalid_argument("Invalid special ability.");
    }
}

void Player::useMosquitoAbility(int mosquitoX, int mosquitoY, ChessBoard& board) {
    auto mosquito = board.getPieceAt(mosquitoX, mosquitoY);
    if (!mosquito || mosquito->getName() != "Mosquito") {
        throw std::runtime_error("No Mosquito at the specified position.");
    }

    auto adjacentPieces = board.getAdjacentPieces(mosquitoX, mosquitoY);
    if (adjacentPieces.empty()) {
        throw std::runtime_error("No adjacent pieces to mimic.");
    }

    // 选择第一个相邻棋子的能力
    auto chosenPiece = adjacentPieces[0];
    mosquito->setPieceType(chosenPiece->getPieceType());
}

void Player::useTidewormAbility(int tidewormX, int tidewormY, int targetX, int targetY, ChessBoard& board) {
    auto tideworm = board.getPieceAt(tidewormX, tidewormY);
    if (!tideworm || tideworm->getName() != "Tideworm") {
        throw std::runtime_error("No Tideworm at the specified position.");
    }

    auto targetPiece = board.getPieceAt(targetX, targetY);
    if (!targetPiece || targetPiece->getName() == "Tideworm") {
        throw std::runtime_error("Invalid target piece to move.");
    }

    // 使用潮虫能力移动棋子
    board.movePiece(targetX, targetY, targetX + 2, targetY, targetPiece->getName(), shared_from_this());
}

void Player::moveLadybug(int fromX, int fromY, int toX, int toY, ChessBoard& board) {
    auto ladybug = board.getPieceAt(fromX, fromY);
    if (!ladybug || ladybug->getName() != "Ladybug") {
        throw std::runtime_error("No Ladybug at the specified position.");
    }

    if (!ladybug->isValidMove(toX, toY, board)) {
        throw std::runtime_error("Invalid move for Ladybug.");
    }

    board.movePiece(fromX, fromY, toX, toY, "Ladybug", shared_from_this());
}

void Player::displayPieceCount() const {
    std::cout << "Piece count for " << name << ":\n";
    for (const auto& [piece, count] : pieceCount) {
        std::cout << "  " << piece << ": " << count << "\n";
    }
}

std::pair<int, int> Player::getQueenBeePosition() const {
    if (!isQueenBeePlaced) {
        throw std::runtime_error("QueenBee has not been placed.");
    }
    return queenBeePosition;
}
