//
//  CSrsPedestalsRoot.cpp
//  eventbrowser
//
//  Created by Nikolaos Tsirintanis on 3/08/2014.
//  Copyright (c) 2014 CERN - PH/ADE. All rights reserved.
//

#include "CSrsPedestalsRoot.h"
#include "SrsChannelId.h"
#include "SrsChipId.h"
#include "DetStrip.h"
#include "DetStripId.h"
#include "Configuration.h"
#include "DetDetector.h"
#include "DetChamber.h"
#include "DetPlane.h"
#include "DetReadout.h"

#include <TTree.h>
#include <TFile.h>
#include <TChain.h>


#include <iostream>
#include <stdexcept>
#include <sstream>

using namespace browser;

CSrsPedestalsRoot::CSrsPedestalsRoot(CConfiguration *config, TFile *file, const std::string& treename) :
CTreeBase(treename), m_config(config), rootfile(0),
m_has_multi_readouts(false),
m_has_planes(false),
m_has_multilayers(false)
{
   std::cout << "CSrsPedestalsRoot created" << std::endl;
   
   if(file == 0)
		throw std::runtime_error("CSrsPedestalsRoot - must have a pointer to TFile");
	else{
		TTree* tree = (TTree*)file->Get(treename.c_str());
      if (!tree) {
         throw std::runtime_error("CSrsPedestalsRoot - no raw tree named '" + treename +"'");
      }
      
		Init(tree);
	}

}


CSrsPedestalsRoot::~CSrsPedestalsRoot()
{
   
}

long CSrsPedestalsRoot::GetEntries() const
{
    return -1;
   
}


void CSrsPedestalsRoot::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed)
   // Set object pointer
   m_evt = 0;
   m_time_s = 0; 
   m_time_us = 0; 
   m_srs_time_stamp = 0; //srs time stamp 
   pfec = 0 ;  
   pchip = 0 ; 
   pchan = 0  ;
   pchamb = 0 ;
   player = 0 ;
   pmlayer = 0 ;
   pread = 0  ;
   pstrip = 0 ;
   pmean = 0 ;
   pstdev = 0 ;
   psigma = 0 ;
   
   
   // Set branch addresses and branch pointers
	if (!tree) {
		std::cerr << "Error: CSrsPedestalsRoot::Init() no such tree" << std::endl;
      ferror = -1;
      throw std::runtime_error("CSrsPedestalsRoot::Init() : no tree");
	}
   
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

	
	ferror = 0;
   ferror |= fChain->SetBranchAddress("evt",          &m_evt, &b_evt);
   ferror |= fChain->SetBranchAddress("daqTimeSec",       &m_time_s, &b_time_s);  
   ferror |= fChain->SetBranchAddress("daqTimeMicroSec",   &m_time_us, &b_time_us);
   ferror |= fChain->SetBranchAddress("srsTimeStamp",  &m_srs_time_stamp, &b_srs_time_stamp);
   ferror |= fChain->SetBranchAddress("srsFec",       &pfec, &b_fec);
   ferror |= fChain->SetBranchAddress("srsChip",      &pchip, &b_chip);
   ferror |= fChain->SetBranchAddress("srsChan",      &pchan, &b_chan);
   ferror |= fChain->SetBranchAddress("mmChamber", &pchamb, &b_chamb);
   if (fChain->GetBranch("mmMultilayer")) {
      ferror |= fChain->SetBranchAddress("mmMultilayer",&pmlayer, &b_mlayer);
      m_has_multilayers = true;
   }
   if (fChain->GetBranch("mmPlane")) {
      ferror |= fChain->SetBranchAddress("mmPlane", &pplane, &b_plane);
      m_has_planes = true;
   }
   if (fChain->GetBranch("mmReadout")) {
      ferror |= fChain->SetBranchAddress("mmReadout", &pread, &b_read);
      m_has_multi_readouts = true;
   }
   ferror |= fChain->SetBranchAddress("mmLayer",   &player,&b_layer);
   ferror |= fChain->SetBranchAddress("mmStrip", &pstrip, &b_strip);
   
   ferror |= fChain->SetBranchAddress("ped_mean",         &pmean, &b_mean);
   ferror |= fChain->SetBranchAddress("ped_stdev",        &pstdev, &b_stdev);
   ferror |= fChain->SetBranchAddress("ped_sigma",        &psigma, &b_sigma);

   fChain->GetEntry(0);
	if (ferror < 0 ) {
		std::cerr << "Error: CSrsPedestalsRoot::Init() bad tree structure. Check if the file contains all apv and mm identification colums." << std::endl;
		throw std::runtime_error("Bad raw tree structure.");
	}
	ferror = 0;
	
  // Notify();
}




