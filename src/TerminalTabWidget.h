#pragma once
#include <QTabWidget>
#include "TerminalWidget.h"

class TerminalTabWidget : public QTabWidget {
    Q_OBJECT
public:
    TerminalTabWidget(QWidget *parent = nullptr);

public slots:
    void newTerminal();
};
