#include "mvatrainer.h"
#include <TFile.h>
#include <QMessageBox>
#include <TMVA/Tools.h>
#include <TMVA/Factory.h>
#include <QFile>
#include <QTextStream>
#include <TLegend.h>
#include <TF1.h>
#include <TMVA/Reader.h>
#include "RootStaticDisplayInterface.h"
#include <TH2F.h>
using namespace std;

MVATrainer::MVATrainer()
{
    m_Classifier.backgroundCut = "is_ignored==0";
    m_Classifier.signalCut = "is_ignored==0";
    m_Classifier.name = "CapFinderBDT";

    //Variables
    m_Classifier.variables.push_back("minDist");
    m_Classifier.variables.push_back("maxDist");
    m_Classifier.variables.push_back("n_I");
    m_Classifier.variables.push_back("steps");
}

void MVATrainer::TrainMVA(QList<const char*>  bgFiles, QList<const char*>  sigFiles)
{
    TMVA::Tools::Instance();
    TString outfileName( "CapFinderTMVA.root" );
    TFile* outputFile = TFile::Open( outfileName, "RECREATE" );
    TMVA::Factory *factory = new TMVA::Factory( "TMVAClassification", outputFile,
                                                "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification" );

    //ADD Variables
    for (size_t i = 0; i != m_Classifier.variables.size(); ++i) {
        factory->AddVariable(m_Classifier.variables[i], m_Classifier.variables[i], "", 'F');
    }

    outputFile->cd();

    foreach(const char* file, bgFiles)
    {
        TTree* tr = (TTree*)TFile::Open(file)->Get("imgTree");
        factory->AddBackgroundTree(tr);
    }

    foreach(const char* file, sigFiles)
    {
        TTree* tr = (TTree*)TFile::Open(file)->Get("imgTree");
        factory->AddSignalTree(tr);
    }

    TCut mycuts = m_Classifier.signalCut.data();
    TCut mycutb = m_Classifier.backgroundCut.data();

    //factory->SetSignalWeightExpression("1/n_I");
    //factory->SetBackgroundWeightExpression("1/n_I");
    factory->PrepareTrainingAndTestTree( mycuts, mycutb,
                                         "nTrain_Signal=0:nTrain_Background=0:SplitMode=Random:NormMode=NumEvents:!V");

    //BDT
    //factory->BookMethod( TMVA::Types::kBDT, m_Classifier.name,"!H:!V:BoostType=AdaBoost:MaxDepth=5:NTrees=1500:MinNodeSize=15" );
    factory->BookMethod( TMVA::Types::kBDT, m_Classifier.name+"_Grad","!H:!V:MaxDepth=5:BoostType=Grad:NTrees=500:Shrinkage=0.38:MinNodeSize=2%" );

    //Backups
    //Very good, 7,10,11,12, Separation:0.238 factory->BookMethod( TMVA::Types::kBDT, m_Classifier.name+"_Grad","!H:!V:MaxDepth=5:BoostType=Grad:NTrees=1500:Shrinkage=0.38:MinNodeSize=2%" );


    factory->TrainAllMethods();
    factory->TestAllMethods();
    factory->EvaluateAllMethods();

    outputFile->Close();
}

