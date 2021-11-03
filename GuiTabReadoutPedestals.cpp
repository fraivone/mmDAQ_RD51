//
//  GuiTabReadoutPedestals.cpp
//  eventbrowser
//
//  Created by Marcin Byszewski on 23/11/2012.
//  Copyright (c) 2012 CERN - PH/ADE. All rights reserved.
//
//  Modified by Nikos Tsirintanis - Nikolaos.Tsirintanis@Cern.ch

#include "GuiTabReadoutPedestals.h"

#include "GuiTabReadout1D.h"
#include "EventBrowser.h"
#include "DetReadout.h"


#include <TH1.h>
#include <TH2.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TCanvas.h>
#include <TGFrame.h>
#include <TRootEmbeddedCanvas.h>
#include <TStyle.h>

#include <boost/foreach.hpp>

#include <iostream>

using namespace browser;

CGuiTabReadoutPedestals::CGuiTabReadoutPedestals(CGuiBase* gui_parent,
                                                 CConfiguration* config,
                                                 CEventBrowser* eb,
                                                 TGTab* main_tabs,
                                                 const std::string& name) :
CGuiTab(gui_parent, config, eb, main_tabs,name),
m_re_canvas(0),
m_canvas(0),
m_pedmean(0),
m_pedstdev(0)
{
   construct_tab();
}


CGuiTabReadoutPedestals::~CGuiTabReadoutPedestals()
{
   delete m_pedmean;
   m_pedmean = 0;
   delete m_pedstdev;
   m_pedstdev = 0;
}

void CGuiTabReadoutPedestals::do_construct_tab()
{
   m_tab_frame->SetLayoutManager(new TGVerticalLayout(m_tab_frame));
   m_re_canvas = new TRootEmbeddedCanvas("CGuiTabStrip_peds", m_tab_frame, 100, 100);
   m_canvas = m_re_canvas->GetCanvas();
   m_canvas->Divide(1,2);
   m_tab_frame->AddFrame(m_re_canvas, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY));
   
//   int optstat = gStyle->GetOptStat();
//   gStyle->SetOptStat(1111111);
   
   m_pedmean  = new TH2F("CGuiTabReadout1D_pedmean", "CGuiTabReadout1D_pedmean", 128, 0, 127, 100, 0, 100);
   m_pedmean->SetFillColor(kOrange+2);
   m_pedmean->SetMinimum(0);
   m_pedmean->SetMarkerStyle(kStar);

   m_pedstdev  = new TH2F("CGuiTabReadout1D_pedstdev", "CGuiTabReadout1D_pedstdev", 128, 0, 127, 100, 0, 100);
   m_pedstdev->SetFillColor(kOrange+2);
   m_pedstdev->SetMinimum(0);
   m_pedstdev->SetMarkerStyle(kStar);
   
//   gStyle->SetOptStat(optstat);
}



void CGuiTabReadoutPedestals::do_update(const CDetBase* detbase)
{
   std::cout << "CGuiTabReadoutPedestals::update() " << detbase->get_full_name() << std::endl;

   const CDetReadoutBase* rd = dynamic_cast<const CDetReadoutBase*>(detbase);
   if (!rd) {
      std::cout << "CGuiTabReadoutPedestals::update()  not a readout " << detbase->get_full_name() << std::endl;
      return;
   }
   
   m_pedmean->Reset();
   m_pedstdev->Reset();
   
//   m_pedmean->SetBit(TH1::kCanRebin);
//   m_pedstdev->SetBit(TH1::kCanRebin);
   
   const CDetReadoutBase::EventShortData& data = rd->get_pedestal_data(); //q,t -> mean, stddev
   
   std::cout << "CGuiTabReadoutPedestals::update()  data sz= " << data.size() << std::endl;

    if (!data.empty()) {
       std::pair<size_t, size_t> sr = rd->get_full_strip_range();
       sr.second += 1;
       
       std::vector<double> x,m,s;
       
       BOOST_FOREACH(CDetReadoutBase::StripShortData sd, data) {
          x.push_back(sd.strip_no);
          m.push_back(sd.charge);
          s.push_back(sd.time);
       }
       
       bool isempty =false;
       if(x.empty() || m.empty() || s.empty()) isempty = true;   
       
       if(!isempty){
       double maxmean = *std::max_element(m.begin(), m.end());
       double maxstd = *std::max_element(s.begin(), s.end());
       double minmean = *std::min_element(m.begin(), m.end());
       
       m_pedmean->SetBins((int)sr.second - (int)sr.first , sr.first, sr.second,
                          50, minmean - 200 , maxmean + 200);
       m_pedstdev->SetBins((int)sr.second - (int)sr.first , sr.first, sr.second,
                            50, 0, maxstd );
       
       BOOST_FOREACH(CDetReadoutBase::StripShortData sd, data) {
          m_pedmean->Fill(sd.strip_no, sd.charge);
          m_pedstdev->Fill(sd.strip_no, sd.time);
       }
      }
       m_canvas->cd(1);
       m_pedmean->Draw("*");
       draw_footer();
       draw_info(rd);
       
       m_canvas->cd(2);
       m_pedstdev->Draw("*");
       draw_footer();
       draw_info(rd);
       
       
       
    }
    else {
       std::cout << "CGuiTabReadoutPedestals::update()  pedestals empty for " << rd->get_full_name() << std::endl;
       m_canvas->Clear("D");
    }
   
    m_canvas->Update();
   
}


void CGuiTabReadoutPedestals::do_clear_tab()
{
     if (TVirtualPad* p1 = m_canvas->GetPad(1)) {
      p1->Clear();
   }
   if (TVirtualPad* p2 = m_canvas->GetPad(2)) {
      p2->Clear();
   }
   
   m_canvas->Modified();
   m_canvas->Update();
}



