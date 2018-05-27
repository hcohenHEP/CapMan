/********************************************************************************
** Form generated from reading UI file 'singlefileprogressdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SINGLEFILEPROGRESSDIALOG_H
#define UI_SINGLEFILEPROGRESSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>

QT_BEGIN_NAMESPACE

class Ui_SingleFileProgressDialog
{
public:
    QProgressBar *progressBar;
    QLabel *label_CurretnFile;

    void setupUi(QDialog *SingleFileProgressDialog)
    {
        if (SingleFileProgressDialog->objectName().isEmpty())
            SingleFileProgressDialog->setObjectName(QStringLiteral("SingleFileProgressDialog"));
        SingleFileProgressDialog->setWindowModality(Qt::NonModal);
        SingleFileProgressDialog->resize(215, 84);
        SingleFileProgressDialog->setWindowTitle(QStringLiteral("File Progress"));
        progressBar = new QProgressBar(SingleFileProgressDialog);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setGeometry(QRect(12, 50, 190, 23));
        progressBar->setValue(24);
        label_CurretnFile = new QLabel(SingleFileProgressDialog);
        label_CurretnFile->setObjectName(QStringLiteral("label_CurretnFile"));
        label_CurretnFile->setGeometry(QRect(10, 10, 191, 30));

        retranslateUi(SingleFileProgressDialog);

        QMetaObject::connectSlotsByName(SingleFileProgressDialog);
    } // setupUi

    void retranslateUi(QDialog *SingleFileProgressDialog)
    {
        label_CurretnFile->setText(QApplication::translate("SingleFileProgressDialog", "<html><head/><body><p align=\"center\">Current File: 7.tif</p></body></html>", 0));
        Q_UNUSED(SingleFileProgressDialog);
    } // retranslateUi

};

namespace Ui {
    class SingleFileProgressDialog: public Ui_SingleFileProgressDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SINGLEFILEPROGRESSDIALOG_H
