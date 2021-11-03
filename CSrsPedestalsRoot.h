//
//  CSrsPedestalsRoot.h
//  eventbrowser
//
//  Created by Nikolaos Tsirintanis on 3/08/2014.
//  Copyright (c) 2014 CERN - PH/ADE. All rights reserved.
//

#ifndef eventbrowser_CSrsPedestalsRoot_h
#define eventbrowser_CSrsPedestalsRoot_h
#include "TreeBase.h"

#include <TROOT.h>
#include <vector>
#include <string>

class TFile;
class TTree;
class TBranch;

namespace browser {
   
   class CConfiguration;

   
   class CSrsPedestalsRoot : public CTreeBase
   {
      
   public:
      CSrsPedestalsRoot(CConfiguration *config, TFile *file, const std::string& treename);

      virtual ~CSrsPedestalsRoot();
      
       virtual long     GetEntries() const;

      virtual void   LoadEventData(DetBasePtrCont& strips, CDetDetector* det);
      virtual void   parse_tree(CDetDetector*, CGuiMainFrame*);
      virtual void   Init(TTree *tree);
      int error() { return ferror;}
      void clear();
      
      virtual size_t size() const { return pstrip->size();}
      
      bool has_readouts() const { return m_has_multi_readouts;}
      bool has_planes() const { return m_has_planes;}
      bool has_multilayers() const { return m_has_multilayers;}
      
      CConfiguration *m_config;
      TFile*   rootfile;
      bool m_has_multi_readouts;
      bool m_has_planes;
      bool m_has_multilayers;

      
     ULong64_t   m_evt;
         int      m_time_s; 
         int      m_time_us; 
         int      m_srs_time_stamp; //srs time stamp 
         std::vector<UInt_t>*       pfec ;  
         std::vector<UInt_t>*       pchip ; 
         std::vector<UInt_t>*       pchan  ;
         std::vector<std::string>*  pchamb ;
         std::vector<int>*          player ;
         std::vector<int>*          pmlayer ;
         std::vector<char>*         pread  ;
         std::vector<int>*          pstrip ;
         std::vector<int>*          pplane ;
         std::vector<double>*       pmean ;
         std::vector<double>*       pstdev ;
         std::vector<double>*       psigma;
         
         // List of branches
         TBranch        *b_evt;   //!
         TBranch        *b_time_s;   //!
         TBranch        *b_time_us;   //!
         TBranch        *b_srs_time_stamp; //!
         TBranch        *b_fec;   //!
         TBranch        *b_chip;   //!
         TBranch        *b_chan;   //!
         TBranch        *b_chamb;   //!
         TBranch        *b_layer;   //!
         TBranch        *b_mlayer;   //!
         TBranch        *b_read;   //!
         TBranch        *b_strip;   //!
         TBranch        *b_plane;   //!
         TBranch        *b_mean;   //!
         TBranch        *b_stdev;   //!
         TBranch        *b_sigma;  //!

	
      
   };
   
} // namespace

#endif
