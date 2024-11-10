#include "BoardUI.h"
#include <QVBoxLayout>
#include <QLabel>

BoardUI::BoardUI(int rows, int cols, int hexWidgetSize, QWidget *parent)
    : QWidget(parent), rows(rows), cols(cols), hexWidgetSize(hexWidgetSize) {

    QGroupBox *groupBox = new QGroupBox("Hive Game", this);
    int groupBoxWidth = cols * (hexWidgetSize * 1.1) + hexWidgetSize;
    int groupBoxHeight = rows * (hexWidgetSize * 0.9) + hexWidgetSize;
    groupBox->setFixedSize(groupBoxWidth, groupBoxHeight);

    // Initialize the hex grid
    createHexGrid(groupBox);

    // Set up the main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(groupBox);
    setLayout(mainLayout);
}

void BoardUI::createHexGrid(QGroupBox *groupBox) {
    const int verticalOffset = hexWidgetSize * 0.9;
    const int horizontalOffset = hexWidgetSize * 1.1;

    // Resize the labels vector to fit the rows and columns
    labels.resize(rows);

    for (int row = 0; row < rows; ++row) {
        QVector<HexagonWidget*> rowWidgets;
        labels[row].resize(cols);  // Resize each row in the labels 2D vector

        for (int col = 0; col < cols; ++col) {
            // Create the HexagonWidget for the current position
            HexagonWidget *hexWidget = new HexagonWidget("", hexWidgetSize, hexWidgetSize, groupBox);
            hexWidget->setHexagonColor(QColor(255, 240, 150, 153));  // 蜂蜜色，80%透明度
            hexWidget->setBorderColor(QColor(139, 69, 19));  // 原木色
            rowWidgets.append(hexWidget);

            // Create the QLabel for the current position
            QString coordinatesText = QString("(%1, %2)").arg(row).arg(col);
            std::shared_ptr<QLabel> label = std::make_shared<QLabel>(coordinatesText, groupBox);
            label->setStyleSheet("QLabel { background-color: transparent; color: black; font-weight: bold; }");
            label->setAlignment(Qt::AlignCenter);
            label->setAttribute(Qt::WA_TransparentForMouseEvents);

            // Calculate the position of the hexagon and label
            int xPos = col * horizontalOffset + hexWidgetSize / 4;
            int yPos = row * verticalOffset + hexWidgetSize / 1.5;

            if (row % 2 != 0) {
                xPos += hexWidgetSize * 0.5;
            }

            // Position the hexagon and label
            hexWidget->move(xPos, yPos);
            label->move(xPos, yPos + hexWidgetSize / 3);
            label->resize(hexWidgetSize, hexWidgetSize / 3);

            // Show the hexagon and label
            hexWidget->show();
            label->show();

            // Store the label in the 2D vector at the correct position
            labels[row][col] = label;
        }
        hexWidgetMatrix.append(rowWidgets);
    }
}

