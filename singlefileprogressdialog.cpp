#include "singlefileprogressdialog.h"
#include "ui_singlefileprogressdialog.h"

SingleFileProgressDialog::SingleFileProgressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SingleFileProgressDialog)
{
    ui->setupUi(this);
}

SingleFileProgressDialog::~SingleFileProgressDialog()
{
    delete ui;
}
void SingleFileProgressDialog::Init(QString file)
{
    ui->label_CurretnFile->setText("Current File: " + file);
    ui->progressBar->setValue(0);
}

void SingleFileProgressDialog::UpdateProgress(int precentage)
{
    ui->progressBar->setValue(precentage);
}
void SingleFileProgressDialog::closeEvent(QCloseEvent *evt)
{
    if ( ui->progressBar->value() != 100 )
    {
        evt->ignore();
    }
}
