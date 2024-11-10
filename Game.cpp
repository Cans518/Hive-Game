#include "Game.h"
#include "GameWindow.h"
#include <iostream>

std::shared_ptr<Game> Game::instance = nullptr;

Game::Game() : isGameOver(false), turnCount(1) {
    board = ChessBoard::getInstance();
}

std::shared_ptr<Game> Game::getInstance() {
    if (!instance) {
        instance = std::shared_ptr<Game>(new Game());
    }
    return instance;
}

void Game::initializeGame(const std::string &player1Name, const std::string &player2Name, bool isDLC, bool isAI) {
    if(isDLC){
        player1 = std::make_shared<Player>(player1Name,Player::DLC_pieceCount);
        player2 = std::make_shared<Player>(player2Name,Player::DLC_pieceCount);
    }
    else{
        player1 = std::make_shared<Player>(player1Name);
        player2 = std::make_shared<Player>(player2Name);
    }

    currentPlayer = player1;
    isGameOver = false;
    turnCount = 1;
    AIPlayer = isAI;
}

void Game::makeAIMove(GameWindow* gameWindow) {
    static int run_time = 0;
    srand(static_cast<unsigned>(time(0)));
    while (true) {
        try {
            bool placePiece = rand() % 2;

            if (placePiece) {
                int x = rand() % 10;
                int y = rand() % 10;
                int pieceType = rand() % 8;
                currentPlayer->placePiece(pieceType, x, y, *board);
                gameWindow->updateBoardUI();
                break;
            } else {
                int fromX = rand() % 10;
                int fromY = rand() % 10;
                int toX = rand() % 10;
                int toY = rand() % 10;
                std::shared_ptr<Piece> piece = getPieceAt(fromX, fromY);
                if (piece && piece->getOwner() == currentPlayer) {
                    board->movePiece(fromX, fromY, toX, toY, piece->getName(), currentPlayer);
                    gameWindow->updateBoardUI();
                    run_time = 0;
                    break;
                }
                throw std::runtime_error("AI tried to move an invalid piece.");
            }
        } catch (const std::exception &) {
            run_time ++;
            continue;
        }
    }
}

void Game::restartGame() {
    std::string player1Name = player1->getName();
    std::string player2Name = player2->getName();
    board->clearBoard();
    initializeGame(player1Name, player2Name,AIPlayer);
}

