/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLineEdit *lineEdit_FileName;
    QLabel *label;
    QGroupBox *groupBox;
    QPushButton *pushButton_Run;
    QGroupBox *groupBox_2;
    QPushButton *pushButton_Cut;
    QGroupBox *groupBox_3;
    QPushButton *pushButton_BDT;
    QDoubleSpinBox *doubleSpinBox_BDTVal;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(224, 265);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        lineEdit_FileName = new QLineEdit(centralWidget);
        lineEdit_FileName->setObjectName(QStringLiteral("lineEdit_FileName"));
        lineEdit_FileName->setGeometry(QRect(40, 10, 171, 21));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 10, 57, 20));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 40, 201, 61));
        pushButton_Run = new QPushButton(groupBox);
        pushButton_Run->setObjectName(QStringLiteral("pushButton_Run"));
        pushButton_Run->setGeometry(QRect(10, 28, 181, 31));
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 110, 201, 71));
        pushButton_Cut = new QPushButton(groupBox_2);
        pushButton_Cut->setObjectName(QStringLiteral("pushButton_Cut"));
        pushButton_Cut->setGeometry(QRect(10, 30, 181, 31));
        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 180, 201, 71));
        pushButton_BDT = new QPushButton(groupBox_3);
        pushButton_BDT->setObjectName(QStringLiteral("pushButton_BDT"));
        pushButton_BDT->setGeometry(QRect(10, 30, 111, 31));
        doubleSpinBox_BDTVal = new QDoubleSpinBox(groupBox_3);
        doubleSpinBox_BDTVal->setObjectName(QStringLiteral("doubleSpinBox_BDTVal"));
        doubleSpinBox_BDTVal->setGeometry(QRect(130, 31, 62, 30));
        doubleSpinBox_BDTVal->setMinimum(-1);
        doubleSpinBox_BDTVal->setMaximum(1);
        doubleSpinBox_BDTVal->setSingleStep(0.1);
        doubleSpinBox_BDTVal->setValue(-0.1);
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Cap Finder", 0));
        lineEdit_FileName->setText(QApplication::translate("MainWindow", "Analysis", 0));
        label->setText(QApplication::translate("MainWindow", "File:", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "1. Meta Data Configuration", 0));
        pushButton_Run->setText(QApplication::translate("MainWindow", "Build Image", 0));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "2. Perform Cuts", 0));
        pushButton_Cut->setText(QApplication::translate("MainWindow", "Open Cut Dialog", 0));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "3. Run BDT", 0));
        pushButton_BDT->setText(QApplication::translate("MainWindow", "BDT", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
