//
//  GuiTabChamber.h
//  browser
//
//  Created by Marcin Byszewski on 10.4.12.
//  Copyright 2012 CERN - PH/ADE. All rights reserved.
//

#ifndef browser_GuiTabChamber_h
#define browser_GuiTabChamber_h

#include "GuiTab.h"

namespace browser {
   
   class CEventBrowser;

   class CGuiTabChamber : public CGuiTab
   {
      
   public:
      CGuiTabChamber()  : CGuiTab(), m_last_detbase(0) {}; //Francesco
      CGuiTabChamber(CGuiBase* gui_parent, CConfiguration* config, CEventBrowser* m_eb, TGTab* main_tabs, const std::string& name);
      virtual ~CGuiTabChamber();
      virtual void do_construct_tab();
      virtual void do_update(const CDetBase* entry);
      virtual void do_clear_tab(); //Francesco

      void update_right_tab(); //Francesco
      
   private: //Francesco
      const CDetBase* m_last_detbase; //Francesco

   };
   
} // namespace

#endif
