//
//  GuiTabMultiLayer.cpp
//  eventbrowser
//
//  Created by Marcin Byszewski on 27/10/2012.
//  Copyright (c) 2012 CERN - PH/ADE. All rights reserved.
//

#include "GuiTabMultiLayer.h"
#include "EventBrowser.h"
#include "GuiMainFrame.h"
#include "GuiTabPlaneAll1D.h"

#include <TGTab.h>

#include <iostream>

using namespace browser;

CGuiTabMultiLayer::CGuiTabMultiLayer(CGuiBase* gui_parent,
                                     CConfiguration* config,
                                     CEventBrowser*   eb,
                                     TGTab* main_tabs,
                                     const std::string& name) :
CGuiTab(gui_parent, config, eb, main_tabs,name)
{
   construct_tab();
}


CGuiTabMultiLayer::~CGuiTabMultiLayer()
{
   
}

void CGuiTabMultiLayer::do_construct_tab()
{
   
}

void CGuiTabMultiLayer::do_update(const CDetBase* entry)
{
   
}

void CGuiTabMultiLayer::do_clear_tab()
{
   std::for_each(m_right_tabs.begin(), m_right_tabs.end(), boost::bind(&CGuiTab::clear_tab, _1));
}



