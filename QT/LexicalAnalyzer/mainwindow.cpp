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

    auto [lexemes, tokens, line_number] = lexer(input.toStdString(), dfa);
    string temp = "";
    for (int i = 0; i < lexemes.size(); ++i) {
        temp += lexemes[i];
        temp += " ------> ";
        temp += tokens[i];
        temp += " ";
        temp += " at line " + to_string(line_number[i]);
        temp += "\n";
    }

    QString output = temp.c_str();
    ui->outputText->setPlainText(output);
}