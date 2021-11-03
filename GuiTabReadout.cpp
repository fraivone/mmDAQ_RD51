//
//  GuiTabReadout.cpp
//  browser
//
//  Created by Marcin Byszewski on 10.4.12.
//  Copyright (c) 2012 CERN - PH/ADE. All rights reserved.
//

#include "GuiTabReadout.h"
#include "EventBrowser.h"
#include "DetReadout.h"
#include "GuiTabReadout1D.h"
#include "GuiTabReadout2D.h"
#include "GuiTabReadoutStats.h"
#include "GuiTabReadoutPedestals.h"
#include "GuiTabReadoutProps.h"
#include "GuiMainFrame.h"

//#include <TH1.h>
//#include <TH2.h>
//#include <TGraph.h>
//#include <TCanvas.h>
//#include <TGFrame.h>
//#include <TRootEmbeddedCanvas.h>
#include <TGTab.h>

#include <boost/foreach.hpp>
#include <boost/bind.hpp>

#include <iostream>

using namespace browser;

CGuiTabReadout::CGuiTabReadout(CGuiBase* gui_parent,
                               CConfiguration* config, 
                               CEventBrowser*   eb,
                               TGTab* main_tabs, 
                               const std::string& name ) :
CGuiTab(gui_parent, config, eb, main_tabs,name),
m_last_detbase(0)

{
   construct_tab();
}


CGuiTabReadout::~CGuiTabReadout()
{
  
}

void CGuiTabReadout::do_construct_tab()
{
   m_tab_frame->SetLayoutManager(new TGVerticalLayout(m_tab_frame));
   
   {
      //tabs
      // tab widget
      m_gui_right_tab = new TGTab(m_tab_frame, 484, 316);
      m_gui_right_tab->Connect("Selected(Int_t)", "browser::CGuiTabReadout", this, "update_right_tab()");
      
      m_right_tabs.push_back(new CGuiTabReadout1D(this,   m_config, m_eb, m_gui_right_tab, "Readout 1D"));   
      m_right_tabs.push_back(new CGuiTabReadout2D(this,   m_config, m_eb, m_gui_right_tab, "Readout 2D"));
      m_right_tabs.push_back(new CGuiTabReadoutStats(this, m_config, m_eb, m_gui_right_tab, "Statistics"));
      m_right_tabs.push_back(new CGuiTabReadoutPedestals(this, m_config, m_eb, m_gui_right_tab, "Pedestals"));
      m_right_tabs.push_back(new CGuiTabReadoutProps(this, m_config, m_eb, m_gui_right_tab, "Properties"));
      
      //tabs - finish up
      m_gui_right_tab->SetTab(0);
      m_gui_right_tab->Resize(m_tab_frame->GetDefaultSize());
      m_tab_frame->AddFrame(m_gui_right_tab, new TGLayoutHints( kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY));
      //end tabs
   }

}

void CGuiTabReadout::update_right_tab()
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

void CGuiTabReadout::do_update(const CDetBase* detbase)
{
//   std::cout << "CGuiTabReadout::update() " << detbase->get_name() << "\n";
   if (!m_gui_right_tab) {
      std::cout << "CGuiTabReadout::update() m_gui_right_tab NULL\n";
      return;
   }
   int ctab = m_gui_right_tab->GetCurrent();
   m_right_tabs[ctab]->update(detbase);
//   std::for_each(m_right_tabs.begin(), m_right_tabs.end(), boost::bind(&CGuiTab::update, _1, detbase));
   m_last_detbase = detbase;
}

void CGuiTabReadout::do_clear_tab()
{
   std::cout << "CGuiTabReadout::do_clear_tab() clear right tabs \n";
   std::for_each(m_right_tabs.begin(), m_right_tabs.end(), boost::bind(&CGuiTab::clear_tab, _1));
}
