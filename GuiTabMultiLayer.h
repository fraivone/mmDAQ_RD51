//
//  GuiTabMultiLayer.h
//  eventbrowser
//
//  Created by Marcin Byszewski on 27/10/2012.
//  Copyright 2012 CERN - PH/ADE. All rights reserved.
//

#ifndef eventbrowser_GuiTabMultiLayer_h
#define eventbrowser_GuiTabMultiLayer_h

#include "GuiTab.h"


namespace browser {
   
   class CGuiBase;
   class CDetBase;
   class CEventBrowser;
   class CConfiguration;
   
   class CGuiTabMultiLayer  : public CGuiTab
   {
      
   public:
      CGuiTabMultiLayer();
      CGuiTabMultiLayer(CGuiBase* gui_parent, CConfiguration* config, CEventBrowser* eb, TGTab* main_tabs, const std::string& name);
      virtual ~CGuiTabMultiLayer();
      virtual void do_construct_tab();
      virtual void do_update(const CDetBase* entry);
      virtual void do_clear_tab();
      
      
   };
   
} // namespace

#endif
