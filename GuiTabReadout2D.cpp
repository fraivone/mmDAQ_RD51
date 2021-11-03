//
//  GuiTabReadout2D.cpp
//  browser
//
//  Created by Marcin Byszewski on 11.4.12.
//  Copyright (c) 2012 CERN - PH/ADE. All rights reserved.
//

#include "GuiTabReadout2D.h"
#include "DetReadout.h"


#include <TH1.h>
#include <TH2.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TGFrame.h>
#include <TRootEmbeddedCanvas.h>

#include <boost/foreach.hpp>

#include <iostream>

using namespace browser;

CGuiTabReadout2D::CGuiTabReadout2D(CGuiBase* gui_parent, CConfiguration* config, CEventBrowser* eb, TGTab* main_tabs, const std::string& name ) :
CGuiTab(gui_parent, config, eb, main_tabs,name),
m_re_canvas(0), 
m_canvas(0),
m_hist(0)
{
   construct_tab();
}


CGuiTabReadout2D::~CGuiTabReadout2D()
{
   
}

void CGuiTabReadout2D::do_construct_tab()
{
   m_tab_frame->SetLayoutManager(new TGVerticalLayout(m_tab_frame));
   
   
   m_re_canvas = new TRootEmbeddedCanvas("CGuiTabReadout2D_rec", m_tab_frame, 100, 100);
   m_canvas = m_re_canvas->GetCanvas();
   m_tab_frame->AddFrame(m_re_canvas, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY));
   
   m_hist = new TH2F("CGuiTabReadout2D_hist", "CGuiTabReadout2D_hist", 30, 0, 30, 10, 0, 10);

}


void CGuiTabReadout2D::do_update(const CDetBase* detbase)
{

   const CDetReadoutBase* rd = dynamic_cast<const CDetReadoutBase*>(detbase);
   if (!rd) {
      return;
   }
   m_hist->Reset();
   
   const CDetReadoutBase::EventLongData& data = rd->get_long_data();
   if (!data.empty()) {
      std::pair<size_t, size_t> sr = rd->get_full_strip_range();
      // std::pair<double, double> trange = rd->get_full_time_range(); //wrong time range
      std::pair<double, double> trange = rd->get_time_range();
      
      //set time range 10, 30 1000 10000

      int tbins = (trange.second-trange.first);
      tbins = tbins/25;
       
      //  ++tbins;
       
      // if (tbins > 5000) {
      //    tbins = 50;
      // }
      // else if (tbins > 1000){
      //    tbins /= 10;
      // }
      // if (tbins > 100) {
      //    tbins /=10;
      // }
      
      m_hist->SetBins(static_cast<int>(sr.second) - static_cast<int>(sr.first) - 1, sr.first, sr.second+1,
                      tbins, trange.first, trange.second);
      
      BOOST_FOREACH(CDetReadoutBase::StripLongData ld, data) {
         size_t ii = 0;
         
         if (!ld.charge.empty()) {
            BOOST_FOREACH(double q, ld.charge) {
               m_hist->Fill(ld.strip_no, ld.timebin_width*ii++, q);
            }
         }
         else {
            CDetReadoutBase::StripShortData sd = rd->get_short_data(ld.strip_no);
            m_hist->Fill(ld.strip_no, sd.time , sd.charge);
         }
         
      }
      m_canvas->cd();
      m_hist->Draw("lego2");   
      draw_footer();
      draw_info(rd);
      
   }
   else {
      m_canvas->Clear();
   }
   m_canvas->Update();
}

void CGuiTabReadout2D::do_clear_tab()
{
   m_hist->Reset();
   m_canvas->Modified();
   m_canvas->Update();   
}

