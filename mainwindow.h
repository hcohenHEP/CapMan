#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "imagedatabuilder.h"
#include "singlefileprogressdialog.h"
#include <QMainWindow>
#include <QMutex>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void RunSingleFile(QString filez);

public slots:
    void Run();
    void Cut();
    void BDT();
    void updateProgress();
    void updateProgressMultipleFiles();


private:
    QMutex mutex;
    Ui::MainWindow *ui;
    QTimer* m_GuiTimer;
    QTimer* m_MultipleFilesTimer;
    ImageDataBuilder* m_ImageDataBuilder;
    bool m_IsBatchRunning;
    SingleFileProgressDialog* m_SingleFileProgressDialog;
    QList<QString> m_Files;
    int m_CurrentFileIndex;
};

#endif // MAINWINDOW_H
