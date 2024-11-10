#ifndef MCTS_H
#define MCTS_H

#include <memory>
#include <vector>
#include <cmath>
#include <limits>
#include <random>
#include <unordered_map>
#include <mutex>
#include "Board.h"
#include "Player.h"

// MCTS 节点类
class MCTSNode {
public:
    MCTSNode(std::shared_ptr<ChessBoard> board, std::shared_ptr<Player> player, MCTSNode* parent = nullptr);

    // 获取 UCB1 值
    double getUCB1(double explorationParam = 1.41) const;

    // 扩展节点
    void expand();

    // 随机选择一个子节点
    MCTSNode* selectRandomChild();

    // 选择 UCB1 最高的子节点
    MCTSNode* selectBestChild();

    // 模拟游戏直到结束
    int simulate();

    // 反向传播
    void backpropagate(int result);

    // 判断是否已完全扩展
    bool isFullyExpanded() const;

    // 判断是否为终止节点
    bool isTerminal() const;

    // 获取当前节点的棋盘
    std::shared_ptr<ChessBoard> getBoard() const;

private:
    std::shared_ptr<ChessBoard> board;
    std::shared_ptr<Player> player;
    MCTSNode* parent;
    std::vector<MCTSNode> children;
    int wins;
    int visits;
};

// MCTS 搜索类
class MCTS {
public:
    MCTS(int simulations = 1000);

    // 为 AI 选择最佳移动
    std::shared_ptr<ChessBoard> findBestMove(std::shared_ptr<ChessBoard> board, std::shared_ptr<Player> player);

private:
    int simulations;
    std::unordered_map<size_t, MCTSNode*> transpositionTable; // 棋盘状态缓存
    std::mutex mtx; // 线程安全

    // 计算棋盘状态的哈希值
    size_t computeBoardHash(const ChessBoard& board) const;

    // 执行 MCTS 一次迭代
    void runIteration(MCTSNode* root);
};

#endif // MCTS_H
