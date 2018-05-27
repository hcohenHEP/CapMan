#include <TGClient.h>
#include <TGButton.h>
#include <TGFrame.h>
#include <TGTab.h>
#include <TRootEmbeddedCanvas.h>
#include <RQ_OBJECT.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TSocket.h>
#include <TMessage.h>
#include <TMap.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <THStack.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TAxis.h>
#include <TGaxis.h>
#include <stdio.h>
#include <stdlib.h>
#include <TChain.h>
#include <TLatex.h>
#include <TBox.h>
#include <TColor.h>
#include <TMath.h>
#include <TSystem.h>
#include <TApplication.h>
#include <TGraph.h>
#include <TText.h>
#include <TSocket.h>
#include <TMessage.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

class RootStaticGUI
{
    RQ_OBJECT("OnlineGUI")
    private:

        TSocket* fSocket;
    std::vector<TH1*> fHists;
    std::vector<TCanvas*> fCanvases;

public:
    RootStaticGUI()
    {
        gSystem->Sleep(1500);//Lets give some time for the tcp server initialize and be ready
        fSocket = new TSocket("localhost",8200);

        TMessage *message = NULL;
        fSocket->Recv(message);
        gSystem->ProcessEvents();
        if ( false == fSocket->IsValid() ) return;

        TList* canvasList = (TList*)message->ReadObject(message->GetClass());
        DisplayNewHists(canvasList);
        delete message;
        fSocket->Close();
        delete fSocket;
    }

    void DisplayNewHists(TList* canvasList)
    {
        for (int i = 0 ; i < canvasList->GetSize() ; i ++ )
        {
            TCanvas* can = (TCanvas*)canvasList->At(i);
            gStyle->SetPalette(55);
            if ( strcmp(can->GetTitle(),"FinalCut") == 0 )
            {
                Int_t palette[3];
                palette[0] = kWhite;
                palette[1] = kRed;
                palette[2] = kGreen;
                gStyle->SetPalette(3,palette);
                can->SetTitle("");
            }
            can->Draw();
            fCanvases.push_back(can);
        }
    }

    virtual ~RootStaticGUI()
    {
        for ( int i = 0; i < fHists.size(); i++)
        {
            TCanvas* can = fCanvases[i];
            delete can;
        }
        fCanvases.clear();

        exit(0);
    }

};

void RootStaticDisplay()
{
    // Popup the GUI...
    //SetAtlasStyle();
    new RootStaticGUI();
}
