//
//  DetMultiLayer.cpp
//  mmdaq3-server
//
//  Created by Marcin Byszewski on 06/07/2012.
//  Copyright (c) 2012 CERN - PH/ADE. All rights reserved.
//

#include "DetMultiLayer.h"
#include "DetStrip.h"
#include "GuiTabMultiLayer.h"

#include <boost/bind.hpp>


#include <iostream>

using namespace browser;


CDetMultiLayer::CDetMultiLayer(CDetDetector* det, CDetBase* parent, const std::string& name,
                               size_t id_number, const TVector3& size, const TVector3& pos, const TVector3& rot) :
CDetBase(det, parent, name, id_number, size, pos, rot)
{
//   std::cout << "CDetMultiLayer() new multilayer '" << name << "'" << std::endl;
}

CDetMultiLayer::~CDetMultiLayer()
{
   
}

void CDetMultiLayer::extract_multilayers(std::vector<CDetMultiLayer*>& vec)
{
   if (std::find(vec.begin(), vec.end(), this) == vec.end()) {
      vec.push_back(this);
   }
   std::for_each(m_parts.begin(), m_parts.end(), boost::bind(&CDetBase::extract_multilayers, _1 , boost::ref(vec)));
}



/**
 within gui tabs search for plane tab and update
 */
int CDetMultiLayer::do_update_my_gui_tab(std::vector<CGuiTab*>& tabs) const
{
   int ii = 0;
   for (std::vector<CGuiTab*>::iterator it = tabs.begin(); it != tabs.end(); ++it, ++ii) {
      if (dynamic_cast<CGuiTabMultiLayer*>(*it)) {
         (*it)->update(this);
         return ii;
      }
   }
   return -1;
}

bool CDetMultiLayer::is_my_strip(const CDetStrip* strip) const
{
   return strip->get_strip_id().multilayer_id() == m_idnumber;
}


//bool CDetMultiLayer::strip_has_my_name(DetBasePtr strip) const
//{
//   const CDetStrip* s = dynamic_cast<const CDetStrip*>(strip.get());
//   return (s->get_multilayer_name() == m_name);
//}
//
//bool CDetMultiLayer::strip_has_my_id(DetBasePtr strip) const
//{
//   const CDetStrip* s = dynamic_cast<const CDetStrip*>(strip.get());
//   return (s->get_multilayer_id() == m_idnumber);
//}

