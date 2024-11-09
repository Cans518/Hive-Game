#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "Player.h"
#include <memory>
#include <string>
#include <iostream>

class GameWindow;  // 前向声明，以避免循环依赖

class Game {
public:
    // 获取 Game 单例实例
    static std::shared_ptr<Game> getInstance();

    // 初始化游戏
    void initializeGame(const std::string &player1Name, const std::string &player2Name, bool isDLC = false, bool isAI = false);

    // 重新开始游戏
    void restartGame();

    // 检查游戏是否结束
    bool checkGameOver();

    // 获取当前回合数
    int getTurnCount() const;

    // 获取当前玩家
    std::shared_ptr<Player> getCurrentPlayer() const;

    // 获取游戏胜利者
    std::shared_ptr<Player> getWinner() const;

    // 执行游戏回合（用于UI）
    void playTurnUI(GameWindow *gameWindow);

    // 获取指定位置的棋子
    std::shared_ptr<Piece> getPieceAt(int x, int y) const;

    std::shared_ptr<ChessBoard> getBoard()const;

    std::shared_ptr<Player> getPlayer(int num)const;

    // 显示棋盘（用于调试）
    void displayBoard() const;

private:
    Game();  // 私有构造函数，单例模式
    static std::shared_ptr<Game> instance;

    // 切换玩家
    void switchPlayer();

    std::shared_ptr<ChessBoard> board;       // 棋盘实例
    std::shared_ptr<Player> player1;         // 玩家1
    std::shared_ptr<Player> player2;         // 玩家2
    std::shared_ptr<Player> currentPlayer;   // 当前玩家

    int turnCount;   // 当前回合数
    bool isGameOver; // 游戏是否结束
    bool AIPlayer;
    void makeAIMove(GameWindow* gameWindow);
};

#endif // GAME_H
