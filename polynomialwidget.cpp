#include "polynomialwidget.h"
#include <QInputDialog>
#include <QMessageBox>
#include "ui_polynomialwidget.h"
#include <sstream>

PolynomialWidget::PolynomialWidget(QWidget *parent, QLayout *layout, List<Polynomial>::Node* p_, List<Polynomial>* lst_, List<QPair<QLabel*, int>>::Node* n_, List<QPair<QLabel*, int>>* num) : QWidget(parent), ui(new Ui::PolynomialWidget), pnt(layout), p(p_), n(n_) {
    ui->setupUi(this);
    ui->label->setText((std::string(p->val)).data());
    lst = lst_;
    nums = num;
    n->val.first = ui->label_2;
    n->val.first->setText(std::to_string(n->val.second).data());
}

PolynomialWidget::~PolynomialWidget() {
    delete ui;
}

void PolynomialWidget::on_editButton_clicked() {
    bool ok;
    std::string s = QInputDialog::getText(nullptr, "", "Enter a polynom", QLineEdit::Normal, std::string(p->val).data(), &ok).toStdString();
    if (!ok) return;
    std::stringstream ss;
    try {
        ss << s;
        ss >> p->val;
    } catch (const char *x) {
        QMessageBox::information(nullptr, "ОШИБКА БЛЯТЬ", x);
        return;
    }
    ui->label->setText((std::string(p->val)).data());
}

void PolynomialWidget::on_deleteButton_clicked() {
    pnt->removeWidget(this);
    for (List<QPair<QLabel*, int>>::Node* x = n; x != nullptr; x = x->next) {
        --x->val.second;
        x->val.first->setText(std::to_string(x->val.second).data());
    }
    lst->erase(p);
    nums->erase(n);
    delete this;
}
