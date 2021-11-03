#include "CApvRawTree.h"
#include "MBtools.h"
#include "SrsChannelId.h"
#include "DetStrip.h"
#include "DetStripId.h"
#include "ParserApvRawTree.h"

//#include <TH2.h>
//#include <TStyle.h>
//#include <TCanvas.h>
//#include <TString.h>

#include <sys/stat.h>
#include <iostream>
#include <stdexcept>

using namespace browser;

CApvRawTree::CApvRawTree(TFile *file, const std::string& treename) :
CTreeBase(treename), rootfile(0), m_has_multi_readouts(false), m_has_planes(false), m_has_multilayers(false), fChain(0), fCurrent(0),  ferror(0), m_current_entry(0),
apv_evt(0), apv_fecNo(0), apv_id(0), apv_ch(0),
mm_id(0), mm_multilayer(0), mm_plane(0), mm_readout(0), mm_strip(0), apv_q()
{
	if(file == 0) 
		throw std::runtime_error("CApvRawTree - must have a pointer to TFile");  
	else{
		TTree* tree = (TTree*)file->Get(treename.c_str());
      if (!tree) {
       throw std::runtime_error("CApvRawTree - no raw tree named '" + treename +"'");  
      }

		Init(tree);
	}
}

//
//CApvRawTree::CApvRawTree(TTree *tree) :
//rootfile(0), m_has_multi_readouts(false), m_has_multi_planes(false), fChain(0), fCurrent(0),  ferror(0), apv_evt(0), apv_fecNo(0), apv_id(0), apv_ch(0),
//mm_id(0), mm_plane(0), mm_readout(0), mm_strip(0), apv_q()
//{
//	// if parameter tree is not specified (or zero), connect the file
//	// used to generate this class and read the Tree.
//   if (tree == 0) {
//      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("run6213.root");
//      if (!f) {
//         f = new TFile("run6213.root");
//      }
//      tree = (TTree*)gDirectory->Get("raw");
//   }
//   Init(tree);
//}


CApvRawTree::~CApvRawTree()
{

}

TTree* CApvRawTree::GetTree() 
{
   return fChain;
}

Int_t CApvRawTree::GetEntry(Long64_t entry)
{
	// Read contents of entry.
   if (!fChain) 
      return 0;
   int rc = fChain->GetEntry(entry);
   if (rc > 0) {
      m_current_entry = entry;
   }
   else {
      // no such entry
   }
   return rc;

}

Long64_t CApvRawTree::GetCurrentEntryNumber() const
{
   return m_current_entry;   
}

long CApvRawTree::GetEntries() const
{
   if (!fChain) return -1;
   std::cout << "CApvRawTree::GetEntries() entries =" << fChain->GetEntries() << std::endl;
   return fChain->GetEntries();
}



void CApvRawTree::LoadEventData(DetBasePtrCont& strips, CDetDetector* det)
{
   
   size_t number_of_strips = size();
//   std::cout << "CApvRawTree::LoadEventData() number_of_strips =" << number_of_strips << std::endl;

   for (size_t ii = 0; ii < number_of_strips; ++ii) {
      
      unsigned fecNo   = (*(apv_fecNo))[ii];
      unsigned chipNo  = (*(apv_id))[ii];
      unsigned chNo    = (*(apv_ch))[ii];
      CSrsChannelId chId(fecNo, chipNo, chNo);
      
      std::string chamber_name = (*(mm_id))[ii];
      
      std::stringstream ss;
      unsigned multilayer_id   = has_multilayers() ? (*(mm_multilayer))[ii] : (unsigned)0;
      ss << multilayer_id;
      std::string multilayer_id_name(ss.str());
      
      ss.str("");
      unsigned plane_id   = has_planes() ? (*(mm_plane))[ii] : (unsigned)0;
      ss << plane_id;
      std::string plane_id_name(ss.str());
      
      ss.str("");
      unsigned readout = has_readouts() ? (*(mm_readout))[ii] : (unsigned)0;
     ss << readout;

      
      unsigned strip_number = (*(mm_strip))[ii];
      std::vector<double> qvec((*(apv_q))[ii].begin(), (*(apv_q))[ii].end());
      ss.str("");
      ss << strip_number;

      CDetStripId stripid(chamber_name, multilayer_id, plane_id, readout, strip_number);
      DetStripPtr strptr(new CDetStrip(det,
                                       chId,
                                       stripid,
                                       qvec,
                                       25.0));
      
      if(strptr) {
//         strip->set_strip_tree_entry(get_striptree_entry(strip_number));
         strips.push_back(strptr);
      }
   } // for ii < number of strips

    m_current_event=apv_evt;

    
}





