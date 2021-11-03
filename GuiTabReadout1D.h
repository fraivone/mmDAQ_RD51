//
//  GuiTabReadout1D.h
//  browser
//
//  Created by Marcin Byszewski on 11.4.12.
//  Copyright 2012 CERN - PH/ADE. All rights reserved.
//

#ifndef browser_GuiTabReadout1D_h
#define browser_GuiTabReadout1D_h

#include "GuiTab.h"

class TRootEmbeddedCanvas;
class TCanvas;
class TH1F;
class TH2F;
class TGraph;
class TMultiGraph;

namespace browser {
   
   class CGuiTabReadout1D : public CGuiTab
   {
      
   public:
      CGuiTabReadout1D(CGuiBase* gui_parent, CConfiguration* config, CEventBrowser* eb, TGTab* main_tabs, const std::string& name );
      virtual ~CGuiTabReadout1D();
      virtual void do_construct_tab();
      virtual void do_update(const CDetBase* entry);
      virtual void do_clear_tab();
   private:
      TRootEmbeddedCanvas*	m_re_canvas; 
      TCanvas*             m_canvas;
      TH1F*                m_qdisp;
      TH2F*                m_tframe;
      TH1F*                m_qover;
      TMultiGraph*         m_tmulti;
//      THStack*             m_qstack;
//      THStack*             m_tstack;
   };
   
} // namespace

#endif
