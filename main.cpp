#include <QApplication>
#include <QFontDatabase>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    int fontId = QFontDatabase::addApplicationFont(":/ttf/LXGWWenKaiMonoGBScreen.ttf");
    if (fontId == -1) {
        qWarning("Failed to load the custom font.");
    }

    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
    if (fontFamilies.isEmpty()) {
        qWarning("No font families found in the loaded font.");
    } else {
        QString fontFamily = fontFamilies.at(0); // Use the first font family
        QFont customFont(fontFamily);
        app.setFont(customFont);  // Set font for the whole application
    }

    MainWindow mainWindow;
    mainWindow.setWindowTitle("Hive Game");
    mainWindow.resize(540, 640);
    // mainWindow.centerWindow();
    mainWindow.show();

    return app.exec();
}
