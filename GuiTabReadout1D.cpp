//
//  GuiTabReadout1D.cpp
//  browser
//
//  Created by Marcin Byszewski on 11.4.12.
//  Copyright (c) 2012 CERN - PH/ADE. All rights reserved.
//

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

CGuiTabReadout1D::CGuiTabReadout1D(CGuiBase* gui_parent, 
                                   CConfiguration* config, 
                                   CEventBrowser* eb, 
                                   TGTab* main_tabs, 
                                   const std::string& name ) :
CGuiTab(gui_parent, config, eb, main_tabs,name),
m_re_canvas(0), 
m_canvas(0),
m_qdisp(0),
m_tframe(0),
m_qover(0),
m_tmulti(0)
{
   construct_tab();
}


CGuiTabReadout1D::~CGuiTabReadout1D()
{
//   std::cout << "CGuiTabReadout1D::update DESTRUCTOR\n";

   delete m_qdisp;
   delete m_qover;
   delete m_tframe;
   delete m_tmulti;
}

void CGuiTabReadout1D::do_construct_tab()
{
   m_tab_frame->SetLayoutManager(new TGVerticalLayout(m_tab_frame));
   
   
   m_re_canvas = new TRootEmbeddedCanvas("CGuiTabStrip_rec", m_tab_frame, 100, 100);
   m_canvas = m_re_canvas->GetCanvas();
   m_canvas->Divide(2,1);
   m_tab_frame->AddFrame(m_re_canvas, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY));
   
   int optstat = gStyle->GetOptStat();
   gStyle->SetOptStat(1111111);
   
   m_qdisp  = new TH1F("CGuiTabReadout1D_hist", "CGuiTabReadout1D_hist", 30, 0, 30);
   m_qdisp->SetFillColor(kOrange+2);
   m_qdisp->SetMinimum(0);
   m_tframe = new TH2F("time", "time", 10,0,10,10,0,10);
   m_tframe->SetMarkerStyle(3);
   m_tframe->SetMarkerSize(1.5);
  
   gStyle->SetOptStat(0);
   
   m_qover  = new TH1F("Readout1D_overlay", "Readout1D_overlay", 30, 0, 30);
   m_qover->SetObjectStat(0);
   m_qover->SetMinimum(0);
   gStyle->SetOptStat(optstat);
   
   m_tmulti = new TMultiGraph();

 
}


