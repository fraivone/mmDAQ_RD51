//
//  GuiTabDaqMonitor.cpp
//  eventbrowser
//
//  Created by Marcin Byszewski on 2/14/13.
//  Copyright (c) 2013 CERN - PH/ADE. All rights reserved.
//

#include "GuiTabDaqMonitor.h"

#include <TGTab.h>
#include <TGButtonGroup.h>
#include <TGButton.h>
#include <TString.h>
#include <TGLabel.h>


using namespace browser;


CGuiTabDaqMonitor::CGuiTabDaqMonitor(CGuiBase* gui_parent, CConfiguration* config,
                                     CEventBrowser* eb, TGTab* main_tabs,
                                     const std::string& name) :
CGuiTab(gui_parent, config, eb, main_tabs, name),
m_ip_label(0), m_frame_label(0)
{
   
   construct_tab();
   
   
   
}

CGuiTabDaqMonitor::~CGuiTabDaqMonitor()
{
   
}

void CGuiTabDaqMonitor::do_construct_tab()
{
   m_tab_frame->SetLayoutManager(new TGHorizontalLayout(m_tab_frame));
   
   {
      TGGroupFrame* group2frame = new TGGroupFrame(m_tab_frame, "Receivers Monitoring", kVerticalFrame);
      {
         
         TGGroupFrame* group3frame = new TGGroupFrame(group2frame, "", kHorizontalFrame);
         {
            m_ip_label  = new TGLabel(group3frame, "IP:");
            group3frame->AddFrame(m_ip_label, new TGLayoutHints(kLHintsLeft | kLHintsTop , 5, 25, 5, 5));
            
            m_frame_label  = new TGLabel(group3frame, "Frames:");
            group3frame->AddFrame(m_frame_label, new TGLayoutHints(kLHintsLeft | kLHintsTop , 5, 50, 5, 5));
            
         }
         group2frame->AddFrame(group3frame, new TGLayoutHints(kLHintsLeft, 2, 2, 2, 2));
         
         //         m_gui_qfitting_show = new TGCheckButton(group2frame, "Overlay fit results on strip data");
         //         group2frame->AddFrame(m_gui_qfitting_show , new TGLayoutHints(kLHintsLeft, 5, 5, 3, 4));
         //         m_gui_qfitting_show->Connect("Toggled(Bool_t)", "browser::CGuiTabMainOptions", this, "gui_handle_overlay_fitting(bool)");
         //         m_gui_clusters_show = new TGCheckButton(group2frame, "Overlay clusters on raw data");
         //         group2frame->AddFrame(m_gui_clusters_show , new TGLayoutHints(kLHintsLeft, 5, 5, 3, 4));
      }
      m_tab_frame->AddFrame(group2frame, new TGLayoutHints(kLHintsLeft, 2, 2, 2, 2));
   }
}


void CGuiTabDaqMonitor::do_update(const CDetBase* entry)
{
   m_ip_label->SetText("hfjgfh");
   m_frame_label->SetText("sdfsd");
}


