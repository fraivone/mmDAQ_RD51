//
//  GuiTabPlane.h
//  browser
//
//  Created by Marcin Byszewski on 10.4.12.
//  Copyright 2012 CERN - PH/ADE. All rights reserved.
//

#ifndef browser_GuiTabPlane_h
#define browser_GuiTabPlane_h

#include "GuiTab.h"

namespace browser {
   
   class CEventBrowser;

   class CGuiTabPlane : public CGuiTab
   {
      
   public:
      CGuiTabPlane() : CGuiTab(), m_last_detbase(0) {};
      CGuiTabPlane(CGuiBase* gui_parent, CConfiguration* config, CEventBrowser* eb, TGTab* main_tabs, const std::string& name);
      virtual ~CGuiTabPlane();
      virtual void do_construct_tab();
      virtual void do_update(const CDetBase* entry);
      virtual void do_clear_tab();
      
      void update_right_tab();

   private:
      const CDetBase* m_last_detbase;
      
   };
   
} // namespace

#endif
