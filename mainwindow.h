#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QPoint>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPaintEvent>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void showRules();
    void startPvPGame();
    void startPvCGame();

private:
    void centerWindow();

    // 窗口拖动相关变量
    bool isDragging;
    QPoint startPos;

    // 鼠标事件重载
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    // UI 元素
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QLabel *backgroundImage;
    QPushButton *rulesButton;
    QPushButton *pvpButton;
    QPushButton *pvcButton;
    QPushButton *exitButton;

    // 音频播放
    QMediaPlayer *player;
    QAudioOutput *audioOutput;
};

#endif // MAINWINDOW_H
