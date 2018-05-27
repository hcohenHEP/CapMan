#ifndef CUTSDIALOG_H
#define CUTSDIALOG_H

#include <QDialog>
#include <TTree.h>
#include <QCoreApplication>
#include <TASImage.h>
#include <QPair>
#include <TTree.h>
#include <TH2D.h>
#include <QList>
#include <TFile.h>
#include <QString>
#include <math.h>
#include <iostream>
#include <numeric>
#include <string>
#include <QMap>
#include <TH1D.h>
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"
#include <stdio.h>
#include <stdlib.h>
namespace Ui {
class CutsDialog;
}

class CutsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CutsDialog(QWidget *parent = 0);
    ~CutsDialog();

    void Init(TFile* file, QString outputDir, QString inputFileName);

    void DisplayImgHist(TTree* tree, UInt_t xPixels, UInt_t yPixels, QString outputDir);


public slots:
    void Refresh();

private:
    Ui::CutsDialog *ui;
    Float_t fVal;
    Float_t fMin;
    Float_t fMax;
    Float_t fNeighborsIntensity;
    Float_t fNeighborsDensity;
    Float_t fSteps;
    Int_t fX;
    Int_t fY;
    Double_t fMVAWeight;
    Bool_t fIsBlack;
    Bool_t fIsIgnored;
    UInt_t fXPixels;
    UInt_t fYPixels;
    Float_t fIgnoredPixels;
    UInt_t fSize;
    TTree* fTree;
    QString fOutputDir;
    QString fInputFileName;
};

#endif // CUTSDIALOG_H
