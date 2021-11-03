//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Nov 15 12:26:50 2012 by ROOT version 5.32/01
// from TTree vmm1TAC/vmm1TAC
// found on file: channelTACs_all.root
//////////////////////////////////////////////////////////

#ifndef RootTreeVmm1Tac_h
#define RootTreeVmm1Tac_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <vector>

// Fixed size dimensions of array or collections stored in the TTree if any.

class RootTreeVmm1Tac
{
   
private:
   
   TFile				*rootfile;
   int				ferror;
   
   public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain
   
   // Declaration of leaf types
   Int_t                board;         /// vmm1 board id = chip id 16-32
   std::vector<int>     *channel;      /// channel number 0-63
   Int_t                set_t_tac;     /// TAC set on chip
   std::vector<double>  *q_pedMean;    /// mean by measured neighbours to pulsed channel (info from K.N. 26/11/2012)
   std::vector<double>  *q_pedSigma;   /// sigma of ped by measured neighbours to pulsed channel (info from K.N. 26/11/2012)
   std::vector<double>  *q_gainSlope;  /// readout value vs pulser amplitude
   std::vector<double>  *q_gainIntercept; /// readout value vs pulser amplitude
   std::vector<double>  *set_q_gainFactor; /// (?) readout value vs pulser amplitude
   std::vector<double>  *t_tacSlope;   /// time form pdo to ns , which is x,y?
   std::vector<double>  *t_tacIntercept; /// time from tdo to ns , intercept at 
   
   // List of branches
   TBranch        *b_board;   //!
   TBranch        *b_channel;   //!
   TBranch        *b_set_t_tac;   //!
   TBranch        *b_q_pedMean;   //!
   TBranch        *b_q_pedSigma;   //!
   TBranch        *b_q_gainSlope;   //!
   TBranch        *b_q_gainIntercept;   //!
   TBranch        *b_set_q_gainFactor;   //!
   TBranch        *b_t_tacSlope;   //!
   TBranch        *b_t_tacIntercept;   //!
   
   
   
   RootTreeVmm1Tac(const char* filename);
//   RootTreeVmm1Tac(TTree *tree=0);
   virtual ~RootTreeVmm1Tac();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   Long64_t GetEntries();
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif
