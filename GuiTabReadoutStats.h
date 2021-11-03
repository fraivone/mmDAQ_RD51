//
//  GuiTabReadoutStats.h
//  browser
//
//  Created by Marcin Byszewski on 11.4.12.
//  Copyright 2012 CERN - PH/ADE. All rights reserved.
//

#ifndef browser_GuiTabReadoutStats_h
#define browser_GuiTabReadoutStats_h

#include "GuiTab.h"

class TRootEmbeddedCanvas;
class TCanvas;
class TH1F;
class TH2F;
class TGraph;

namespace browser {
   
   class CGuiTabReadoutStats : public CGuiTab
   {
      
   public:
      CGuiTabReadoutStats(CGuiBase* gui_parent, CConfiguration* config, CEventBrowser* eb, TGTab* main_tabs, const std::string& name );
      virtual ~CGuiTabReadoutStats();
      virtual void do_construct_tab();
      virtual void do_update(const CDetBase* entry);
      
   private:
      TRootEmbeddedCanvas*	m_re_canvas; 
      TCanvas*             m_canvas;
      TH1F*                m_hitmap;
      TH1F*                m_qmax;
      TH1F*                m_maxqmax;
      TH1F*                m_tbqmax;
      TH1F*                m_tbmaxqmax;
      TH1F*                m_nstrips;
      TH1F*                m_lumi;

   };
   
} // namespace

#endif
