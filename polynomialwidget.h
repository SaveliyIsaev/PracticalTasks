#ifndef POLYNOMIALWIDGET_H
#define POLYNOMIALWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include "Polynomial.hpp"
namespace Ui {
class PolynomialWidget;
}

class PolynomialWidget : public QWidget {
    Q_OBJECT

public:
    explicit PolynomialWidget(QWidget *parent = nullptr, QVBoxLayout* layout = nullptr, Polynomial p_ = Polynomial());
    ~PolynomialWidget();

private slots:
    void on_editButton_clicked();

    void on_deleteButton_clicked();

private:
    QVBoxLayout* pnt;
    Ui::PolynomialWidget *ui;
    Polynomial p;

};

#endif // POLYNOMIALWIDGET_H
