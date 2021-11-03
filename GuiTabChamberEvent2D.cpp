//
//  GuiTabChamberEvent2D.cpp
//  eventbrowser
//
//  Created by Francesco Cirotto on 3.08.14.
//  Copyright (c) 2012 CERN - PH/ADE. All rights reserved.
//

#include "GuiTabChamberEvent2D.h"
#include "MBtools.h"
#include "DetChamber.h"
#include "DetReadout.h"
#include "GuiMainFrame.h"


#include <TH1.h>
#include <TH2.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TGFrame.h>
#include <TRandom.h>
#include <TRootEmbeddedCanvas.h>
#include <TGListTree.h>
#include <TString.h>

#include <boost/foreach.hpp>


#include <cmath>
#include <iostream>

using namespace browser;

CGuiTabChamberEvent2D::CGuiTabChamberEvent2D(CGuiBase* gui_parent,
                                               CConfiguration* config,
                                               CEventBrowser* eb,
                                               TGTab* main_tabs,
                                               const std::string& name)
:
CGuiTab(gui_parent, config, eb, main_tabs,name),
m_re_canvas(0), m_canvas(0)
{
   construct_tab();
}


CGuiTabChamberEvent2D::~CGuiTabChamberEvent2D()
{

}



void CGuiTabChamberEvent2D::do_construct_tab()
{
   m_tab_frame->SetLayoutManager(new TGVerticalLayout(m_tab_frame));
   
   m_re_canvas = new TRootEmbeddedCanvas("CGuiTabStrip_rec", m_tab_frame, 100, 100);
   m_canvas = m_re_canvas->GetCanvas();
   m_tab_frame->AddFrame(m_re_canvas, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY));
//   m_qdisp  = new TH1F("CGuiTabReadout1D_hist", "CGuiTabReadout1D_hist", 30, 0, 30);

}

void CGuiTabChamberEvent2D::do_update(const CDetBase* detbase)
{
   
  const CDetChamber* chamber = dynamic_cast<const CDetChamber*>(detbase);
  if (!chamber) {
     return;
   }
  TString chamber_name = chamber->get_full_name();
   
  clear_tab();
  
  //get checked readouts from gui tree
  CGuiMainFrame* main_frame = dynamic_cast<CGuiMainFrame*>(get_main_frame());
  if (!main_frame) {
     std::cout << "no main frame !" << std::endl;
      return;
   }
   
  std::vector<TGListTreeItem*> all_items;
  main_frame->get_tree_checked_items(all_items);
  if(all_items.empty()) {
    return;
   }
   
  //get all readouts
  std::vector<const CDetReadoutBase*> all_readouts;
  BOOST_FOREACH(TGListTreeItem* item, all_items) {
    const CDetBase* base = static_cast<const CDetBase*>(item->GetUserData());
    if (const CDetReadoutBase* rd = dynamic_cast<const CDetReadoutBase*>(base)) {
	    TString rd_name = rd->get_full_name();
	    if(rd_name.Contains(chamber_name) ){
	      all_readouts.push_back(rd);
	    }
    }
  }
   
  if (all_readouts.empty()) {
      return;
   }

  float fontsize = 0.02 + 0.01 * pow((double)(all_readouts.size()), 1.1);
  BOOST_FOREACH(const CDetReadoutBase* rd, all_readouts) {
    
    if (rd->get_name() == "E") {
      continue;
    }

  TString readout_name = rd->get_full_name();
  // Fetching beam profile data
  RecHit_Collection[std::string(readout_name)] = rd->get_beamprofile_hitmap();
	      
  } // End loop on all the readouts belonging to the chamber in the GUI Tab
  

  TH1::AddDirectory(kFALSE);
       
  // Creating beam profile histogram
  m_beamProfile_container.insert(m_beamProfile_container.end(),
				boost::shared_ptr<TH2F> (new TH2F(std::string(chamber_name).c_str(),std::string(chamber_name).c_str(),200,0,380,200,0,380) ) ); // FIX : Binning according to number of strips
  TH1::AddDirectory(kTRUE); 


  std::string m_RecHit_x_key = std::string(chamber_name) + std::string("-0-0-X");
  std::string m_RecHit_y_key = std::string(chamber_name) + std::string("-0-0-Y");
  
  // Checking for actual data in X and Y
  int size_x = RecHit_Collection[m_RecHit_x_key].size(); 
  int size_y = RecHit_Collection[m_RecHit_y_key].size();
  
  // Clearing the canvas if no data or bad data are found  
  if(size_x == 0 || size_y == 0 || size_x != size_y)
    m_canvas->Clear();

  else{
    // Filling beamProfile 2DHistogram
    for(std::vector<int>::size_type i = 0; i != size_x; i++) 
    {
      x = RecHit_Collection[m_RecHit_x_key][i]; 
      y = RecHit_Collection[m_RecHit_y_key][i];
      if(x!=-1 && y!=-1) m_beamProfile_container.back()->Fill(x,y);
    }

    m_canvas->Clear();
  
    m_beamProfile_container.back()->Draw("COLZ");
    m_canvas->Update();
  }
   
 
}

void CGuiTabChamberEvent2D::do_clear_tab()
{
  m_beamProfile_container.clear();
}