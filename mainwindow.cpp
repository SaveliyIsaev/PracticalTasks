#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "polynomialwidget.h"
#include <QInputDialog>
#include <sstream>
#include "Polynomial.hpp"
#include <QMessageBox>
#include <QScrollArea>
#include "choicedialog.h"
#include <fstream>
#include <QPaintEvent>
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->scrollAreaWidgetContents->setLayout(ui->verticalLayoutWidget->layout());
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::AddAPolynomial(Polynomial p) {
    QLayout* layout = ui->scrollAreaWidgetContents->layout();
    List<Polynomial>::Node* node = lst.insert(nullptr, p);
    List<QPair<QLabel*, int>>::Node* n = num.insert(nullptr, QPair(nullptr, layout->count() + 1));
    PolynomialWidget* pw = new PolynomialWidget(ui->centralwidget, layout, node, &lst, n, &num);
    layout->addWidget(pw);
}

void MainWindow::on_multButton_clicked() {
    Polynomial p1, p2;
    bool ok;
    try {
        ChoiceDialog d(&lst, &p1, &ok);
        d.exec();
        if (!ok) return;
        ChoiceDialog d2(&lst, &p2, &ok);
        d2.exec();
        if (!ok) return;
    } catch(const char* x) {
        QMessageBox::information(nullptr, "ОШИБКА БЛЯТЬ", x);
        return;
    }
    Polynomial p = p1 * p2;
    if (QMessageBox::question(nullptr, "Multiplication result", (std::string(p) + "\nAdd it to the base?").data())) {
        AddAPolynomial(p);
    }
}


void MainWindow::on_divButton_clicked() {
    Polynomial p1, p2;
    bool ok;
    try {
        ChoiceDialog d(&lst, &p1, &ok);
        d.exec();
        if (!ok) return;
        ChoiceDialog d2(&lst, &p2, &ok);
        d2.exec();
        if (!ok) return;
    } catch(const char* x) {
        QMessageBox::information(nullptr, "ОШИБКА БЛЯТЬ", x);
        return;
    }
    QPair<Polynomial, Polynomial> p;
    try {
        p = p1 / p2;
    } catch (const char* x) {
        QMessageBox::information(nullptr, "ОШИБКА БЛЯТЬ", x);
        return;
    }

    if (QMessageBox::question(nullptr, "Division quotient", (std::string(p.first) + "\nAdd it to the base?").data())) {
        AddAPolynomial(p.first);
    }
    if (QMessageBox::question(nullptr, "Division remainder", (std::string(p.second) + "\nAdd it to the base?").data())) {
        AddAPolynomial(p.second);
    }
}


void MainWindow::on_minusButton_clicked() {
    Polynomial p1, p2;
    bool ok;
    try {
        ChoiceDialog d(&lst, &p1, &ok);
        d.exec();
        if (!ok) return;
        ChoiceDialog d2(&lst, &p2, &ok);
        d2.exec();
        if (!ok) return;
    } catch(const char* x) {
        QMessageBox::information(nullptr, "ОШИБКА БЛЯТЬ", x);
        return;
    }
    Polynomial p = p1 - p2;
    if (QMessageBox::question(nullptr, "Substraction result", (std::string(p) + "\nAdd it to the base?").data())) {
        AddAPolynomial(p);
    }
}


void MainWindow::on_plusButton_clicked() {
    Polynomial p1, p2;
    bool ok;
    try {
        ChoiceDialog d(&lst, &p1, &ok);
        d.exec();
        if (!ok) return;
        ChoiceDialog d2(&lst, &p2, &ok);
        d2.exec();
        if (!ok) return;
    } catch(const char* x) {
        QMessageBox::information(nullptr, "ОШИБКА БЛЯТЬ", x);
        return;
    }
    Polynomial p = p1 + p2;
    if (QMessageBox::question(nullptr, "Addition result", (std::string(p) + "\nAdd it to the base?").data())) {
        AddAPolynomial(p);
    }
}


