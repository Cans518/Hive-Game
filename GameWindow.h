#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QFile>
#include <QTextStream>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QInputDialog>
#include <QMessageBox>
#include <QComboBox>
#include "Game.h"
#include "BoardUI.h"


class GameWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit GameWindow(bool isAI = false, QWidget *parent = nullptr);
    void updateTurnInfo(int turn, const QString &playerName);
    void updateBoardUI();
    void updatePlayerInfo();
    void showMessage(const QString &title, const QString &message);
    QString showInputDialog(const QString &title, const QString &label, const QString &defaultText, bool *ok);
    QString showInputDialog(const QString &title, const QString &label, const QStringList &items, bool *ok);
    int showIntegerInputDialog(const QString &title, const QString &label, int defaultValue, int minValue, int maxValue, int step, bool *ok);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void playTurn();
    void restartGame();

private:
    void initializeGameUI();
    void updatePlayerGroup(QGroupBox *group, const QMap<QString, int> &pieces);
    QString loadStyleSheet(const QString &sheetName);
    void setStyle();
    QGroupBox* createPlayerGroup(const QString &playerName, const QMap<QString, int> &pieces);

    QLabel *turnLabel;
    QPushButton *exitButton;
    QPushButton *restartButton;
    QPushButton *playTurnButton;

    QGroupBox *playerOneGroup;
    QGroupBox *playerTwoGroup;
    BoardUI *chessBoard;

    QString playerOneName;
    QString playerTwoName;
    std::shared_ptr<Game> game;

    int currentTurn;
    bool isPlayerOneTurn;
    bool isDragging;
    QPoint dragStartPos;
};

#endif // GAMEWINDOW_H
