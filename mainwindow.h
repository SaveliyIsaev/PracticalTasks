#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "List.hpp"
#include "Polynomial.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_multButton_clicked();

    void on_divButton_clicked();

    void on_minusButton_clicked();

    void on_plusButton_clicked();

    void on_equalButton_clicked();

    void on_enterButton_clicked();

    void on_derButton_clicked();

    void on_valueButton_clicked();

    void on_rootsButton_clicked();

    void on_loadButton_clicked();

    void on_saveButton_clicked();

    void on_magicButton_clicked();

private:
    Ui::MainWindow *ui;
    List<Polynomial> lst;
    List<QPair<QLabel*, int>> num;

    void AddAPolynomial(Polynomial);
};
#endif // MAINWINDOW_H
