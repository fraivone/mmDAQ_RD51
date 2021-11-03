//
//  GuiTab.h
//  browser
//
//  Created by Marcin Byszewski on 7.4.12.
//  Copyright 2012 CERN - PH/ADE. All rights reserved.
//

#ifndef browser_GuiTab_h
#define browser_GuiTab_h

#include "GuiBase.h"
#include "RootThread.h"


#include <string>
#include <vector>

class TGTab;
class TGCompositeFrame;
class TGListTreeItem;
class TText;
class TPaveText;

namespace browser {
   
   class CEventBrowser;
   class CDetBase;
   
   class CGuiTab : public CGuiBase
   {
      
   public:
      CGuiTab() : CGuiBase(), m_eb(0), m_parent_tabs(0), m_name(), m_tab_frame(0),
      m_footer_text(0), m_info(0), m_gui_right_tab(0), m_right_tabs() { };
      CGuiTab(CGuiBase* gui_parent,
              CConfiguration* config, 
              CEventBrowser* eb, 
              TGTab* main_tabs, 
              const std::string& name);
      virtual ~CGuiTab() = 0;
      
      virtual void construct_tab() {
         do_construct_tab();
      };
      virtual void update(const CDetBase* detbase) {
         do_update(detbase);
      }
      
      virtual void clear_tab() {
         do_clear_tab();
      }
      
      TGCompositeFrame* get_frame() {
         return m_tab_frame;
      }
   protected:       
      CEventBrowser*    m_eb;
      TGTab*            m_parent_tabs; ///< parent tab
      std::string       m_name;
      TGCompositeFrame* m_tab_frame;   ///< this tab's frame
      TText*            m_footer_text;
      TPaveText*        m_info;

      //children tabs
      TGTab*                 m_gui_right_tab;
      std::vector<CGuiTab*>  m_right_tabs;
      
      void draw_footer();
      void draw_info(const CDetBase* base);

   private:   
      virtual void do_construct_tab() = 0;
      virtual void do_update(const CDetBase* detbase) = 0;
      virtual void do_clear_tab() { };
      
      
   };
   
} // namespace

#endif
