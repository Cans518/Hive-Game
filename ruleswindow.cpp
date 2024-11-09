#include "ruleswindow.h"
#include <QFile>
#include <QTextStream>
#include <QAudioOutput>
#include <QCloseEvent>
#include <QHBoxLayout>
#include <QTimer>
#include <QScreen>
#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QRegion>

void RulesWindow::centerWindow() {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);
}

RulesWindow::RulesWindow(QMediaPlayer *backgroundMusicPlayer, QWidget *parent)
    : QDialog(parent), backgroundMusicPlayer(backgroundMusicPlayer) {
    // 设置无边框和透明背景
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);

    setWindowTitle("规则说明");
    setWindowIcon(QIcon(":/pic/images/logo.jpg"));
    resize(400, 300);
    originalSize = size();

    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(40, 30, 40, 30);  // Adjust values as desired

    rulesText = new QTextBrowser(this);
    rulesText->setOpenExternalLinks(true);

    QFile styleFile(":/qss/qss/rulewindows.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        this->setStyleSheet(styleSheet);
        styleFile.close();
    }

    QFile file(":/doc/doc/rule.html");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString htmlContent = in.readAll();
        rulesText->setHtml(htmlContent);
    }

    mainLayout->addWidget(rulesText);

    QHBoxLayout *buttonLayout = new QHBoxLayout;

    playVideoButton = new QPushButton("播放规则视频", this);
    playVideoButton->setObjectName("playVideoButton");
    connect(playVideoButton, &QPushButton::clicked, this, &RulesWindow::onPlayVideoButtonClicked);
    buttonLayout->addWidget(playVideoButton);

    okButton = new QPushButton("OK", this);
    okButton->setObjectName("okButton");
    connect(okButton, &QPushButton::clicked, this, &RulesWindow::onOkButtonClicked);
    buttonLayout->addWidget(okButton);

    mainLayout->addLayout(buttonLayout);

    rulesText->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    playVideoButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    okButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    setLayout(mainLayout);

    setupVideoPlayer();
    centerWindow();
}

void RulesWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 绘制圆角矩形和边框
    QRect rect = this->rect().adjusted(10, 10, -10, -10);  // 调整大小以容纳阴影
    QColor borderColor(0, 0, 0, 50);  // 边框颜色，带透明度
    QColor backgroundColor(255, 255, 255);  // 背景颜色

    QPainterPath path;
    path.addRoundedRect(rect, 15, 15);  // 圆角矩形
    painter.fillPath(path, backgroundColor);  // 填充背景色
    painter.setPen(QPen(borderColor, 2));
    painter.drawPath(path);  // 绘制边框

    QWidget::paintEvent(event);
}

void RulesWindow::setupVideoPlayer() {
    mediaPlayer = new QMediaPlayer(this);
    videoWidget = new QVideoWidget;

    mediaPlayer->setVideoOutput(videoWidget);

    QAudioOutput *audioOutput = new QAudioOutput(this);
    audioOutput->setVolume(1.0);
    mediaPlayer->setAudioOutput(audioOutput);

    videoContainer = new QWidget(this);
    QVBoxLayout *videoLayout = new QVBoxLayout(videoContainer);
    videoLayout->addWidget(videoWidget);

    progressSlider = new QSlider(Qt::Horizontal, this);
    progressSlider->setRange(0, 100);
    connect(progressSlider, &QSlider::sliderMoved, this, &RulesWindow::onSliderMoved);
    videoLayout->addWidget(progressSlider);

    pauseResumeButton = new QPushButton("暂停", this);
    pauseResumeButton->setObjectName("pauseResumeButton");
    connect(pauseResumeButton, &QPushButton::clicked, this, &RulesWindow::onPauseResumeButtonClicked);
    videoLayout->addWidget(pauseResumeButton);

    mainLayout->addWidget(videoContainer);
    videoContainer->setVisible(false);

    videoContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    sliderUpdateTimer = new QTimer(this);
    connect(sliderUpdateTimer, &QTimer::timeout, this, &RulesWindow::updateSliderPosition);
}

void RulesWindow::showVideoControls(bool visible) {
    videoContainer->setVisible(visible);

    if (visible) {
        int extraHeight = 300;
        resize(width(), originalSize.height() + extraHeight);
        sliderUpdateTimer->start(500);
    } else {
        resize(originalSize);
        sliderUpdateTimer->stop();
    }

    centerWindow();
}

void RulesWindow::onOkButtonClicked() {
    if (videoWidget->isVisible()) {
        videoWidget->close();
    }
    close();
}

void RulesWindow::onPlayVideoButtonClicked() {
    if (backgroundMusicPlayer->playbackState() == QMediaPlayer::PlayingState) {
        backgroundMusicPlayer->pause();
    }

    mediaPlayer->setSource(QUrl("qrc:/video/video/rule.mp4"));
    showVideoControls(true);
    mediaPlayer->play();
}

void RulesWindow::onPauseResumeButtonClicked() {
    if (mediaPlayer->playbackState() == QMediaPlayer::PlayingState) {
        mediaPlayer->pause();
        pauseResumeButton->setText("继续");

        if (backgroundMusicPlayer->playbackState() == QMediaPlayer::PausedState) {
            backgroundMusicPlayer->play();
        }
    } else if (mediaPlayer->playbackState() == QMediaPlayer::PausedState) {
        mediaPlayer->play();
        pauseResumeButton->setText("暂停");

        if (backgroundMusicPlayer->playbackState() == QMediaPlayer::PlayingState) {
            backgroundMusicPlayer->pause();
        }
    }
}

void RulesWindow::onSliderMoved(int position) {
    qint64 duration = mediaPlayer->duration();
    qint64 newPosition = (position * duration) / 100;
    mediaPlayer->setPosition(newPosition);
}

void RulesWindow::updateSliderPosition() {
    qint64 duration = mediaPlayer->duration();
    qint64 position = mediaPlayer->position();

    if (duration > 0) {
        int sliderValue = (position * 100) / duration;
        progressSlider->setValue(sliderValue);
    }
}

void RulesWindow::onVideoWidgetClosed() {
    if (mediaPlayer->playbackState() == QMediaPlayer::PlayingState) {
        mediaPlayer->stop();
    }

    showVideoControls(false);

    if (backgroundMusicPlayer->playbackState() == QMediaPlayer::PausedState) {
        backgroundMusicPlayer->play();
    }
}

void RulesWindow::closeEvent(QCloseEvent *event) {
    if (videoWidget->isVisible()) {
        videoWidget->close();
        onVideoWidgetClosed();
    }

    if (backgroundMusicPlayer->playbackState() == QMediaPlayer::PausedState) {
        backgroundMusicPlayer->play();
    }

    QDialog::closeEvent(event);
}

void RulesWindow::hideEvent(QHideEvent *event) {
    if (backgroundMusicPlayer->playbackState() == QMediaPlayer::PausedState) {
        backgroundMusicPlayer->play();
    }

    QDialog::hideEvent(event);
}

void RulesWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        isDragging = true;
        dragStartPos = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void RulesWindow::mouseMoveEvent(QMouseEvent *event) {
    if (isDragging) {
        move(event->globalPosition().toPoint() - dragStartPos);
        event->accept();
    }
}

void RulesWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        isDragging = false;
        event->accept();
    }
}
