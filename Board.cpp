#include "Board.h"
#include <iostream>

// 静态成员变量初始化
std::shared_ptr<ChessBoard> ChessBoard::instance = nullptr;

// 私有构造函数
ChessBoard::ChessBoard() {
    board.resize(size, std::vector<std::vector<std::shared_ptr<Piece>>>(size));
}

// 获取唯一实例
std::shared_ptr<ChessBoard> ChessBoard::getInstance() {
    if (!instance) {
        instance = std::shared_ptr<ChessBoard>(new ChessBoard());
    }
    return instance;
}

// 检查坐标是否在棋盘范围内
bool ChessBoard::isWithinBounds(int x, int y) const {
    return x >= 0 && x < size && y >= 0 && y < size;
}

// 放置棋子
void ChessBoard::placePiece(int x, int y, uint8_t pieceType, std::shared_ptr<Player> player) {
    if (!isWithinBounds(x, y)) {
        throw std::out_of_range("Coordinates out of bounds.");
    }

    std::shared_ptr<Piece> newPiece;
    int pieceId = pieceType;
    switch (pieceId) {
        case PieceType::QueenBee_id:
            newPiece = std::make_shared<QueenBee>(x, y, player);
            break;
        case PieceType::Beetle_id:
            newPiece = std::make_shared<Beetle>(x, y, player);
            break;
        case PieceType::Grasshopper_id:
            newPiece = std::make_shared<Grasshopper>(x, y, player);
            break;
        case PieceType::Spider_id:
            newPiece = std::make_shared<Spider>(x, y, player);
            break;
        case PieceType::Ant_id:
            newPiece = std::make_shared<Ant>(x, y, player);
            break;
        case PieceType::Ladybug_id:
            newPiece = std::make_shared<Ladybug>(x, y, player);
            break;
        case PieceType::Mosquito_id:
            newPiece = std::make_shared<Mosquito>(x, y, player);
            break;
        case PieceType::Pillbug_id:
            newPiece = std::make_shared<Pillbug>(x, y, player);
            break;
        default:
            throw std::invalid_argument("Invalid piece type.");
    }

    // 如果是空位置或者放置的是 Beetle，可以放置棋子
    if (board[x][y].empty() || newPiece->getPieceType() == PieceType::Beetle_id) {
        board[x][y].push_back(newPiece);
    } else {
        throw std::runtime_error("Cannot place piece here.");
    }
}

// 移动棋子
void ChessBoard::movePiece(int fromX, int fromY, int toX, int toY, const std::string& pieceType, std::shared_ptr<Player> player) {
    if (!isWithinBounds(fromX, fromY) || !isWithinBounds(toX, toY)) {
        throw std::out_of_range("Coordinates out of bounds.");
    }

    if (board[fromX][fromY].empty() || board[fromX][fromY].back()->getName() != pieceType) {
        throw std::invalid_argument("No piece of specified type at the given position.");
    }

    auto piece = board[fromX][fromY].back();
    if (!piece->isValidMove(toX, toY, *this)) {
        throw std::runtime_error("Invalid move for this piece type.");
    }

    board[fromX][fromY].pop_back();
    placePiece(toX, toY, piece->getPieceType(), player);
}

// 显示棋盘
void ChessBoard::displayBoard() const {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (!board[i][j].empty() && board[i][j].back()) {
                std::cout << board[i][j].back()->getName() << " ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << std::endl;
    }
}

// 检查某个位置是否有相邻棋子
bool ChessBoard::hasAdjacentPiece(int x, int y) const {
    const int directions[6][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, -1}, {-1, 1}};
    for (const auto& dir : directions) {
        int adjX = x + dir[0];
        int adjY = y + dir[1];
        // 如果相邻的位置在棋盘范围内，并且有棋子，则返回 true
        if (isWithinBounds(adjX, adjY) && !board[adjX][adjY].empty()) {
            return true;
        }
    }
    return false;
}

// 查看棋子周围所有棋子
std::vector<std::shared_ptr<Piece>> ChessBoard::getAdjacentPieces(int x, int y) const {
    std::vector<std::shared_ptr<Piece>> adjacentPieces;
    const int directions[6][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, -1}, {-1, 1}};
    for (const auto& dir : directions) {
        int adjX = x + dir[0];
        int adjY = y + dir[1];
        if (isWithinBounds(adjX, adjY) && !board[adjX][adjY].empty()) {
            adjacentPieces.push_back(board[adjX][adjY].back());
        }
    }
    return adjacentPieces;
}

// 检查某个位置是否被包围
bool ChessBoard::isPositionSurrounded(int x, int y, bool considerEdge) const {
    const int directions_ji[6][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {-1, 1}};
    const int directions_ou[6][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, -1}, {-1, -1}};
    if (y%2 == 0){
        for (const auto& dir : directions_ou) {
            int adjX = x + dir[0];
            int adjY = y + dir[1];
            if (isWithinBounds(adjX, adjY)) {
                if (board[adjX][adjY].empty()) {
                    return false; // 如果某个方向没有棋子，则位置未被完全包围
                }
            } else if (!considerEdge) {
                return false; // 如果不考虑边缘，边缘视为未被包围
            }
        }
    }
    else{
        for (const auto& dir : directions_ji) {
            int adjX = x + dir[0];
            int adjY = y + dir[1];
            if (isWithinBounds(adjX, adjY)) {
                if (board[adjX][adjY].empty()) {
                    return false; // 如果某个方向没有棋子，则位置未被完全包围
                }
            } else if (!considerEdge) {
                return false; // 如果不考虑边缘，边缘视为未被包围
            }
        }
    }
    return true; // 所有方向都有棋子，或者边缘被视为封闭，位置被完全包围
}

// 清空棋盘
void ChessBoard::clearBoard() {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            board[i][j].clear();  // 清空每个位置的棋子堆栈
        }
    }
    std::cout << "The board has been cleared." << std::endl;
}

std::shared_ptr<Piece> ChessBoard::getPieceAt(int x, int y) const {
    if (!isWithinBounds(x, y) || board[x][y].empty()) {
        return nullptr;  // 如果位置不在边界内或没有棋子，返回空指针
    }
    return board[x][y].back();  // 返回堆栈顶部的棋子
}
