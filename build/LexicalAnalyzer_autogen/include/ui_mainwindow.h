/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTextEdit *inputText;
    QPushButton *tokenizeButton;
    QTextEdit *outputText;
    QMenuBar *menubar;
    QMenu *menuLexical_Analyzer;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        inputText = new QTextEdit(centralwidget);
        inputText->setObjectName("inputText");
        inputText->setGeometry(QRect(10, 0, 771, 201));
        tokenizeButton = new QPushButton(centralwidget);
        tokenizeButton->setObjectName("tokenizeButton");
        tokenizeButton->setGeometry(QRect(690, 220, 90, 29));
        outputText = new QTextEdit(centralwidget);
        outputText->setObjectName("outputText");
        outputText->setGeometry(QRect(13, 274, 761, 261));
        outputText->setReadOnly(true);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 25));
        menuLexical_Analyzer = new QMenu(menubar);
        menuLexical_Analyzer->setObjectName("menuLexical_Analyzer");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuLexical_Analyzer->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        tokenizeButton->setText(QCoreApplication::translate("MainWindow", "Tokenize", nullptr));
        menuLexical_Analyzer->setTitle(QCoreApplication::translate("MainWindow", "Lexical Analyzer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
