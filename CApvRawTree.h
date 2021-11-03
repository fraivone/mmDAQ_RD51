//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Nov 11 22:23:18 2010 by ROOT version 5.27/04
// from TTree raw/rawdata
// found on file: run6213.root
//////////////////////////////////////////////////////////

#ifndef CApvRawTree_h
#define CApvRawTree_h

#include "TreeBase.h"

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

#include <vector>
#include <string>

using std::vector;
using std::string;


namespace browser {
   
   class CApvRawTree : public CTreeBase {
      
      TFile *rootfile;
      bool m_has_multi_readouts;
      bool m_has_planes;
      bool m_has_multilayers;
      
   public :
      TTree *fChain;   //!pointer to the analyzed TTree or TChain
      Int_t fCurrent; //!current Tree number in a TChain
      int   ferror;
      Long64_t m_current_entry;

      // Declaration of leaf types
      unsigned					apv_evt;
      Long64_t             time_s;
      int                  time_us;
      //pointers
      vector<unsigned>     *apv_fecNo;
      vector<unsigned>     *apv_id;
      vector<unsigned>     *apv_ch;
      vector<string>			*mm_id;
      vector<unsigned>     *mm_multilayer;
      vector<unsigned>     *mm_plane;
      vector<unsigned>     *mm_readout;
      vector<unsigned>     *mm_strip;
      vector<vector<Short_t> > *apv_q;      
      
      // List of branches
      TBranch        *b_apv_evt;   //!
      TBranch        *b_time_s;   //!
      TBranch        *b_time_us;   //!
      TBranch        *b_apv_fecNo;   //!
      TBranch        *b_apv_id;   //!
      TBranch        *b_apv_ch;   //!
      TBranch        *b_mm_id;   //!
      TBranch        *b_mm_mlayer;   //! 
      TBranch        *b_mm_plane;   //!
      TBranch        *b_mm_readout;   //!
      TBranch        *b_mm_strip;   //!
      TBranch        *b_apv_q;   //!
      
      CApvRawTree(TFile *file, const std::string& treename);
//      CApvRawTree(TTree *tree=0);
      
      virtual ~CApvRawTree();
      //   virtual Int_t    Cut(Long64_t entry);
      virtual Int_t    GetEntry(Long64_t entry);
      virtual Long64_t GetCurrentEntryNumber() const;
      virtual void LoadEventData(DetBasePtrCont& strips, CDetDetector* det);
      virtual void parse_tree(CDetDetector*, CGuiMainFrame*);

      
      virtual long      GetEntries() const;
      virtual size_t size() const { return mm_strip->size();}

      virtual Long64_t LoadTree(Long64_t entry);
      virtual void     Init(TTree *tree);
      virtual void     Loop();
      virtual Bool_t   Notify();
      virtual void     Show(Long64_t entry = -1);
      int error() { return ferror;}
      void clear();
      bool has_readouts() const { return m_has_multi_readouts;}
      bool has_planes() const { return m_has_planes;}
      bool has_multilayers() const { return m_has_multilayers;}
//      virtual size_t size() const { return mm_strip->size();}
      
      //   void AddApvRawBranches(TTree* atree);
      
      std::vector<unsigned> get_all_unique_readouts();
      TTree* GetTree();

   };
   
   
}


#endif
