#ifndef RESULTOUTPUT_H
#define RESULTOUTPUT_H

#include <QDialog>

namespace Ui {
class ResultOutput;
}

class ResultOutput : public QDialog
{
    Q_OBJECT

public:
    explicit ResultOutput(QWidget *parent = nullptr);
    ~ResultOutput();

private:
    Ui::ResultOutput *ui;
};

#endif // RESULTOUTPUT_H
