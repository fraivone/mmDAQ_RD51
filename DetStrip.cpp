//
//  DetStrip.cpp
//  browser
//
//  Created by Marcin Byszewski on 7.4.12.
// modified by Edoardo Farina on 24.08.2014
//  Copyright (c) 2012 CERN - PH/ADE. All rights reserved.
//

#include "DetStrip.h"
#include "SrsChannelId.h"
#include "GuiTab.h"
#include "GuiTabStrip.h"
#include "StripsTree.h"
#include "DetStripId.h"
#include "ShmemReader.h"

#include <TGListTree.h>

#include <boost/bind.hpp>

#include <iostream>
#include <sstream>
#include <iterator>


using namespace browser;

CDetStrip::CDetStrip(CDetDetector* det, 
                     CDetBase* parent, 
                     const std::string& name, 
                     size_t id_number, 
                     const TVector3& size, 
                     const TVector3& pos) :
CDetBase(det, parent, name, id_number, size, pos, TVector3()),
m_srs_channel_id(),
m_strip_id(),
m_raw_charge(),
m_charge(0),
m_time(0),
m_strip_tree_entry(),
m_timebin_width(1.0),
m_timebin(CShmemReader::n_tbins)
{
    m_new_data_to_draw=true;

}


CDetStrip::CDetStrip(CDetDetector* det,
                     const CSrsChannelId& channel_id,
                     const CDetStripId& strip_id,
                     const std::vector<double>& qvec,
                     double timebin_width_ns
          )
:CDetBase(det, 0,  CDetStripId::id_to_name(strip_id.strip_no()) , strip_id.strip_no(), TVector3(), TVector3(), TVector3()),
m_srs_channel_id(channel_id),
m_strip_id(strip_id),
m_raw_charge(qvec),
m_charge(0),
m_time(0),
m_strip_tree_entry(),
m_timebin_width(timebin_width_ns),
  m_timebin(1) //Offline events
{ 
    m_new_data_to_draw=true;

std::vector<double>::iterator maxel = std::max_element(m_raw_charge.begin(), m_raw_charge.end());
m_charge = *maxel;
m_time = std::distance(m_raw_charge.begin(), maxel);
m_time *= m_timebin_width; // APV25 timebin 
m_timebin = m_raw_charge.size(); //For the offline events
}





CDetStrip::CDetStrip(
                     CDetDetector* det,
                     const CSrsChannelId& channel_id,
                     const CDetStripId& strip_id,
                     double charge,
                     double time)
:CDetBase(det, 0, CDetStripId::id_to_name(strip_id.strip_no()), strip_id.strip_no(), TVector3(), TVector3(), TVector3()),
m_srs_channel_id(channel_id),
m_strip_id(strip_id),
m_raw_charge(),
m_charge(charge),
m_time(time),
m_strip_tree_entry(),
m_timebin_width(1.0),
m_timebin(CShmemReader::n_tbins) //For Online plots
{   
    m_new_data_to_draw=true;

}

CDetStrip::~CDetStrip()
{
   
}

int CDetStrip::get_timebin() const
{
  return m_timebin;
}

double CDetStrip::get_charge() const
{
   return m_charge;
}

double CDetStrip::get_time() const
{
   return m_time;
}


void CDetStrip::set_strip_tree_entry(boost::shared_ptr<CStripsTreeEntry> entry)
{
   m_strip_tree_entry = entry;   
}

const CStripsTreeEntry* CDetStrip::get_strip_tree_entry() const
{
   return m_strip_tree_entry.get();   
}


////////////////////////////////////////////////////////////////////////////////
// GUI related
////////////////////////////////////////////////////////////////////////////////


void CDetStrip::gui_populate_tree(TGListTree* tree, TGListTreeItem* parent_item) const
{
   CDetBase::gui_populate_tree(tree, parent_item);
   
//   std::stringstream ss;
//   ss << m_name << " (" << *std::max_element(m_raw_charge.begin(), m_raw_charge.end()) << ")";
//   
   TGListTreeItem * current_item = tree->FindChildByName(parent_item, m_name.c_str());
//   current_item->SetText(ss.str().c_str());

//   TGListTreeItem *current_item = tree->AddItem(parent_item, m_name.c_str());
//   current_item->SetText(ss.str().c_str());
//   current_item->SetUserData( static_cast<void*>(  (CDetBase*)this )  );
   current_item->SetOpen(false);
//   std::for_each(m_parts.begin(), m_parts.end(), boost::bind(&CDetBase::gui_populate_tree, _1 , tree, current_item));
}

int CDetStrip::do_update_my_gui_tab(std::vector<CGuiTab*>& tabs) const
{
   int ii = 0;
   for (std::vector<CGuiTab*>::iterator it = tabs.begin(); it != tabs.end(); ++it, ++ii) {
      if (dynamic_cast<CGuiTabStrip*>(*it)) {
         (*it)->update(this);
         return ii;
      }
   }
   return -1;
}





