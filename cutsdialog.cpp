#include "cutsdialog.h"
#include "ui_cutsdialog.h"
#include <TH2I.h>
#include <TCanvas.h>
#include "RootStaticDisplayInterface.h"
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include "imagehelper.h"
#include <TStyle.h>
#include <TLegend.h>
#include <TF1.h>

CutsDialog::CutsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CutsDialog)
{
    ui->setupUi(this);
    connect(ui->pushButton_Refresh,SIGNAL(clicked()),this,SLOT(Refresh()));

}

void CutsDialog::Init(TFile *file, QString outputDir, QString inputFileName)
{
    fInputFileName = inputFileName;
    fTree = (TTree*)file->Get("imgTree");
    TH2F* histMin= (TH2F*)file->Get("histMin");histMin->GetZaxis()->SetLabelSize(0.025);
    TH2F* histMax= (TH2F*)file->Get("histMax");histMax->GetZaxis()->SetLabelSize(0.025);
    TH2F* histInt= (TH2F*)file->Get("histInt");histInt->GetZaxis()->SetLabelSize(0.025);
    TH2D* histMap= (TH2D*)file->Get("histMap");histMap->GetZaxis()->SetLabelSize(0.025);
    TH2F* histSteps= (TH2F*)file->Get("histSteps");histSteps->GetZaxis()->SetLabelSize(0.02);

    RootStaticDisplayInterface::Instance().AddHist(histMap,"COLZ",false);
    RootStaticDisplayInterface::Instance().FlushData(true,true,"Output","OutputMap" + QString(file->GetName()));
    RootStaticDisplayInterface::Instance().AddHist(histMax,"COLZ",false);
    RootStaticDisplayInterface::Instance().FlushData(false,true,"Output","OutputMax" + QString(file->GetName()));
    RootStaticDisplayInterface::Instance().AddHist(histMin,"COLZ",false);
    RootStaticDisplayInterface::Instance().FlushData(false,true,"Output","OutputMin" + QString(file->GetName()));
    RootStaticDisplayInterface::Instance().AddHist(histSteps,"COLZ",false);
    RootStaticDisplayInterface::Instance().FlushData(false,true,"Output","OutputSteps" + QString(file->GetName()));
    RootStaticDisplayInterface::Instance().AddHist(histInt,"COLZ",false);
    RootStaticDisplayInterface::Instance().FlushData(false,true,"Output","OutputInt" + QString(file->GetName()));



    //RootStaticDisplayInterface::Instance().AddHist(histDen,"COLZ",false);





    fXPixels = ((TObjString*)file->Get("xPixels"))->String().Atoi();
    fYPixels = ((TObjString*)file->Get("yPixels"))->String().Atoi();
    fIgnoredPixels = ((TObjString*)file->Get("ignoredPixels"))->String().Atoi();
    fTree->SetBranchAddress("x",&fX);
    fTree->SetBranchAddress("y",&fY);
    fTree->SetBranchAddress("val", &fVal);
    fTree->SetBranchAddress("minDist", &fMin);
    fTree->SetBranchAddress("maxDist",&fMax);
    fTree->SetBranchAddress("n_I", &fNeighborsIntensity);
    fTree->SetBranchAddress("n_D", &fNeighborsDensity);
    fTree->SetBranchAddress("is_black",&fIsBlack);
    fTree->SetBranchAddress("steps",&fSteps);
    fTree->SetBranchAddress("is_ignored",&fIsIgnored);
    fOutputDir = outputDir;
    fSize = fTree->GetEntriesFast();
}

