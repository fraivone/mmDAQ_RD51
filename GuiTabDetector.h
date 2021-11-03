//
//  GuiTabDetector.h
//  eventbrowser
//
//  Created by Marcin Byszewski on 5.5.12.
//  Copyright 2012 CERN - PH/ADE. All rights reserved.
//

#ifndef eventbrowser_GuiTabDetector_h
#define eventbrowser_GuiTabDetector_h

#include "GuiTab.h"

namespace browser {
   
   class CDetBase;
   
   class CGuiTabDetector : public CGuiTab
   {
      
   public:
      CGuiTabDetector() : CGuiTab(), m_last_detbase(0) {};
      CGuiTabDetector(CGuiBase* gui_parent, CConfiguration* config, CEventBrowser* m_eb, TGTab* main_tabs, const std::string& name);
      virtual ~CGuiTabDetector();
      virtual void do_construct_tab();
      virtual void do_update(const CDetBase* entry);
      virtual void do_clear_tab();

      
      //handlers
      void update_right_tab();

   private:
      const CDetBase* m_last_detbase;
      
   };
   
} // namespace

#endif
