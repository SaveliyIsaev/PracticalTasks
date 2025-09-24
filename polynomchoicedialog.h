#ifndef POLYNOMCHOICEDIALOG_H
#define POLYNOMCHOICEDIALOG_H

#include <QDialog>

namespace Ui {
class PolynomChoiceDialog;
}

class PolynomChoiceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PolynomChoiceDialog(QWidget *parent = nullptr);
    ~PolynomChoiceDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::PolynomChoiceDialog *ui;
};

#endif // POLYNOMCHOICEDIALOG_H
