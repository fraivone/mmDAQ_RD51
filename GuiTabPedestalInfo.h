//
//  GuiTabReadoutPedestals.h
//  eventbrowser
//
//  Created by Marcin Byszewski on 23/11/2012.
//  Copyright 2012 CERN - PH/ADE. All rights reserved.
//

#ifndef eventbrowser_GuiTabPedestalInfo_h
#define eventbrowser_GuiTabPedestalInfo_h

#include "GuiTab.h"
#include "DetReadout.h"

class TRootEmbeddedCanvas;
class TCanvas;
class TH1F;
class TH2F;
class TGraph;
class TMultiGraph;

namespace browser {
   
   class CGuiTabPedestalInfo : public CGuiTab
   {
      
   public:
      CGuiTabPedestalInfo(CGuiBase* gui_parent, CConfiguration* config, CEventBrowser* eb, TGTab* main_tabs, const std::string& name );
      virtual ~CGuiTabPedestalInfo();
      
      virtual void do_construct_tab();
      virtual void do_update(const CDetBase* entry);
      virtual void do_clear_tab();
   private:
      TRootEmbeddedCanvas*	m_re_canvas;
      TCanvas*             m_canvas;
      TH2F*                m_pedmean;
      TH2F*                m_pedstdev;
   };
   
} // namespace

#endif
