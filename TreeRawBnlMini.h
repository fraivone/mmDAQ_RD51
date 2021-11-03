//
//  TreeRawBnlMini.h
//  eventbrowser
//
//  Created by Marcin Byszewski on 07/08/2012.
//  Copyright 2012 CERN - PH/ADE. All rights reserved.
//

#ifndef eventbrowser_TreeRawBnlMini_h
#define eventbrowser_TreeRawBnlMini_h
#include "TreeBase.h"

#include <TROOT.h>
#include <vector>
#include <string>

class TFile;
class TTree;

class VMM1Parameters;

namespace browser {
   
   class CConfiguration;

   
   class CTreeRawBnlMini : public CTreeBase
   {
      
   public:
      CTreeRawBnlMini(CConfiguration *config, TFile *file, const std::string& treename);
      virtual ~CTreeRawBnlMini();
      
      
      virtual void   LoadEventData(DetBasePtrCont& strips, CDetDetector* det);
      virtual void   parse_tree(CDetDetector*, CGuiMainFrame*);      
      virtual void   Init(TTree *tree);
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
      int      m_srs_time_stamp;       // "srsTimeStamp", srs time stamp (counter of clock cycles)
      UInt_t   m_srs_trigger;  // "srsTrigger", trigger numbber
      
      
      std::vector<UInt_t>*      m_fec;     ///electronics address - FEC id number
      std::vector<UInt_t>*      m_chip;    ///electronics address - Chip number within FEC
      std::vector<UInt_t>*      m_channel; ///electronics address - Channel number within chip
      std::vector<std::string>* m_chamber; ///detector address - Chamber name (possibly 'unmapped')
      std::vector<int>*         m_layer;   ///detector address - Chamber Layer number
      std::vector<char>*        m_readout; ///detector address - Chamber readout strips (a letter, 'E' means error/unmapped)
      std::vector<int>*         m_strip;   ///detector address - Strip number within readout
      
      //branches specific to VMM1
      std::vector<double>* m_charge; ///VMM1 charge value, PDO
      std::vector<double>* m_time;   ///VMM1 time value, TDO
      
      
//      //common branches
//      UInt_t   m_evt;
//      UInt_t   m_evt_error;
//      int      m_time_s; 
//      int      m_time_us; 
//      int      m_srs_time_stamp; //srs time stamp 
//      UInt_t   m_srs_trigger;
//      
//      
//      std::vector<UInt_t>*      m_fec;
//      std::vector<UInt_t>*      m_chip;
//      std::vector<UInt_t>*      m_channel;
//      
//      std::vector<std::string>* m_chamber;  //mm_id
//      std::vector<int>*         m_layer;    //plane(aka layer, gas gap) id number
//      std::vector<char>*        m_readout;  //mm_readout
//      std::vector<int>*         m_strip;    //mm_strip
//      
//      //specific branches
//      std::vector<double>* m_charge;
//      std::vector<double>* m_time; 
      
      
   };
   
} // namespace

#endif