void CutsDialog::Refresh()
{
    //New cut results
    static int histIndex = 0;

    histIndex++;
    TH2F* hist= new TH2F(TString::Format("hist%d",histIndex),"",fXPixels,0,fXPixels,fYPixels,0,fYPixels);
    hist->SetStats(0);
    float smallPixelsCount = 0;
    float bigPixelCount = 0;
    for ( UInt_t i = 0 ; i < fSize ; i ++)
    {
        fTree->GetEntry(i);
        if (fIsIgnored == 1.0 )
        {
            hist->SetBinContent(fX+1,fY+1,1);
        }
        else if (   fMax <= ui->doubleSpinBox_Max->value() &&
                    fMin <= ui->doubleSpinBox_Min->value() &&
                    fNeighborsDensity <= ui->doubleSpinBox_Density->value() &&
                    fNeighborsIntensity <= ui->doubleSpinBox_Intensity->value()  &&
                    fSteps <= ui->doubleSpinBox_Steps->value() )
        {
            hist->SetBinContent(fX+1,fY+1,10);
            smallPixelsCount++;
        }
        else
        {
            hist->SetBinContent(fX+1,fY+1,5);
            bigPixelCount++;
        }
    }

    ui->label_SmallPixels->setText("Small Pixels: " + QString::number(smallPixelsCount));
    ui->label_LargePixels->setText("Big Pixels: " + QString::number(bigPixelCount));

    float ratio = (smallPixelsCount/bigPixelCount) ;
    float rationSmall = smallPixelsCount/(fYPixels*fXPixels - fIgnoredPixels - bigPixelCount) ;
    float rationBig = bigPixelCount/(fYPixels*fXPixels - fIgnoredPixels) ;
    ui->label_Ratio->setText("Ratio(S/ALL-B): " + QString::number(ratio));
    ui->label_RatioSmall->setText("Ratio(S/ALL): " + QString::number(rationSmall));
    ui->label_RatioBig->setText("Ratio(B/ALL): " + QString::number(rationBig));

    QString title = "Cut Params - Min:" +QString::number(ui->doubleSpinBox_Min->value()) +
            " Max:"  + QString::number(ui->doubleSpinBox_Max->value()) +" Intensity:" + QString::number(ui->doubleSpinBox_Intensity->value()) +
            " Density:" +QString::number(ui->doubleSpinBox_Density->value()) + " RSteps:" +QString::number(ui->doubleSpinBox_Steps->value()) +
            " Ratio:" + QString::number(ratio) + "  BigPixels:" + QString::number(bigPixelCount);
    hist->SetTitle("");

    TCanvas* can = new TCanvas(hist->GetTitle(),"FinalCut",700,700);


    can->SetFrameFillColor(kBlack);
    hist->GetXaxis()->SetLabelOffset(999);
    hist->GetXaxis()->SetLabelSize(0);

    hist->GetYaxis()->SetLabelOffset(999);
    hist->GetYaxis()->SetLabelSize(0);

    hist->Draw("COL");

    TLegend * legend = new TLegend(0.68,0.805,.895,0.895);
    legend->SetBorderSize(1);
    legend->SetTextSize(.02);
    legend->SetFillColorAlpha(kWhite,100);

    //TMPPP
    TF1 *f1=new TF1("f1","1000*TMath::Abs(sin(x)/x)",-10,10);
    f1->SetLineColor(kRed);
    f1->SetLineWidth(4);

    TF1 *f2=new TF1("f2","1000*TMath::Abs(sin(x)/x)",-10,10);
    f2->SetLineColor(kGreen);
    f2->SetLineWidth(4);

    //TMPP
    legend->AddEntry(f1,"Arteries and Veins","l");
    legend->AddEntry(f2,"Choroid Plexus","l");
    legend->Draw();

    RootStaticDisplayInterface::Instance().AddCanvas(can);
    RootStaticDisplayInterface::Instance().FlushData(false);


    //Write out all of the parameters used in order to create the analysis
    FILE* settingsFile = fopen(QString(fOutputDir + "/Final.params").toStdString().c_str(),"w");
    QTextStream settingsStream(settingsFile);
    settingsStream<<"File Name:   "<<title<<endl;
    settingsStream<<"Input Settings"<<endl;
    settingsStream<<"--------------"<<endl;
    settingsStream<<"File\t"<<fInputFileName<<endl;
    settingsStream<<"X Pixels\t"<<fXPixels<<endl;
    settingsStream<<"Y Pixels\t"<<fYPixels<<endl;
    settingsStream<<"Ignored Pixels\t"<<fIgnoredPixels<<endl;
    settingsStream<<"\n\n\nParameters Used To Produce Final.root"<<endl;
    settingsStream<<"-------------------------------------"<<endl;
    settingsStream<<"Min\t"<<ui->doubleSpinBox_Min->value()<<endl;
    settingsStream<<"Max\t"<<ui->doubleSpinBox_Max->value()<<endl;
    settingsStream<<"Intensity\t"<<ui->doubleSpinBox_Intensity->value()<<endl;
    settingsStream<<"Density\t"<<ui->doubleSpinBox_Density->value()<<endl;
    settingsStream<<"R Steps\t"<<ui->doubleSpinBox_Steps->value()<<endl;
    settingsStream<<"\n\n\nResults Obtained"<<endl;
    settingsStream<<"----------------------"<<endl;
    settingsStream<<"Ration (S/B)\t"<<ratio<<endl;
    settingsStream<<"Ration (S/ALL-BIG)\t"<<rationSmall<<endl;
    settingsStream<<"Ration (B/ALL)\t"<<rationBig<<endl;
    settingsStream.flush();
    fclose(settingsFile);
}

CutsDialog::~CutsDialog()
{
    delete ui;
}
