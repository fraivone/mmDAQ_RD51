//
//  GuiTabDaqMonitor.h
//  eventbrowser
//
//  Created by Marcin Byszewski on 2/14/13.
//  Copyright (c) 2013 CERN - PH/ADE. All rights reserved.
//

#ifndef __eventbrowser__GuiTabDaqMonitor__
#define __eventbrowser__GuiTabDaqMonitor__

#include "GuiTab.h"


#include <iostream>
class TGLabel;

namespace browser {

   class CGuiTabDaqMonitor : public CGuiTab
   {
   public:
      CGuiTabDaqMonitor() : CGuiTab(), m_ip_label(0), m_frame_label(0) {};
      CGuiTabDaqMonitor(CGuiBase* gui_parent, CConfiguration* config, CEventBrowser* m_eb, TGTab* main_tabs, const std::string& name);
      virtual ~CGuiTabDaqMonitor();
      virtual void do_construct_tab();
      virtual void do_update(const CDetBase* entry);

   private:
      TGLabel* m_ip_label;
      TGLabel* m_frame_label;
   };
}


#endif /* defined(__eventbrowser__GuiTabDaqMonitor__) */
