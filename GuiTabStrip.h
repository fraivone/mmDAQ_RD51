//
//  GuiTabStrip.h
//  browser
//
//  Created by Marcin Byszewski on 7.4.12.
//  Copyright 2012 CERN - PH/ADE. All rights reserved.
//

#ifndef browser_GuiTabStrip_h
#define browser_GuiTabStrip_h

#include "GuiTab.h"

class TRootEmbeddedCanvas;
class TCanvas;
class TH1F;
class TGraph;
class TF1;
class TPaveText;
class TText;
namespace browser {
   
   class CEventBrowser;
   
   class CGuiTabStrip : public CGuiTab
   {
      
   public:
      CGuiTabStrip(CGuiBase* gui_parent, CConfiguration* config, CEventBrowser* eb, TGTab* main_tabs, const std::string& name);
      virtual ~CGuiTabStrip();
      virtual void do_construct_tab();
      virtual void do_update(const CDetBase* entry);
      virtual void do_clear_tab();

//      double fitFunctionFinal(double *x, double *par);
      
   private:
      
      
      TRootEmbeddedCanvas*	m_re_canvas; 
      TCanvas*             m_canvas;
      TH1F*                m_hist;
      TGraph*              m_fit;
      TF1*                 m_fitfun;
//      TPaveText*           m_info;
   };
   
} // namespace

#endif
