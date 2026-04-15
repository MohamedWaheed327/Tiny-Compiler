#include "mainwindow.h"
#include "../../get_DFA.h"
#include "../../lexical_analyzer.h"
#include "./ui_mainwindow.h"

DFA dfa = get_DFA();

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    connect(ui->tokenizeButton, &QPushButton::clicked, this, &MainWindow::onTokenizeClicked);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onTokenizeClicked() {
    QString input = ui->inputText->toPlainText();
    QString output = lexer(input.toStdString(), dfa).c_str();
    ui->outputText->setPlainText(output);
}