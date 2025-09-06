#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QFile qss(":/resources/darktheme.qss");
    if (qss.open(QFile::ReadOnly)) {
        app.setStyleSheet(qss.readAll());
    }

    MainWindow w;
    w.resize(900, 600);
    w.show();
    return app.exec();
}
