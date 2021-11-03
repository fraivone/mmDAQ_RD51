//
//  GuiTabReadoutProps.cpp
//  browser
//
//  Created by Marcin Byszewski on 13.4.12.
//  Copyright (c) 2012 CERN - PH/ADE. All rights reserved.
//

#include "GuiTabReadoutProps.h"
#include "Configuration.h"


#include <TGButtonGroup.h>
#include <TGButton.h>
#include <TString.h>


#include <iostream>

using namespace browser;

CGuiTabReadoutProps::CGuiTabReadoutProps(CGuiBase* gui_parent, 
                                         CConfiguration* config, 
                                         CEventBrowser* eb, 
                                         TGTab* main_tabs, 
                                         const std::string& name) :
CGuiTab(gui_parent, config, eb, main_tabs,name)
{
   construct_tab();
}


CGuiTabReadoutProps::~CGuiTabReadoutProps()
{
   
}



void CGuiTabReadoutProps::do_construct_tab()
{
   
}

void CGuiTabReadoutProps::do_update(const CDetBase* )
{
   
}


