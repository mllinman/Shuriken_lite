/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QLabel *titleLabel;
    QLineEdit *repoInput;
    QPushButton *browseButton;
    QPushButton *buildButton;
    QPushButton *packageButton;
    QTextEdit *logOutput;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        titleLabel = new QLabel(centralwidget);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(titleLabel);

        repoInput = new QLineEdit(centralwidget);
        repoInput->setObjectName("repoInput");

        verticalLayout->addWidget(repoInput);

        browseButton = new QPushButton(centralwidget);
        browseButton->setObjectName("browseButton");

        verticalLayout->addWidget(browseButton);

        buildButton = new QPushButton(centralwidget);
        buildButton->setObjectName("buildButton");

        verticalLayout->addWidget(buildButton);

        packageButton = new QPushButton(centralwidget);
        packageButton->setObjectName("packageButton");

        verticalLayout->addWidget(packageButton);

        logOutput = new QTextEdit(centralwidget);
        logOutput->setObjectName("logOutput");

        verticalLayout->addWidget(logOutput);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Shuriken Lite", nullptr));
        titleLabel->setText(QCoreApplication::translate("MainWindow", "Shuriken Lite Builder", nullptr));
        repoInput->setPlaceholderText(QCoreApplication::translate("MainWindow", "Source folder path...", nullptr));
        browseButton->setText(QCoreApplication::translate("MainWindow", "Browse", nullptr));
        buildButton->setText(QCoreApplication::translate("MainWindow", "Build", nullptr));
        packageButton->setText(QCoreApplication::translate("MainWindow", "Package Installer", nullptr));
        logOutput->setPlaceholderText(QCoreApplication::translate("MainWindow", "Build and packaging logs...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
