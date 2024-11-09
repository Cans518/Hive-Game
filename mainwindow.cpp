#include "mainwindow.h"
#include "ruleswindow.h"
#include "GameWindow.h"
#include <QMessageBox>
#include <QHBoxLayout>
#include <QScreen>
#include <QGuiApplication>
#include <QDebug>
#include <QFile>
#include <QPainterPath>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), isDragging(false) {

    resize(480, 320);  // 设置窗口默认大小
    setWindowTitle("Hive Game");
    setWindowIcon(QIcon(":/pic/images/logo.jpg"));

    // 去除窗口边框并启用拖动
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    setAttribute(Qt::WA_TranslucentBackground);

    // 设置阴影效果
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(20);
    shadowEffect->setOffset(0, 0);
    shadowEffect->setColor(QColor(0, 0, 0, 100));  // 阴影颜色
    setGraphicsEffect(shadowEffect);

    // 创建中心窗口和布局
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    mainLayout = new QVBoxLayout(centralWidget);

    // 创建背景图片标签并设置样式
    backgroundImage = new QLabel(this);
    backgroundImage->setObjectName("backgroundImage");
    QPixmap bgImage(":/pic/images/image1.jpg");
    backgroundImage->setPixmap(bgImage.scaled(this->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    backgroundImage->setAlignment(Qt::AlignCenter);

    // 创建内容布局并添加背景图片
    QVBoxLayout *contentLayout = new QVBoxLayout();
    contentLayout->addWidget(backgroundImage);

    // 创建按钮布局
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    rulesButton = new QPushButton("规则说明", this);
    pvpButton = new QPushButton("PvP 对战", this);
    pvcButton = new QPushButton("人机对战", this);
    exitButton = new QPushButton("退出", this);

    // 将按钮添加到按钮布局
    buttonLayout->addWidget(rulesButton);
    buttonLayout->addWidget(pvpButton);
    buttonLayout->addWidget(pvcButton);
    buttonLayout->addWidget(exitButton);

    // 将内容布局和按钮布局添加到主布局
    mainLayout->addLayout(contentLayout);
    mainLayout->addLayout(buttonLayout);

    // 设置主布局属性
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setSpacing(20);

    // 加载QSS样式
    QFile styleFile(":/qss/qss/mainwindows.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        this->setStyleSheet(styleSheet);
        styleFile.close();
    }

    // 设置背景音乐
    player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    player->setAudioOutput(audioOutput);
    player->setSource(QUrl("qrc:/audio/music/background_music.mp3"));
    audioOutput->setVolume(0.3);
    player->setLoops(QMediaPlayer::Infinite);
    player->play();

    // 添加媒体加载状态调试输出
    connect(player, &QMediaPlayer::mediaStatusChanged, [](QMediaPlayer::MediaStatus status) {
        qDebug() << "Media status:" << status;
        if (status == QMediaPlayer::NoMedia) {
            qWarning() << "No media loaded.";
        } else if (status == QMediaPlayer::LoadedMedia) {
            qDebug() << "Media loaded successfully.";
        } else if (status == QMediaPlayer::InvalidMedia) {
            qWarning() << "Failed to load media.";
        }
    });

    // 设置按钮对象名称
    rulesButton->setObjectName("rulesButton");
    pvpButton->setObjectName("pvpButton");
    pvcButton->setObjectName("pvcButton");
    exitButton->setObjectName("exitButton");

    // 按钮信号和槽连接
    connect(rulesButton, &QPushButton::clicked, this, &MainWindow::showRules);
    connect(pvpButton, &QPushButton::clicked, this, &MainWindow::startPvPGame);
    connect(pvcButton, &QPushButton::clicked, this, &MainWindow::startPvCGame);
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::close);

    // 居中窗口
    centerWindow();
}

MainWindow::~MainWindow() {}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 绘制圆角矩形和边框
    QRect rect = this->rect().adjusted(10, 10, -10, -10);  // 调整大小以容纳阴影
    QColor borderColor(0, 0, 0, 50);  // 边框颜色，带透明度
    QColor backgroundColor(255, 255, 255);  // 灰色背景颜色

    QPainterPath path;
    path.addRoundedRect(rect, 15, 15);  // 圆角矩形
    painter.fillPath(path, backgroundColor);  // 填充背景色
    painter.setPen(QPen(borderColor, 2));
    painter.drawPath(path);  // 绘制边框

    QWidget::paintEvent(event);
}

void MainWindow::showRules() {
    RulesWindow *rulesWindow = new RulesWindow(player);  // 传递背景音乐播放器
    this->hide();
    rulesWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(rulesWindow, &QDialog::finished, this, &MainWindow::show);
    rulesWindow->show();
}

void MainWindow::startPvPGame() {
    GameWindow *GameWindows = new GameWindow();
    this->hide();
    GameWindows->setWindowTitle("PvP 对战");
    GameWindows->setAttribute(Qt::WA_DeleteOnClose);
    connect(GameWindows, &GameWindow::destroyed, this, &MainWindow::show);
    GameWindows->show();
}

void MainWindow::startPvCGame() {
    GameWindow *GameWindows = new GameWindow(true);
    this->hide();
    GameWindows->setWindowTitle("人机对战");
    GameWindows->setAttribute(Qt::WA_DeleteOnClose);
    connect(GameWindows, &GameWindow::destroyed, this, &MainWindow::show);
    GameWindows->show();
}

void MainWindow::centerWindow() {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    QRect windowFrameGeometry = this->frameGeometry();
    int x = (screenGeometry.width() - windowFrameGeometry.width()) / 2;
    int y = (screenGeometry.height() - windowFrameGeometry.height()) / 4;
    this->move(x, y);
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        isDragging = true;
        startPos = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (isDragging && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPosition().toPoint() - startPos);
        event->accept();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        isDragging = false;
        event->accept();
    }
}
