#include "polynomchoicedialog.h"
#include "ui_polynomchoicedialog.h"

PolynomChoiceDialog::PolynomChoiceDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PolynomChoiceDialog)
{
    ui->setupUi(this);
}

PolynomChoiceDialog::~PolynomChoiceDialog()
{
    delete ui;
}

void PolynomChoiceDialog::on_buttonBox_accepted()
{

}

