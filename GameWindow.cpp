#include "GameWindow.h"

QMap<QString, int> convertToQMap(const std::map<std::string, int>& stdMap) {
    QMap<QString, int> qMap;
    for (const auto& [key, value] : stdMap) {
        qMap.insert(QString::fromStdString(key), value);
    }
    return qMap;
}

GameWindow::GameWindow(bool isAI, QWidget *parent)
    : QMainWindow(parent), chessBoard(new BoardUI(10, 10, 60, this)), currentTurn(1), isPlayerOneTurn(true), isDragging(false) {

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    setAttribute(Qt::WA_TranslucentBackground);

    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(10);
    shadowEffect->setColor(Qt::black);
    shadowEffect->setOffset(0);
    setGraphicsEffect(shadowEffect);

    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(240, 240, 240));
    this->setPalette(palette);

    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->addWidget(chessBoard);

    QVBoxLayout *rightLayout = new QVBoxLayout();

    QString globalStyle = loadStyleSheet(":/qss/qss/GameWindows.qss");
    this->setStyleSheet(globalStyle);

    bool ok,isDLC;
    QString DLC = showInputDialog("DLC拓展", "是否启用DLC，Yes or No", "No", &ok);
    if (DLC == "Yes")
        isDLC = true;

    playerOneName = showInputDialog("玩家一", "请输入玩家一的名字:", "Player1", &ok);
    if (!ok || playerOneName.isEmpty()) {
        playerOneName = "Player1";
    }

    if (isAI) {
        QStringList aiOptions;
        aiOptions << "在线 AI" << "本地 AI";
        playerTwoName = showInputDialog("选择 AI 类型", "请选择 AI 类型:", aiOptions, &ok);
        if (!ok || playerTwoName.isEmpty()) {
            playerTwoName = "AI";
        }
        playerTwoName = "AI";
    } else {
        playerTwoName = showInputDialog("玩家二", "请输入玩家二的名字:", "Player2", &ok);
        if (!ok || playerTwoName.isEmpty()) {
            playerTwoName = "Player2";
        }
    }

    // playerOneName = "Romain";
    // playerTwoName = "Leo";
    game = Game::getInstance();
    game->initializeGame(playerOneName.toStdString(), playerTwoName.toStdString(),isDLC,isAI);

    QGroupBox *turnInfoGroup = new QGroupBox("回合信息", this);
    QVBoxLayout *turnInfoLayout = new QVBoxLayout();
    turnInfoGroup->setObjectName("turnInfoGroup");


    turnLabel = new QLabel(this);
    updateTurnInfo(currentTurn, playerOneName);
    turnInfoLayout->addWidget(turnLabel);

    turnInfoGroup->setLayout(turnInfoLayout);
    rightLayout->addWidget(turnInfoGroup);



    playerOneGroup = createPlayerGroup(playerOneName,  convertToQMap(game->getPlayer(1)->pieceCount));
    playerTwoGroup = createPlayerGroup(playerTwoName, convertToQMap(game->getPlayer(2)->pieceCount));
    rightLayout->addWidget(playerOneGroup);
    rightLayout->addWidget(playerTwoGroup);
    playerOneGroup -> setObjectName("playerOneGroup");
    playerTwoGroup -> setObjectName("playerTwoGroup");

    exitButton = new QPushButton("退出", this);
    restartButton = new QPushButton("重新开始", this);
    playTurnButton = new QPushButton("进行回合", this);
    exitButton -> setObjectName("exitButton");
    restartButton -> setObjectName("restartButton");
    playTurnButton -> setObjectName("playTurnButton");

    connect(exitButton, &QPushButton::clicked, this, &GameWindow::close);
    connect(restartButton, &QPushButton::clicked, this, &GameWindow::restartGame);
    connect(playTurnButton, &QPushButton::clicked, this, &GameWindow::playTurn);

    rightLayout->addWidget(playTurnButton);
    rightLayout->addWidget(restartButton);
    rightLayout->addWidget(exitButton);
    rightLayout->addStretch();

    QWidget *rightWidget = new QWidget(this);
    rightWidget->setLayout(rightLayout);
    rightWidget->setFixedWidth(250);

    mainLayout->addWidget(rightWidget, 0, Qt::AlignTop);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    setCentralWidget(centralWidget);
    setWindowTitle("游戏界面（Hive Game）");
    setWindowIcon(QIcon(":/pic/images/logo.jpg"));
    setStyle();
}

