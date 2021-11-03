//
//  TreeRawApvPedestals.h
//  eventbrowser
//
//  Created by Marcin Byszewski on 23/11/2012.
//  Copyright 2012 CERN - PH/ADE. All rights reserved.
//

#ifndef eventbrowser_TreeRawApvPedestals_h
#define eventbrowser_TreeRawApvPedestals_h
#include "TreeBase.h"

#include <TROOT.h>
#include <vector>
#include <string>

class TFile;
class TTree;
class TBranch;

namespace browser {
   
   class CConfiguration;

   
   class CTreeRawApvPedestals : public CTreeBase
   {
      
   public:
      CTreeRawApvPedestals(CConfiguration *config, TFile *file, const std::string& treename);
      virtual ~CTreeRawApvPedestals();
      
       virtual long     GetEntries() const;

      virtual void   LoadEventData(DetBasePtrCont& strips, CDetDetector* det);
      virtual void   parse_tree(CDetDetector*, CGuiMainFrame*);
      virtual void   Init(TTree *tree);
      int error() { return ferror;}
      void clear();
      
      virtual size_t size() const { return apv_ch->size();}
      
      bool has_readouts() const { return m_has_multi_readouts;}
      bool has_planes() const { return m_has_planes;}
      bool has_multilayers() const { return m_has_multilayers;}
      
      CConfiguration *m_config;
      TFile*   rootfile;
      bool m_has_multi_readouts;
      bool m_has_planes;
      bool m_has_multilayers;

      
      // Declaration of leaf types
      UInt_t          apv_evt;
      Int_t           time_s;
      Int_t           time_us;
      std::vector<unsigned int> *apv_fecNo;
      std::vector<unsigned int> *apv_id;
      std::vector<unsigned int> *apv_ch;
      std::vector<std::string>  *mm_id;
      std::vector<unsigned>     *mm_multilayer;
      std::vector<unsigned>     *mm_plane;
      std::vector<unsigned int> *mm_readout;
      std::vector<unsigned int> *mm_strip;
      std::vector<float>   *apv_pedmean;
      std::vector<float>   *apv_pedsigma;
      std::vector<float>   *apv_pedstd;
      
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
      TBranch        *b_apv_pedmean;   //!
      TBranch        *b_apv_pedsigma;   //!
      TBranch        *b_apv_pedstd;   //!
          
      
   };
   
} // namespace

#endif
