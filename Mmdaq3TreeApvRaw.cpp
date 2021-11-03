//
//  Mmdaq3TreeApvRaw.cpp
//  eventbrowser
//
//  Created by Marcin Byszewski on 01/08/2012.
//  Copyright (c) 2012 CERN - PH/ADE. All rights reserved.
//

#include "Mmdaq3TreeApvRaw.h"
#include "SrsChannelId.h"
#include "DetStripId.h"
#include "DetStrip.h"
#include "ParserMmdaq3ApvRawTree.h"


#include <TTree.h>
#include <TFile.h>
#include <TChain.h>


#include <iostream>
#include <stdexcept>
#include <sstream>


using namespace browser;



CMmdaq3TreeApvRaw::CMmdaq3TreeApvRaw(TFile *file, const std::string& treename) :
CTreeBase(treename), rootfile(0), m_has_multilayers(false),
m_evt(0),
m_evt_error(0),
m_time_s(0),
m_time_us(0),
m_srs_time_stamp(0),
m_srs_trigger(0),
m_fec(),
m_chip(),
m_channel(),
m_chamber(),
m_multilayer(),
m_layer(),
m_readout(),
m_strip(),
m_charge()
{
	if(file == 0)
		throw std::runtime_error("CMmdaq3TreeApvRaw - must have a pointer to TFile");  
	else{
		TTree* tree = (TTree*)file->Get(treename.c_str());
      if (!tree) {
         throw std::runtime_error("CApvRawTree - no raw tree named '" + treename +"'");  
      }
      
		Init(tree);
	}
}




CMmdaq3TreeApvRaw::~CMmdaq3TreeApvRaw()
{
   
}


//TTree* CMmdaq3TreeApvRaw::GetTree() 
//{
//   return fChain;
//}



void CMmdaq3TreeApvRaw::LoadEventData(DetBasePtrCont& strips, CDetDetector* det)
{

   
   size_t number_of_strip = size();
//   std::cout << "CMmdaq3TreeApvRaw::LoadEventData() number_of_strips =" << number_of_strip << std::endl;

   for (size_t ii = 0; ii < number_of_strip; ++ii) {
      unsigned fecNo   = (*(m_fec))[ii];
      unsigned chipNo  = (*(m_chip))[ii];
      unsigned chNo    = (*(m_channel))[ii];
      CSrsChannelId chId(fecNo, chipNo, chNo);
      
      std::string chamber_name = (*(m_chamber))[ii];

      std::stringstream ss;
      unsigned mlayer   = has_multilayers() ? (*(m_multilayer))[ii] : (unsigned)0;
      ss << mlayer;
      std::string multilayer_id_name(ss.str());
      
      ss.str("");
      unsigned plane = (*(m_layer))[ii];
      ss << plane;
      std::string plane_id_name(ss.str());
      
      ss.str("");
      char readout = (*(m_readout))[ii] ;
      ss << readout;
      std::string readout_name(ss.str());
      
      
      unsigned strip_number = (*(m_strip))[ii];
      
      
      
      std::vector<double> qvec((*(m_charge))[ii].begin(), (*(m_charge))[ii].end());
      ss.str("");
      ss << strip_number;
      
      CDetStripId stripid(chamber_name, mlayer, plane, readout, strip_number);
      DetStripPtr strptr(new CDetStrip(det,
                                       chId,
                                       stripid,
                                       qvec,
                                       25.0));
      
      if(strptr) {
//         strip->set_strip_tree_entry(get_striptree_entry(strip_number));
         strips.push_back(strptr);
      }
   } // for ii < number of strips

    m_current_event=m_evt;

    
}




void CMmdaq3TreeApvRaw::parse_tree(CDetDetector* det, CGuiMainFrame* gui) 
{
   try {
      CParserMmdaq3ApvRawTree parser(this, det, gui);
//      return parser.get_detector();
   } catch (std::runtime_error &e) {
      std::cout << "ERROR: CRootReader::parse_raw_tree() " << e.what() << std::endl;
   }
}




void CMmdaq3TreeApvRaw::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).
	
   // Set object pointer

   m_evt = 0;
   m_evt_error = 0;
   m_time_s = 0; 
   m_time_us = 0; 
   m_srs_time_stamp = 0;
   m_srs_trigger = 0;
   
   m_fec = 0;
   m_chip = 0;
   m_channel = 0;
   m_chamber = 0;
   m_layer = 0;  
   m_readout = 0;
   m_strip = 0;  
   m_charge = 0;
   
	//apv_tb = 0;
   
   // Set branch addresses and branch pointers
	if (!tree) {
		std::cerr << "Error: CApvRawTree::Init() no such tree" << std::endl;
      ferror = -1;
      throw std::runtime_error("CApvRawTree::Init() : no tree");
	}
   
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);
	
	ferror = 0;
   
   fChain->SetBranchAddress("evt",          &m_evt);
   fChain->SetBranchAddress("error",        &m_evt_error);
   fChain->SetBranchAddress("daqTimeSec",       &m_time_s);
   fChain->SetBranchAddress("daqTimeMicroSec",  &m_time_us);
   fChain->SetBranchAddress("srsTimeStamp", &m_srs_time_stamp);
   fChain->SetBranchAddress("srsTrigger", &m_srs_trigger);
   fChain->SetBranchAddress("srsFec",    &m_fec);
   fChain->SetBranchAddress("srsChip",   &m_chip);
   fChain->SetBranchAddress("srsChan",   &m_channel);
   fChain->SetBranchAddress("mmChamber", &m_chamber);
   if (fChain->GetBranch("mmMultilayer")) {
      ferror |= fChain->SetBranchAddress("mmMultilayer", &m_multilayer);
      m_has_multilayers = true;
   }

   fChain->SetBranchAddress("mmLayer",   &m_layer);
   fChain->SetBranchAddress("mmReadout", &m_readout);
   fChain->SetBranchAddress("mmStrip",   &m_strip);
   fChain->SetBranchAddress("raw_q",     &m_charge);
   
	if (ferror < 0 ) {
		std::cerr << "Error: CApvRawTree::Init() bad tree structure. Check if the file contains all colums." << std::endl;
		throw std::runtime_error("Bad raw tree structure.");
	}
	ferror = 0;
	
   //Notify();
}

