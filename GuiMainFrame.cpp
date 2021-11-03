//
//  GuiMainFrame.cpp
//  browser
//
//  Created by Marcin Byszewski on 4/5/12.
//  Copyright (c) 2012 CERN - PH/ADE. All rights reserved.
//

#include "GuiMainFrame.h"
//#include "EventBrowser.h"
//#include "Configuration.h"

//#include "DetBase.h"
//


#include <TApplication.h>

//
//
//

#include <iostream>

using namespace browser;

void CGuiMainFrame::CloseWindow()
{
   gApplication->Terminate(0);
}