void MVATrainer::ApplyMVA(QStringList files, QString dir, double val)
{
    Float_t minDist,maxDist,n_I,steps,isIgnored;
    Int_t x,y;
    double small=0, big=0;

    Int_t palette[3];
    palette[0] = kWhite;
    palette[1] = kRed;
    palette[2] = kGreen;

    TMVA::Reader* reader = new TMVA::Reader();
    reader->AddVariable("minDist",&minDist);
    reader->AddVariable("maxDist",&maxDist);
    reader->AddVariable("n_I",&n_I);
    reader->AddVariable("steps",&steps);
    reader->BookMVA("CapFinderBDT_Grad","weights/TMVAClassification_CapFinderBDT_Grad.weights.xml");

    QFile data(dir + "/BDT_Results.csv");
    if(data.open(QFile::WriteOnly |QFile::Truncate))
    {
        QTextStream output(&data);
        output << "File Name,ALL,Ignored,Small,Big,Small/(ALL-Big-Ignored),Big/(ALL-Ignored)"<<endl;

        foreach(QString fileName, files)
        {
            fileName = dir +"/" +  fileName;
            if ( fileName.endsWith(".root") == false ) continue;

            TFile* file = TFile::Open( fileName.toStdString().c_str(), "READ" );
            TTree* tree = (TTree*)file->Get("imgTree");

            Int_t xPixels = ((TObjString*)file->Get("xPixels"))->String().Atoi();
            Int_t yPixels = ((TObjString*)file->Get("yPixels"))->String().Atoi();
            Int_t ignoredPixels = ((TObjString*)file->Get("ignoredPixels"))->String().Atoi();
            tree->SetBranchAddress("minDist",&minDist);
            tree->SetBranchAddress("maxDist",&maxDist);
            tree->SetBranchAddress("n_I",&n_I);
            tree->SetBranchAddress("steps",&steps);
            tree->SetBranchAddress("x",&x);
            tree->SetBranchAddress("y",&y);
            tree->SetBranchAddress("is_ignored",&isIgnored);

            TCanvas* can = new TCanvas("BDT","FinalCut",700,700);
            TH2F* hist= new TH2F("hist","",xPixels,0,xPixels,yPixels,0,yPixels);
            hist->SetStats(0);

            Long64_t size = tree->GetEntries();
            small=0;
            big=0;

            for (Long64_t i=0 ; i < size ; i++ )
            {
                tree->GetEntry(i);
                if ( isIgnored == 1.0 )
                {
                    hist->SetBinContent(x+1,y+1,1);
                    continue;
                }
                Float_t mvaValue = reader->EvaluateMVA("CapFinderBDT_Grad");
                hist->SetBinContent(x+1,y+1,mvaValue);
                if ( mvaValue > val )
                {
                    hist->SetBinContent(x+1,y+1,10);
                    small++;
                }
                else
                {
                    hist->SetBinContent(x+1,y+1,5);
                    big++;
                }
            }


            double ratioSmall = small/(yPixels*xPixels - big - ignoredPixels) ;
            double ratioBig = big/(yPixels*xPixels - ignoredPixels) ;


            output << fileName<<","<<yPixels*xPixels<<","<< ignoredPixels <<","<<small<<","<<big<<","<<ratioSmall<<","<<ratioBig<<endl;

            can->SetFrameFillColor(kBlack);
            hist->GetXaxis()->SetLabelOffset(999);
            hist->GetXaxis()->SetLabelSize(0);

            hist->GetYaxis()->SetLabelOffset(999);
            hist->GetYaxis()->SetLabelSize(0);

            hist->Draw("COL");

            /*TLegend * legend = new TLegend(0.68,0.805,.895,0.895);
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
            legend->Draw();*/


            if ( files.count()== 1 )
            {
                RootStaticDisplayInterface::Instance().AddCanvas(can);
                RootStaticDisplayInterface::Instance().FlushData(false);
            }

            gStyle->SetPalette(3,palette);
            can->SetTitle("");
            can->Print(fileName.replace(".tif.root",".png").toStdString().c_str());

            delete tree;
            file->Close();
            delete file;
        }
        data.close();
    }


    delete reader;
}

void MVATrainer::PlotConfusionMatrix(TString tmvaOutput, double val)
{
    Float_t minDist,maxDist,n_I,steps;
    Int_t  classID;
    double TS=0,TB=0,FS=0,FB=0; // TS - True Signal, TB - True Background ....

    TMVA::Reader* reader = new TMVA::Reader();
    reader->AddVariable("minDist",&minDist);
    reader->AddVariable("maxDist",&maxDist);
    reader->AddVariable("n_I",&n_I);
    reader->AddVariable("steps",&steps);
    reader->BookMVA("CapFinderBDT_Grad","weights/TMVAClassification_CapFinderBDT_Grad.weights.xml");

    TFile* file = TFile::Open( tmvaOutput, "READ" );
    TTree* tree = (TTree*)file->Get("TestTree");

    tree->SetBranchAddress("minDist",&minDist);
    tree->SetBranchAddress("maxDist",&maxDist);
    tree->SetBranchAddress("n_I",&n_I);
    tree->SetBranchAddress("steps",&steps);
    tree->SetBranchAddress("classID",&classID);

    Long64_t size = tree->GetEntries();

    for (Long64_t i=0 ; i < size ; i++ )
    {
        tree->GetEntry(i);
        Float_t mvaValue = reader->EvaluateMVA("CapFinderBDT_Grad");
        if ( mvaValue > val )
        {//MVA -> Signal
            if (classID == 1)
                TS++;
            else
                FS++;
        }
        else
        {//MVA -> Bkg
            if (classID == 1)
                FB++;
            else
                TB++;
        }
    }


    double PPV = TS/(TS+FS);
    double NPV = TB/(TB+FB);
    double ACC = (TS+TB)/size;
    double F1 = (2*TS)/(2*TS+FS+FB);
    cout<<"PPV: "<<PPV<<endl;
    cout<<"NPV: "<<NPV<<endl;
    cout<<"ACC: "<<ACC<<endl;
    cout<<"F1: "<<F1<<endl;
    cout<<TS<<", "<<FS<<", "<<TB<<",  "<<FB<<endl;
    cout<<"Size: "<<size<<endl;



    delete tree;
    file->Close();
    delete file;
    delete reader;
}

