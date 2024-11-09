#ifndef HEXAGONWIDGET_H
#define HEXAGONWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QColor>

class HexagonWidget : public QWidget {
    Q_OBJECT

public:
    HexagonWidget(const QString& iconPath = "", int width = 100, int height = 100, QWidget* parent = nullptr);

    void setHexagonColor(const QColor& color);
    void setBorderColor(const QColor& color);
    void setImage(const QString& iconPath);
    void setSize(int width, int height);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QPixmap pixmap;
    bool hasImage;
    QColor hexagonColor;
    QColor borderColor;
};

#endif // HEXAGONWIDGET_H
