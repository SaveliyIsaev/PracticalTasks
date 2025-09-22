#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "polynomialwidget.h"
#include <QInputDialog>
#include <sstream>
#include "Polynomial.hpp"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_multButton_clicked() {

}


void MainWindow::on_divButton_clicked() {

}


void MainWindow::on_minusButton_clicked() {

}


void MainWindow::on_plusButton_clicked() {

}


void MainWindow::on_equalButton_clicked() {

}


void MainWindow::on_enterButton_clicked() {
    bool ok;
    std::string s = QInputDialog::getText(nullptr, "", "Enter a polynom", QLineEdit::Normal, "", &ok).toStdString();
    QMessageBox::information(nullptr, "", s.data());
    if (!ok) return;
    std::stringstream ss;
    Polynomial p;
    try {
        ss << s;
        ss >> p;
    } catch (const char* x) {
        QMessageBox::information(nullptr, "Error", x);
        return;
    }
    QMessageBox::information(nullptr, "", (std::string(p)).data());
    QVBoxLayout* layout = new QVBoxLayout(ui->scrollAreaWidgetContents);
    PolynomialWidget* pw = new PolynomialWidget(ui->scrollAreaWidgetContents, layout, p);
    layout->insertWidget(layout->count() - 1, pw);

}


void MainWindow::on_derButton_clicked() {

}


void MainWindow::on_rootsButton_clicked() {

}

