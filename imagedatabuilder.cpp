#include "imagedatabuilder.h"
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
#include <QMessageBox>
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TMVA/Factory.h"
#include "TMVA/Reader.h"
#include "TMVA/Tools.h"
#include "general.h"
#include "imagehelper.h"
#include <stdio.h>
#include <stdlib.h>
#include "mainwindow.h"
#include "RootStaticDisplayInterface.h"

void FormatHistogram(TH1* hist)
{
    hist->GetXaxis()->SetLabelOffset(999);
    hist->GetXaxis()->SetLabelSize(0);

    hist->GetYaxis()->SetLabelOffset(999);
    hist->GetYaxis()->SetLabelSize(0);

    hist->GetZaxis()->SetTickLength(0.005);
    hist->GetZaxis()->SetLabelOffset(-0.04);
    hist->GetZaxis()->SetLabelSize(0.03);
    hist->GetZaxis()->SetTitleOffset(0.44);
}

void WorkDistance(int xA, int yA, int xB, int yB, float* min, float* max)
{
    float dis = sqrt( pow((xA-xB),2) + pow((yA-yB),2));
    if ( *min > dis ) (*min) = dis;
    if ( *max < dis ) (*max) = dis;
}

ImageDataBuilder::ImageDataBuilder(QString fileName, int blackPixelThreshold, int distanceRadius)
{
    Progress = 0;
    m_FileName = fileName;
    m_BlackPixelThreshold = blackPixelThreshold;
    m_DistanceRadius = distanceRadius;

}


