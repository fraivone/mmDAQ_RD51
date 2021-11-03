//
//  GuiTabChamber.cpp
//  browser
//
//  Created by Marcin Byszewski on 10.4.12.
//  Copyright (c) 2012 CERN - PH/ADE. All rights reserved.
//

#include "GuiTabChamber.h"
#include "EventBrowser.h"
#include "GuiTabChamberEvent1D.h"  
#include "GuiTabChamberEvent2D.h"  
#include "GuiTabChamberStats.h"  
#include "GuiMainFrame.h"

#include <TGTab.h> 

#include <iostream>

using namespace browser;

CGuiTabChamber::CGuiTabChamber(CGuiBase* gui_parent,
                               CConfiguration* config, 
                               CEventBrowser*   eb,
                               TGTab* main_tabs, 
                               const std::string& name) :
  CGuiTab(gui_parent, config, eb, main_tabs,name),
m_last_detbase(0)  
{
    construct_tab();
}


CGuiTabChamber::~CGuiTabChamber()
{
   
}

void CGuiTabChamber::do_construct_tab()
{
   m_tab_frame->SetLayoutManager(new TGVerticalLayout(m_tab_frame));
   
   {
      //tabs
      // tab widget
      m_gui_right_tab = new TGTab(m_tab_frame, 484, 316);
      m_gui_right_tab->Connect("Selected(Int_t)", "browser::CGuiTabChamber", this, "update_right_tab()");
      
      //TODO: \todo replace by derived TabDetectorOffline/Online
      // m_right_tabs.push_back(new CGuiTabDetectorEvent1D(this, m_config, m_eb, m_gui_right_tab, "Event1D"));
      // m_right_tabs.push_back(new CGuiTabReadoutStats(this, m_config, m_eb, m_gui_right_tab, "Statistics"));
      m_right_tabs.push_back(new CGuiTabChamberEvent1D(this, m_config, m_eb, m_gui_right_tab, "Event 1D")); 
      m_right_tabs.push_back(new CGuiTabChamberEvent2D(this, m_config, m_eb, m_gui_right_tab, "RecHit 2D")); 
      m_right_tabs.push_back(new CGuiTabChamberStats(this, m_config, m_eb, m_gui_right_tab, "Statistics")); 
      
      //tabs - finish up
      m_gui_right_tab->SetTab(0);
      m_gui_right_tab->Resize(m_tab_frame->GetDefaultSize());
      m_tab_frame->AddFrame(m_gui_right_tab, new TGLayoutHints( kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY));
      //end tabs
   }

   
}

void CGuiTabChamber::update_right_tab()  
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

void CGuiTabChamber::do_update(const CDetBase* detbase)
{
  
  // std::cout << "CGuiTabChamber::update() " << detbase->get_name() << "\n";
   if (!m_gui_right_tab) {
      std::cout << "CGuiTabChamber::update() m_gui_right_tab NULL\n";
      return;
   }
   int ctab = m_gui_right_tab->GetCurrent();
   m_right_tabs[ctab]->update(detbase);
//   std::for_each(m_right_tabs.begin(), m_right_tabs.end(), boost::bind(&CGuiTab::update, _1, detbase));
   m_last_detbase = detbase;
}

void CGuiTabChamber::do_clear_tab() 
{
   std::cout << "CGuiTabChamber::do_clear_tab() clear right tabs \n";
   std::for_each(m_right_tabs.begin(), m_right_tabs.end(), boost::bind(&CGuiTab::clear_tab, _1));
}
