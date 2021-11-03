//
//  DetDetector.cpp
//  browser
//
//  Created by Marcin Byszewski on 12/16/11.
//  Copyright (c) 2011 CERN - PH/ADE. All rights reserved.
//

#include "DetTypes.h"
#include "DetDetector.h"
#include "DetChamber.h"
#include "DetPlane.h"
#include "DetReadout.h"
#include "DetStrip.h"
#include "Configuration.h"
#include "RootReader.h"
#include "GuiTabDetector.h"
#include "SrsChipId.h"

#include <TEnv.h>
#include <TVector3.h>

#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/thread.hpp>

#include <iostream>
#include <algorithm>
#include <sstream>
#include <cmath>

using namespace browser;

CDetDetector::CDetDetector(const CReaderBase* reader, const CConfiguration* config, const std::string& name, const TVector3& size, const TVector3& pos, const TVector3& rot) :
CDetBase(this, this, name, 0, size, pos, rot),
m_reader(reader), 
m_config(config),
m_srs_chips(),
m_xtalkmap_all(),
m_xtalkmap_32(),
m_xtalk_all(128, 128),
m_xtalk_32(128, 128),
m_event_raw_udp(),
m_mutex()
{
   boost::lock_guard<boost::mutex> lock(m_mutex);
   build();
     
}


CDetDetector::~CDetDetector()
{
   
}


void CDetDetector::build()
{
   clear();
   
}


TEnv* CDetDetector::make_tenv()
{
   TEnv* env = new TEnv();
   std::vector<CDetChamber*> chambers;
   std::for_each(m_parts.begin(), m_parts.end(), boost::bind(&CDetBase::extract_chambers, _1, boost::ref(chambers)) );
   
   std::stringstream ss;
   ss << "mmdaq.Chambers: ";
//   std::copy(chambers.begin(), chambers.end(), ); //stream to ss?
   
   
   for (std::vector<CDetChamber*>::const_iterator ich = chambers.begin(); ich != chambers.end(); ++ich) {
      std::vector<CDetPlane*> planes;
      (*ich)->extract_planes(planes);

      for (std::vector<CDetPlane*>::const_iterator ipl = planes.begin(); ipl != planes.end(); ++ipl) {
         std::vector<CDetReadoutBase*> readouts;
         (*ipl)->extract_readouts(readouts);

         
         //save to env
         
         
      }
   }
   return env;
}


void CDetDetector::load_event_data(DetBasePtrCont& source_strips, DetBasePtrCont& overlay_strips)
{
   
  // std::cout << "   ----- CDetDetector::load_event_data() -----\n";

   
   DetBasePtrCont strips(source_strips);
   std::for_each(strips.begin(), strips.end(), boost::bind(&CDetBase::set_detector, _1, this));
  
   DetBasePtrCont ostrips(overlay_strips);
   std::for_each(ostrips.begin(), ostrips.end(), boost::bind(&CDetBase::set_detector, _1, this));
   CDetBase::load_event_data(strips, ostrips);
}


void CDetDetector::load_pedestal_data(DetBasePtrCont& pedestal_strips)
{  
   //   std::cout << "CDetDetector::load_pedestal_data into " << m_name << " (from sz=" << pedestal_strips.size() << ")\n";
   DetBasePtrCont strips(pedestal_strips);
   std::for_each(strips.begin(), strips.end(), boost::bind(&CDetBase::set_detector, _1, this));
   CDetBase::load_pedestal_data(strips);
}


int CDetDetector::do_update_my_gui_tab(std::vector<CGuiTab*>& tabs ) const
{
   int ii = 0;
   for (std::vector<CGuiTab*>::iterator it = tabs.begin(); it != tabs.end(); ++it, ++ii) {
      if (dynamic_cast<CGuiTabDetector*>(*it)) {
         (*it)->update(this);
         return ii;
      }
   }
   return -1;
}


void CDetDetector::add_srs_chip(const CSrsChipId& cid)
{
   if (!std::binary_search(m_srs_chips.begin(), m_srs_chips.end(), cid)) {
      m_srs_chips.push_back(cid);
      std::sort(m_srs_chips.begin(), m_srs_chips.end());
      m_xtalkmap_all[cid.chip_id()] = xtalk_matrix_type(128,128);
    // std::cout << "added chip " << cid.chip_id() << std::endl;
    //  std::cout <<   m_xtalkmap_all[cid.chip_id()] << std::endl;
   }
   
}

void CDetDetector::add_xtalk_map(const std::map<CSrsChipId, DetStripPtrVec >& xtalk_map)
{
   std::map<uint32_t, int> counter;
   typedef std::pair<CSrsChipId, DetStripPtrVec > chip_strips_type;
   //loop chips
   BOOST_FOREACH(chip_strips_type p, xtalk_map) {
//      std::cout << "xtalk map adding "<< p.first.chip_id()  << std::endl;
      DetStripPtrVec& strips = p.second;
      DetStripPtrVec::const_iterator maxqstrip = std::max_element(strips.begin(), 
                                                              strips.end(), 
                                                              CDetStrip::LessChargePtr());

      DetStripPtr ghostr;
      
      BOOST_FOREACH(DetStripPtr str, strips) {
         
         if (std::fabs((int)str->get_channel_id().channel_no() - (int)(*maxqstrip)->get_channel_id().channel_no() ) < 15 ) {
            continue;
         }
         
         if (!ghostr) {
            ghostr = str;
            continue;
         }
         
         if (ghostr->get_charge() < str->get_charge()) {
            ghostr = str;
         }
         
//         xtalk_matrix_type& apvmat = m_xtalkmap_all[p.first.chip_id()];
//         apvmat((*maxqstrip)->get_channel_id().channel_no() ,str->get_channel_id().channel_no() ) += str->get_charge()/(*maxqstrip)->get_charge();
      }
      
      if(ghostr) {
         xtalk_matrix_type& apvmat = m_xtalkmap_all[p.first.chip_id()];
         uint32_t chan = (*maxqstrip)->get_channel_id().channel_id();
         counter[chan] += 1;
         xtalk_matrix_type::reference val = apvmat( (*maxqstrip)->get_channel_id().channel_no() ,ghostr->get_channel_id().channel_no() );
         val += ghostr->get_charge()/(*maxqstrip)->get_charge();
//         val = (val*counter[chan] + (ghostr->get_charge()/(*maxqstrip)->get_charge()) ) / (counter[chan] + 1);
      }
     
//      std::cout <<  m_xtalkmap_all[p.first.chip_id()] << std::endl;
   }
   
   
}

const std::map<int, CDetDetector::xtalk_matrix_type>& CDetDetector::get_xtalk_map_all() const
{
   return m_xtalkmap_all;  
}


void CDetDetector::set_event_raw_udp_data(RawUdpMap const& event_raw_data)
{
   m_event_raw_udp = event_raw_data;
}

CDetDetector::RawUdpMap const& CDetDetector::get_event_raw_udp_data() const
{
   return m_event_raw_udp;
}


void CDetDetector::lock()
{
//   std::cout << "CDetDetector::lock()\n";
   m_mutex.lock();
}

void CDetDetector::unlock()
{
//   std::cout << "CDetDetector::unlock()\n";
   m_mutex.unlock();
}

