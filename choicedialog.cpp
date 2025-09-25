#include "choicedialog.h"
#include "ui_choicedialog.h"
#include <sstream>
#include <QVBoxLayout>
#include <QMessageBox>
ChoiceDialog::ChoiceDialog(List<Polynomial>* lst_, Polynomial* p_, bool* ok_, QWidget *parent) : QDialog(parent), ui(new Ui::ChoiceDialog) {
    ui->setupUi(this);
    p = p_;
    lst = lst_;
    ui->spinBox->setMaximum(lst->size);
    ok = ok_;
}

ChoiceDialog::~ChoiceDialog() {
    delete ui;
}

void ChoiceDialog::on_ChoiceDialog_accepted() {
    *ok = 1;
    if (ui->radioButton->isChecked()) {
        List<Polynomial>::Node* n = lst->begin;
        for (int i = 1; i < ui->spinBox->value(); ++i, n = n->next) {}
        *p = n->val;
        return;
    }
    if (ui->radioButton_2->isChecked()) {
        std::stringstream s(ui->lineEdit->text().toStdString());
        try {
            s >> *p;
        } catch (const char* x) {
            QMessageBox::information(nullptr, "ОШИБКА ЕПТА", x);
            *ok = 0;
            return;
        }
        return;
    }
    *ok = 0;
}


void ChoiceDialog::on_ChoiceDialog_rejected() {
    *ok = 0;
}

