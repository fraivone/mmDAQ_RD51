//
//  GuiTabReadoutProps.h
//  browser
//
//  Created by Marcin Byszewski on 13.4.12.
//  Copyright 2012 CERN - PH/ADE. All rights reserved.
//

#ifndef browser_GuiTabReadoutProps_h
#define browser_GuiTabReadoutProps_h

#include "GuiTab.h"


namespace browser {
   
   class CGuiTabReadoutProps : public CGuiTab
   {
      
   public:
      CGuiTabReadoutProps(CGuiBase* gui_parent, CConfiguration* config, CEventBrowser* eb, TGTab* main_tabs, const std::string& name);
      virtual ~CGuiTabReadoutProps();
      virtual void do_construct_tab();
      virtual void do_update(const CDetBase* entry);
   };
   
} // namespace

#endif
