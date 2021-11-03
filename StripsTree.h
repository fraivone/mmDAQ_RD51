//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Apr 13 22:25:18 2012 by ROOT version 5.32/01
// from TTree strips/strips_all
// found on file: /Volumes/HDD/data/2010/testbeam_Nov/analysis/250um_vs_500um/fitted/run6296_10_250um_fit_dr.root
//////////////////////////////////////////////////////////

#ifndef StripsTree_h
#define StripsTree_h

#include <TROOT.h>
//#include <TChain.h>
//#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <vector>
#include <string>

// Fixed size dimensions of array or collections stored in the TTree if any.

class TBranch;
class TTree;
class TFile;

namespace browser {
   class CStripsTree;
   
      
   
   class CStripsTree {
      
      TFile*   rootfile;

   public:
      CStripsTree(TFile *file, const std::string& treename);
//      CStripsTree(TTree *tree=0);
      
      virtual ~CStripsTree();
      virtual Int_t    Cut(Long64_t entry);
      virtual Int_t    GetEntry(Long64_t entry);
      virtual Long64_t LoadTree(Long64_t entry);
      virtual void     Init(TTree *tree);
      virtual void     Loop();
      virtual Bool_t   Notify();
      virtual void     Show(Long64_t entry = -1);
      long	           GetEntries() const;
      TTree* GetTree();
      int error();
      void clear();
      long get_index_by_number(unsigned number);
      
      TTree*                     fChain;   //!pointer to the analyzed TTree or TChain
      Int_t                      fCurrent; //!current Tree number in a TChain
      int                        ferror;

      // Declaration of leaf types
      UInt_t                     evt;
      std::vector<std::string>*  mm_id;
      std::vector<unsigned int>* mm_plane;
      std::vector<unsigned int>* mm_readout;
      std::vector<unsigned int>* mm_strip;
      std::vector<short>*        qmax;
      std::vector<short>*        tbqmax;
      std::vector<short>*        tbcfd;
      std::vector<double>*       qfit;
      std::vector<double>*       tfit;
      std::vector<double>*       tfitcfd;
      std::vector<std::vector<double> > * fitpars;
      
      // List of branches
      TBranch        *b_evt;   //!
      TBranch        *b_mm_id;   //!
      TBranch        *b_mm_plane;   //!
      TBranch        *b_mm_readout;   //!
      TBranch        *b_mm_strip;   //!
      TBranch        *b_qmax;   //!
      TBranch        *b_tbqmax;   //!
      TBranch        *b_tbcfd;   //!
      TBranch        *b_qfit;   //!
      TBranch        *b_tfit;   //!
      TBranch        *b_tfitcfd;   //!
      TBranch        *b_fitpars;   //!
      

   };
   
   class CStripsTreeEntry 
   {
   public:
      explicit CStripsTreeEntry() :
      evt(0),
      mm_id(""),
      mm_plane(0),
      mm_readout(0),
      mm_strip(0),
      qmax(0),
      tbqmax(0),
      tbcfd(0),
      qfit(0),
      tfit(0),
      tfitcfd(0),
      fitpars()
      { }
      
      CStripsTreeEntry(CStripsTree* st, size_t n) :
      evt(st->evt),
      mm_id((*(st->mm_id))[n]),
      mm_plane((*(st->mm_plane))[n]),
      mm_readout((*(st->mm_readout))[n]),
      mm_strip((*(st->mm_strip))[n]),
      qmax((*(st->qmax))[n]),
      tbqmax((*(st->tbqmax))[n]),
      tbcfd((*(st->tbcfd))[n]),
      qfit((*(st->qfit))[n]),
      tfit((*(st->tfit))[n]),
      tfitcfd((*(st->tfitcfd))[n]),
      fitpars((*(st->fitpars))[n])
      { }
      
      // Declaration of leaf types
      UInt_t       evt;
      std::string  mm_id;
      unsigned int mm_plane;
      unsigned int mm_readout;
      unsigned int mm_strip;
      short        qmax;
      short        tbqmax;
      short        tbcfd;
      double       qfit;
      double       tfit;
      double       tfitcfd;
      std::vector<double> fitpars;
   };

}
#endif
