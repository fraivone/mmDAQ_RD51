//
//  TreeRawApvPedestals.cpp
//  eventbrowser
//
//  Created by Marcin Byszewski on 23/11/2012.
//  Copyright (c) 2012 CERN - PH/ADE. All rights reserved.
//

#include "TreeRawApvPedestals.h"
#include "SrsChannelId.h"
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

CTreeRawApvPedestals::CTreeRawApvPedestals(CConfiguration *config, TFile *file, const std::string& treename) :
CTreeBase(treename), m_config(config), rootfile(0),
m_has_multi_readouts(false),
m_has_planes(false),
m_has_multilayers(false)
{
   std::cout << "CTreeRawApvPedestals created" << std::endl;
   
   if(file == 0)
		throw std::runtime_error("CTreeRawApvPedestals - must have a pointer to TFile");
	else{
		TTree* tree = (TTree*)file->Get(treename.c_str());
      if (!tree) {
         throw std::runtime_error("CTreeRawApvPedestals - no raw tree named '" + treename +"'");
      }
      
		Init(tree);
	}

}


CTreeRawApvPedestals::~CTreeRawApvPedestals()
{
   
}

long CTreeRawApvPedestals::GetEntries() const
{
    return -1;
}


void CTreeRawApvPedestals::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).
	
   // Set object pointer
	apv_fecNo = 0;
   apv_id = 0;
   apv_ch = 0;
	mm_id = 0;
   mm_multilayer = 0;
   mm_plane = 0;
   mm_readout =0;
   mm_strip = 0;
   
   apv_pedmean = 0;
   apv_pedsigma = 0;
   apv_pedstd = 0;
   
	//apv_tb = 0;
   
   // Set branch addresses and branch pointers
	if (!tree) {
		std::cerr << "Error: CTreeRawApvPedestals::Init() no such tree" << std::endl;
      ferror = -1;
      throw std::runtime_error("CTreeRawApvPedestals::Init() : no tree");
	}
   
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);
	
	ferror = 0;
   ferror |= fChain->SetBranchAddress("time_s", &time_s, &b_time_s);
   //kludge : there is a typo in some of the old  mmDAQ versions um instead of us ..
   if (fChain->FindBranch("time_us")) {
		ferror = fChain->SetBranchAddress("time_us", &time_us, &b_time_us);
	}
	else {
		ferror |=  fChain->SetBranchAddress("time_um", &time_us, &b_time_us); //time_um : see comment above
	}
   ferror |= fChain->SetBranchAddress("apv_evt", &apv_evt, &b_apv_evt);
   ferror |= fChain->SetBranchAddress("apv_fecNo", &apv_fecNo, &b_apv_fecNo);
   ferror |= fChain->SetBranchAddress("apv_id", &apv_id, &b_apv_id);
   ferror |= fChain->SetBranchAddress("apv_ch", &apv_ch, &b_apv_ch);
   ferror |= fChain->SetBranchAddress("mm_id", &mm_id, &b_mm_id);
   if (fChain->GetBranch("mm_multilayer")) {
      ferror |= fChain->SetBranchAddress("mm_multilayer", &mm_multilayer, &b_mm_mlayer);
      m_has_multilayers = true;
   }
   if (fChain->GetBranch("mm_plane")) {
      ferror |= fChain->SetBranchAddress("mm_plane", &mm_plane, &b_mm_plane);
      m_has_planes = true;
   }
   if (fChain->GetBranch("mm_readout")) {
      ferror |= fChain->SetBranchAddress("mm_readout", &mm_readout, &b_mm_readout);
      m_has_multi_readouts = true;
   }
   ferror |= fChain->SetBranchAddress("mm_strip", &mm_strip, &b_mm_strip);
   
   ferror |= fChain->SetBranchAddress("apv_pedmean", &apv_pedmean, &b_apv_pedmean);
   ferror |= fChain->SetBranchAddress("apv_pedsigma", &apv_pedsigma, &b_apv_pedsigma);
   ferror |= fChain->SetBranchAddress("apv_pedstd", &apv_pedstd, &b_apv_pedstd);
   
	if (ferror < 0 ) {
		std::cerr << "Error: CTreeRawApvPedestals::Init() bad tree structure. Check if the file contains all apv and mm identification colums." << std::endl;
		throw std::runtime_error("Bad raw tree structure.");
	}
	ferror = 0;
	
  // Notify();
}




