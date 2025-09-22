#include "polynomialwidget.h"
#include <QInputDialog>
#include <QMessageBox>
#include "ui_polynomialwidget.h"
#include <sstream>

PolynomialWidget::PolynomialWidget(QWidget *parent, QVBoxLayout *layout, Polynomial p_) : QWidget(parent), ui(new Ui::PolynomialWidget), pnt(layout), p(p_) {
    ui->setupUi(this);
    ui->label->setText((std::string(p)).data());
}

PolynomialWidget::~PolynomialWidget()
{
    delete ui;
}

void PolynomialWidget::on_editButton_clicked()
{
    bool ok;
    std::string s = QInputDialog::getText(nullptr, "", "Enter a polynom", QLineEdit::Normal, std::string(p).data(), &ok).toStdString();
    if (!ok) return;
    std::stringstream ss;
    try {
        ss << s;
        ss >> p;
    } catch (const char *x) {
        QMessageBox::information(nullptr, "Error", x);
        return;
    }
    ui->label->setText((std::string(p)).data());
}

void PolynomialWidget::on_deleteButton_clicked()
{
    pnt->removeWidget(this);
    delete this;
}
