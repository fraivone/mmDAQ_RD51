//
//  GuiTabDetectorEvent1D.cpp
//  eventbrowser
//
//  Created by Marcin Byszewski on 31/10/2012.
//  Copyright (c) 2012 CERN - PH/ADE. All rights reserved.
//

#include "GuiTabPedestalEvent1D.h"
#include "MBtools.h"
#include "DetDetector.h"
#include "DetReadout.h"
#include "GuiMainFrame.h"

#include <TH1.h>
#include <TH2.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TGFrame.h>
#include <TRootEmbeddedCanvas.h>
#include <TGListTree.h>

#include <boost/foreach.hpp>


#include <cmath>
#include <iostream>

using namespace browser;

CGuiTabPedestalEvent1D::CGuiTabPedestalEvent1D(CGuiBase* gui_parent,
                                               CConfiguration* config,
                                               CEventBrowser* eb,
                                               TGTab* main_tabs,
                                               const std::string& name)
:
CGuiTab(gui_parent, config, eb, main_tabs,name),
m_re_canvas(0), m_canvas(0), m_qdisp(), m_tdisp()
{
   construct_tab();
}


CGuiTabPedestalEvent1D::~CGuiTabPedestalEvent1D()
{

}



void CGuiTabPedestalEvent1D::do_construct_tab()
{
   m_tab_frame->SetLayoutManager(new TGVerticalLayout(m_tab_frame));
   
   m_re_canvas = new TRootEmbeddedCanvas("CGuiTabStrip_rec", m_tab_frame, 100, 100);
   m_canvas = m_re_canvas->GetCanvas();
   m_canvas->Divide(2,1);
   m_tab_frame->AddFrame(m_re_canvas, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY));
//   m_qdisp  = new TH1F("CGuiTabReadout1D_hist", "CGuiTabReadout1D_hist", 30, 0, 30);

}

void CGuiTabPedestalEvent1D::do_update(const CDetBase* detbase)
{
   
   const CDetDetector* det = dynamic_cast<const CDetDetector*>(detbase);
   if (!det) {
      return;
   }
   
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
          all_readouts.push_back(rd);
       }
    }
//   detbase->extract_readouts(all_readouts);
   
   if (all_readouts.empty()) {
      return;
   }
   
   float fontsize = 0.02 + 0.01 * pow((double)(all_readouts.size()), 1.1);

   std::vector< std::vector<double> > xall;
   std::vector< std::vector<double> > tall;
   
   BOOST_FOREACH(const CDetReadoutBase* rd, all_readouts) {
      
      if (rd->get_name() == "E") {
         continue;
      }
      
      std::string name = rd->get_full_name();
      std::pair<size_t,size_t> sr = rd->get_full_strip_range();
      std::pair<double,double> tr = rd->get_full_time_range();
//      std::pair<double,double> qr = rd->get_full_charge_range();
      
      sr.second += 1;
      tr.second += 1;
      
      TH1::AddDirectory(kFALSE);
      m_qdisp.insert(m_qdisp.end(),
                     boost::shared_ptr<TH1F> (new TH1F(std::string(name +"_detevt1d_q").c_str(),
                                                       name.c_str(),
                                                       (int)sr.second-(int)sr.first ,
                                                       sr.first,
                                                       sr.second)) );
      m_tdisp.insert(m_tdisp.end(),
                     boost::shared_ptr<TH2F> (new TH2F(std::string(name +"_detevt1d_t").c_str(),
                                                       name.c_str(),
                                                       10,0,10,
                                                       10,0,10) ) );
      TH1::AddDirectory(kTRUE);

      m_qdisp.back()->GetXaxis()->SetLabelSize(fontsize);
      m_qdisp.back()->GetYaxis()->SetLabelSize(fontsize);
      //m_qdisp.back()->GetXaxis()->SetTextFont((m_qdisp.back()->GetXaxis()->GetTextFont() | 0x11));
      m_tdisp.back()->GetXaxis()->SetLabelSize(fontsize);
      m_tdisp.back()->GetYaxis()->SetLabelSize(fontsize);
      
      
      //set time range 10, 30 1000 10000
      int tbins = 10;
      if (tr.second-tr.first <= 100) {
         tbins = tr.second - tr.first;
      }
      else if (tr.second-tr.first <= 1000) {
         tbins = (tr.second-tr.first)/10;
      }
      else {
         tbins = (tr.second-tr.first)/100;
      }
      
      m_tdisp.back()->SetBins((int)sr.second - (int)sr.first - 1, sr.first, sr.second,
                               tbins, tr.first, tr.second+1);

      const CDetReadoutBase::EventShortData& event_data = rd->get_short_data();
      BOOST_FOREACH(CDetReadoutBase::StripShortData esd, event_data) {
         m_qdisp.back()->Fill(esd.strip_no, esd.charge);
         m_tdisp.back()->Fill(esd.strip_no, esd.time);
      }
   }
   
   
   //////////////////////////////////////////////////////
   
   m_canvas->Divide(1,1);
   m_canvas->Clear();
   m_canvas->Divide(2, (int)(m_qdisp.size()) );

   //draw
   for (unsigned ii = 0; ii < m_qdisp.size()*2 ; ii+=2) {
      //draw q
      m_canvas->cd(ii+1);//left
      m_qdisp[ii/2]->SetFillColor(kOrange+2);
      m_qdisp[ii/2]->Draw();

      //draw t
      m_canvas->cd(ii+2);//right
      m_tdisp[ii/2]->SetMarkerStyle(kFullDotMedium);
      m_tdisp[ii/2]->Draw();
   }
   m_canvas->Update();
}


void CGuiTabPedestalEvent1D::do_clear_tab()
{
   m_qdisp.clear();
   m_tdisp.clear();
}