bool Game::checkGameOver() {
    bool player1QueenSurrounded = false;
    bool player2QueenSurrounded = false;

    if (player1->isQueenBeePlaced)
        player1QueenSurrounded = board->isPositionSurrounded(player1->getQueenBeePosition().first,player1->getQueenBeePosition().second);
    if (player2->isQueenBeePlaced)
        player2QueenSurrounded = board->isPositionSurrounded(player2->getQueenBeePosition().first,player2->getQueenBeePosition().second);

    if (player1QueenSurrounded || player2QueenSurrounded) {
        isGameOver = true;
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

int Game::getTurnCount() const {
    return turnCount;
}

std::shared_ptr<Player> Game::getCurrentPlayer() const {
    return currentPlayer;
}

std::shared_ptr<Player> Game::getWinner() const {
    if (isGameOver) {
        bool player1QueenSurrounded = false;
        bool player2QueenSurrounded = false;

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
            return player2;
        } else if (player2QueenSurrounded && !player1QueenSurrounded) {
            return player1;
        } else {
            return nullptr;
        }
    }
    return nullptr;
}

void Game::playTurnUI(GameWindow *gameWindow) {
    checkGameOver();
    if (isGameOver) {
        gameWindow->showMessage("Game Over", "Game is already over.");
        return;
    }

    std::shared_ptr<Player> currentPlayer = getCurrentPlayer();
    QString currentPlayerName = QString::fromStdString(currentPlayer->getName());
    gameWindow->updateTurnInfo(turnCount, currentPlayerName);

    if (currentPlayer == player2 && AIPlayer) {
        // 如果玩家二是AI，自动进行AI操作
        makeAIMove(gameWindow);
    }else{
        bool ok;
        QString action = gameWindow -> showInputDialog("操作选择", "P是放置，M是移动", "P", &ok);
        if (!ok || action.isEmpty()) return;

        if (action.toLower() == "p") {
            int x = gameWindow->showIntegerInputDialog("Place Piece", "Enter x coordinate:", 0, 0, 9, 1, &ok);
            int y = gameWindow->showIntegerInputDialog("Place Piece", "Enter y coordinate:", 0, 0, 9, 1, &ok);
            int pieceType = gameWindow->showIntegerInputDialog("Place Piece", "Enter piece type ID:", 0, 0, 7, 1, &ok);

            if (turnCount == 4 && !player1->isQueenBeePlaced && currentPlayer == player1 && pieceType != 0){
                gameWindow->showMessage("Invalid Move", "You must place your QueenBee befor 4.");
                return;
            }
            if (turnCount == 4 && !player2->isQueenBeePlaced && currentPlayer == player2 && pieceType != 0){
                gameWindow->showMessage("Invalid Move", "You must place your QueenBee befor 4.");
                return;
            }

            try {
                currentPlayer->placePiece(pieceType, x, y, *board);
                gameWindow->updateBoardUI();
            } catch (const std::exception &e) {
                gameWindow->showMessage("Invalid Move", e.what());
                return;
            }
        } else if (action.toLower() == "m") {
            if (!currentPlayer->isQueenBeePlaced){
                gameWindow->showMessage("Invalid Move", "Only place your QueenBee, \nyou can move other pieces.");
                return;
            }

            int fromX = gameWindow->showIntegerInputDialog( "Move Piece", "Enter from x coordinate:", 0, 0, 9, 1, &ok);
            int fromY = gameWindow->showIntegerInputDialog("Move Piece", "Enter from y coordinate:", 0, 0, 9, 1, &ok);
            int toX = gameWindow->showIntegerInputDialog( "Move Piece", "Enter to x coordinate:", 0, 0, 9, 1, &ok);
            int toY = gameWindow->showIntegerInputDialog( "Move Piece", "Enter to y coordinate:", 0, 0, 9, 1, &ok);
            QString pieceName = gameWindow -> showInputDialog("移动棋子", "输入需要移动的棋子名称", "", &ok);

            if (turnCount == 4 && !player1->isQueenBeePlaced && currentPlayer == player1){
                gameWindow->showMessage("Invalid Move", "You must place your QueenBee befor 4.");
                return;
            }
            if (turnCount == 4 && !player2->isQueenBeePlaced && currentPlayer == player2){
                gameWindow->showMessage("Invalid Move", "You must place your QueenBee befor 4.");
                return;
            }

            try {
                board->movePiece(fromX, fromY, toX, toY, pieceName.toStdString(), currentPlayer);
                gameWindow->updateBoardUI();
            } catch (const std::exception &e) {
                gameWindow->showMessage("Invalid Move", e.what());
                return;
            }
        } else {
            gameWindow->showMessage("Invalid Action", "Invalid action. Please choose P or M.");
            return;
        }
    }
    if (checkGameOver()) {
        std::shared_ptr<Player> winner = getWinner();
        QString winnerMessage = winner ? QString("%1 wins the game!").arg(QString::fromStdString(winner->getName())) : "The game ended in a draw!";
        gameWindow->showMessage("Game Over", winnerMessage);
        return;
    }
    switchPlayer();
}

void Game::switchPlayer() {
    if (currentPlayer == player1) {
        currentPlayer = player2;
    } else {
        currentPlayer = player1;
        turnCount++;
    }
}

std::shared_ptr<Piece> Game::getPieceAt(int x, int y) const {
    if (board->isWithinBounds(x, y)) {
        return board->getBoard()[x][y].empty() ? nullptr : board->getBoard()[x][y].back();
    }
    return nullptr;
}

void Game::displayBoard() const {
    board->displayBoard();
}

std::shared_ptr<ChessBoard> Game::getBoard() const {
    return board;
}

std::shared_ptr<Player> Game::getPlayer(int num) const {
    switch(num){
        case 0: return currentPlayer;
        case 1: return player1;
        case 2: return player2;
    }

    return nullptr;
}
