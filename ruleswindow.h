#ifndef RULESWINDOW_H
#define RULESWINDOW_H

#include <QDialog>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QTimer>

class RulesWindow : public QDialog {
    Q_OBJECT

public:
    explicit RulesWindow(QMediaPlayer *backgroundMusicPlayer, QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void hideEvent(QHideEvent *event) override;

private:
    void centerWindow();
    void setupVideoPlayer();
    void applyRoundedCorners();  // 设置圆角
    void showVideoControls(bool visible);

private slots:
    void onOkButtonClicked();
    void onPlayVideoButtonClicked();
    void onPauseResumeButtonClicked();
    void onSliderMoved(int position);
    void updateSliderPosition();
    void onVideoWidgetClosed();

private:
    QMediaPlayer *backgroundMusicPlayer;
    QMediaPlayer *mediaPlayer;
    QVideoWidget *videoWidget;
    QTextBrowser *rulesText;
    QVBoxLayout *mainLayout;
    QPushButton *playVideoButton;
    QPushButton *okButton;
    QPushButton *pauseResumeButton;
    QSlider *progressSlider;
    QTimer *sliderUpdateTimer;
    QWidget *videoContainer;
    QSize originalSize;

    void paintEvent(QPaintEvent *event) override; // 自定义绘制圆角和边框

    bool isDragging = false;        // 用于拖动窗口
    QPoint dragStartPos;            // 拖动时的起始位置
};

#endif // RULESWINDOW_H