void MainWindow::on_equalButton_clicked() {
    Polynomial p1, p2;
    bool ok;
    try {
        ChoiceDialog d(&lst, &p1, &ok);
        d.exec();
        if (!ok) return;
        ChoiceDialog d2(&lst, &p2, &ok);
        d2.exec();
        if (!ok) return;
    } catch(const char* x) {
        QMessageBox::information(nullptr, "ОШИБКА БЛЯТЬ", x);
        return;
    }
    bool p = (p1 == p2);
    QMessageBox::information(nullptr, "", (std::string("These polynoms are") + (p ? "" : " not") + "equal").data());
}


void MainWindow::on_enterButton_clicked() {
    bool ok;
    std::string s = QInputDialog::getText(nullptr, "", "Enter a polynom", QLineEdit::Normal, "", &ok).toStdString();
    if (!ok) return;
    std::stringstream ss;
    Polynomial p;
    try {
        ss << s;
        ss >> p;
    } catch (const char* x) {
        QMessageBox::information(nullptr, "ОШИБКА БЛЯТЬ", x);
        return;
    }
    AddAPolynomial(p);
}


void MainWindow::on_derButton_clicked() {
    Polynomial p;
    bool ok;
    try {
        ChoiceDialog d(&lst, &p, &ok);
        d.exec();
        if (!ok) return;
    } catch (const char* x) {
        QMessageBox::information(nullptr, "ОШИБКА БЛЯТЬ", x);
        return;
    }
    ll x, n;
    if (!ok) return;
    std::string s = QInputDialog::getText(nullptr, "", "Enter the variable", QLineEdit::Normal, "", &ok).toStdString();
    if (!ok) return;
    if (s.size() != 1 || s[0] < 'a' || s[0] > 'z') {
        QMessageBox::information(nullptr, "ОШИБКА БЛЯТЬ", "ЧЕ ЗА ХУЙНЮ ТЫ ВВЕЛ ЕПТА");
        return;
    }
    n = s[0] - 'a';
    s = QInputDialog::getText(nullptr, "", "Enter the order", QLineEdit::Normal, "", &ok).toStdString();
    if (!ok) return;
    if (s.empty()) {
        QMessageBox::information(nullptr, "ОШИБКА БЛЯТЬ", "ЧЕ ЗА ХУЙНЮ ТЫ ВВЕЛ ЕПТА");
        return;
    }
    for (int i = 0; i < s.size(); ++i) {
        if (s[i] < '0' || s[i] > '9') {
            QMessageBox::information(nullptr, "ОШИБКА БЛЯТЬ", "ЧЕ ЗА ХУЙНЮ ТЫ ВВЕЛ ЕПТА");
            return;
        }
    }
    x = std::stoi(s);
    Polynomial d = p.derivative(x, n);
    if (QMessageBox::question(nullptr, "Derivative", (std::string(d) + "\nAdd it to the base?").data())) {
        AddAPolynomial(d);
    }
}


void MainWindow::on_valueButton_clicked() {
    Polynomial p;
    bool ok;
    try {
        ChoiceDialog d(&lst, &p, &ok);
        d.exec();
        if (!ok) return;
    } catch(const char* x) {
        QMessageBox::information(nullptr, "ОШИБКА БЛЯТЬ", x);
        return;
    }
    std::vector<ld> pnt(26, 1);
    std::bitset<26> u = p.used();
    for (int i = 0; i < 26; ++i) {
        if (!u[i]) continue;
        std::stringstream sss;
        std::string s = QInputDialog::getText(nullptr, "", ((std::string)"Enter the value of variable " + (char)(i + 'a')).data(), QLineEdit::Normal, "", &ok).toStdString();
        if (!ok) return;
        if (s.size() == 0) {
            QMessageBox::information(nullptr, "ОШИБКА БЛЯТЬ", "ЧЕ ЗА ХУЙНЮ ТЫ ВВЕЛ ЕПТА");
            return;
        }
        sss.str("");
        sss.str(s);
        Monomial m;
        try {
            sss >> m;
            if (m.used().count()) {
                throw "ЧЕ ЗА ХУЙНЮ ТЫ ВВЕЛ ЕПТА";
            }
            pnt[i] = m.getCoef();
        } catch (const char* x) {
            QMessageBox::information(nullptr, "ОШИБКА БЛЯТЬ", x);
            return;
        }
    }
    QMessageBox::information(nullptr, "Value at a point", std::to_string(p[pnt]).data());
}

