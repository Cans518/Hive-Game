#include "HexagonWidget.h"
#include <QPainter>
#include <QPolygon>
#include <QtMath>

HexagonWidget::HexagonWidget(const QString& iconPath, int width, int height, QWidget* parent)
    : QWidget(parent), pixmap(iconPath), hasImage(!iconPath.isEmpty()), hexagonColor(Qt::blue), borderColor(Qt::black) {
    setFixedSize(width, height);
}

void HexagonWidget::setHexagonColor(const QColor& color) {
    hexagonColor = color;
    update();  // Request a repaint
}

void HexagonWidget::setBorderColor(const QColor& color) {
    borderColor = color;
    update();
}

void HexagonWidget::setImage(const QString& iconPath) {
    pixmap = QPixmap(iconPath);
    hasImage = !iconPath.isEmpty();
    update();
}

void HexagonWidget::setSize(int width, int height) {
    setFixedSize(width, height);
    update();
}

void HexagonWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    QPolygon hexagon;
    int sideLength = qMin(width(), height()) / 2;
    for (int i = 0; i < 6; ++i) {
        float angle = M_PI / 3 * i + M_PI / 2;
        int x = width() / 2 + sideLength * cos(angle);
        int y = height() / 2 + sideLength * sin(angle);
        hexagon << QPoint(x, y);
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 设置加粗边框
    QPen pen(borderColor);
    pen.setWidth(3);  // 设置边框宽度为 3
    painter.setPen(pen);
    painter.setBrush(hexagonColor);  // 设置填充颜色
    painter.drawPolygon(hexagon);

    // 如果有图像，显示图像
    if (hasImage) {
        painter.setClipRegion(QRegion(hexagon));
        QPixmap scaledPixmap = pixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        painter.drawPixmap((width() - scaledPixmap.width()) / 2, (height() - scaledPixmap.height()) / 2, scaledPixmap);
    }
}
