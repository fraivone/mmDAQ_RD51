//
//

#include "StripsTree.h"

#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TChain.h>
#include <TFile.h>

#include <stdexcept>
#include <algorithm>

using namespace  browser;

CStripsTree::CStripsTree(TFile *file, const std::string& treename)  :
rootfile(0), 
fChain(0),
fCurrent(0),
ferror(0),
evt(0),
mm_id(0),
mm_plane(0),
mm_readout(0),
mm_strip(0),
qmax(0),
tbqmax(0),
tbcfd(0),
qfit(0),
tfit(0),
tfitcfd(0),
fitpars(0)
{
	if(file == 0) 
		throw std::runtime_error("CStripsTree - must have a pointer to TFile");  
	else{
		TTree* tree = (TTree*)file->Get(treename.c_str());
      if (!tree) {
         throw std::runtime_error("CStripsTree - no raw tree named '" + treename +"'");  
      }
      
		Init(tree);
	}
}

   

                       
                       
//CStripsTree::CStripsTree(TTree *tree) : fChain(0) 
//{
//   // if parameter tree is not specified (or zero), connect the file
//   // used to generate this class and read the Tree.
//   if (tree == 0) {
//      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/Volumes/HDD/data/2010/testbeam_Nov/analysis/250um_vs_500um/fitted/run6296_10_250um_fit_dr.root");
//      if (!f || !f->IsOpen()) {
//         f = new TFile("/Volumes/HDD/data/2010/testbeam_Nov/analysis/250um_vs_500um/fitted/run6296_10_250um_fit_dr.root");
//      }
//      f->GetObject("strips",tree);
//      
//   }
//   Init(tree);
//}

CStripsTree::~CStripsTree()
{

}

Int_t CStripsTree::GetEntry(Long64_t entry)
{
   // Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t CStripsTree::LoadTree(Long64_t entry)
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

void CStripsTree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).
   
   // Set object pointer
   mm_id = 0;
   mm_plane = 0;
   mm_readout = 0;
   mm_strip = 0;
   qmax = 0;
   tbqmax = 0;
   tbcfd = 0;
   qfit = 0;
   tfit = 0;
   tfitcfd = 0;
   fitpars = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);
   
   fChain->SetBranchAddress("evt", &evt, &b_evt);
   fChain->SetBranchAddress("mm_id", &mm_id, &b_mm_id);
   fChain->SetBranchAddress("mm_plane", &mm_plane, &b_mm_plane);
   fChain->SetBranchAddress("mm_readout", &mm_readout, &b_mm_readout);
   fChain->SetBranchAddress("mm_strip", &mm_strip, &b_mm_strip);
   fChain->SetBranchAddress("qmax", &qmax, &b_qmax);
   fChain->SetBranchAddress("tbqmax", &tbqmax, &b_tbqmax);
   fChain->SetBranchAddress("tbcfd", &tbcfd, &b_tbcfd);
   fChain->SetBranchAddress("qfit", &qfit, &b_qfit);
   fChain->SetBranchAddress("tfit", &tfit, &b_tfit);
   fChain->SetBranchAddress("tfitcfd", &tfitcfd, &b_tfitcfd);
   fChain->SetBranchAddress("fitpars", &fitpars, &b_fitpars);
   Notify();
}

Bool_t CStripsTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.
   
   return kTRUE;
}

void CStripsTree::Show(Long64_t entry)
{
   // Print contents of entry.
   // If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}

Int_t CStripsTree::Cut(Long64_t )
{
   // This function may be called from Loop.
   // returns  1 if entry is accepted.
   // returns -1 otherwise.
   return 1;
}

long CStripsTree::GetEntries() const
{
   return fChain->GetEntries();
}

TTree* CStripsTree::GetTree()
{
   return fChain;
}

int CStripsTree::error() 
{
   return ferror;
}

void CStripsTree::clear()
{
   
}

long CStripsTree::get_index_by_number(unsigned number)
{
   std::vector<unsigned int>::iterator found = std::find(mm_strip->begin(), 
                                                         mm_strip->end(), 
                                                         (unsigned int)(number));
   if (found != mm_strip->end()) {
      return std::distance(mm_strip->begin(), found);
   }
   return -1;
}



void CStripsTree::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L CStripsTree.C
//      Root > CStripsTree t
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