void GameWindow::updatePlayerInfo() {
    auto playerOnePieces = game->getPlayer(1)->pieceCount;
    auto playerTwoPieces = game->getPlayer(2)->pieceCount;

    updatePlayerGroup(playerOneGroup, convertToQMap(playerOnePieces));
    updatePlayerGroup(playerTwoGroup, convertToQMap(playerTwoPieces));
}

void GameWindow::updatePlayerGroup(QGroupBox *group, const QMap<QString, int> &pieces) {
    QList<QWidget *> widgets = group->findChildren<QWidget *>();
    for (QWidget *widget : widgets) {
        delete widget;
    }

    // 获取或创建布局
    QVBoxLayout *playerLayout = qobject_cast<QVBoxLayout *>(group->layout());
    if (!playerLayout) {
        playerLayout = new QVBoxLayout(group);
        group->setLayout(playerLayout);
    }

    // 添加新的控件
    for (auto it = pieces.begin(); it != pieces.end(); ++it) {
        QString labelText = QString("%1: %2").arg(it.key()).arg(it.value());
        QLabel *pieceLabel = new QLabel(labelText, this);
        playerLayout->addWidget(pieceLabel);
    }

}

void GameWindow::playTurn() {
    game->playTurnUI(this);
    updateBoardUI();
    updateTurnInfo(game->getTurnCount(), playerOneName);
    updatePlayerInfo();
}

void GameWindow::restartGame() {
    game->restartGame();
    initializeGameUI();
}

void GameWindow::initializeGameUI() {
    game->initializeGame(playerOneName.toStdString(), playerTwoName.toStdString());
    updateTurnInfo(game->getTurnCount(), playerOneName);
    updateBoardUI();
    updatePlayerInfo();
}

void GameWindow::updateTurnInfo(int turn, const QString &playerName) {
    turnLabel->setText(QString("当前回合: %1\n当前操作: %2").arg(turn).arg(playerName));
}

void GameWindow::updateBoardUI() {
    std::shared_ptr<ChessBoard> chessBoardData = game->getBoard();
    chessBoard->updateBoardUI(chessBoardData);
}

QGroupBox* GameWindow::createPlayerGroup(const QString &playerName, const QMap<QString, int> &pieces) {
    QGroupBox *playerGroup = new QGroupBox(playerName, this);
    QVBoxLayout *playerLayout = new QVBoxLayout();

    for (auto it = pieces.begin(); it != pieces.end(); ++it) {
        QString labelText = QString("%1: %2").arg(it.key()).arg(it.value());
        QLabel *pieceLabel = new QLabel(labelText, this);
        playerLayout->addWidget(pieceLabel);
    }

    playerGroup->setLayout(playerLayout);
    return playerGroup;
}

