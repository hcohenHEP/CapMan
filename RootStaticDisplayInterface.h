#ifndef ROOTSTATICDISPLAYINTERFACE_H
#define ROOTSTATICDISPLAYINTERFACE_H

#include <TStyle.h>
#include <TSocket.h>
#include <TObject.h>
#include <QProcess>
#include <TFile.h>
#include <TMap.h>
#include <TList.h>
#include <TH1.h>
#include <TServerSocket.h>
#include <QDir>
#include <TMessage.h>
#include <TCanvas.h>
#include <TObjString.h>
#include <TTree.h>


/*      RootStaticDisplayInterface Class
 *      Author: Hadar Cohen
 *
 *
 *      Important: the class isn't responsible for memory managment, anything you passes (hists & canvases) YOU must delete!
 *
 *
 *
 */


class RootStaticDisplayInterface
{
public:
    static RootStaticDisplayInterface& Instance()
    {
        static RootStaticDisplayInterface    instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }

    //Adding object, this object will not be displyed but will be written to the output.root file.
    void AddObject(TObject* obj)
    {
        m_ObjectList->Add(obj->Clone());
    }

    //Adding Canvas, will be displyed and written to output.root file
    void AddCanvas(TCanvas* can)
    {
        m_CanvasList->Add((TCanvas*)can->Clone());
    }
    //Adding Canvas, will be displyed and written to output.root file as hist object
    void AddHist(TH1* hist, const char* drawOption="", bool showStat = true)
    {
        TCanvas* can = new TCanvas(hist->GetTitle(),hist->GetTitle(),700,700);
        can->cd();
        if ( showStat == false ) hist->SetStats(0);
        hist->Draw(drawOption);
        AddCanvas(can);

        m_ObjectList->Add(hist->Clone());
    }
    //Adding Tree Draw,  will be displyed
    void AddTTree(TTree* tree, const char* drawOption)
    {
        TCanvas* can = new TCanvas(drawOption,drawOption,600,600);
        can->cd();
        tree->Draw(drawOption);
        AddCanvas(can);
    }

    //Adding the hist and immediatly flushes the data, good for online testing and quick debugging
    void AddHistAndFlush(TH1* hist, const char* drawOption="", bool showStat = true)
    {
        AddHist(hist,drawOption,showStat);
        FlushData();
    }

    //Flushes the data to root display. By default doesn't saves the data and kill the previous flush.
    void FlushData( bool killPreviousRoot = true,bool bSaveSnapshot = false, QString dirName = "Output", QString outputFileName = "")
    {
        static int emptyHistIndex = 0;
        if ( true == killPreviousRoot &&  NULL != m_PreviousRootProcess )
        {
            m_PreviousRootProcess->kill();
            delete m_PreviousRootProcess;
            m_PreviousRootProcess = NULL;
        }

        m_PreviousRootProcess = new QProcess();
        QString program = "root -l .x RootStaticDisplay.C";
        m_PreviousRootProcess->start(program);
        m_RootDisplaySocket = m_ServerSocket->Accept();

        TMessage ocuMessage(kMESS_OBJECT);
        ocuMessage.WriteObject(m_CanvasList);
        m_RootDisplaySocket->Send(ocuMessage);

        if ( true == bSaveSnapshot )
        {
            //Just in case some latex chars are in there
            dirName.replace("#","");
            dirName.replace("^","");
            dirName.replace("{","");
            dirName.replace("}","");

            //Save Snapshot
            QDir().mkdir(dirName); //Adding the split incase of branch arithmetics
            TFile* outputFile = NULL;
            if ( outputFileName != "" )
                 outputFile = new TFile(TString::Format("%s/%s",dirName.toStdString().c_str(),outputFileName.toStdString().c_str()),"RECREATE");

            for (int i = 0 ; i < m_CanvasList->GetSize(); i++ )
            {
                TCanvas* can = (TCanvas*)m_CanvasList->At(i);
                gStyle->SetPalette(55);
                QString filename(can->GetTitle());
                if ( filename == "" ) filename = QString::number(emptyHistIndex++);
                can->Print(TString::Format("%s/%s.png",dirName.toStdString().c_str(),filename.toStdString().c_str())   );
                can->Write();
            }

            if ( outputFileName != "" )
            {

                for ( int i = 0 ; i < m_ObjectList->GetSize(); i++ )
                {
                    TObject* obj = (TObject*)m_ObjectList->At(i);
                    obj->Write();
                }
                outputFile->Save();
                outputFile->Close();
            }
        }
        //Clear and delete
        m_CanvasList->Delete();
        m_ObjectList->Delete();
        m_RootDisplaySocket->Close();
        delete m_RootDisplaySocket;
    }
private:
    RootStaticDisplayInterface()
    {
        m_ServerSocket = new TServerSocket(8200,kTRUE);
        m_CanvasList = new TList();
        m_ObjectList = new TList();
        m_PreviousRootProcess = NULL;
    }
    ~RootStaticDisplayInterface()
    {
        m_CanvasList->Delete();
        delete m_CanvasList;
        m_ObjectList->Delete();
        delete m_ObjectList;
        m_ServerSocket->Close();
        delete m_ServerSocket;
        if ( NULL != m_PreviousRootProcess )
        {
            delete m_PreviousRootProcess;
        }
    }

private:
    TServerSocket *m_ServerSocket;
    TSocket* m_RootDisplaySocket;
    TList* m_CanvasList;
    TList* m_ObjectList;
    QProcess* m_PreviousRootProcess;
};
#endif // ROOTSTATICDISPLAYINTERFACE_H