void CApvRawTree::parse_tree(CDetDetector* det, CGuiMainFrame* gui) 
{
   
   try {
      CParserApvRawTree parser(this, det, gui);
//      return parser.get_detector();
   } catch (std::runtime_error &e) {
      std::cout << "ERROR: CApvRawTree::parse_tree() " << e.what() << std::endl;
   }
  
}





Long64_t CApvRawTree::LoadTree(Long64_t entry)
{
	// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (!fChain->InheritsFrom(TChain::Class()))  return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}



void CApvRawTree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).
	
   // Set object pointer
	apv_fecNo = 0;
   apv_id = 0;
   apv_ch = 0;
	mm_id = 0;
   mm_plane = 0;
   mm_readout =0;
   mm_strip = 0;
   apv_q = 0;
   
	//apv_tb = 0;

   // Set branch addresses and branch pointers
	if (!tree) {
		std::cerr << "Error: CApvRawTree::Init() no such tree" << std::endl;
      ferror = -1;
      throw std::runtime_error("CApvRawTree::Init() : no tree");
	}
   
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);
	
	ferror = 0;
   ferror |= fChain->SetBranchAddress("time_s", &time_s, &b_time_s);
   //kludge : there is a typo in some of the old  mmDAQ versions um instead of us ..
   if (fChain->FindBranch("time_us")) {
		ferror = fChain->SetBranchAddress("time_us", &time_us, &b_time_us);
	}
	else {
		ferror |=  fChain->SetBranchAddress("time_um", &time_us, &b_time_us); //time_um : see comment above
	}   
   ferror |= fChain->SetBranchAddress("apv_evt", &apv_evt, &b_apv_evt);
   ferror |= fChain->SetBranchAddress("apv_fecNo", &apv_fecNo, &b_apv_fecNo);
   ferror |= fChain->SetBranchAddress("apv_id", &apv_id, &b_apv_id);
   ferror |= fChain->SetBranchAddress("apv_ch", &apv_ch, &b_apv_ch);
   ferror |= fChain->SetBranchAddress("mm_id", &mm_id, &b_mm_id);
   if (fChain->GetBranch("mm_multilayer")) {
      ferror |= fChain->SetBranchAddress("mm_multilayer", &mm_multilayer, &b_mm_mlayer);
      m_has_multilayers = true;
   }
   if (fChain->GetBranch("mm_plane")) {
      ferror |= fChain->SetBranchAddress("mm_plane", &mm_plane, &b_mm_plane);
      m_has_planes = true;
   }
   if (fChain->GetBranch("mm_readout")) {
      ferror |= fChain->SetBranchAddress("mm_readout", &mm_readout, &b_mm_readout);
      m_has_multi_readouts = true;
   }
   ferror |= fChain->SetBranchAddress("mm_strip", &mm_strip, &b_mm_strip);
   ferror |= fChain->SetBranchAddress("apv_q", &apv_q, &b_apv_q);
      
	if (ferror < 0 ) {
		std::cerr << "Error: CApvRawTree::Init() bad tree structure. Check if the file contains all apv and mm identification colums." << std::endl;
		throw std::runtime_error("Bad raw tree structure.");
	}
	ferror = 0;
	
   Notify();
}

Bool_t CApvRawTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.
	
   return kTRUE;
}


void CApvRawTree::Show(Long64_t entry)
{
	// Print contents of entry.
	// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}


//Int_t CApvRawTree::Cut(Long64_t entry)
//{
//	// This function may be called from Loop.
//	// returns  1 if entry is accepted.
//	// returns -1 otherwise.
//   return 1;
//}


void CApvRawTree::Loop()
{
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


void CApvRawTree::clear(){
   apv_evt = 0;
   apv_fecNo->clear();
   apv_id->clear();
   apv_ch->clear();
	mm_id->clear();
   mm_plane->clear();
   mm_readout->clear();
   mm_strip->clear();
   apv_q->clear();
}


std::vector<unsigned> CApvRawTree::get_all_unique_readouts()
{
   std::vector<unsigned> vec;
   
   if (!m_has_multi_readouts) {
      return vec;
   }
   long nentries = GetEntries();
   
   for (int ii = 0 ; ii < nentries; ++ii) {
      GetEntry(ii);
      std::vector<unsigned> evtrd = MakeElementsUniqueCopy(*mm_readout);
      vec.insert(vec.end(), evtrd.begin(), evtrd.end());
   }
   
   MakeElementsUnique(vec);
   return vec;
}




