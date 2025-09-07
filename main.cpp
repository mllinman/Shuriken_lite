#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    // Load dark theme
    QFile qss(":/resources/darktheme.qss");
    if (qss.open(QFile::ReadOnly)) {
        app.setStyleSheet(qss.readAll());
    }

    MainWindow w;
    w.setWindowTitle("Shuriken — C++ Builder");
    w.resize(1000, 700);
    w.show();

    return app.exec();
}
