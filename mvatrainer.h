#ifndef MVATRAINER_H
#define MVATRAINER_H
#include <QString>
#include <TTree.h>

struct Classifier
{
  std::string name;
  std::vector<std::string> variables;
  TTree* BGTree;
  TTree* SIGTree;

  std::string signalCut;
  std::string backgroundCut;
};


class MVATrainer
{
public:
    MVATrainer();
    void TrainMVA(QList<const char*>  bgFiles, QList<const char*>  sigFiles);

    static void ApplyMVA(QStringList files, QString dir, double val);
    static void PlotConfusionMatrix(TString tmvaOutput, double val);

protected:
    Classifier m_Classifier;

};
#endif // MVATRAINER_H
