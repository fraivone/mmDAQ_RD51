//
//  DetChamber.cpp
//  browser
//
//  Created by Marcin Byszewski on 12/18/11.
//  Copyright (c) 2011 CERN - PH/ADE. All rights reserved.
//

#include "DetChamber.h"
#include "DetStrip.h"
#include "GuiTabChamber.h"
#include <boost/bind.hpp>

#include <iostream>

using namespace browser;

CDetChamber::CDetChamber(CDetDetector* det, CDetBase* parent, const std::string& name, size_t id_number, const TVector3& size, const TVector3& pos, const TVector3& rot) :
CDetBase(det, parent, name, id_number, size, pos, rot), m_config_file()
{
   
}


CDetChamber::~CDetChamber()
{
   
}

void CDetChamber::extract_chambers(std::vector<CDetChamber *>& vec)
{
   if (std::find(vec.begin(), vec.end(), this) == vec.end()) {
      vec.push_back( this);
   }
   std::for_each(m_parts.begin(), m_parts.end(), boost::bind(&CDetBase::extract_chambers, _1 , boost::ref(vec)));
}


int CDetChamber::do_update_my_gui_tab(std::vector<CGuiTab*>& tabs ) const
{
   int ii = 0;
   for (std::vector<CGuiTab*>::iterator it = tabs.begin(); it != tabs.end(); ++it, ++ii) {
      if (dynamic_cast<CGuiTabChamber*>(*it)) {
         (*it)->update(this);
         return ii;
      }
   }
   return -1;
}

void CDetChamber::set_config_file(const std::string& config_file)
{
   m_config_file = config_file;
}


bool CDetChamber::is_my_strip(const CDetStrip* strip) const
{
   return strip->get_strip_id().chamber_name() == m_name;
}

