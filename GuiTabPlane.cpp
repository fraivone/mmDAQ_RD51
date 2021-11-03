//
//  GuiTabPlane.cpp
//  browser
//
//  Created by Marcin Byszewski on 10.4.12.
//  Copyright (c) 2012 CERN - PH/ADE. All rights reserved.
//

#include "GuiTabPlane.h"
#include "EventBrowser.h"
#include "GuiMainFrame.h"
#include "GuiTabPlaneAll1D.h"

#include <TGTab.h>

#include <iostream>

using namespace browser;

CGuiTabPlane::CGuiTabPlane(CGuiBase* gui_parent,
                           CConfiguration* config, 
                           CEventBrowser*   eb,
                           TGTab* main_tabs, 
                           const std::string& name) :
CGuiTab(gui_parent, config, eb, main_tabs,name),
m_last_detbase(0)
{
   construct_tab();
}


CGuiTabPlane::~CGuiTabPlane()
{
   
}

void CGuiTabPlane::do_construct_tab()
{
   
   m_tab_frame->SetLayoutManager(new TGVerticalLayout(m_tab_frame));
   
   {
      //tabs
      // tab widget
      m_gui_right_tab = new TGTab(m_tab_frame, 484, 316);
      m_gui_right_tab->Connect("Selected(Int_t)", "browser::CGuiTabPlane", this, "update_right_tab()");
      
      m_right_tabs.push_back(new CGuiTabPlaneAll1D(this,   m_config, m_eb, m_gui_right_tab, "All readouts 1D"));   
//      m_right_tabs.push_back(new CGuiTabReadout2D(this,   m_config, m_eb, m_gui_right_tab, "Readout 2D"));
//      m_right_tabs.push_back(new CGuiTabReadoutStats(this, m_config, m_eb, m_gui_right_tab, "Statistics"));
      
      //tabs - finish up
      m_gui_right_tab->SetTab(0);
      m_gui_right_tab->Resize(m_tab_frame->GetDefaultSize());
      m_tab_frame->AddFrame(m_gui_right_tab, new TGLayoutHints( kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY));
      //end tabs
   }
   
}


/**
 on user click - rename to handle_...?
 */
void CGuiTabPlane::update_right_tab()
{
   CGuiMainFrame* mf = dynamic_cast<CGuiMainFrame*>(m_gui_parent);
   if (!mf) {
      return;
   }
   
   m_last_detbase = mf->get_tree_selected_user_data();
   if (m_last_detbase) {
      update(m_last_detbase);   
   }
   
}


/**
 update currevt active tab with data from det element
 */
void CGuiTabPlane::do_update(const CDetBase* detbase)
{
   int ctab = m_gui_right_tab->GetCurrent();
   m_right_tabs[ctab]->update(detbase);
   m_last_detbase = detbase;

}

void CGuiTabPlane::do_clear_tab()
{
   std::for_each(m_right_tabs.begin(), m_right_tabs.end(), boost::bind(&CGuiTab::clear_tab, _1));
}
