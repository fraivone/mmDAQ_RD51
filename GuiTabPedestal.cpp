//
//  GuiTabPedestal.cpp
//  eventbrowser
//
//  Created by Marcin Byszewski on 5.5.12.
//  Copyright (c) 2012 CERN - PH/ADE. All rights reserved.
//

#include "GuiTabPedestal.h"
#include "EventBrowser.h"
#include "GuiMainFrame.h"
#include "GuiTabPedestalRawUdp.h"
#include "GuiTabPedestalEvent1D.h"
#include "GuiTabPedestalCrosstalk.h"
#include "GuiTabPedestalInfo.h"
#include "GuiMainFrameOffline.h"
#include "GuiMainFrameOnline.h"
#include <TGTab.h>

#include "DetReadout.h"

#include <boost/foreach.hpp>


#include <TH1.h>
#include <TH2.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TCanvas.h>
#include <TGFrame.h>
#include <TRootEmbeddedCanvas.h>
#include <TStyle.h>

#include <iostream>

using namespace browser;

CGuiTabPedestal::CGuiTabPedestal(CGuiBase* gui_parent,
                                 CConfiguration* config, 
                                 CEventBrowser*   eb,
                                 TGTab* main_tabs, 
                                 const std::string& name) :
CGuiTab(gui_parent, config, eb, main_tabs,name),
m_last_detbase(0)

{
   construct_tab();
}


CGuiTabPedestal::~CGuiTabPedestal()
{

}

void CGuiTabPedestal::do_construct_tab()
{
   
    m_tab_frame->SetLayoutManager(new TGVerticalLayout(m_tab_frame));
    
   
      //tabs
      // tab widget
      m_gui_right_tab = new TGTab(m_tab_frame, 484, 316);
      m_gui_right_tab->Connect("Selected(Int_t)", "browser::CGuiTabPedestal", this, "update_right_tab()");    
      m_right_tabs.push_back(new CGuiTabPedestalInfo(this, m_config, m_eb, m_gui_right_tab, "Info"));
      
      //tabs - finish up
      m_gui_right_tab->SetTab(0);
      m_gui_right_tab->Resize(m_tab_frame->GetDefaultSize());
      m_tab_frame->AddFrame(m_gui_right_tab, new TGLayoutHints( kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY));
      //end tabs

}

/**
 on user click - rename to handle_...?
 */
void CGuiTabPedestal::update_right_tab()
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
void CGuiTabPedestal::do_update(const CDetBase* detbase)   
{
   int ctab = m_gui_right_tab->GetCurrent();
   m_right_tabs[ctab]->update(detbase);
   m_last_detbase = detbase;
   
}

void CGuiTabPedestal::do_clear_tab()
{
   std::for_each(m_right_tabs.begin(), m_right_tabs.end(), boost::bind(&CGuiTab::clear_tab, _1));
}