void MainWindow::on_rootsButton_clicked() {
    Polynomial p;
    bool ok;
    try {
        ChoiceDialog d(&lst, &p, &ok);
        d.exec();
        if (!ok) return;
    } catch (const char* x) {
        QMessageBox::information(nullptr, "ОШИБКА БЛЯТЬ", x);
        return;
    }
    std::vector<ll> ans = p.roots();
    std::sort(ans.begin(), ans.end());
    ll sz = std::unique(ans.begin(), ans.end()) - ans.begin();
    while (ans.size() > sz) ans.pop_back();
    if (ans.empty()) QMessageBox::information(nullptr, "", "This polynom has no roots");
    else if (ans.size() == 1 && ans[0] == INT64_MAX) QMessageBox::information(nullptr, "", "Every number is a root of this polynom");
    else {
        std::string s;
        for (ll &x : ans) {
            s += std::to_string(x) + ", ";
        }
        s.pop_back();
        s.pop_back();
        QMessageBox::information(nullptr, "Roots", ((ans.size() == 1 ? "The root of this polynom is " : "The roots of this polynom are ") + s).data());
    }
}


void MainWindow::on_loadButton_clicked() {
    std::ifstream fin("polynoms.txt");
    ll n;
    fin >> n;
    Polynomial p;
    for (int i = 0; i < n; ++i) {
        fin >> p;
        AddAPolynomial(p);
    }
}


void MainWindow::on_saveButton_clicked() {
    std::ofstream fout("polynoms.txt");
    fout << lst.size << '\n';
    for (List<Polynomial>::Node* x = lst.begin; x != nullptr; x = x->next) fout << x->val << '\n';
}

void MainWindow::on_magicButton_clicked() {
    std::vector<QColor> colors{
        QColor(255, 0, 244),
        QColor(255, 234, 18),
        QColor(255, 0, 0),
        QColor(68, 253, 11),
        QColor(225, 255, 52),
        QColor(50, 26, 123)
    };
    std::mt19937 rnd(clock());
    ui->derButton->setPalette(QPalette(colors[rnd() % 6]));
    ui->divButton->setPalette(QPalette(colors[rnd() % 6]));
    ui->multButton->setPalette(QPalette(colors[rnd() % 6]));
    ui->plusButton->setPalette(QPalette(colors[rnd() % 6]));
    ui->minusButton->setPalette(QPalette(colors[rnd() % 6]));
    ui->enterButton->setPalette(QPalette(colors[rnd() % 6]));
    ui->saveButton->setPalette(QPalette(colors[rnd() % 6]));
    ui->loadButton->setPalette(QPalette(colors[rnd() % 6]));
    ui->magicButton->setPalette(QPalette(colors[rnd() % 6]));
    ui->valueButton->setPalette(QPalette(colors[rnd() % 6]));
    ui->equalButton->setPalette(QPalette(colors[rnd() % 6]));
    ui->rootsButton->setPalette(QPalette(colors[rnd() % 6]));
    ui->scrollAreaWidgetContents->setPalette(QPalette(colors[rnd() % 6]));
    ui->scrollArea->setPalette(QPalette(colors[rnd() % 6]));
    ui->centralwidget->setPalette(QPalette(colors[rnd() % 6]));;
    ui->menubar->setPalette(QPalette(colors[rnd() % 6]));
    ui->statusbar->setPalette(QPalette(colors[rnd() % 6]));
    ui->verticalLayoutWidget->setPalette(QPalette(colors[rnd() % 6]));
}

