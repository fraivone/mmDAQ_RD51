//
//  GuiBase.cpp
//  browser
//
//  Created by Marcin Byszewski on 4/5/12.
//  Copyright (c) 2012 CERN - PH/ADE. All rights reserved.
//

#include "GuiBase.h"

#include <iostream>

using namespace browser;

CGuiBase::CGuiBase(CGuiBase* gui_parent, CConfiguration* config)
: m_id(), m_gui_parent(gui_parent), m_config(config)
{
   
}


CGuiBase::~CGuiBase()
{
   
}

