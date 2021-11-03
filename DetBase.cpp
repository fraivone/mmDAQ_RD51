//
//  DetBase.cpp
//  browser
//
//  Created by Marcin Byszewski on 12/16/11.
//  Copyright (c) 2011 CERN - PH/ADE. All rights reserved.
//

#include "DetBase.h"
#include "GuiTab.h"
#include "DetStrip.h"


#include <TGListTree.h>

#include <boost/bind.hpp>

#include <iostream>

using namespace browser;

CDetBase::CDetBase(CDetDetector* det, CDetBase* parent, const std::string& name, size_t id_number, const TVector3& size, const TVector3& pos, const TVector3& rot) :
m_detector(det),
m_parent(parent),
m_parts(),
m_name(name),
m_idnumber(id_number),
m_size(size),
m_position(pos),
m_rotation(rot),
m_new_data_to_draw(false)
{
   
}


CDetBase::~CDetBase()
{
   clear();
}

void CDetBase::clear()
{
   std::for_each(m_parts.begin(), m_parts.end(), 
                 boost::bind(&CDetBase::clear, _1));
   m_parts.clear();
}

void CDetBase::print() const
{
   std::cout << "name=" << m_name << " m_idnumber=" << m_idnumber << std::endl;
   std::for_each(m_parts.begin(), m_parts.end(), 
                 boost::bind(&CDetBase::print, _1));
}

void CDetBase::add_part(CDetBase* part) 
{
   m_parts.push_back(boost::shared_ptr<CDetBase>(part));
}

void CDetBase::add_part(boost::shared_ptr<CDetBase> part)
{
   m_parts.push_back(part);
}

bool CDetBase::add_part_if_unique(CDetBase* part) 
{
   DetBasePtrCont::iterator it = std::find_if(m_parts.begin(), m_parts.end(), PartPtrCompare(part));
   if (it == m_parts.end()) {
      m_parts.push_back(boost::shared_ptr<CDetBase>(part));
      return true;
   }
   return false;
}

void CDetBase::set(size_t idnumber, const std::string& name, const TVector3& size, const TVector3& pos, const TVector3& rot)
{
   m_idnumber = idnumber;
   m_name = name;
   m_size = size;
   m_position = pos;
   m_rotation = rot;
}

CDetBase* CDetBase::get_part(const std::string& name)
{
   DetBasePtrCont::iterator it = std::find_if(m_parts.begin(), m_parts.end(), PartNameCompare(name));
   if (it != m_parts.end()) {
      return it->get();
   }
   return 0;
}


CDetBase* CDetBase::get_part_by_id(size_t idnum) 
{
   DetBasePtrCont::iterator it = std::find_if(m_parts.begin(), m_parts.end(), PartIdCompare(idnum));
   if (it != m_parts.end()) {
      return it->get();
   }
   return 0;
}

std::string CDetBase::get_full_name() const
{
   if (m_parent && m_parent != this) {
      return m_parent->get_full_name() + "-" + m_name;
   }
   return m_name;
}

void CDetBase::extract_chambers(std::vector<CDetChamber*> &vec)
{
   std::for_each(m_parts.begin(), m_parts.end(), 
                 boost::bind(&CDetBase::extract_chambers, _1 , boost::ref(vec)));
}

void CDetBase::extract_multilayers(std::vector<CDetMultiLayer*> &vec)
{
   std::for_each(m_parts.begin(), m_parts.end(),
                 boost::bind(&CDetBase::extract_multilayers, _1 , boost::ref(vec)));
}

void CDetBase::extract_planes(std::vector<CDetPlane*> &vec)
{
   std::for_each(m_parts.begin(), m_parts.end(), 
                 boost::bind(&CDetBase::extract_planes, _1 , boost::ref(vec)));
}

void CDetBase::extract_readouts(std::vector<CDetReadoutBase*> &vec)
{
   void  (CDetBase::*exrd) (std::vector<CDetReadoutBase*> &vec) = &CDetBase::extract_readouts;
   std::for_each(m_parts.begin(), m_parts.end(),
                 boost::bind ( exrd, _1 , boost::ref(vec)));
}