void CGuiTabReadout1D::do_update(const CDetBase* detbase)
{
//   std::cout << "CGuiTabReadout1D::update(" <<detbase->get_name() << ")\n";
   const CDetReadoutBase* rd = dynamic_cast<const CDetReadoutBase*>(detbase);
   if (!rd) {
      return;
   }
   
   

   if(m_qdisp)
      m_qdisp->Reset("ICES");
   if(m_tframe)
      m_tframe->Reset("ICES");
   if (m_qover)
      m_qover->Reset("ICES");
   
   
   delete m_tmulti;
   m_tmulti = new TMultiGraph();

   
   TGraph* tdisp  = new TGraph();
   TGraph* tover  = new TGraph();
   
   tdisp->SetMarkerStyle(kStar);
   tdisp->SetMarkerSize(1.5);
   tover->SetMarkerStyle(kOpenSquare);
   tover->SetMarkerColor(kRed);
   tover->SetMarkerSize(1.5);
   
   
   if (TVirtualPad* p = m_canvas->GetPad(2)) {
      p->Clear();
   }

   const CDetReadoutBase::EventShortData& data = rd->get_short_data();
   const CDetReadoutBase::EventShortData& overlay = rd->get_overlay_short_data();
   
   double qmin  = 0.0;
   if (!data.empty()) {
      std::pair<size_t, size_t> sr = rd->get_full_strip_range();
      sr.second += 1;
    
      // std::pair<double, double> trange = rd->get_full_time_range(); //OLD code: wrong time range
      //trange.second += 1;
      
      std::pair<double,double> trange = rd->get_time_range();
      int tbins = (trange.second - trange.first)/25;
     
      //std::cout << "CGuiTabReadout1D::get time trange=" << trange.first << " " << trange.second << std::endl;
      //std::cout << "CGuiTabReadout1D:: #bin =" << tbins << std::endl;
      
//      std::cout << "CGuiTabReadout1D::update trange=" << trange.first << " " << trange.second << std::endl;
      m_qdisp->SetBins((int)sr.second - (int)sr.first , sr.first, sr.second);
      m_qover->SetBins((int)sr.second - (int)sr.first , sr.first, sr.second);
      
      std::vector<double> x;
      std::vector<double> q;
      std::vector<double> t;
      //   double tmax=0.0;
      int ii = 0;
      BOOST_FOREACH(CDetReadoutBase::StripShortData sd, data) {
         x.push_back(sd.strip_no);
         q.push_back(sd.charge);
         t.push_back(sd.time);
         tdisp->SetPoint(ii++, sd.strip_no, sd.time);
      }
      
      double xmin = *std::min_element(x.begin(), x.end());
      double xmax = *std::max_element(x.begin(), x.end()) + 1.0;
      qmin = std::min(qmin, *std::min_element(q.begin(), q.end()));
      
      m_qdisp->SetBins((int)xmax - (int)xmin  ,xmin , xmax);
      m_qover->SetBins((int)xmax - (int)xmin , xmin , xmax);
      m_qdisp->FillN((int)(x.size()), &x[0], &q[0]);
      
      
      
      m_tmulti->Add(tdisp, "p");
      if(TGraph* g1 = (TGraph*)(m_tmulti->FindObject("tdisp"))) {
         g1->SetMarkerColor(kGreen);
//         mg->GetXaxis()->SetLimits(1.5,7.5);
//         mg->SetMinimum(0.);
//         mg->SetMaximum(10.);
//         mygraph->GetHistogram()->SetMaximum(mymaximum);
//         mygraph->GetHistogram()->SetMinimum(myMinimum);

      }
      
//      m_canvas->cd();
//      draw_footer();
//      draw_info(rd);
      
      m_canvas->cd(1);
      m_qdisp->Draw();
      draw_footer();
      draw_info(rd);
      
      //OLD bin setting
      //set time range 10, 30 1000 10000 
      // int tbins = 10;
      // if (trange.second-trange.first <= 100) {
      //    tbins = (int)trange.second-(int)trange.first;
      // }
      // else if (trange.second-trange.first <= 1000) {
      //    tbins = (trange.second-trange.first)/10;
      // }
      // else {
      //    tbins = (trange.second-trange.first)/100;
      // }
      
      m_tframe->SetBins(static_cast<int>(sr.second) - static_cast<int>(sr.first) - 1 , sr.first, sr.second,
                        tbins, trange.first, trange.second);

      BOOST_FOREACH(CDetReadoutBase::StripShortData sd, data) {
      	m_tframe->Fill(sd.strip_no,sd.time); 
      }
      
      if (!overlay.empty()) {
         int ii = 0;
         BOOST_FOREACH(CDetReadoutBase::StripShortData sd, overlay) {
            
            double qtrack = m_qdisp->GetBinContent(m_qdisp->GetXaxis()->FindBin(sd.strip_no));
            
            double ttrack = 0.0;
            std::vector<double>::iterator xit = std::find(x.begin(), x.end(), sd.strip_no);
            if (xit != x.end()) {
               ttrack = t[std::distance(x.begin(), xit)];
            }
            
            
            
            if (qtrack <= m_qdisp->GetYaxis()->GetXmin()) {
               qtrack =m_qdisp->GetYaxis()->GetXmax();
            }
           
            tover->SetPoint(ii++, sd.strip_no, sd.time); // time from the BNL trigger FPGA box
            tover->SetPoint(ii++, sd.strip_no, ttrack);
            m_qover->Fill(sd.strip_no, qtrack);
         }
         m_canvas->cd(1);
         m_qover->SetFillColor(kBlue+1);
         m_qover->Draw("sames");
         
         m_qover->SetMinimum(qmin);
         m_qdisp->SetMinimum(qmin);
         
         m_tmulti->Add(tover, "p");
      }
      
      m_canvas->cd(2);
      // m_tmulti->Draw("A");
      m_tframe->Draw();
      draw_footer();
      draw_info(rd);
   }
   else {
      m_canvas->Clear("D");
   }
   m_canvas->Update();
//   if (m_tmulti->GetXaxis()) {
//      m_tmulti->GetXaxis()->SetRange(m_qdisp->GetXaxis()->GetXmin(), m_qdisp->GetXaxis()->GetXmax());
//   }

}

void CGuiTabReadout1D::do_clear_tab()
{
//   m_canvas->Clear();
//   m_qdisp->Reset();
//   m_tframe->Reset();
//   m_tdisp->Clear(); //?
//
   
   if (TVirtualPad* p1 = m_canvas->GetPad(1)) {
      p1->Clear();
   }
   if (TVirtualPad* p2 = m_canvas->GetPad(2)) {
      p2->Clear();
   }
   
   m_canvas->Modified();
   m_canvas->Update();
}
