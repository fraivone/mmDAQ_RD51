//
//  ParserMmdaq3ApvRawTree.cpp
//  eventbrowser
//
//  Created by Marcin Byszewski on 01/08/2012.
//  Copyright (c) 2012 CERN - PH/ADE. All rights reserved.
//

#include "ParserMmdaq3ApvRawTree.h"
#include "Mmdaq3TreeApvRaw.h"
#include "DetDetector.h"
#include "GuiMainFrame.h"
#include "SrsChannelId.h"
#include "DetChamber.h"
#include "DetMultiLayer.h"
#include "DetPlane.h"
#include "DetReadout.h"
#include "DetStrip.h"
#include "DetStripId.h"

#include "MBtools.h"


#include <TVector3.h>


#include <boost/bind.hpp>
#include <boost/foreach.hpp>

#include <iostream>
#include <stdexcept>

using namespace browser;

CParserMmdaq3ApvRawTree::CParserMmdaq3ApvRawTree(CMmdaq3TreeApvRaw* rawtree, CDetDetector* det, CGuiMainFrame* gui) :
m_rawtree(rawtree), m_env(0), m_detector(det), m_gui(gui)
{
   if (m_rawtree == 0) {
      throw std::runtime_error("CParserMmdaq3ApvRawTree: NULL raw tree");
   }
   
   //try?
   if (!m_detector) {
     throw std::runtime_error("CParserMmdaq3ApvRawTree: NULL detector pointer");
   }
   
   n_entries = m_rawtree->GetEntries();
  std::cout << "CParserMmdaq3ApvRawTree n_entries=" << n_entries << std::endl;
   for (size_t ii = 0; ii < n_entries; ++ii) {
      m_rawtree->GetEntry(ii);
      parse_entry(m_detector);
      
      progress(double(ii)/double(n_entries));
   }
//   m_detector->print();
   
}



CParserMmdaq3ApvRawTree::~CParserMmdaq3ApvRawTree()
{
   
}