void BoardUI::updateBoardUI(std::shared_ptr<ChessBoard> chessBoard) {
    if (!chessBoard) return;  // 确保棋盘指针有效

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            auto piece = chessBoard->getPieceAt(row, col);
            if (piece != NULL) {
                auto owner = piece->getOwner();

                // 根据所属玩家设置边框颜色
                if (owner->getName() == "Romain") {
                    hexWidgetMatrix[row][col]->setBorderColor(Qt::red);  // 玩家1边框为红色
                    labels[row][col] -> hide();

                    // 根据棋子类型设置填充颜色
                    switch (piece->getPieceType()) {
                    case PieceType::QueenBee_id:
                        hexWidgetMatrix[row][col]->setHexagonColor(Qt::yellow);
                        hexWidgetMatrix[row][col]->setImage(":/pic/ButtonImage/10.png");
                        break;
                    case PieceType::Beetle_id:
                        hexWidgetMatrix[row][col]->setHexagonColor(Qt::darkGray);
                        hexWidgetMatrix[row][col]->setImage(":/pic/ButtonImage/11.png");
                        break;
                    case PieceType::Grasshopper_id:
                        hexWidgetMatrix[row][col]->setHexagonColor(Qt::green);
                        hexWidgetMatrix[row][col]->setImage(":/pic/ButtonImage/14.png");
                        break;
                    case PieceType::Spider_id:
                        hexWidgetMatrix[row][col]->setHexagonColor(QColorConstants::Svg::brown);
                        hexWidgetMatrix[row][col]->setImage(":/pic/ButtonImage/12.png");
                        break;
                    case PieceType::Ant_id:
                        hexWidgetMatrix[row][col]->setImage(":/pic/ButtonImage/13.png");
                        hexWidgetMatrix[row][col]->setHexagonColor(Qt::black);
                        break;
                    case PieceType::Ladybug_id:
                        hexWidgetMatrix[row][col]->setImage(":/pic/ButtonImage/15.png");
                        break;
                    case PieceType::Mosquito_id:
                        hexWidgetMatrix[row][col]->setImage(":/pic/ButtonImage/16.png");
                        break;
                    case PieceType::Pillbug_id:
                        hexWidgetMatrix[row][col]->setImage(":/pic/ButtonImage/17.png");
                        break;
                    default:
                        hexWidgetMatrix[row][col]->setHexagonColor(QColor(255, 240, 150, 153));  // 蜂蜜色，80%透明度
                        break;
                    }
                } else if (owner->getName() == "Leo" || owner->getName() == "本地AI" || owner->getName() == "在线AI" || owner->getName() == "AI") {
                    hexWidgetMatrix[row][col]->setBorderColor(Qt::blue); // 玩家2边框为蓝色
                    labels[row][col] -> hide();
                    // 根据棋子类型设置填充颜色
                    switch (piece->getPieceType()) {
                    case PieceType::QueenBee_id:
                        hexWidgetMatrix[row][col]->setHexagonColor(Qt::yellow);
                        hexWidgetMatrix[row][col]->setImage(":/pic/ButtonImage/20.png");
                        break;
                    case PieceType::Beetle_id:
                        hexWidgetMatrix[row][col]->setHexagonColor(Qt::darkGray);
                        hexWidgetMatrix[row][col]->setImage(":/pic/ButtonImage/21.png");
                        break;
                    case PieceType::Grasshopper_id:
                        hexWidgetMatrix[row][col]->setHexagonColor(Qt::green);
                        hexWidgetMatrix[row][col]->setImage(":/pic/ButtonImage/24.png");
                        break;
                    case PieceType::Spider_id:
                        hexWidgetMatrix[row][col]->setHexagonColor(QColorConstants::Svg::brown);
                        hexWidgetMatrix[row][col]->setImage(":/pic/ButtonImage/22.png");
                        break;
                    case PieceType::Ant_id:
                        hexWidgetMatrix[row][col]->setImage(":/pic/ButtonImage/23.png");
                        hexWidgetMatrix[row][col]->setHexagonColor(Qt::black);
                        break;
                    case PieceType::Ladybug_id:
                        hexWidgetMatrix[row][col]->setImage(":/pic/ButtonImage/25.png");
                        hexWidgetMatrix[row][col]->setHexagonColor(Qt::red);
                        break;
                    case PieceType::Mosquito_id:
                        hexWidgetMatrix[row][col]->setImage(":/pic/ButtonImage/26.png");
                        hexWidgetMatrix[row][col]->setHexagonColor(Qt::cyan);
                        break;
                    case PieceType::Pillbug_id:
                        hexWidgetMatrix[row][col]->setImage(":/pic/ButtonImage/27.png");
                        hexWidgetMatrix[row][col]->setHexagonColor(Qt::magenta);
                        break;
                    default:
                        hexWidgetMatrix[row][col]->setHexagonColor(QColor(255, 240, 150, 153));  // 蜂蜜色，80%透明度
                        break;
                    }
                } else {
                    hexWidgetMatrix[row][col]->setBorderColor(QColor(139, 69, 19));  // 原木色
                }
            } else {
                // 无棋子时，默认灰色填充和黑色边框
                hexWidgetMatrix[row][col]->setHexagonColor(QColor(255, 240, 150, 153));  // 蜂蜜色，80%透明度
                hexWidgetMatrix[row][col]->setBorderColor(QColor(139, 69, 19));  // 原木色
                hexWidgetMatrix[row][col]->setImage("");
                labels[row][col] -> show();
            }
        }
    }

    repaint();
}
