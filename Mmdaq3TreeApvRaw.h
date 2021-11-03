//
//  Mmdaq3TreeApvRaw.h
//  eventbrowser
//
//  Created by Marcin Byszewski on 01/08/2012.
//  Copyright 2012 CERN - PH/ADE. All rights reserved.
//

#ifndef eventbrowser_Mmdaq3TreeApvRaw_h
#define eventbrowser_Mmdaq3TreeApvRaw_h

#include "TreeBase.h"

#include <TROOT.h>
#include <vector>
#include <string>


class TFile;
class TTree;

namespace browser {
   

   class CMmdaq3TreeApvRaw : public CTreeBase
   {
      
   public:
      CMmdaq3TreeApvRaw(TFile *file, const std::string& treename);
      virtual ~CMmdaq3TreeApvRaw();
      
      virtual void LoadEventData(DetBasePtrCont& strips, CDetDetector* det);
      virtual void parse_tree(CDetDetector*, CGuiMainFrame*);
      virtual void Init(TTree *tree);
     
      int error() { return ferror;}
      void clear();
     
      virtual size_t size() const { return m_strip->size();}

      bool has_multilayers() const { return m_has_multilayers; }
      
      TFile *rootfile;
      bool m_has_multilayers;
      
      //common branches
      UInt_t   m_evt;
      UInt_t   m_evt_error;
      int      m_time_s; 
      int      m_time_us; 
      int      m_srs_time_stamp; //srs time stamp
      UInt_t   m_srs_trigger;
      
      std::vector<UInt_t>*      m_fec;
      std::vector<UInt_t>*      m_chip;
      std::vector<UInt_t>*      m_channel;
      
      std::vector<std::string>* m_chamber;  //mm_id
      std::vector<int>*         m_multilayer; // mm_mlayer
      std::vector<int>*         m_layer;    //plane(aka layer, gas gap) id number
      std::vector<char>*        m_readout;  //mm_readout
      std::vector<int>*         m_strip;    //mm_strip
      
      //specific branches
      std::vector<std::vector<short> >* m_charge; //apv_q was Short_t
      std::vector<short>         m_max_charge;

      
      
      
   };
   
} // namespace

#endif
