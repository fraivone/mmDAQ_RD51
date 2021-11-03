//
//  GuiTabDetector.cpp
//  eventbrowser
//
//  Created by Marcin Byszewski on 5.5.12.
//  Copyright (c) 2012 CERN - PH/ADE. All rights reserved.
//

#include "GuiTabDetector.h"
#include "EventBrowser.h"
#include "GuiMainFrame.h"
#include "GuiTabDetectorRawUdp.h"
#include "GuiTabDetectorEvent1D.h"
#include "GuiTabDetectorCrosstalk.h"
#include "GuiTabDetectorPedestal1D.h"
#include "GuiMainFrameOffline.h"
#include <TGTab.h>

#include "GuiTabDetectorStats.h" //Francesco

#include <iostream>

using namespace browser;

CGuiTabDetector::CGuiTabDetector(CGuiBase* gui_parent,
                                 CConfiguration* config, 
                                 CEventBrowser*   eb,
                                 TGTab* main_tabs, 
                                 const std::string& name) :
CGuiTab(gui_parent, config, eb, main_tabs,name),
m_last_detbase(0)
{
   construct_tab();
}


CGuiTabDetector::~CGuiTabDetector()
{
   
}

void CGuiTabDetector::do_construct_tab()
{
   
   m_tab_frame->SetLayoutManager(new TGVerticalLayout(m_tab_frame));
   
   {
      //tabs
      // tab widget
      m_gui_right_tab = new TGTab(m_tab_frame, 484, 316);
      m_gui_right_tab->Connect("Selected(Int_t)", "browser::CGuiTabDetector", this, "update_right_tab()");
      
      //TODO: \todo replace by derived TabDetectorOffline/Online
      if(dynamic_cast<const CGuiMainFrameOffline*>(m_gui_parent)) {
         m_right_tabs.push_back(new CGuiTabDetectorCrosstalk(this, m_config, m_eb, m_gui_right_tab, "Crosstalk"));
      }
      else {
         m_right_tabs.push_back(new CGuiTabDetectorEventRawUdp(this, m_config, m_eb, m_gui_right_tab, "RawUdp"));
      }
      m_right_tabs.push_back(new CGuiTabDetectorEvent1D(this, m_config, m_eb, m_gui_right_tab, "Event1D"));
      //      m_right_tabs.push_back(new CGuiTabReadout2D(this,   m_config, m_eb, m_gui_right_tab, "Readout 2D"));
      m_right_tabs.push_back(new CGuiTabDetectorStats(this, m_config, m_eb, m_gui_right_tab, "Statistics")); //Francesco
      m_right_tabs.push_back(new CGuiTabDetectorPedestal1D(this, m_config, m_eb, m_gui_right_tab, "Pedestal1D"));
      
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
void CGuiTabDetector::update_right_tab()
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
void CGuiTabDetector::do_update(const CDetBase* detbase)
{
   int ctab = m_gui_right_tab->GetCurrent();
   m_right_tabs[ctab]->update(detbase);
   m_last_detbase = detbase;
    
}

void CGuiTabDetector::do_clear_tab()
{
   std::for_each(m_right_tabs.begin(), m_right_tabs.end(), boost::bind(&CGuiTab::clear_tab, _1));
}

