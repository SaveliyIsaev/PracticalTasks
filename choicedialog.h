#ifndef CHOICEDIALOG_H
#define CHOICEDIALOG_H

#include <QDialog>
#include "Polynomial.hpp"
namespace Ui {
class ChoiceDialog;
}

class ChoiceDialog : public QDialog {
    Q_OBJECT

public:
    explicit ChoiceDialog(List<Polynomial>* lst_, Polynomial* p_, bool* ok_, QWidget *parent = nullptr);
    ~ChoiceDialog();

private slots:
    void on_ChoiceDialog_accepted();

    void on_ChoiceDialog_rejected();

private:
    Ui::ChoiceDialog *ui;
    Polynomial* p;
    List<Polynomial>* lst;
    bool* ok;
};

#endif // CHOICEDIALOG_H