void CSrsPedestalsRoot::LoadEventData(DetBasePtrCont& strips, CDetDetector* det)
{
   
   size_t number_of_strip = size();
   
   for (size_t ii = 0; ii < number_of_strip; ++ii) {
 	 unsigned fecNo  = (*(this->pfec))[ii];
         unsigned chipNo = (*(this->pchip))[ii];
         unsigned chNo   = (*(this->pchan))[ii];
   //      CSrsChipId chipid(fecNo, chipNo);
         CSrsChannelId channelid(fecNo, chipNo, chNo);
//         CSrsChannelId channelid(chipid, chNo);
//      

        std::string chamber_name   = (*(this->pchamb))[ii];
	//if(chamber_name == "unmapped") continue;
        std::stringstream ss;
        unsigned multilayer_id  =this->has_multilayers()? (*(this->pmlayer))[ii]:(unsigned)0;

      ss << multilayer_id;
     std::string multilayer_id_name(ss.str());
//      
      ss.str("");
       unsigned plane_id   = (*(this->player))[ii];

      ss << plane_id;
      std::string plane_id_name(ss.str());
//      
     ss.str("");
char readout = this->has_readouts() ? (*(this->pread))[ii] : (unsigned)0;

     ss << readout;
     std::string readout_name = ss.str();
//
//      

        unsigned strip_number = (*(this->pstrip))[ii];      


         double pedmean = this->pmean->at(ii);
         double pedstdev = this->pstdev->at(ii);
      CDetStripId stripid(chamber_name, multilayer_id, plane_id,readout,strip_number);
      DetStripPtr strptr(new CDetStrip(det,
                                       channelid,
                                       stripid,
                                       pedmean,
                                       pedstdev));
     

	 if(strptr) {
         strips.push_back(strptr);
      }
      
   }
}



void CSrsPedestalsRoot::parse_tree(CDetDetector* det, CGuiMainFrame* gui)
{
    size_t n_strips = this->size();
   
   std::cout << "CSrsPedestalsRoot::parse_tree nstrips=" << n_strips << std::endl;
   
   std::vector<CDetReadoutBase*> allreadouts;
   std::stringstream ssname;
   
   
   
   int evt = 0;
   while (this->GetEntry(evt++)) {

      size_t vecsize = this->pfec->size();
      //cout << "vecsize = " << vecsize << endl;
      
      for (size_t ii = 0; ii < vecsize; ++ii) {
         unsigned fecNo  = (*(this->pfec))[ii];
         unsigned chipNo = (*(this->pchip))[ii];
         unsigned chNo   = (*(this->pchan))[ii];
         CSrsChannelId channelid(fecNo, chipNo, chNo);
         det->add_srs_chip(channelid.chip_id());
         std::string chname   = (*(this->pchamb))[ii];
         unsigned mlayername   = (*(this->player))[ii];//(unsigned)0;
         unsigned planename   = this->has_planes() ? (*(this->pplane))[ii] : (unsigned)0;
         unsigned readoutname = this->has_readouts() ? (*(this->pread))[ii] : (unsigned)0;
         unsigned strip_number = (*(this->pstrip))[ii];
         
         
         double pedmean = this->pmean->at(ii);
         double pedstdev = this->pstdev->at(ii);
         double sigma = this->psigma->at(ii);
        
         
         CDetChamber* chamber = dynamic_cast<CDetChamber*>( det->get_part(chname));
         ssname.str("");
         ssname << planename;
         CDetPlane* plane = dynamic_cast<CDetPlane*>( chamber->get_part(ssname.str()));
         ssname.str("");
         ssname << readoutname;
         if (CDetReadoutBase* readout = dynamic_cast<CDetReadoutBase*>( plane->get_part(ssname.str()))) {
            readout->add_strip_to_pedestal(strip_number, pedmean, pedstdev);
         }
         else if (CDetReadoutBase* readout = dynamic_cast<CDetReadoutBase*>( plane->get_part_by_id(readoutname)) ) {
            readout->add_strip_to_pedestal(strip_number, pedmean, pedstdev);
         }
         else {
            // noreadout
         }
         

         
      
      }
      
   }//while getentry()

   
   
   
//   
//   
//   for (size_t ii = 0; ii < n_strips; ++ii) {
//      unsigned fecNo  = (*(this->apv_fecNo))[ii];
//      unsigned chipNo = (*(this->apv_id))[ii];
//      unsigned chNo   = (*(this->apv_ch))[ii];      
//      CSrsChannelId channelid(fecNo, chipNo, chNo);
//      det->add_srs_chip(channelid.chip_id());
//      std::string chname   = (*(this->mm_id))[ii];
//      unsigned mlayername   = (unsigned)0;
//      unsigned planename   = this->has_planes() ? (*(this->mm_plane))[ii] : (unsigned)0;
//      unsigned readoutname = this->has_readouts() ? (*(this->mm_readout))[ii] : (unsigned)0;
//      unsigned strip_number = (*(this->mm_strip))[ii];
//    
//      
//      CDetChamber* chamber = dynamic_cast<CDetChamber*>( det->get_part(chname));
//      ssname.str("");
//      ssname << planename;
//      CDetPlane* plane = dynamic_cast<CDetPlane*>( chamber->get_part(ssname.str()));
//      ssname.str("");
//      ssname << readoutname;
//      CDetReadoutBase* readout = dynamic_cast<CDetReadoutBase*>( plane->get_part(ssname.str())) ;
//      
//            
//      double pedmean = (*(apv_pedmean))[ii];
//      double pedstdev = (*(apv_pedstd))[ii];
//
//      readout->add_strip_to_pedestal(strip_number, pedmean, pedstdev);
//      
//      
//      
//   }

}




