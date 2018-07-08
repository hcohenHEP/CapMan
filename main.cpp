#include "mainwindow.h"
#include <TStyle.h>
#include <QList>
#include <TH2F.h>
#include "RootStaticDisplayInterface.h"
#include "mvatrainer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Regular Work
    MainWindow w;w.show();

    //MVA Training
    //MVATrainer trainer;
    //trainer.TrainMVA({"7_bkg.tif.root","10_bkg.tif.root","11_bkg.tif.root","12_bkg.tif.root"},{"7_sig.tif.root","10_sig.tif.root","11_sig.tif.root","12_sig.tif.root"});

    return a.exec();
}
