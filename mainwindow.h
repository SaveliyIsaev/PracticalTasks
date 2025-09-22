#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

    void on_rootsButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
