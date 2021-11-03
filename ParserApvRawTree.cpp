//
//  ParserApvRawTree.cpp
//  browser
//
//  Created by Marcin Byszewski on 12/17/11.
//  Copyright (c) 2011 CERN - PH/ADE. All rights reserved.
//

#include "ParserApvRawTree.h"
#include "CApvRawTree.h"
#include "DetDetector.h"
#include "DetChamber.h"
#include "DetPlane.h"
#include "DetReadout.h"
#include "DetStrip.h"
#include "DetStripId.h"
#include "GuiMainFrame.h"

#include "MBtools.h"


#include <TEnv.h>
#include <TVector3.h>

#include <boost/bind.hpp>

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <map>

using namespace browser;

CParserApvRawTree::CParserApvRawTree(CApvRawTree* rawtree, CDetDetector* det, CGuiMainFrame* gui) :
m_rawtree(rawtree), m_env(0), m_detector(det), m_gui(gui)
{
   if (m_rawtree == 0) {
      throw std::runtime_error("CParserApvRawTree: NULL raw tree");
   }
    
//try?
   if (!m_detector) {
      throw std::runtime_error("CParserApvRawTree: NULL detector pointer");
   }
   
   n_entries = m_rawtree->GetEntries();
   std::cout << "CParserApvRawTree n_entries=" << n_entries << std::endl;
   for (size_t ii = 0; ii < n_entries; ++ii) {
      m_rawtree->GetEntry(ii);
      parse_entry(m_detector);
      progress(double(ii)/double(n_entries));
   }
//   m_detector->print();
   //make a Tenv out of the detector
//   TEnv* det_config = m_detector->make_tenv();

   
}


CParserApvRawTree::~CParserApvRawTree()
{
   delete m_env; m_env = 0;
}


void CParserApvRawTree::parse_entry(CDetDetector* detector)
{

   std::map<CSrsChipId, std::vector<DetStripPtr> > xtalk_map; // list of strips in the chipId
   
   size_t n_strips = m_rawtree->size();
   

//   std::cout <<  "CParserApvRawTree::parse_entry n_strips= " << n_strips << std::endl;

   std::vector<CDetReadoutBase*> allreadouts;
   std::stringstream ssname;
   for (size_t ii = 0; ii < n_strips; ++ii) {

      unsigned fecNo  = (*(m_rawtree->apv_fecNo))[ii];
      unsigned chipNo = (*(m_rawtree->apv_id))[ii];
      unsigned chNo   = (*(m_rawtree->apv_ch))[ii];
      
      CSrsChannelId channelid(fecNo, chipNo, chNo);
      detector->add_srs_chip(channelid.chip_id());
//      std::cout << "parsing chip id = "<< channelid.chip_id().chip_id() << std::endl;
      std::string chname   = (*(m_rawtree->mm_id))[ii];
      
      unsigned mlayername   = (unsigned)0;
      unsigned planename   = m_rawtree->has_planes() ? (*(m_rawtree->mm_plane))[ii] : (unsigned)0;
      unsigned readoutname = m_rawtree->has_readouts() ? (*(m_rawtree->mm_readout))[ii] : (unsigned)0;
      
      unsigned strip_number = (*(m_rawtree->mm_strip))[ii];
      
      CDetChamber* chamber = 0;
      if ( (chamber =  dynamic_cast<CDetChamber*>( detector->get_part(chname)) ) == 0) {
         chamber = new CDetChamber(detector, detector, chname, 0, TVector3(), TVector3(), TVector3());
         detector->add_part(chamber);
      }
      
      ;
      ssname.str("");
      ssname << planename;
      CDetPlane* plane = 0;
      if ( (plane =  dynamic_cast<CDetPlane*>( chamber->get_part(ssname.str())) ) == 0) {
         plane = new CDetPlane(detector, chamber, ssname.str(), planename, TVector3(), TVector3(), TVector3());
         chamber->add_part(plane);
      }
      
      ssname.str("");
      ssname << readoutname;
      


      CDetReadoutBase* readout = dynamic_cast<CDetReadoutBase*>( plane->get_part(ssname.str())) ;
      readout = dynamic_cast<CDetReadoutBase*>( plane->get_part_by_id(readoutname)) ;
      if ( readout == 0) {
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

      CDetStripId stripid(chname, mlayername, planename, readoutname, strip_number);

      std::vector<double> qvec((*(m_rawtree->apv_q))[ii].begin(), (*(m_rawtree->apv_q))[ii].end());
      DetStripPtr strip_ptr = DetStripPtr(new CDetStrip(detector,
                                                        channelid,
                                                        stripid,
                                                        qvec,
                                                        25.0));
      
      readout->add_strip_to_stats(strip_ptr.get());
      xtalk_map[channelid.chip_id()].push_back(strip_ptr);
      if (ii==0) {
         readout->add_entry_to_stats(m_rawtree->time_s, m_rawtree->time_us);
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
   detector->add_xtalk_map(xtalk_map);
   
   MakeElementsUnique(allreadouts);
   std::for_each(allreadouts.begin(), allreadouts.end(), boost::bind(&CDetReadoutBase::reset_event_stats, _1));
}

void CParserApvRawTree::progress(double val)
{
   m_gui->update_progress(val);
}


