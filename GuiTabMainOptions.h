//
//  GuiTabMainOptions.h
//  browser
//
//  Created by Marcin Byszewski on 12.4.12.
//  Copyright 2012 CERN - PH/ADE. All rights reserved.
//

#ifndef browser_GuiTabMainOptions_h
#define browser_GuiTabMainOptions_h

#include "GuiTab.h"

#include <vector>

class TGRadioButton;
class TGCheckButton;
class TGVButtonGroup;
class TGTextButton;
class TGNumberEntry;


namespace browser {
   
   class CEventBrowser;
   class CEventBrowserOnline;
   
   class CGuiTabMainOptions : public CGuiTab
   {
      
   public:
      CGuiTabMainOptions() : CGuiTab(), m_gui_radio_raw(0), m_gui_raw_source(),
      m_gui_qfitting_show(0), m_gui_clusters_show(0), m_gui_load_threshold_file(0) {};//-//
      CGuiTabMainOptions(CGuiBase* gui_parent, CConfiguration* config, CEventBrowser* eb, TGTab* main_tabs, const std::string& name);
      virtual ~CGuiTabMainOptions();
      virtual void do_construct_tab();
      virtual void do_update(const CDetBase* entry);
	  CEventBrowserOnline* m_eb;
      //GUI callbacks / slots
      void gui_handle_raw_selection(int) const;
      void gui_handle_overlay_fitting(bool) const;
      void gui_handle_load_thresholds_file();
      void gui_handle_clear_thresholds_file();
      
      
   private:
      TGVButtonGroup*               m_gui_radio_raw;
      std::vector<TGRadioButton*>   m_gui_raw_source;
      TGCheckButton*                m_gui_qfitting_show;
      TGCheckButton*                m_gui_clusters_show;
      TGTextButton*                 m_gui_load_threshold_file;
      

      void load_thresholds_root_file(const std::string& filename);
      void load_thresholds_text_file(const std::string& filename);
	  
	  IDList IDs;
   };
   
} // namespace

#endif