void ImageDataBuilder::run()
{
    //Preform Median Filter For a better image processing
    //PreMedianFilterImage(2,fileName,TString::Format("%s.MED",fileName).Data());

    TASImage image(TString::Format("%s",m_FileName.toStdString().c_str()).Data());
    TFile f(TString::Format("%s.root",m_FileName.toStdString().c_str()),"RECREATE");
    UInt_t yPixels = image.GetHeight();
    UInt_t xPixels = image.GetWidth();
    UInt_t ignoredPixels = 0;
    TArrayD* arr = image.GetArray();
    float progressBarInc = 100./(float)(3*xPixels*yPixels);

    ImageManager::getInstance().Init(xPixels,yPixels,arr);

    //Tree vars
    TTree* tree = new TTree("imgTree","Img Tree");
    Float_t fVal=0;
    Float_t fMin=0,fMax=0,fNeighborsIntensity=0,fNeighborsDensity=0;
    Int_t fX,fY;
    Float_t fSteps;
    Float_t fIsIgnored;
    tree->Branch("x",&fX,"x/I");
    tree->Branch("y",&fY,"y/I");
    tree->Branch("val",&fVal,"val/F");
    tree->Branch("minDist",&fMin,"minDist/F");
    tree->Branch("maxDist",&fMax,"maxDist/F");
    tree->Branch("n_I",&fNeighborsIntensity,"n_I/F");
    tree->Branch("n_D",&fNeighborsDensity,"n_D/F");
    tree->Branch("steps",&fSteps,"steps/F");
    tree->Branch("is_ignored",&fIsIgnored,"is_ignored/F");

    //Setup the histograms
    TH2D* histMap = new TH2D("histMap","Raw Image",xPixels,0,xPixels,yPixels,0,yPixels);FormatHistogram(histMap);
    TH2F* histDen= new TH2F("histDen","Relative Density",xPixels,0,xPixels,yPixels,0,yPixels);FormatHistogram(histDen);
    TH2F* histInt= new TH2F("histInt","Intensity Map",xPixels,0,xPixels,yPixels,0,yPixels);FormatHistogram(histInt);
    TH2F* histMin= new TH2F("histMin","Oriented Minimum Distance Map",xPixels,0,xPixels,yPixels,0,yPixels);FormatHistogram(histMin);
    TH2F* histMax= new TH2F("histMax","Oriented Maximum Distance Map",xPixels,0,xPixels,yPixels,0,yPixels);FormatHistogram(histMax);
    TH2F* histSteps = new TH2F("histSteps","Maximum Random Step Map",xPixels,0,xPixels,yPixels,0,yPixels);FormatHistogram(histSteps);

    TH2D* histInitialBig = new TH2D("histInitialBig","histInitialBig",xPixels,0,xPixels,yPixels,0,yPixels);FormatHistogram(histInitialBig);
    TH2D* histInitialSmall = new TH2D("histInitialSmall","histInitialSmall",xPixels,0,xPixels,yPixels,0,yPixels);FormatHistogram(histInitialSmall);

    histMax->GetZaxis()->SetTitle("$\\LEFTRIGHT\\langle{\\mu m}\\rangle$");
    histMin->GetZaxis()->SetTitle("$\\LEFTRIGHT\\langle{\\mu m}\\rangle$");
    histDen->GetZaxis()->SetTitle("arbitrary units");
    histDen->GetZaxis()->SetTitleSize(0.035);
    histInt->GetZaxis()->SetTitle("arbitrary units");
    histSteps->GetZaxis()->SetTitle("$\\LEFTRIGHT\\langle{\\mu m}\\rangle$");

    //1. Build the initial parameters

    for (UInt_t yi=0; yi<yPixels; yi++)
    {
        for (UInt_t xi=0; xi<xPixels; xi++)
        {
            fVal = ImageManager::getInstance().GetPixelValue(xi,yi);

            Progress+=progressBarInc;
            if ( fVal > 0.98 ) // it's a cut color, raise the ignored pixels count and move on
            {
                ignoredPixels++;
                continue;
            }

            if (fVal <= 0.0)
            {
                continue;
            }

            histMap->SetBinContent(xi+1,yi+1,fVal);

            fX = xi;
            fY = yi;

            fNeighborsIntensity = ImageHelper::Instance().FindNeighborsIntensity(xi,yi,m_DistanceRadius);
            ImageManager::getInstance().AddInitialIntensity(xi,yi,fNeighborsIntensity);

//0701 - useless, work with onlyh 1 histogram!
            histInitialSmall->SetBinContent(xi+1,yi+1,1);
            histInitialBig->SetBinContent(xi+1,yi+1,1);

            /*
            //2. Cut an initial cut based only on the intensity of the pixel
            if ( fNeighborsIntensity <= ImageManager::getInstance().avgValue)
            {
                histInitialSmall->SetBinContent(xi+1,yi+1,1);
            }
            else
            {
                histInitialBig->SetBinContent(xi+1,yi+1,1);
            }*/

        }
    }

    //3.Now we have 2 seperate histograms, lets run on them and combine the results

    for (UInt_t yi=0; yi<yPixels; yi++)
    {
        for (UInt_t xi=0; xi<xPixels; xi++)
        {
            Progress+=progressBarInc;

            fVal = ImageManager::getInstance().GetPixelValue(xi,yi);

            if (fVal == 0 || fVal > 0.98 )
            {
                continue;
            }

            bool isBig = (histInitialBig->GetBinContent(xi+1,yi+1) == 1 ) ? true : false;
            TH2D* histInitial = ( isBig == true ) ? histInitialBig : histInitialSmall;

            //4. Calculate Min/Max initial values using the current hist (small/big)
            float dis = -1;
            fMin = 1000000;
            fMax = 0;

            int dX,dY,uX,uY,ulX,ulY,urX,urY,dlX,dlY,drX,drY,lX,lY,rX,rY;
            int ddlX,ddlY,uulX,uulY,ullX,ullY,urrX,urrY,dllX,dllY,drrX,drrY,uurX,uurY,ddrX,ddrY;

            ImageHelper::Instance().FindNearestBlack(histInitial,xi,yi,UP,1,&uX,&uY);
            ImageHelper::Instance().FindNearestBlack(histInitial,xi,yi,DOWN,1,&dX,&dY);
            ImageHelper::Instance().FindNearestBlack(histInitial,xi,yi,LEFT,1,&lX,&lY);
            ImageHelper::Instance().FindNearestBlack(histInitial,xi,yi,RIGHT,1,&rX,&rY);
            ImageHelper::Instance().FindNearestBlack(histInitial,xi,yi,UP_RIGHT,1,&urX,&urY);
            ImageHelper::Instance().FindNearestBlack(histInitial,xi,yi,UP_LEFT,1,&ulX,&ulY);
            ImageHelper::Instance().FindNearestBlack(histInitial,xi,yi,DOWN_RIGHT,1,&drX,&drY);
            ImageHelper::Instance().FindNearestBlack(histInitial,xi,yi,DOWN_LEFT,1,&dlX,&dlY);
            ImageHelper::Instance().FindNearestBlack(histInitial,xi,yi,UP_UP_LEFT,1,&uulX,&uulY);
            ImageHelper::Instance().FindNearestBlack(histInitial,xi,yi,DOWN_DOWN_RIGHT,1,&ddrX,&ddrY);
            ImageHelper::Instance().FindNearestBlack(histInitial,xi,yi,UP_LEFT_LEFT,1,&ullX,&ullY);
            ImageHelper::Instance().FindNearestBlack(histInitial,xi,yi,DOWN_RIGHT_RIGHT,1,&drrX,&drrY);
            ImageHelper::Instance().FindNearestBlack(histInitial,xi,yi,UP_UP_RIGHT,1,&uurX,&uurY);
            ImageHelper::Instance().FindNearestBlack(histInitial,xi,yi,DOWN_DOWN_LEFT,1,&ddlX,&ddlY);
            ImageHelper::Instance().FindNearestBlack(histInitial,xi,yi,UP_RIGHT_RIGHT,1,&urrX,&urrY);
            ImageHelper::Instance().FindNearestBlack(histInitial,xi,yi,DOWN_LEFT_LEFT,1,&dllX,&dllY);

            WorkDistance(uX,uY,dX,dY,&fMin,&fMax);
            WorkDistance(rX,rY,lX,lY,&fMin,&fMax);
            WorkDistance(urX,urY,dlX,dlY,&fMin,&fMax);
            WorkDistance(ulX,ulY,drX,drY,&fMin,&fMax);

            WorkDistance(uurX,uurY,ddlX,ddlY,&fMin,&fMax);
            WorkDistance(uulX,uulY,ddrX,ddrY,&fMin,&fMax);
            WorkDistance(ullX,ullY,drrX,drrY,&fMin,&fMax);
            WorkDistance(urrX,urrY,dllX,dllY,&fMin,&fMax);

            WorkDistance(uurX,uurY,dX,dY,&fMin,&fMax);
            WorkDistance(uulX,uulY,dX,dY,&fMin,&fMax);
            WorkDistance(uX,uY,ddlX,ddlY,&fMin,&fMax);
            WorkDistance(uX,uY,ddrX,ddrY,&fMin,&fMax);

            WorkDistance(urX,urY,ddlX,ddlY,&fMin,&fMax);
            WorkDistance(ulX,ulY,ddrX,ddrY,&fMin,&fMax);
            WorkDistance(uurX,uurY,dlX,dlY,&fMin,&fMax);
            WorkDistance(uulX,uulY,drX,drY,&fMin,&fMax);

            ImageManager::getInstance().AddInitialMinDist(xi,yi,fMin,isBig);//TBD do that for small initial and big intial seperatly
            ImageManager::getInstance().AddInitialMaxDist(xi,yi,fMax,isBig);
            ImageManager::getInstance().AddInitialRandomWalkSteps(xi,yi,ImageHelper::Instance().RandomWalkSteps(histInitial,xi,yi),isBig);
        }
    }

    //5. Combine results
    for (UInt_t yi=0; yi<yPixels; yi++)
    {
        for (UInt_t xi=0; xi<xPixels; xi++)
        {
            Progress+=progressBarInc;

            fVal = ImageManager::getInstance().GetPixelValue(xi,yi);

            if (fVal == 0 )
            {
                continue;
            }

            fX = xi;
            fY = yi;
            fIsIgnored = 0.0;

            if ( fVal > 0.98 )
            {
                fIsIgnored = 1.0;
                tree->Fill();
                continue;
            }

            bool isBig = (histInitialBig->GetBinContent(xi+1,yi+1) == 1 ) ? true : false;

            fNeighborsIntensity = ImageHelper::Instance().FindNeighborsIntensity(xi,yi,2);
            histInt->SetBinContent(xi+1,yi+1,fNeighborsIntensity);
            fNeighborsDensity = ImageHelper::Instance().FindNeighborsDensity(xi,yi,m_DistanceRadius*4,isBig);
            histDen->SetBinContent(xi+1,yi+1,fNeighborsDensity);
            fSteps = ImageHelper::Instance().FindAvgRandomWalkSteps(xi,yi,m_DistanceRadius,isBig);
            fSteps*=0.578;
            histSteps->SetBinContent(xi+1,yi+1,fSteps);

            fMin = ImageHelper::Instance().FindAvgMinDistance(xi,yi,m_DistanceRadius,isBig);
            fMin*=0.578;//Convert to micro meter
            fMax = ImageHelper::Instance().FindAvgMaxDistance(xi,yi,m_DistanceRadius,isBig);
            fMax*=0.578;//Convert to micro meter
            histMin->SetBinContent(xi+1,yi+1,fMin);
            histMax->SetBinContent(xi+1,yi+1,fMax);

            tree->Fill();
        }
    }

    Progress = 100;
    /*RootStaticDisplayInterface::Instance().AddHist(histMap,"COLZ",false);
    RootStaticDisplayInterface::Instance().AddHist(histInt,"COLZ",false);
    RootStaticDisplayInterface::Instance().AddHist(histDen,"COLZ",false);
    RootStaticDisplayInterface::Instance().AddHist(histMin,"COLZ",false);
    RootStaticDisplayInterface::Instance().AddHist(histMax,"COLZ",false);
    RootStaticDisplayInterface::Instance().AddHist(histSteps,"COLZ",false);
    RootStaticDisplayInterface::Instance().FlushData();*/

    //Write all to files
    tree->Write();
    histDen->Write();
    histInt->Write();
    histMap->Write();
    histMin->Write();
    histMax->Write();
    histSteps->Write();
    histInitialBig->Write();
    histInitialSmall->Write();
    char xBuf [10];
    char yBuf [10];
    char ignoredPixelsBuf [10];
    sprintf(xBuf, "%d", xPixels);
    sprintf(yBuf, "%d", yPixels);
    sprintf(ignoredPixelsBuf,"%d",ignoredPixels);
    TObjString strXPixels(xBuf);
    TObjString strYPixels(xBuf);
    TObjString strIgnPixel(ignoredPixelsBuf);
    strXPixels.Write("xPixels");
    strYPixels.Write("yPixels");
    strIgnPixel.Write("ignoredPixels");
    f.Save();
    f.Close();
}
