#include "MCTS.h"
#include <algorithm>
#include <random>
#include <thread>
#include <future>

// MCTSNode 构造函数
MCTSNode::MCTSNode(std::shared_ptr<ChessBoard> board, std::shared_ptr<Player> player, MCTSNode* parent)
    : board(board), player(player), parent(parent), wins(0), visits(0) {}

// 获取 UCB1 值
double MCTSNode::getUCB1(double explorationParam) const {
    if (visits == 0) {
        return std::numeric_limits<double>::infinity();
    }
    return static_cast<double>(wins) / visits + explorationParam * std::sqrt(std::log(parent->visits) / visits);
}

扩展节点
void MCTSNode::expand() {
    if (!children.empty()) return; // 已经扩展过

    auto legalMoves = board->getLegalMoves(player); // 获取所有合法的移动
    for (const auto& move : legalMoves) {
        auto newBoard = std::make_shared<ChessBoard>(*board); // 复制当前棋盘
        newBoard->makeMove(move, player); // 执行移动
        children.emplace_back(new MCTSNode(newBoard, player->getOpponent(), this));
    }
}

// 随机选择一个子节点
MCTSNode* MCTSNode::selectRandomChild() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, children.size() - 1);
    return &children[dis(gen)];
}

// 选择 UCB1 最高的子节点
MCTSNode* MCTSNode::selectBestChild() {
    return *std::max_element(children.begin(), children.end(), [](const MCTSNode& a, const MCTSNode& b) {
        return a.getUCB1() < b.getUCB1();
    });
}

// 模拟游戏直到结束
int MCTSNode::simulate() {
    auto simBoard = std::make_shared<ChessBoard>(*board);
    auto simPlayer = player;

    while (!simBoard->isGameOver()) {
        auto legalMoves = simBoard->getLegalMoves(simPlayer);
        if (legalMoves.empty()) break;

        // 随机选择一个移动
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, legalMoves.size() - 1);
        simBoard->makeMove(legalMoves[dis(gen)], simPlayer);

        simPlayer = simPlayer->getOpponent();
    }

    return simBoard->getWinner() == player ? 1 : 0;
}

// 反向传播
void MCTSNode::backpropagate(int result) {
    visits++;
    wins += result;
    if (parent) {
        parent->backpropagate(result);
    }
}

// 判断是否已完全扩展
bool MCTSNode::isFullyExpanded() const {
    return !children.empty();
}

// 判断是否为终止节点
bool MCTSNode::isTerminal() const {
    return board->isGameOver();
}

// 获取当前节点的棋盘
std::shared_ptr<ChessBoard> MCTSNode::getBoard() const {
    return board;
}

// MCTS 构造函数
MCTS::MCTS(int simulations) : simulations(simulations) {}

// 计算棋盘状态的哈希值
size_t MCTS::computeBoardHash(const ChessBoard& board) const {
    return std::hash<std::string>{}(board.toString()); // 假设 ChessBoard 有 toString() 方法
}

// 执行 MCTS 一次迭代
void MCTS::runIteration(MCTSNode* root) {
    MCTSNode* node = root;

    // 1. 选择
    while (node->isFullyExpanded() && !node->isTerminal()) {
        node = node->selectBestChild();
    }

    // 2. 扩展
    if (!node->isTerminal()) {
        node->expand();
        node = node->selectRandomChild();
    }

    // 3. 模拟
    int result = node->simulate();

    // 4. 反向传播
    node->backpropagate(result);
}

// 为 AI 选择最佳移动
std::shared_ptr<ChessBoard> MCTS::findBestMove(std::shared_ptr<ChessBoard> board, std::shared_ptr<Player> player) {
    MCTSNode root(board, player);

    // 使用多线程并行执行模拟
    std::vector<std::future<void>> futures;
    for (int i = 0; i < simulations; ++i) {
        futures.push_back(std::async(std::launch::async, &MCTS::runIteration, this, &root));
    }

    // 等待所有模拟完成
    for (auto& future : futures) {
        future.get();
    }

    // 选择访问次数最多的子节点作为最终选择
    MCTSNode* bestMove = root.selectBestChild();
    return bestMove->getBoard();
}
