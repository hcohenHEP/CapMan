/********************************************************************************
** Form generated from reading UI file 'cutsdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CUTSDIALOG_H
#define UI_CUTSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_CutsDialog
{
public:
    QGroupBox *groupBox;
    QDoubleSpinBox *doubleSpinBox_Min;
    QLabel *label;
    QDoubleSpinBox *doubleSpinBox_Max;
    QLabel *label_2;
    QDoubleSpinBox *doubleSpinBox_Intensity;
    QLabel *label_3;
    QDoubleSpinBox *doubleSpinBox_Density;
    QLabel *label_4;
    QPushButton *pushButton_Refresh;
    QLabel *label_5;
    QDoubleSpinBox *doubleSpinBox_Steps;
    QLabel *label_6;
    QLabel *label_7;
    QGroupBox *groupBox_2;
    QLabel *label_SmallPixels;
    QLabel *label_LargePixels;
    QLabel *label_Ratio;
    QLabel *label_RatioSmall;
    QLabel *label_RatioBig;

    void setupUi(QDialog *CutsDialog)
    {
        if (CutsDialog->objectName().isEmpty())
            CutsDialog->setObjectName(QStringLiteral("CutsDialog"));
        CutsDialog->setWindowModality(Qt::WindowModal);
        CutsDialog->resize(211, 411);
        groupBox = new QGroupBox(CutsDialog);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 191, 231));
        doubleSpinBox_Min = new QDoubleSpinBox(groupBox);
        doubleSpinBox_Min->setObjectName(QStringLiteral("doubleSpinBox_Min"));
        doubleSpinBox_Min->setGeometry(QRect(80, 33, 66, 24));
        doubleSpinBox_Min->setDecimals(1);
        doubleSpinBox_Min->setMaximum(1000);
        doubleSpinBox_Min->setValue(15);
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(19, 40, 57, 15));
        doubleSpinBox_Max = new QDoubleSpinBox(groupBox);
        doubleSpinBox_Max->setObjectName(QStringLiteral("doubleSpinBox_Max"));
        doubleSpinBox_Max->setGeometry(QRect(80, 68, 66, 24));
        doubleSpinBox_Max->setDecimals(1);
        doubleSpinBox_Max->setMaximum(1000);
        doubleSpinBox_Max->setValue(200);
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 75, 57, 15));
        doubleSpinBox_Intensity = new QDoubleSpinBox(groupBox);
        doubleSpinBox_Intensity->setObjectName(QStringLiteral("doubleSpinBox_Intensity"));
        doubleSpinBox_Intensity->setGeometry(QRect(80, 98, 66, 24));
        doubleSpinBox_Intensity->setDecimals(3);
        doubleSpinBox_Intensity->setMaximum(100);
        doubleSpinBox_Intensity->setSingleStep(0.01);
        doubleSpinBox_Intensity->setValue(0.1);
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 104, 57, 15));
        doubleSpinBox_Density = new QDoubleSpinBox(groupBox);
        doubleSpinBox_Density->setObjectName(QStringLiteral("doubleSpinBox_Density"));
        doubleSpinBox_Density->setGeometry(QRect(80, 128, 66, 24));
        doubleSpinBox_Density->setDecimals(2);
        doubleSpinBox_Density->setMaximum(1);
        doubleSpinBox_Density->setSingleStep(0.01);
        doubleSpinBox_Density->setValue(1);
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(20, 133, 57, 15));
        pushButton_Refresh = new QPushButton(groupBox);
        pushButton_Refresh->setObjectName(QStringLiteral("pushButton_Refresh"));
        pushButton_Refresh->setGeometry(QRect(10, 190, 151, 31));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(20, 160, 57, 15));
        doubleSpinBox_Steps = new QDoubleSpinBox(groupBox);
        doubleSpinBox_Steps->setObjectName(QStringLiteral("doubleSpinBox_Steps"));
        doubleSpinBox_Steps->setGeometry(QRect(80, 155, 91, 24));
        doubleSpinBox_Steps->setDecimals(0);
        doubleSpinBox_Steps->setMaximum(500000);
        doubleSpinBox_Steps->setSingleStep(0.01);
        doubleSpinBox_Steps->setValue(600);
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(149, 36, 57, 20));
        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(148, 70, 57, 20));
        groupBox_2 = new QGroupBox(CutsDialog);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 250, 191, 101));
        label_SmallPixels = new QLabel(groupBox_2);
        label_SmallPixels->setObjectName(QStringLiteral("label_SmallPixels"));
        label_SmallPixels->setGeometry(QRect(20, 30, 141, 16));
        label_LargePixels = new QLabel(groupBox_2);
        label_LargePixels->setObjectName(QStringLiteral("label_LargePixels"));
        label_LargePixels->setGeometry(QRect(20, 50, 141, 16));
        label_Ratio = new QLabel(groupBox_2);
        label_Ratio->setObjectName(QStringLiteral("label_Ratio"));
        label_Ratio->setGeometry(QRect(20, 80, 141, 16));
        label_RatioSmall = new QLabel(CutsDialog);
        label_RatioSmall->setObjectName(QStringLiteral("label_RatioSmall"));
        label_RatioSmall->setGeometry(QRect(30, 355, 141, 16));
        label_RatioBig = new QLabel(CutsDialog);
        label_RatioBig->setObjectName(QStringLiteral("label_RatioBig"));
        label_RatioBig->setGeometry(QRect(30, 380, 141, 16));

        retranslateUi(CutsDialog);

        QMetaObject::connectSlotsByName(CutsDialog);
    } // setupUi

    void retranslateUi(QDialog *CutsDialog)
    {
        CutsDialog->setWindowTitle(QApplication::translate("CutsDialog", "Cut Dialog", 0));
        groupBox->setTitle(QApplication::translate("CutsDialog", "Cut Parameters", 0));
        label->setText(QApplication::translate("CutsDialog", "Min", 0));
        label_2->setText(QApplication::translate("CutsDialog", "Max", 0));
        label_3->setText(QApplication::translate("CutsDialog", "<html><head/><body><p>Intensity</p></body></html>", 0));
        label_4->setText(QApplication::translate("CutsDialog", "Density", 0));
        pushButton_Refresh->setText(QApplication::translate("CutsDialog", "Generate New Cut", 0));
        label_5->setText(QApplication::translate("CutsDialog", "R Steps", 0));
        label_6->setText(QApplication::translate("CutsDialog", "<html><head/><body><p>\316\274m</p></body></html>", 0));
        label_7->setText(QApplication::translate("CutsDialog", "<html><head/><body><p>\316\274m</p></body></html>", 0));
        groupBox_2->setTitle(QApplication::translate("CutsDialog", "Latest Cut Results", 0));
        label_SmallPixels->setText(QApplication::translate("CutsDialog", "Small Count:", 0));
        label_LargePixels->setText(QApplication::translate("CutsDialog", "Big Count:", 0));
        label_Ratio->setText(QApplication::translate("CutsDialog", "<html><head/><body><p>Ratio(S/B):</p></body></html>", 0));
        label_RatioSmall->setText(QApplication::translate("CutsDialog", "<html><head/><body><p>Ratio(S/B):</p></body></html>", 0));
        label_RatioBig->setText(QApplication::translate("CutsDialog", "<html><head/><body><p>Ratio(S/B):</p></body></html>", 0));
    } // retranslateUi

};

namespace Ui {
    class CutsDialog: public Ui_CutsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CUTSDIALOG_H
