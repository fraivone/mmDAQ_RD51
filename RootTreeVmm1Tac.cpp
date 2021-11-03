
#include "RootTreeVmm1Tac.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include <TFile.h>
#include <TTree.h>

#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <iostream>
#include <sys/stat.h>
#include <sstream>


RootTreeVmm1Tac::RootTreeVmm1Tac(const char* filename):
rootfile(0), ferror(0), fChain(0)
{
   struct stat stFileInfo;
   if (!filename || strlen(filename) == 0 || stat(filename, &stFileInfo)) {
      std::stringstream ss;
      ss << "RootTreeVmm1Tac - no file '" << filename << "'";
      throw std::runtime_error(ss.str().c_str());
      
   }
   
	TString thename(filename);
   rootfile = new TFile(thename);
   if (rootfile->IsOpen()) {
      TTree *tree = 0;
      rootfile->GetObject("VMM1_Parameters", tree);
      Init(tree);
   }
   else {
      throw std::runtime_error("RootTreeVmm1Tac - can not open file");
   }
}

RootTreeVmm1Tac::~RootTreeVmm1Tac()
{
   if (rootfile) {
		delete rootfile;// automatically calls rootfile->Close();
		rootfile = 0;
	}
   
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t RootTreeVmm1Tac::GetEntry(Long64_t entry)
{
   // Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}

Long64_t RootTreeVmm1Tac::GetEntries()
{
   // Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntries();
}


Long64_t RootTreeVmm1Tac::LoadTree(Long64_t entry)
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

void RootTreeVmm1Tac::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).
   
   // Set object pointer
   board = 0;
   channel = 0;
   set_t_tac = 0;
   q_pedMean = 0;
   q_pedSigma = 0;
   q_gainSlope = 0;
   q_gainIntercept = 0;
   set_q_gainFactor = 0;
   t_tacSlope = 0;
   t_tacIntercept = 0;
   
   
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);
   
//   fChain->SetBranchAddress("TAC", &TAC, &b_TAC);
//   fChain->SetBranchAddress("board", &board, &b_board);
//   fChain->SetBranchAddress("channel", &channel, &b_channel);
//   fChain->SetBranchAddress("slope", &slope, &b_slope);
//   fChain->SetBranchAddress("intercept", &intercept, &b_intercept);
   
   
   fChain->SetBranchAddress("board", &board, &b_board);
   fChain->SetBranchAddress("channel", &channel, &b_channel);
   fChain->SetBranchAddress("set_t_tac", &set_t_tac, &b_set_t_tac);
   fChain->SetBranchAddress("q_pedMean", &q_pedMean, &b_q_pedMean);
   fChain->SetBranchAddress("q_pedSigma", &q_pedSigma, &b_q_pedSigma);
   fChain->SetBranchAddress("q_gainSlope", &q_gainSlope, &b_q_gainSlope);
   fChain->SetBranchAddress("q_gainIntercept", &q_gainIntercept, &b_q_gainIntercept);
   fChain->SetBranchAddress("set_q_gainFactor", &set_q_gainFactor, &b_set_q_gainFactor);
   fChain->SetBranchAddress("t_tacSlope", &t_tacSlope, &b_t_tacSlope);
   fChain->SetBranchAddress("t_tacIntercept", &t_tacIntercept, &b_t_tacIntercept);

   
   
   Notify();
}

Bool_t RootTreeVmm1Tac::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.
   
   return kTRUE;
}

void RootTreeVmm1Tac::Show(Long64_t entry)
{
   // Print contents of entry.
   // If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t RootTreeVmm1Tac::Cut(Long64_t entry)
{
   // This function may be called from Loop.
   // returns  1 if entry is accepted.
   // returns -1 otherwise.
   return 1;
}





void RootTreeVmm1Tac::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L RootTreeVmm1Tac.C
//      Root > RootTreeVmm1Tac t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
   }
}
