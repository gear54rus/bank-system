#include "form.h"
#include "ui_form.h"

Form::Form(ClientController* cc, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    _cc = cc;
}

Form::~Form()
{
    delete ui;
}

void Form::on_pushButton_clicked()
{
    _cc->next();
}
