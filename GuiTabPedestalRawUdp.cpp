//
//  GuiTabPedestalRawUdp.cpp
//  eventbrowser
//
//  Created by Marcin Byszewski on 4/8/13.
//  Copyright (c) 2013 CERN - PH/ADE. All rights reserved.
//

#include "GuiTabPedestalRawUdp.h"

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
#include <boost/make_shared.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>

#include <cmath>
#include <iostream>
#include <cstdlib> 
#include <stddef.h>
#include <stdint.h>


using namespace browser;

CGuiTabPedestalEventRawUdp::CGuiTabPedestalEventRawUdp(CGuiBase* gui_parent,
                                               CConfiguration* config,
                                               CEventBrowser* eb,
                                               TGTab* main_tabs,
                                               const std::string& name)
:
CGuiTab(gui_parent, config, eb, main_tabs,name),
m_re_canvas(0), m_canvas(0), m_rawhist()
{
   construct_tab();
}


CGuiTabPedestalEventRawUdp::~CGuiTabPedestalEventRawUdp()
{
   
}


void CGuiTabPedestalEventRawUdp::do_construct_tab()
{
   m_tab_frame->SetLayoutManager(new TGVerticalLayout(m_tab_frame));
   m_re_canvas = new TRootEmbeddedCanvas("CGuiTabDetEvtRawUdp_rec", m_tab_frame, 100, 100);
   m_canvas = m_re_canvas->GetCanvas();
   m_canvas->Divide(2,1);
   m_tab_frame->AddFrame(m_re_canvas, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY));
}


void CGuiTabPedestalEventRawUdp::do_update(const CDetBase* entry)
{

   const CDetDetector* det = dynamic_cast<const CDetDetector*>(entry);
   if (!det) {
      std::cout << "CGuiTabPedestalEventRawUdp::do_update no DET\n" ;
      return;
   }
   std::cout << "CGuiTabPedestalEventRawUdp::do_update ()\n" ;
   clear_tab();
   
   m_canvas->Divide(1,1);
   m_canvas->Clear();

   for(ChipHistMap::iterator ih = m_rawhist.begin(); ih != m_rawhist.end(); ++ih) {
      //      ih->second.get()->Set(0);
      unsigned npoints = ih->second.get()->GetN();
      for (unsigned ii = 0; ii < npoints; ++ii) {
         ih->second.get()->SetPoint(ii, ii, 0);
      }
   }
   
   
   //get checked readouts from gui tree
   CGuiMainFrame* main_frame = dynamic_cast<CGuiMainFrame*>(get_main_frame());
   if (!main_frame) {
      std::cout << "no main frame !" << std::endl;
      m_canvas->Update();
      return;
   }
   
   std::vector<TGListTreeItem*> all_items;
   main_frame->get_tree_checked_items(all_items);
   
   CDetDetector::RawUdpMap const& rawudp = det->get_event_raw_udp_data();
   //get all chips/udp data
   //...

   if (rawudp.empty()) {
      m_canvas->Update();
      return;
   }


   for (CDetDetector::RawUdpMap::const_iterator it = rawudp.begin(); it != rawudp.end(); ++it) {
      ChipHistMap::iterator fnd = m_rawhist.find(it->first);
      if(m_rawhist.end() == fnd) {
         //set name it->first
         TH1::AddDirectory(kFALSE);
         std::pair<ChipHistMap::iterator, bool> res =
         m_rawhist.insert(ChipHistMap::value_type(it->first,
                                                  boost::make_shared<TGraph>(
//                                                                             hname.c_str(),
//                                                                           hname.c_str(),
//                                                                           0, 4000, 4000
                                                                           ) ));
         TH1::AddDirectory(kTRUE);
         if(res.second) {
            fnd = res.first;
         } else {
            continue;
         }
      }
      
      TGraph* hist = fnd->second.get();
      
      //fill data it->second
      unsigned npoints = (unsigned) it->second.size();
      hist->Set(npoints);
      for (unsigned ii = 0; ii < npoints; ++ii) {
         hist->SetPoint(ii, ii, it->second[ii]);
      }
      
//      for (CDetDetector::RawUdpMap::mapped_type::const_iterator ir = it->second.begin() ; ir != it->second.end(); ++ir) {
//         hist->Fill(*ir);
//      }
      
   }

   size_t vecsize = m_rawhist.size();
   int columns = (int) std::ceil(double(vecsize) / 4.0);
   if (vecsize > 1) {
      int n = (int)std::ceil((double)(vecsize) / (double)columns);
      m_canvas->Divide( static_cast<int>(columns), n);
   }
   else {
      m_canvas->Divide(1, 1);
   }
   
   unsigned ii = 1;
   BOOST_FOREACH(ChipHistMap::value_type& p, m_rawhist) {
      m_canvas->cd(ii);
      double xmax = p.second.get()->GetMaximum();
      double xmin = p.second.get()->GetMinimum();

      p.second.get()->Draw("ALP");
      std::string hname("UDP_" + CSrsChipId(p.first).get_string());
//      p.second.get()->SetName(hname.c_str());
      p.second.get()->SetTitle(hname.c_str());
      p.second.get()->SetMaximum(xmax);
      p.second.get()->SetMinimum(xmin);

      std::string padname(std::string(m_canvas->GetName()) + "_" + boost::lexical_cast<std::string>(ii));
//      TPad* pad = (TPad*) m_canvas->GetPrimitive(padname.c_str());
//      if (pad) pad->Modified();
      ++ii;
      
   }
   m_canvas->Update();
   
   
}


void CGuiTabPedestalEventRawUdp::do_clear_tab()
{
   //m_rawhist.clear();
}

