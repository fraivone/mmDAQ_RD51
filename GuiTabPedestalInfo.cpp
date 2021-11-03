//
//  GuiTabReadoutPedestals.cpp
//  eventbrowser
//
//  Created by Marcin Byszewski on 23/11/2012.
//  Copyright (c) 2012 CERN - PH/ADE. All rights reserved.
//

#include "GuiTabPedestalInfo.h"

#include "GuiTabReadout1D.h"
#include "EventBrowser.h"
#include "DetReadout.h"
#include "RootReader.h"


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

CGuiTabPedestalInfo::CGuiTabPedestalInfo(CGuiBase* gui_parent,
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


CGuiTabPedestalInfo::~CGuiTabPedestalInfo()
{
   delete m_pedmean;
   m_pedmean = 0;
   delete m_pedstdev;
   m_pedstdev = 0;
}

void CGuiTabPedestalInfo::do_construct_tab()
{
   m_tab_frame->SetLayoutManager(new TGVerticalLayout(m_tab_frame));
   m_re_canvas = new TRootEmbeddedCanvas("CGuiTabStrip_peds_c", m_tab_frame, 100, 100);
   m_canvas = m_re_canvas->GetCanvas();
   m_canvas->Divide(1,2);
   m_tab_frame->AddFrame(m_re_canvas, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY));
   
//   int optstat = gStyle->GetOptStat();
//   gStyle->SetOptStat(1111111);
   
   m_pedmean  = new TH2F("CGuiTab1D_pedmean", "CGuiTab1D_pedmean", 128, 0, 127, 100, 0, 100);
   m_pedmean->SetFillColor(kOrange+2);
   m_pedmean->SetMinimum(0);
   m_pedmean->SetMarkerStyle(kStar);

   m_pedstdev  = new TH2F("CGuiTab1D_pedstdev", "CGuiTab1D_pedstdev", 128, 0, 127, 100, 0, 100);
   m_pedstdev->SetFillColor(kOrange+2);
   m_pedstdev->SetMinimum(0);
   m_pedstdev->SetMarkerStyle(kStar);
   
//   gStyle->SetOptStat(optstat);
}



void CGuiTabPedestalInfo::do_update(const CDetBase* detbase)
{
   std::cout << "CGuiTabPedestalInfo::update() " << detbase->get_full_name() << std::endl;

   const CDetReadoutBase* rd = dynamic_cast<const CDetReadoutBase*>(detbase);
   if (!rd) {
      std::cout << "CGuiTabPedestalInfo::update()  not a readout " << detbase->get_full_name() << std::endl;
      return;
   }
   
   m_pedmean->Reset();
   m_pedstdev->Reset();
   
//   m_pedmean->SetBit(TH1::kCanRebin);
//   m_pedstdev->SetBit(TH1::kCanRebin);
   
   const CDetReadoutBase::EventShortData& data = rd->get_pedestal_data(); //q,t -> mean, stddev
   
   std::cout << "CGuiTabPedestalInfo::update()  data sz= " << data.size() << std::endl;

    if (!data.empty()) {
       std::pair<size_t, size_t> sr = rd->get_full_strip_range();
       sr.second += 1;
       
       std::vector<double> x,m,s;
       
       BOOST_FOREACH(CDetReadoutBase::StripShortData sd, data) {
          x.push_back(sd.strip_no);
          m.push_back(sd.charge);
          s.push_back(sd.time);
       }
       double maxmean = *std::max_element(m.begin(), m.end());
       double minmean = *std::max_element(m.begin(), m.end());
       double maxstd = *std::max_element(s.begin(), s.end());
       double minstd = *std::max_element(s.begin(), s.end());
       
       m_pedmean->SetBins((int)sr.second - (int)sr.first , sr.first, sr.second,
                          50, 0, maxmean );
       m_pedstdev->SetBins((int)sr.second - (int)sr.first , sr.first, sr.second,
                            50, 0, maxstd );
       
       BOOST_FOREACH(CDetReadoutBase::StripShortData sd, data) {
		  int k = 1;
		  int y = 1;
		  // m_pedmean->Fill(sd.strip_no, sd.charge);
          // m_pedstdev->Fill(sd.strip_no, sd.time);
			m_pedmean->Fill(++k, ++y);
          m_pedstdev->Fill(++y, ++k);
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
       std::cout << "CGuiTabPedestalInfo::update()  pedestals empty for " << rd->get_full_name() << std::endl;
       m_canvas->Clear("D");
    }
   
    m_canvas->Update();
   
}


void CGuiTabPedestalInfo::do_clear_tab()
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



