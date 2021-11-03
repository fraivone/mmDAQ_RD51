//
//  TreeRawBnlMini.cpp
//  eventbrowser
//
//  Created by Marcin Byszewski on 07/08/2012.
//  Copyright (c) 2012 CERN - PH/ADE. All rights reserved.
//

#include "TreeRawBnlMini.h"
#include "SrsChannelId.h"
#include "DetStrip.h"
#include "DetStripId.h"
#include "ParserMmdaq3BnlMiniRawTree.h"
#include "VMM1Parameters.h"
#include "Configuration.h"

#include <TTree.h>
#include <TFile.h>
#include <TChain.h>


#include <iostream>
#include <stdexcept>
#include <sstream>

using namespace browser;

CTreeRawBnlMini::CTreeRawBnlMini(CConfiguration *config, TFile *file, const std::string& treename):
CTreeBase(treename), m_config(config), rootfile(0)
{
   if(file == 0) 
		throw std::runtime_error("CTreeRawBnlMini - must have a pointer to TFile");  
	else{
		TTree* tree = (TTree*)file->Get(treename.c_str());
      if (!tree) {
         throw std::runtime_error("CTreeRawBnlMini - no raw tree named '" + treename +"'");  
      }
      
		Init(tree);
	}
}


CTreeRawBnlMini::~CTreeRawBnlMini()
{
   
}


void CTreeRawBnlMini::LoadEventData(DetBasePtrCont& strips, CDetDetector* det)
{
   size_t number_of_strip = size();
//   std::cout << "CTreeRawBnlMini::LoadEventData() number_of_strips =" << number_of_strip << std::endl;

   for (size_t ii = 0; ii < number_of_strip; ++ii) {
      unsigned fecNo   = (*(m_fec))[ii];
      unsigned chipNo  = (*(m_chip))[ii];
      unsigned chNo    = (*(m_channel))[ii];
      CSrsChannelId chId(fecNo, chipNo, chNo);
//      std::cout << "CTreeRawBnlMini::LoadEventData() CSrsChannelId =" << chId.chip_no() <<" " << chId.channel_no() << std::endl;

      
      std::string chamber_name = (*(m_chamber))[ii];
      
      unsigned multilayer = 0;
      std::string multilayer_name = "0";

      std::stringstream ss;
      
      unsigned plane = (*(m_layer))[ii];
      ss << plane;
      std::string plane_name(ss.str());
      
      ss.str("");
      char readout = (*(m_readout))[ii] ;
      ss << readout;
      std::string readout_name(ss.str());
      
      
      unsigned strip_number = (*(m_strip))[ii];
      
      
      double charge = (*(m_charge))[ii];
      double time = (*(m_time))[ii];
      

      VMM1Parameters pars;
      if ( m_config->get_vmm1_parameters(chId, pars)) {
         //translate tdo, pdo to volts and apply offset sorrection

//         std::cout << "tdo: " << time  ;
         time -= pars.time_offset;
         time /= pars.time_slope;
         time = pars.tac - time;
//         std::cout << " -> ns:" << time  << std::endl;

         
         charge *= VMM1Parameters::VMM1PdoVoltsCoef;
         charge -= pars.charge_offset;

      }
      

      
      ss.str("");
      ss << strip_number;
      
      
      CDetStripId stripid(chamber_name, multilayer, plane, readout, strip_number);

      DetStripPtr strptr(new CDetStrip(det, //detector=0
                                       chId,
                                       stripid,
                                       charge,
                                       time));
      
      if(strptr) {
         //         strip->set_strip_tree_entry(get_striptree_entry(strip_number));
         strips.push_back(strptr);
      }
      else {
         std::cout << "CTreeRawBnlMini::LoadEventData() NULL stripptr " << std::endl;

      }
   } // for ii < number of strips
   
}




void CTreeRawBnlMini::parse_tree(CDetDetector* det, CGuiMainFrame* gui) 
{
   try {
      CParserMmdaq3BnlMiniRawTree parser(this, m_config, det, gui);
      //      return parser.get_detector();
   } catch (std::runtime_error &e) {
      std::cout << "ERROR: CRootReader::parse_raw_tree() " << e.what() << std::endl;
   }
}


void CTreeRawBnlMini::Init(TTree *tree)
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
   m_time = 0;   
	//apv_tb = 0;
   
   // Set branch addresses and branch pointers
	if (!tree) {
		std::cerr << "Error: CTreeRawBnlMini::Init() no such tree" << std::endl;
      ferror = -1;
      throw std::runtime_error("CTreeRawBnlMini::Init() : no tree");
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
   fChain->SetBranchAddress("mmLayer",   &m_layer);
   fChain->SetBranchAddress("mmReadout", &m_readout);
   fChain->SetBranchAddress("mmStrip",   &m_strip);
   fChain->SetBranchAddress("raw_q",     &m_charge);
   fChain->SetBranchAddress("raw_t",     &m_time);
   
	if (ferror < 0 ) {
		std::cerr << "Error: CTreeRawBnlMini::Init() bad tree structure. Check if the file contains all apv and mm identification colums." << std::endl;
		throw std::runtime_error("Bad raw tree structure.");
	}
	ferror = 0;
	
   //Notify();
}