void CParserMmdaq3ApvRawTree::parse_entry(CDetDetector* detector)
{
   
//   std::map<CSrsChipId, std::vector<DetStripPtr> > xtalk_map; // list of strips in the chipId
   
   size_t n_strips = m_rawtree->size();
   
   
//   std::cout <<  "CParserMmdaq3ApvRawTree::parse_entry n_strips= " << n_strips << std::endl;
   
   std::vector<CDetReadoutBase*> allreadouts;
   std::stringstream ssname;
   for (size_t ii = 0; ii < n_strips; ++ii) {
      
      unsigned fecNo  = (*(m_rawtree->m_fec))[ii];
      unsigned chipNo = (*(m_rawtree->m_chip))[ii];
      unsigned chNo   = (*(m_rawtree->m_channel))[ii];
      
      CSrsChannelId channelid(fecNo, chipNo, chNo);
      detector->add_srs_chip(channelid.chip_id());
      //      std::cout << "parsing chip id = "<< channelid.chip_id().chip_id() << std::endl;
      std::string chname   = (*(m_rawtree->m_chamber))[ii];
      unsigned mlname    = 0;  //(*(m_rawtree->m_layer))[ii];
      unsigned planename    = (*(m_rawtree->m_layer))[ii];
      char readoutname  = (*(m_rawtree->m_readout))[ii];
      unsigned strip_number = (*(m_rawtree->m_strip))[ii];
      
      //chamber
      CDetChamber* chamber = 0;
      if ( (chamber =  dynamic_cast<CDetChamber*>( detector->get_part(chname)) ) == 0) {
         chamber = new CDetChamber(detector, detector, chname, 0, TVector3(), TVector3(), TVector3());
         detector->add_part(chamber);
      }
      
      //multilayer
      ssname.str("");
      ssname << mlname;
      CDetMultiLayer* multilayer = 0;
      if ( (multilayer =  dynamic_cast<CDetMultiLayer*>( chamber->get_part(ssname.str())) ) == 0) {
         std::cout << "CParserMmdaq3ApvRawTree: mlayer not found: id=" << ssname.str() << std::endl;

         multilayer = new CDetMultiLayer(detector, chamber, ssname.str(), mlname, TVector3(), TVector3(), TVector3());
         chamber->add_part(multilayer);
      }
      
      //plane
      ssname.str("");
      ssname << planename;
      CDetPlane* plane = 0;
      if ( (plane =  dynamic_cast<CDetPlane*>( multilayer->get_part(ssname.str())) ) == 0) {
         std::cout << "CParserMmdaq3ApvRawTree: plane not found: id=" << ssname.str() << std::endl;

         plane = new CDetPlane(detector, multilayer, ssname.str(), planename, TVector3(), TVector3(), TVector3());
         multilayer->add_part(plane);
      }
      
      //readout
      ssname.str("");
      ssname << readoutname;
     std::string readout_name = ssname.str();

      CDetReadoutBase* readout = dynamic_cast<CDetReadoutBase*>( plane->get_part(ssname.str())) ;
     
//      CDetBase* base = plane->get_part_by_id(readoutname);
      
      readout = dynamic_cast<CDetReadoutBase*>( plane->get_part(ssname.str())) ;
      
      if ( readout == 0) {
         std::cout << "CParserMmdaq3ApvRawTree: readout not found: id=" << readoutname << std::endl;
         BOOST_FOREACH(DetBasePtr b, plane->get_parts()) {
            std::cout << " plane child: " << b->get_number() << ":" << b->get_name() << std::endl;
         }

         //this readout could be a H or V readout (class siblings)
         readout = new CDetReadout(detector, plane, ssname.str(), readoutname, 
                                   TVector3(), TVector3(), TVector3(), 0.0, 1.0,
                                   std::pair<long, long>(0,0));
         plane->add_part(readout);
      }
      
      allreadouts.push_back(readout);
      
      //      double max_time = 
      //      double max_charge = 
      
      //      readout->add_strip_number(strip_number /*max_time, max_charge*/);
      
      std::stringstream mlayer_id_name;
      mlayer_id_name << multilayer->get_number();
      std::stringstream plane_id_name;
      plane_id_name << plane->get_number();
      

      std::vector<double> qvec((*(m_rawtree->m_charge))[ii].begin(), (*(m_rawtree->m_charge))[ii].end());
      CDetStripId stripid(chname, mlname, planename, readoutname, strip_number);
      DetStripPtr strip_ptr = DetStripPtr(new CDetStrip(detector,
                                                        channelid,
                                                        stripid,
                                                        qvec,
                                                        25.0));
      
      readout->add_strip_to_stats(strip_ptr.get());
      readout->add_strip_to_event(strip_ptr.get());
//      xtalk_map[channelid.chip_id()].push_back(strip_ptr);
      if (ii==0) {
         readout->add_entry_to_stats(m_rawtree->m_time_s, m_rawtree->m_time_us);
      }
     
   }//for strips
   
   //xtalk in detector
   bool is_tmm2_x = false;
   bool is_tmm2_y = false;
   bool is_tmm5_x = false;
   bool is_tmm5_y = false;
//   detector->add_xtalk_map(xtalk_map);
   MakeElementsUnique(allreadouts);
   for (auto const& rd : allreadouts) {
       std::string name = rd->get_full_name();
       
       if (name == "H4 Tower-Tmm2-0-0-Y") is_tmm2_y = true;
       if (name == "H4 Tower-Tmm2-0-0-X") is_tmm2_x = true;
       if (name == "H4 Tower-Tmm5-0-0-Y") is_tmm5_y = true;
       if (name == "H4 Tower-Tmm5-0-0-X") is_tmm5_x = true;
       rd->add_RecHit_to_beamProfile();
}
   for (auto const& rd : allreadouts) {
      if(rd->get_full_name().find("H4 Tower-Tmm2-0-0")!=std::string::npos){
      if (is_tmm2_y==false or is_tmm2_x==false) rd->remove_last_RecHit_from_beamProfile();}

      if(rd->get_full_name().find("H4 Tower-Tmm5-0-0")!=std::string::npos){
      if (is_tmm5_y==false or is_tmm5_x==false) rd->remove_last_RecHit_from_beamProfile();}
}  
   
   std::for_each(allreadouts.begin(), allreadouts.end(), boost::bind(&CDetReadoutBase::reset_event_stats, _1));

}

void CParserMmdaq3ApvRawTree::progress(double val)
{
   m_gui->update_progress(val);
}



