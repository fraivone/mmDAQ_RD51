//
//  ParserMmdaq3BnlMiniRawTree.cpp
//  eventbrowser
//
//  Created by Marcin Byszewski on 07/08/2012.
//  Copyright (c) 2012 CERN - PH/ADE. All rights reserved.
//

#include "ParserMmdaq3BnlMiniRawTree.h"
#include "TreeRawBnlMini.h"
#include "DetDetector.h"
#include "GuiMainFrame.h"
#include "SrsChannelId.h"
#include "DetChamber.h"
#include "DetMultiLayer.h"
#include "DetPlane.h"
#include "DetReadout.h"
#include "DetStrip.h"
#include "DetStripId.h"
#include "VMM1Parameters.h"
#include "Configuration.h"

#include "MBtools.h"


#include <TVector3.h>


#include <boost/bind.hpp>
#include <boost/foreach.hpp>

#include <iostream>
#include <stdexcept>

#include <iostream>

using namespace browser;


CParserMmdaq3BnlMiniRawTree::CParserMmdaq3BnlMiniRawTree(CTreeRawBnlMini* rawtree, CConfiguration* config,
                                                         CDetDetector* det, CGuiMainFrame* gui)
: m_config(config), m_rawtree(rawtree), m_env(0), m_detector(det), m_gui(gui), n_entries(0)
{
   if (m_rawtree == 0) {
      throw std::runtime_error("CParserMmdaq3BnlMiniRawTree: NULL raw tree");
   }
   
   //try?
   if (!m_detector) {
      throw std::runtime_error("CParserMmdaq3BnlMiniRawTree: NULL detector pointer");
   }
   
   n_entries = m_rawtree->GetEntries();
   for (size_t ii = 0; ii < n_entries; ++ii) {
      m_rawtree->GetEntry(ii);
      parse_entry(m_detector);
      progress(double(ii)/double(n_entries));
   }
//   m_detector->print();
}

CParserMmdaq3BnlMiniRawTree::~CParserMmdaq3BnlMiniRawTree()
{
   
}




void CParserMmdaq3BnlMiniRawTree::parse_entry(CDetDetector* detector)
{
   typedef std::map<CSrsChannelId, CDetStripId> StripMapType;
   StripMapType strip_map;
   
//   std::map<CSrsChipId, std::vector<DetStripPtr> > xtalk_map; // list of strips in the chipId
   
   size_t n_strips = m_rawtree->size();
   
   
//   std::cout <<  "CParserMmdaq3BnlMiniRawTree::parse_entry n_strips= " << n_strips << std::endl;
   
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
      unsigned mlname    = 0; //(*(m_rawtree->m_layer))[ii];
      unsigned planename    = (*(m_rawtree->m_layer))[ii];
      char readoutname  = (*(m_rawtree->m_readout))[ii];
      unsigned strip_number = (*(m_rawtree->m_strip))[ii];

      ssname.str("");
      ssname << readoutname;


      CDetStripId stripid(chname, mlname, planename, readoutname, strip_number);
      strip_map[channelid] = stripid;
      
      
      //chamber
      CDetChamber* chamber = 0;
      if ( (chamber =  dynamic_cast<CDetChamber*>( detector->get_part(chname)) ) == 0) {
         std::cout << "CParserMmdaq3BnlMiniRawTree: chamber not found: " << chname << std::endl;
         chamber = new CDetChamber(detector, detector, chname, 0, TVector3(), TVector3(), TVector3());
         detector->add_part(chamber);
      }
      
      //multilayer
      ssname.str("");
      ssname << mlname;
      CDetMultiLayer* multilayer = 0;
      if ( (multilayer =  dynamic_cast<CDetMultiLayer*>( chamber->get_part(ssname.str())) ) == 0) {
         std::cout << "CParserMmdaq3BnlMiniRawTree: mlay not found: " << ssname.str() << std::endl;
         multilayer = new CDetMultiLayer(detector, chamber, ssname.str(), mlname, TVector3(), TVector3(), TVector3());
         chamber->add_part(multilayer);
      }
      
      
      //plane
      ssname.str("");
      ssname << planename;
      CDetPlane* plane = 0;
      if ( (plane =  dynamic_cast<CDetPlane*>( multilayer->get_part(ssname.str())) ) == 0) {
         std::cout << "CParserMmdaq3BnlMiniRawTree: plane not found: " << ssname.str() << std::endl;
         plane = new CDetPlane(detector, chamber, ssname.str(), planename, TVector3(), TVector3(), TVector3());
         multilayer->add_part(plane);
      }
      
      
      //readout
      ssname.str("");
      ssname << readoutname;
      CDetReadoutBase* readout = dynamic_cast<CDetReadoutBase*>( plane->get_part(ssname.str())) ;
//      readout = dynamic_cast<CDetReadoutBase*>( plane->get_part_by_id(readoutname)) ;
      
      
      if ( readout == 0) {
         std::cout << "CParserMmdaq3BnlMiniRawTree: reader not found: id=" << readoutname << std::endl;
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
      
      double charge = (*(m_rawtree->m_charge))[ii];
      double time = (*(m_rawtree->m_time))[ii];
      

      VMM1Parameters pars;
      if ( m_config->get_vmm1_parameters(channelid, pars)) {
         //translate tdo, pdo to volts and apply offset correction

         if ( std::fabs(pars.time_slope )< 0.01 ){
            std::cout << "bad time: " << channelid.to_string() << "  pars.time_slope = "<< pars.time_slope  <<"\n";
         }
         time -= pars.time_offset;
         time /= pars.time_slope;
         time = pars.tac - time;
         
         if (time > 1000.0) {
            std::cout << "bad time: " << channelid.to_string() << "\n"
            << " raw_t="<<(*(m_rawtree->m_charge))[ii] << "\n"
            << " tac_p0=" << pars.time_offset << "\n"
            << " tac_p1=" << pars.time_slope << "\n"
            << " TAC   =" << pars.tac << "\n"
            << " t = " << time 
            << std::endl;
         }
         
         charge -= pars.charge_offset;
         charge *= VMM1Parameters::VMM1PdoVoltsCoef;
      }
      

      std::stringstream mlayer_id_name;
      mlayer_id_name << multilayer->get_number();
      std::stringstream plane_id_name;
      plane_id_name << plane->get_number();
      
      DetStripPtr strip_ptr = DetStripPtr(new CDetStrip(detector,
                                                        channelid,
                                                        stripid,
                                                        charge,
                                                        time));
      
      readout->add_strip_to_stats(strip_ptr.get());
//      xtalk_map[channelid.chip_id()].push_back(strip_ptr);
      if (ii==0) {
         readout->add_entry_to_stats(m_rawtree->m_time_s, m_rawtree->m_time_us);
      }
      //      ssname.str("");
      //      ssname << strip_number;
      //      CDetStrip* strip = 0;
      //      if ( (strip =  dynamic_cast<CDetStrip*>( readout->get_part(ssname.str())) ) == 0) {
      //         strip = new CDetStrip(&detector, readout, ssname.str(), strip_number, TVector3(), TVector3());
      //         readout->add_part(strip);
      //      }       
      
   }//for strips
   
   //xtalk in detector
//   detector->add_xtalk_map(xtalk_map);
   
   MakeElementsUnique(allreadouts);
   std::for_each(allreadouts.begin(), allreadouts.end(), boost::bind(&CDetReadoutBase::reset_event_stats, _1));
   
   m_config->add_mapped_strips(strip_map);
}

void CParserMmdaq3BnlMiniRawTree::progress(double val)
{
   m_gui->update_progress(val);
}



