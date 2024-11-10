#include "Game.h"
#include <iostream>
#include <stdexcept>

// 静态成员变量初始化
std::shared_ptr<Game> Game::instance = nullptr;

// 私有构造函数
Game::Game() : isGameOver(false), turnCount(1) {
    board = ChessBoard::getInstance();
}

// 获取唯一实例
std::shared_ptr<Game> Game::getInstance() {
    if (!instance) {
        instance = std::shared_ptr<Game>(new Game());
    }
    return instance;
}

// 初始化游戏，接受玩家名字作为参数
void Game::initializeGame(const std::string& player1Name, const std::string& player2Name) {
    // 创建两个玩家，使用传入的名字
    player1 = std::make_shared<Player>(player1Name);
    player2 = std::make_shared<Player>(player2Name);

    // 设置当前玩家为 player1
    currentPlayer = player1;

    // 重置游戏状态
    isGameOver = false;
    turnCount = 1;  // 重置回合数

    std::cout << "Game initialized. " << player1Name << " starts." << std::endl;
}

// 重新开始游戏
void Game::restartGame() {
    // 获取当前玩家的名字
    std::string player1Name = player1->getName();
    std::string player2Name = player2->getName();

    // 清空棋盘
    board = ChessBoard::getInstance();  // 获取棋盘实例
    board->clearBoard();

    // 重新初始化游戏状态，保持原有玩家名字
    initializeGame(player1Name, player2Name);

    std::cout << "Game has been restarted." << std::endl;
}

// 切换当前玩家
void Game::switchPlayer() {
    if (currentPlayer == player1) {
        currentPlayer = player2;
    } else {
        currentPlayer = player1;
        // 切换到 player1 表示一个完整的回合结束，增加回合数
        turnCount++;
    }
}

// 检查游戏是否结束
bool Game::checkGameOver() {
    // 检查两个玩家的 QueenBee 是否被完全包围
    bool player1QueenSurrounded = false;
    bool player2QueenSurrounded = false;

    // 遍历棋盘，检查 QueenBee 是否被包围
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            if (!board->getBoard()[i][j].empty()) {
                auto piece = board->getBoard()[i][j].back();
                if (piece->getPieceType() == PieceType::QueenBee_id) {
                    if (piece->getOwner() == player1) {
                        player1QueenSurrounded = board->isPositionSurrounded(i, j);
                    } else if (piece->getOwner() == player2) {
                        player2QueenSurrounded = board->isPositionSurrounded(i, j);
                    }
                }
            }
        }
    }

    if (player1QueenSurrounded || player2QueenSurrounded) {
        isGameOver = true;
        std::cout << "Game Over!" << std::endl;

        if (player1QueenSurrounded && player2QueenSurrounded) {
            std::cout << "It's a draw!" << std::endl;
        } else if (player1QueenSurrounded) {
            std::cout << "Player 2 wins!" << std::endl;
        } else {
            std::cout << "Player 1 wins!" << std::endl;
        }

        return true;
    }

    return false;
}

// 获取胜利者
std::shared_ptr<Player> Game::getWinner() const {
    if (isGameOver) {
        bool player1QueenSurrounded = false;
        bool player2QueenSurrounded = false;

        // 遍历棋盘，检查 QueenBee 是否被包围
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 10; ++j) {
                if (!board->getBoard()[i][j].empty()) {
                    auto piece = board->getBoard()[i][j].back();
                    if (piece->getPieceType() == PieceType::QueenBee_id) {
                        if (piece->getOwner() == player1) {
                            player1QueenSurrounded = board->isPositionSurrounded(i, j);
                        } else if (piece->getOwner() == player2) {
                            player2QueenSurrounded = board->isPositionSurrounded(i, j);
                        }
                    }
                }
            }
        }

        if (player1QueenSurrounded && !player2QueenSurrounded) {
            return player2;  // Player 2 wins
        } else if (player2QueenSurrounded && !player1QueenSurrounded) {
            return player1;  // Player 1 wins
        } else {
            return nullptr;  // It's a draw
        }
    }
    return nullptr;  // Game is not over
}

// 进行游戏回合
void Game::playTurn() {
    if (isGameOver) {
        std::cout << "Game is already over." << std::endl;
        return;
    }

    // 显示当前玩家
    std::cout << currentPlayer->getName() << "'s turn." << std::endl;

    // 获取玩家的输入（例如放置棋子或移动棋子）
    int x, y, toX, toY;
    int pieceType;
    char action;
    std::cout << "Enter action (P for place, M for move): ";
    std::cin >> action;

    if (action == 'P' || action == 'p') {
        std::cout << "Enter coordinates (x y) and piece type: ";
        std::cin >> x >> y >> pieceType;
        try {
            currentPlayer->placePiece(pieceType, x, y, *board);
        } catch (const std::exception& e) {
            std::cout << "Invalid move: " << e.what() << std::endl;
            return;  // 结束回合，等待下次输入
        }
    } else if (action == 'M' || action == 'm') {
        std::cout << "Enter from coordinates (x y) and to coordinates (toX toY): ";
        std::cin >> x >> y >> toX >> toY;
        std::string pieceName;
        std::cout << "Enter piece name to move: ";
        std::cin >> pieceName;
        try {
            board->movePiece(x, y, toX, toY, pieceName, currentPlayer);
        } catch (const std::exception& e) {
            std::cout << "Invalid move: " << e.what() << std::endl;
            return;  // 结束回合，等待下次输入
        }
    } else {
        std::cout << "Invalid action." << std::endl;
        return;
    }

    // 显示棋盘
    displayBoard();

    // 显示当前回合数
    displayTurnCount();

    // 检查游戏是否结束
    if (checkGameOver()) {
        std::shared_ptr<Player> winner = getWinner();
        if (winner) {
            std::cout << winner->getName() << " wins the game!" << std::endl;
        } else {
            std::cout << "The game ended in a draw!" << std::endl;
        }
        return;
    }

    // 切换到下一个玩家
    switchPlayer();
}

// 显示当前棋盘
void Game::displayBoard() const {
    board->displayBoard();
}

// 显示当前回合数
void Game::displayTurnCount() const {
    std::cout << "Current Turn: " << turnCount << std::endl;
}

// 结束游戏
void Game::endGame() {
    isGameOver = true;
    std::cout << "Game has ended." << std::endl;
}

// 获取当前玩家
std::shared_ptr<Player> Game::getCurrentPlayer() const {
    return currentPlayer;
}
