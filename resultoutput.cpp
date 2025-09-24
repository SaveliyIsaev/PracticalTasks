#include "resultoutput.h"
#include "ui_resultoutput.h"

ResultOutput::ResultOutput(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ResultOutput)
{
    ui->setupUi(this);
}

ResultOutput::~ResultOutput()
{
    delete ui;
}
