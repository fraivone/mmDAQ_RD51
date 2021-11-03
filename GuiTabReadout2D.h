//
//  GuiTabReadout2D.h
//  browser
//
//  Created by Marcin Byszewski on 11.4.12.
//  Copyright 2012 CERN - PH/ADE. All rights reserved.
//

#ifndef browser_GuiTabReadout2D_h
#define browser_GuiTabReadout2D_h

#include "GuiTab.h"

class TRootEmbeddedCanvas;
class TCanvas;
class TH1F;
class TH2F;
class TGraph;

namespace browser {
   
   class CGuiTabReadout2D : public CGuiTab
   {
      
   public:
      CGuiTabReadout2D(CGuiBase* gui_parent, CConfiguration* config, CEventBrowser* eb, TGTab* main_tabs, const std::string& name );
      virtual ~CGuiTabReadout2D();
      virtual void do_construct_tab();
      virtual void do_update(const CDetBase* entry);
      virtual void do_clear_tab();
      
   private:
      TRootEmbeddedCanvas*	m_re_canvas; 
      TCanvas*             m_canvas;
      TH2F*                m_hist;
      
   };
   
} // namespace

#endif