// 无边框输入对话框（文字输入）
QString GameWindow::showInputDialog(const QString &title, const QString &label, const QString &defaultText, bool *ok) {
    QDialog dialog(this);
    dialog.setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    dialog.setStyleSheet(this->styleSheet());

    // 设置样式表：背景白色，半透明，倒圆角
    dialog.setStyleSheet("QDialog { border-radius: 10px; background-color: white; }");  // 设置倒圆角和背景颜色

    QVBoxLayout layout(&dialog);
    QLabel titleLabel(title, &dialog);
    titleLabel.setAlignment(Qt::AlignCenter);
    layout.addWidget(&titleLabel);

    QLabel labelWidget(label, &dialog);
    layout.addWidget(&labelWidget);

    QLineEdit lineEdit(defaultText, &dialog);
    layout.addWidget(&lineEdit);

    QHBoxLayout buttonLayout;
    QPushButton okButton("确定", &dialog);
    QPushButton cancelButton("取消", &dialog);
    okButton.setObjectName("okButton");
    cancelButton.setObjectName("cancelButton");
    buttonLayout.addWidget(&okButton);
    buttonLayout.addWidget(&cancelButton);
    layout.addLayout(&buttonLayout);

    connect(&okButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    *ok = (dialog.exec() == QDialog::Accepted);
    return *ok ? lineEdit.text() : QString();
}

// 无边框输入对话框（选择列表）
QString GameWindow::showInputDialog(const QString &title, const QString &label, const QStringList &items, bool *ok) {
    QDialog dialog(this);
    dialog.setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    dialog.setStyleSheet(this->styleSheet());

    // 设置样式表：背景白色，半透明，倒圆角
    dialog.setStyleSheet("QDialog { border-radius: 10px; background-color: white; }");  // 设置倒圆角和背景颜色

    QVBoxLayout layout(&dialog);
    QLabel titleLabel(title, &dialog);
    titleLabel.setAlignment(Qt::AlignCenter);
    layout.addWidget(&titleLabel);

    QLabel labelWidget(label, &dialog);
    layout.addWidget(&labelWidget);

    QComboBox comboBox(&dialog);
    comboBox.addItems(items);
    layout.addWidget(&comboBox);

    QHBoxLayout buttonLayout;
    QPushButton okButton("确定", &dialog);
    QPushButton cancelButton("取消", &dialog);
    okButton.setObjectName("okButton");
    cancelButton.setObjectName("cancelButton");
    buttonLayout.addWidget(&okButton);
    buttonLayout.addWidget(&cancelButton);
    layout.addLayout(&buttonLayout);

    connect(&okButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    *ok = (dialog.exec() == QDialog::Accepted);
    return *ok ? comboBox.currentText() : QString();
}

int GameWindow::showIntegerInputDialog(const QString &title, const QString &label, int defaultValue, int minValue, int maxValue, int step, bool *ok) {
    QDialog dialog(this);
    dialog.setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    dialog.setStyleSheet(this->styleSheet());

    // 设置样式表：背景白色，半透明，倒圆角
    dialog.setStyleSheet("QDialog { border-radius: 10px; background-color: white; }");  // 设置倒圆角和背景颜色

    QVBoxLayout layout(&dialog);
    QLabel titleLabel(title, &dialog);
    titleLabel.setAlignment(Qt::AlignCenter);
    layout.addWidget(&titleLabel);

    QLabel labelWidget(label, &dialog);
    layout.addWidget(&labelWidget);

    QLineEdit lineEdit(QString::number(defaultValue), &dialog);
    lineEdit.setValidator(new QIntValidator(minValue, maxValue, &lineEdit));  // 设置整数输入验证器，限制范围
    layout.addWidget(&lineEdit);

    QHBoxLayout buttonLayout;
    QPushButton okButton("确定", &dialog);
    QPushButton cancelButton("取消", &dialog);
    okButton.setObjectName("okButton");
    cancelButton.setObjectName("cancelButton");
    buttonLayout.addWidget(&okButton);
    buttonLayout.addWidget(&cancelButton);
    layout.addLayout(&buttonLayout);

    // 添加步进功能
    connect(&lineEdit, &QLineEdit::textEdited, [&lineEdit, minValue, maxValue, step]() {
        bool ok;
        int value = lineEdit.text().toInt(&ok);
        if (ok) {
            // 确保输入值在步进范围内
            value = std::max(minValue, std::min(maxValue, (value / step) * step));
            lineEdit.setText(QString::number(value));
        }
    });

    connect(&okButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    *ok = (dialog.exec() == QDialog::Accepted);
    return *ok ? lineEdit.text().toInt() : minValue;  // 返回整数值，如果取消则返回最小值
}





QString GameWindow::loadStyleSheet(const QString &sheetName) {
    QFile file(sheetName);
    if (!file.open(QFile::ReadOnly)) {
        qWarning("无法打开样式表文件: %s", qPrintable(sheetName));
        return QString();
    }
    QTextStream stream(&file);
    return stream.readAll();
}

void GameWindow::setStyle() {
    QString styleSheet = loadStyleSheet(":/qss/qss/GameWindows.qss");
    if (!styleSheet.isEmpty()) {
        this->setStyleSheet(styleSheet);
    } else {
        qWarning() << "无法加载样式表文件";
    }
}

void GameWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QRect rect = this->rect();
    QColor borderColor(0, 0, 0, 50);
    QColor backgroundColor(240, 240, 240);

    QPainterPath path;
    path.addRoundedRect(rect.adjusted(1, 1, -1, -1), 15, 15);
    painter.fillPath(path, backgroundColor);
    painter.setPen(QPen(borderColor, 2));
    painter.drawPath(path);

    QWidget::paintEvent(event);
}

void GameWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        isDragging = true;
        dragStartPos = event->globalPosition().toPoint() - this->frameGeometry().topLeft();
        event->accept();
    }
}

void GameWindow::mouseMoveEvent(QMouseEvent *event) {
    if (isDragging && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPosition().toPoint() - dragStartPos);
        event->accept();
    }
}

void GameWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        isDragging = false;
        event->accept();
    }
}

void GameWindow::showMessage(const QString &title, const QString &message) {
    QDialog dialog(this);
    dialog.setWindowTitle(title);
    dialog.setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    // 设置样式，背景色为白色，圆角效果
    //dialog.setStyleSheet("background-color: white; border-radius: 15px; padding: 10px;");

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLabel *label = new QLabel(message);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    QPushButton *okButton = new QPushButton("OK");
    okButton -> setObjectName("okButton");
    connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    layout->addWidget(okButton);

    dialog.exec();
}
