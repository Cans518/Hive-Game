#include <iostream>
#include "Game.h"
#include <cstdlib>  // For system()

// 清空屏幕的函数
void clearScreen() {
    #ifdef _WIN32
        system("cls");  // Windows 系统
    #else
        system("clear");  // Linux 和 macOS 系统
    #endif
}

int main() {
    // 获取游戏实例（单例模式）
    std::shared_ptr<Game> game = Game::getInstance();

    // 初始化游戏
    game->initializeGame("Romain","Leo");

    char choice;
    bool exitGame = false;

    // 游戏主循环
    while (!exitGame) {
        clearScreen();  // 清空屏幕

        // 显示当前回合数
        game->displayTurnCount();

        // 显示当前玩家的名字
        std::shared_ptr<Player> currentPlayer = game->getCurrentPlayer();
        std::cout << "Current Player: " << currentPlayer->getName() << std::endl;

        // 显示当前玩家的手牌情况
        std::cout << "Player's Hand (Remaining Pieces):" << std::endl;
        currentPlayer->displayPieceCount();

        // 显示当前棋盘
        game->displayBoard();

        // 显示菜单
        std::cout << "\n--- Main Menu ---\n";
        std::cout << "1. Play Turn\n";
        std::cout << "2. Restart Game\n";
        std::cout << "3. End Game\n";
        std::cout << "4. Exit\n";
        std::cout << "Choose an option: ";
        std::cin >> choice;

        switch (choice) {
            case '1':
                // 进行游戏回合
                game->playTurn();
                break;

            case '2':
                // 重新开始游戏
                game->restartGame();
                break;

            case '3':
                // 结束当前游戏
                game->endGame();
                break;

            case '4':
                // 退出游戏
                exitGame = true;
                std::cout << "Exiting the game. Goodbye!" << std::endl;
                break;

            default:
                std::cout << "Invalid option. Please choose between 1 and 4." << std::endl;
                break;
        }

        // 暂停一会儿以便用户查看输出
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore();  // 忽略之前输入的换行符
        std::cin.get();     // 等待用户按下 Enter 键
    }

    return 0;
}
