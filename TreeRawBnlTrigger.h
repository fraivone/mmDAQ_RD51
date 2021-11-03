//
//  TreeRawBnlTrigger.h
//  eventbrowser
//
//  Created by Marcin Byszewski on 21/11/2012.
//  Copyright 2012 CERN - PH/ADE. All rights reserved.
//

#ifndef eventbrowser_TreeRawBnlTrigger_h
#define eventbrowser_TreeRawBnlTrigger_h
#include "TreeBase.h"

#include <TROOT.h>
#include <vector>
#include <string>

class TFile;
class TTree;

namespace browser {
   
   class CConfiguration;

   
   class CTreeRawBnlTrigger  : public CTreeBase
   {
      
   public:
      CTreeRawBnlTrigger(CConfiguration *config, TFile *file, const std::string& treename);
      virtual ~CTreeRawBnlTrigger();
      
      virtual void LoadEventData(DetBasePtrCont& strips, CDetDetector* det);
      virtual void parse_tree(CDetDetector*, CGuiMainFrame*);
      virtual void Init(TTree *tree);
     
      int error() { return ferror;}
      void clear();
      
      virtual size_t size() const { return m_strip->size();}
      
      CConfiguration *m_config;
      TFile*   rootfile;
            
      //common branches (tree branch name in quotes)
      ULong64_t   m_evt;               // "evt",  event id, is one of: DaqTime, SrsTimeStamp, SrsTriggerNumber
      UInt_t   m_evt_error;            // "error" , data consistency error (n/a yet)
      int      m_time_s;               // "daqTimeSec "DaqTime (stamped by the daq)
      int      m_time_us;              // "daqTimeMicroSec", DaqTime (stamped by the daq)
            
      std::vector<UInt_t>*      m_fec;     ///electronics address - FEC id number
      std::vector<UInt_t>*      m_chip;    ///electronics address - Chip number within FEC
      std::vector<UInt_t>*      m_channel; ///electronics address - Channel number within chip
      std::vector<std::string>* m_chamber; ///detector address - Chamber name (possibly 'unmapped')
      std::vector<int>*         m_layer;   ///detector address - Chamber Layer number
      std::vector<char>*        m_readout; ///detector address - Chamber readout strips (a letter, 'E' means error/unmapped)
      std::vector<int>*         m_strip;   ///detector address - Strip number within readout
      
      //branches specific to VMM1 trigger bnl box
      std::vector<double>* m_time; ///VMM1 time value
      std::vector<char>* m_flag;

      
      
   };
   
} // namespace

#endif
