#ifndef BOARDUI_H
#define BOARDUI_H

#include <QWidget>
#include <QGroupBox>
#include <QVector>
#include <memory>
#include "HexagonWidget.h"
#include "Board.h"
#include "Piece.h"
#include "Player.h"
#include <vector>
#include <QLabel>

class BoardUI : public QWidget {
    Q_OBJECT

public:
    explicit BoardUI(int rows = 10, int cols = 10, int hexWidgetSize = 60, QWidget *parent = nullptr);

    // 用于更新棋盘图形界面的函数
    void updateBoardUI(std::shared_ptr<ChessBoard> chessBoard);

private:
    int rows;
    int cols;
    int hexWidgetSize;

     std::vector<std::vector<std::shared_ptr<QLabel>>> labels;

    QVector<QVector<HexagonWidget*>> hexWidgetMatrix;

    // 创建六边形网格布局
    void createHexGrid(QGroupBox *groupBox);
};

#endif // BOARDUI_H
