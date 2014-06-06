#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <clientcontroller.h>

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(ClientController* cc, QWidget *parent = 0);
    ~Form();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Form *ui;
    ClientController* _cc;
};

#endif // FORM_H
