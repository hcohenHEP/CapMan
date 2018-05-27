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
#include "mvatrainer.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TMVA/Factory.h"
#include "TMVA/Reader.h"
#include "TMVA/Tools.h"
#include <stdio.h>
#include <stdlib.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "RootStaticDisplayInterface.h"
#include "cutsdialog.h"
#include "imagedatabuilder.h"
#include "singlefileprogressdialog.h"
#include <QThread>
using namespace std;
bool fileExists(QString path) {
    QFileInfo checkFile(path);
    // check if file exists and if yes: Is it really a file and no directory?
    if (checkFile.exists() && checkFile.isFile()) {
        return true;
    } else {
        return false;
    }
}
void MainWindow::Run()
{
    if ( ui->lineEdit_FileName->text().endsWith(".tif") )
    {
        mutex.lock();
        RunSingleFile(ui->lineEdit_FileName->text());
    }
    else
    {
        QDir inputDir(ui->lineEdit_FileName->text());
        QStringList files = inputDir.entryList(QDir::Readable | QDir::NoDotAndDotDot | QDir::Files);
        m_Files.clear();
        m_CurrentFileIndex = 0;
        //Check that the data files exist
        if (files.count() > 0 )
        {
            for ( int i = 0 ; i < files.count() ; i ++ )
            {
                if ( files.at(i).endsWith(".tif") == true )
                {
                    //m_Files.append(ui->lineEdit_FileName->text() + "/" + files.at(i));
                    if ( m_ImageDataBuilder != NULL )
                    {
                        m_ImageDataBuilder->terminate();
                        delete m_ImageDataBuilder;
                        m_ImageDataBuilder = NULL;
                    }
                    m_ImageDataBuilder = new ImageDataBuilder(ui->lineEdit_FileName->text() + "/" + files.at(i),1,3);
                    m_ImageDataBuilder->run();
                }
            }
        }
        //RunSingleFile(m_Files[m_CurrentFileIndex]);
        //m_MultipleFilesTimer->start();
    }
}

void MainWindow::RunSingleFile(QString file)
{
    if (false==fileExists(file) )
    {
        QMessageBox msgBox;
        msgBox.setText("Cannot find tif file:  " + file);
        msgBox.exec();
        return;
    }
    if ( m_ImageDataBuilder != NULL )
    {
        m_ImageDataBuilder->terminate();
        delete m_ImageDataBuilder;
        m_ImageDataBuilder = NULL;
    }
    m_ImageDataBuilder = new ImageDataBuilder(file,1,3);
    m_ImageDataBuilder->start(QThread::TimeCriticalPriority);
    m_GuiTimer->start();
    ui->pushButton_Run->setEnabled(false);
    ui->groupBox_2->setEnabled(false);
    m_SingleFileProgressDialog->Init(file);
    m_SingleFileProgressDialog->show();
}

void MainWindow::Cut()
{
    if (false==fileExists(ui->lineEdit_FileName->text() + ".root") )
    {
        QMessageBox msgBox;
        msgBox.setText("Cannot find Meta-Data Info for the file " + ui->lineEdit_FileName->text() + "\nPlease Use the Build Option First and only then perform the cuts");
        msgBox.exec();
    }
    else
    {
        CutsDialog* cutD = new CutsDialog(this);
        QString fileName = ui->lineEdit_FileName->text()+".root";
        TFile* file = new TFile(fileName.toStdString().data(),"UPDATE");
        cutD->Init(file,"Output_" + ui->lineEdit_FileName->text(),ui->lineEdit_FileName->text());
        cutD->show();
    }
}

void MainWindow::BDT()
{
    if ( ui->lineEdit_FileName->text().endsWith(".tif") )
    {
        if (false==fileExists(ui->lineEdit_FileName->text() + ".root") )
        {
            QMessageBox msgBox;
            msgBox.setText("Cannot find Meta-Data Info for the file " + ui->lineEdit_FileName->text() + "\nPlease Use the Build Option First");
            msgBox.exec();
        }
        else
        {
            QString dir = "";
            QStringList dirs = ui->lineEdit_FileName->text().split("/");
            QString fileName = ui->lineEdit_FileName->text();
            for ( int i = 0 ; i < dirs.count()-1;i++)
                dir+= dirs.at(i);

            MVATrainer::ApplyMVA( {dirs.at(dirs.count()-1) + ".root"},dir,ui->doubleSpinBox_BDTVal->value());
        }
    }
    else
    {
        QDir inputDir(ui->lineEdit_FileName->text());
        QStringList files = inputDir.entryList(QDir::Readable | QDir::NoDotAndDotDot | QDir::Files);
        MVATrainer::ApplyMVA(files,ui->lineEdit_FileName->text()+"/" ,ui->doubleSpinBox_BDTVal->value());
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton_Run,SIGNAL(clicked()),this,SLOT(Run()));
    connect(ui->pushButton_Cut,SIGNAL(clicked()),this,SLOT(Cut()));
    connect(ui->pushButton_BDT,SIGNAL(clicked()),this,SLOT(BDT()));

    m_GuiTimer = new QTimer(this);
    m_MultipleFilesTimer = new QTimer(this);

    connect(m_MultipleFilesTimer,SIGNAL(timeout()),this,SLOT(updateProgressMultipleFiles()));
    connect(m_GuiTimer,SIGNAL(timeout()),this,SLOT(updateProgress()));

    m_ImageDataBuilder = NULL;
    m_SingleFileProgressDialog = new SingleFileProgressDialog(this);
}

void MainWindow::updateProgress()
{
    int p = m_ImageDataBuilder->Progress;
    if(p<100)
    {
        m_SingleFileProgressDialog->UpdateProgress(p);
        if (m_SingleFileProgressDialog->isHidden() )
        {
            m_SingleFileProgressDialog->show();
        }
    }
    else
    {
        m_SingleFileProgressDialog->UpdateProgress(100);
        m_SingleFileProgressDialog->close();
    }

    if (p >= 100)
    {
        m_GuiTimer->stop();
        ui->groupBox_2->setEnabled(true);
        ui->pushButton_Run->setEnabled(true);
    }
}

void MainWindow::updateProgressMultipleFiles()
{
    int p = m_ImageDataBuilder->Progress;
    if (p >= 100 && m_CurrentFileIndex+1 < m_Files.count())
    {
        m_CurrentFileIndex++;
        RunSingleFile(m_Files[m_CurrentFileIndex]);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
