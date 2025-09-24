#ifndef POLYNOMIALWIDGET_H
#define POLYNOMIALWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include "Polynomial.hpp"
#include "List.hpp"
namespace Ui {
class PolynomialWidget;
}

class PolynomialWidget : public QWidget {
    Q_OBJECT

public:
    explicit PolynomialWidget(QWidget *parent = nullptr, QLayout* layout = nullptr, List<Polynomial>::Node* p_ = nullptr, List<Polynomial>* lst = nullptr, List<QPair<QLabel*, int>>::Node* n_ = nullptr, List<QPair<QLabel*, int>>* num = nullptr);
    ~PolynomialWidget();

private slots:
    void on_editButton_clicked();

    void on_deleteButton_clicked();

private:
    QLayout* pnt;
    Ui::PolynomialWidget *ui;
    List<Polynomial>::Node* p;
    List<Polynomial>* lst;
    List<QPair<QLabel*, int>>::Node* n;
    List<QPair<QLabel*, int>>* nums;
};

#endif // POLYNOMIALWIDGET_H
