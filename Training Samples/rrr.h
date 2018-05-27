//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Dec 30 11:54:40 2016 by ROOT version 6.04/16
// from TTree TestTree/TestTree
// found on file: CapFinderTMVA.root
//////////////////////////////////////////////////////////

#ifndef rrr_h
#define rrr_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class rrr {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           classID;
   Char_t          className[11];
   Float_t         minDist;
   Float_t         maxDist;
   Float_t         n_I;
   Float_t         steps;
   Float_t         weight;
   Float_t         CapFinderBDT_Grad;

   // List of branches
   TBranch        *b_classID;   //!
   TBranch        *b_className;   //!
   TBranch        *b_minDist;   //!
   TBranch        *b_maxDist;   //!
   TBranch        *b_n_I;   //!
   TBranch        *b_steps;   //!
   TBranch        *b_weight;   //!
   TBranch        *b_CapFinderBDT_Grad;   //!

   rrr(TTree *tree=0);
   virtual ~rrr();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef rrr_cxx
rrr::rrr(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("CapFinderTMVA.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("CapFinderTMVA.root");
      }
      f->GetObject("TestTree",tree);

   }
   Init(tree);
}

rrr::~rrr()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t rrr::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t rrr::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void rrr::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("classID", &classID, &b_classID);
   fChain->SetBranchAddress("className", className, &b_className);
   fChain->SetBranchAddress("minDist", &minDist, &b_minDist);
   fChain->SetBranchAddress("maxDist", &maxDist, &b_maxDist);
   fChain->SetBranchAddress("n_I", &n_I, &b_n_I);
   fChain->SetBranchAddress("steps", &steps, &b_steps);
   fChain->SetBranchAddress("weight", &weight, &b_weight);
   fChain->SetBranchAddress("CapFinderBDT_Grad", &CapFinderBDT_Grad, &b_CapFinderBDT_Grad);
   Notify();
}

Bool_t rrr::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void rrr::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t rrr::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef rrr_cxx
