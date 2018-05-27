#ifndef SINGLEFILEPROGRESSDIALOG_H
#define SINGLEFILEPROGRESSDIALOG_H

#include <QCloseEvent>
#include <QDialog>

namespace Ui {
class SingleFileProgressDialog;
}

class SingleFileProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SingleFileProgressDialog(QWidget *parent = 0);
    void Init(QString file);
    void UpdateProgress(int precentage);
    ~SingleFileProgressDialog();

    virtual void closeEvent(QCloseEvent *evt);

private:
    Ui::SingleFileProgressDialog *ui;
};

#endif // SINGLEFILEPROGRESSDIALOG_H
