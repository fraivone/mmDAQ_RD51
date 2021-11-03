//
//  DetPlane.cpp
//  browser
//
//  Created by Marcin Byszewski on 12/18/11.
//  Copyright (c) 2011 CERN - PH/ADE. All rights reserved.
//

#include "DetPlane.h"
#include "DetStrip.h"
#include "GuiTabPlane.h"

#include <boost/bind.hpp>
#include <iostream>

using namespace browser;

CDetPlane::CDetPlane(CDetDetector* det, CDetBase* parent, const std::string& name, size_t id_number, const TVector3& size, const TVector3& pos, const TVector3& rot) :
CDetBase(det, parent, name, id_number, size, pos, rot)
{
   
}


CDetPlane::~CDetPlane()
{
   
}

void CDetPlane::extract_planes(std::vector<CDetPlane*>& vec)
{
   if (std::find(vec.begin(), vec.end(), this) == vec.end()) {
      vec.push_back(this);
   }
   std::for_each(m_parts.begin(), m_parts.end(), boost::bind(&CDetBase::extract_planes, _1 , boost::ref(vec)));
}


/**
 within gui tabs search for plane tab and update
 */
int CDetPlane::do_update_my_gui_tab(std::vector<CGuiTab*>& tabs) const
{
   int ii = 0;
   for (std::vector<CGuiTab*>::iterator it = tabs.begin(); it != tabs.end(); ++it, ++ii) {
      if (dynamic_cast<CGuiTabPlane*>(*it)) {
         (*it)->update(this);
         return ii;
      }
   }
   return -1;
}

bool CDetPlane::is_my_strip(const CDetStrip* strip) const
{
   return strip->get_strip_id().layer_id() == m_idnumber;
}


//bool CDetPlane::strip_has_my_name(DetBasePtr strip) const
//{
//   const CDetStrip* s = dynamic_cast<const CDetStrip*>(strip.get());
//   return (s->get_plane_name() == m_name);
//}
//
//bool CDetPlane::strip_has_my_id(DetBasePtr strip) const
//{
//   const CDetStrip* s = dynamic_cast<const CDetStrip*>(strip.get());
//   return (s->get_plane_id() == m_idnumber);
//}


