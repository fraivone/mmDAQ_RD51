//
//  GuiTab.cpp
//  browser
//
//  Created by Marcin Byszewski on 7.4.12.
//  Copyright (c) 2012 CERN - PH/ADE. All rights reserved.
//

#include "GuiTab.h"
#include "EventBrowser.h"
#include "DetBase.h"


#include <TGTab.h>
#include <TText.h>
#include <TPaveText.h>

#include <boost/date_time/posix_time/posix_time.hpp>

#include <iostream>
#include <sstream>

using namespace browser;

CGuiTab::CGuiTab(CGuiBase* gui_parent, 
                 CConfiguration* config, 
                 CEventBrowser* eb, 
                 TGTab* main_tabs, 
                 const std::string& name) :
CGuiBase(gui_parent, config),
m_eb(eb),
m_parent_tabs(main_tabs), 
m_name(name),
m_tab_frame(m_parent_tabs->AddTab(m_name.c_str()) ),
m_footer_text(new TText(0.0, 0.0, "footer text")),
m_info(new TPaveText(0.1, 0.8, 0.3, 0.9, "NDC")),
m_gui_right_tab(0),
m_right_tabs()
{
//   m_tab_frame = m_parent_tabs->AddTab(m_name.c_str());
   m_tab_frame->SetLayoutManager(new TGVerticalLayout(m_tab_frame)); 
   
//   m_footer_text = new TText(0.0, 0.0, "footer text");
   m_footer_text->SetTextSize(0.015);
   m_footer_text->SetNDC(kTRUE);
   
//   m_info = new TPaveText(0.1, 0.8, 0.3, 0.9, "NDC");
   m_info->AddText("This is CGuiBase");
   m_info->SetTextSize(0.02);
   
}


CGuiTab::~CGuiTab()
{
   delete m_footer_text;
   delete m_info;
}


void CGuiTab::draw_footer()
{
   //boost::posix_time::ptime time_stamp(boost::posix_time::second_clock::local_time() );
   
   //TODO: replace with event time stamp
   std::stringstream ss;
   ss.str("");
   ss << m_config->get_current_file_name() 
   << " (# " << m_eb->get_current_event_number() << ") "
//   "printed " << boost::posix_time::to_simple_string(time_stamp) << " " 
   << " ";
   
   
   m_footer_text->Clear();
   m_footer_text->SetNDC(kTRUE);
   m_footer_text->DrawText(0.1, 0.04, ss.str().c_str());
}

void CGuiTab::draw_info(const CDetBase* base)
{
   std::stringstream ss;
   m_info->Clear();
   m_info->AddText(m_config->get_current_file_name().c_str());
   ss.str("");
   ss <<  "# " << m_eb->get_current_event_number();
   m_info->AddText(ss.str().c_str()) ;
   std::string fullname(base->get_full_name());
   size_t dash = fullname.find_first_of("-");
   if (dash != std::string::npos) {
      fullname = fullname.erase(0, dash+1 );
   }
   
   m_info->AddText(fullname.c_str());   
}