void CDetBase::extract_readouts(std::vector<const CDetReadoutBase*> &vec) const
{
   void  (CDetBase::*exrd) (std::vector<const CDetReadoutBase*> &vec) const = &CDetBase::extract_readouts;
   std::for_each(m_parts.begin(), m_parts.end(),
                 boost::bind ( exrd, _1 , boost::ref(vec)));
}


void CDetBase::load_event_data(DetBasePtrCont& parent_strips, DetBasePtrCont& overlay_strips)
{
   m_new_data_to_draw = true;
   DetBasePtrCont strips;
//   std::cout << "CDetBase::load_event_data into " << m_name << " (from sz=" << parent_strips.size() << ")\n";
   DetBasePtrCont::iterator firstbad = std::partition(parent_strips.begin(),
                                                      parent_strips.end(),
                                                      CDetBase::IsMyStrip(this) );
   strips.splice(strips.begin(), parent_strips, parent_strips.begin(), firstbad);
   
   DetBasePtrCont ostrips;
   DetBasePtrCont::iterator ofirstbad = std::partition(overlay_strips.begin(),
                                                       overlay_strips.end(),
                                                       CDetBase::IsMyStrip(this) );
   ostrips.splice(ostrips.begin(), overlay_strips, overlay_strips.begin(), ofirstbad);

   if (!strips.empty() || !ostrips.empty()) {
      std::for_each(m_parts.begin(), m_parts.end(), boost::bind(&CDetBase::load_event_data, _1 , boost::ref(strips), boost::ref(ostrips) ));
   }
   
   if (!strips.empty()) {
      std::cout << "++++ ERROR load_event_data unassigned strips " << strips.size()  << ": "<< strips.front() ->get_number() << " (in " << get_full_name() << ") ++++"<< std::endl;
   }
   
   if (!ostrips.empty()) {
      std::cout << "++++ ERROR load_event_data unassigned overlay strips " << ostrips.size()  << ": "<< ostrips.front()->get_number() << " (in " << get_full_name() << ") ++++"<< std::endl;
   }
}


void CDetBase::add_loaded_event_to_stats()
{
   std::for_each(m_parts.begin(), m_parts.end(),
                 boost::bind (&CDetBase::add_loaded_event_to_stats, _1));
}


void CDetBase::load_pedestal_data(DetBasePtrCont& parent_strips)
{
   //std::cout << "CDetBase::load_pedestal_data into " << m_name << " (from sz=" << parent_strips.size() << ")\n";
   DetBasePtrCont strips;
   DetBasePtrCont::iterator firstbad = std::partition(parent_strips.begin(),
                                                      parent_strips.end(),
                                                      CDetBase::IsMyStrip(this) );
   strips.splice(strips.begin(), parent_strips, parent_strips.begin(), firstbad);
   
   if (!strips.empty() ) {
      std::for_each(m_parts.begin(), m_parts.end(), boost::bind(&CDetBase::load_pedestal_data, _1 , boost::ref(strips) ));
   }
   
  // if (!strips.empty()) {
  //    std::cout << "++++ ERROR load_pedestal_data unassigned strips " << strips.size()  << ": "<< strips.front() ->get_number() << " (in " << get_full_name() << ") ++++"<< std::endl;
   //}
  
}



void CDetBase::gui_populate_tree(TGListTree* tree, TGListTreeItem* parent_item) const
{   
   if (!tree) {
      return;
   }
   
   TGListTreeItem * current_item = tree->FindChildByName(parent_item, m_name.c_str());
   
   if (!current_item) {
      current_item = tree->AddItem(parent_item, m_name.c_str());
      current_item->SetUserData( static_cast<void*>( const_cast<CDetBase*>(this)  ));
      current_item->SetOpen(true);
      current_item->SetCheckBox();
   }
   std::for_each(m_parts.begin(), m_parts.end(),
                 boost::bind(&CDetBase::gui_populate_tree, _1 , tree, current_item));
}


void CDetBase::clear_event_data()
{
   std::for_each(m_parts.begin(), m_parts.end(), 
                 boost::bind(&CDetBase::clear_event_data, _1));
}


bool CDetBase::IsMyStrip::operator() (const boost::shared_ptr<CDetBase> rhs) const
{
   const CDetStrip* strip = dynamic_cast<const CDetStrip*>(rhs.get());
   return m_detelement->is_my_strip(strip);
}