void CTreeRawApvPedestals::LoadEventData(DetBasePtrCont& strips, CDetDetector* det)
{
 //  return;
//   
   size_t number_of_strip = size();
   //   std::cout << "CTreeRawBnlMini::LoadEventData() number_of_strips =" << number_of_strip << std::endl;
   
   for (size_t ii = 0; ii < number_of_strip; ++ii) {
      unsigned fecNo   = (*(apv_fecNo))[ii];
      unsigned chipNo  = (*(apv_id))[ii];
      unsigned chNo    = (*(apv_ch))[ii];
      CSrsChannelId chId(fecNo, chipNo, chNo);
      
      std::string chamber_name = (*(mm_id))[ii];
      
      std::stringstream ss;
      unsigned multilayer_id   = has_multilayers() ? (*(mm_multilayer))[ii] : (unsigned)0;
      ss << multilayer_id;
      std::string multilayer_id_name(ss.str());
      
      ss.str("");
      unsigned plane_id   = has_planes() ? (*(mm_plane))[ii] : (unsigned)0;
      ss << plane_id;
      std::string plane_id_name(ss.str());
      
      ss.str("");
      unsigned readout = has_readouts() ? (*(mm_readout))[ii] : (unsigned)0;
      ss << readout;
      std::string readout_name(ss.str());

      
     unsigned strip_number = (*(mm_strip))[ii];
      
      double pedmean = (*(apv_pedmean))[ii];
      double pedstdev = (*(apv_pedstd))[ii];

      CDetStripId stripid(chamber_name, multilayer_id, plane_id, readout_name.at(0), strip_number);
      DetStripPtr strptr(new CDetStrip(det,
                                       chId,
                                       stripid,
                                       pedmean,
                                       pedstdev));
      if(strptr) {
         strips.push_back(strptr);
      }
      
   }
}



void CTreeRawApvPedestals::parse_tree(CDetDetector* det, CGuiMainFrame* gui)
{
    size_t n_strips = this->size();
   
   std::cout << "CTreeRawApvPedestals::parse_tree nstrips=" << n_strips << std::endl;
   
   std::vector<CDetReadoutBase*> allreadouts;
   std::stringstream ssname;
   
   
   
   int evt = 0;
   while (this->GetEntry(evt++)) {

      size_t vecsize = this->apv_fecNo->size();
      //cout << "vecsize = " << vecsize << endl;
      
      for (size_t ii = 0; ii < vecsize; ++ii) {
         unsigned fecNo  = (*(this->apv_fecNo))[ii];
         unsigned chipNo = (*(this->apv_id))[ii];
         unsigned chNo   = (*(this->apv_ch))[ii];
         CSrsChannelId channelid(fecNo, chipNo, chNo);
         det->add_srs_chip(channelid.chip_id());
         std::string chname   = (*(this->mm_id))[ii];
         unsigned mlayername   = (unsigned)0;
         unsigned planename   = this->has_planes() ? (*(this->mm_plane))[ii] : (unsigned)0;
         unsigned readoutname = this->has_readouts() ? (*(this->mm_readout))[ii] : (unsigned)0;
         unsigned strip_number = (*(this->mm_strip))[ii];
         
         
         double pedmean = this->apv_pedmean->at(ii);
         double pedstdev = this->apv_pedstd->at(ii);
         double sigma = this->apv_pedsigma->at(ii);
        
         
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




