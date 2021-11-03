//
//  GuiTabReadout.h
//  browser
//
//  Created by Marcin Byszewski on 10.4.12.
//  Copyright 2012 CERN - PH/ADE. All rights reserved.
//

#ifndef browser_GuiTabReadout_h
#define browser_GuiTabReadout_h

#include "GuiTab.h"

class TRootEmbeddedCanvas;
class TCanvas;
class TH1F;
class TH2F;
class TGraph;

namespace browser {
   
   class CEventBrowser;

   class CGuiTabReadout : public CGuiTab
   {
      
      
   public:
      CGuiTabReadout(): CGuiTab(), m_last_detbase(0) {};
      CGuiTabReadout(CGuiBase* gui_parent, CConfiguration* config, CEventBrowser* eb, TGTab* main_tabs, const std::string& name );
      virtual ~CGuiTabReadout();
      virtual void do_construct_tab();
      virtual void do_update(const CDetBase* entry);
      virtual void do_clear_tab();
      
      void update_right_tab();

   private:
      const CDetBase* m_last_detbase;
   };
   
} // namespace

#endif
