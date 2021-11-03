//
//  GuiTabPlaneAll1D.cpp
//  browser
//
//  Created by Marcin Byszewski on 12.4.12.
//  Copyright (c) 2012 CERN - PH/ADE. All rights reserved.
//

#include "GuiTabPlaneAll1D.h"
#include "DetPlane.h"
#include "DetReadout.h"

#include <TGTab.h>
#include <TH1.h>
#include <TH2.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TGFrame.h>
#include <TRootEmbeddedCanvas.h>

#include <boost/foreach.hpp>

#include <iostream>
#include <sstream>
#include <cmath>

using namespace browser;

CGuiTabPlaneAll1D::CGuiTabPlaneAll1D(CGuiBase* gui_parent, CConfiguration* config, CEventBrowser* eb, TGTab* main_tabs, const std::string& name) :
CGuiTab(gui_parent, config, eb, main_tabs,name),
m_re_canvas(0),
m_canvas(0),
m_qhists(),
m_tframes(),
m_tgraphs()
{
   construct_tab();
}


CGuiTabPlaneAll1D::~CGuiTabPlaneAll1D()
{
   
}

void CGuiTabPlaneAll1D::do_construct_tab()
{
   m_tab_frame->SetLayoutManager(new TGVerticalLayout(m_tab_frame));
   
   m_re_canvas = new TRootEmbeddedCanvas("CGuiTabPlaneAll1D_rec", m_tab_frame, 100, 100);
   m_canvas = m_re_canvas->GetCanvas();
   m_canvas->Divide(2,2);
   m_tab_frame->AddFrame(m_re_canvas, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY));
}



void CGuiTabPlaneAll1D::do_update(const CDetBase* detbase)
{
   // get number of readouts in this plane
   
   const CDetPlane* plane = dynamic_cast<const CDetPlane*>(detbase);
   if (!plane) {
      return;
   }
   
   m_qhists.clear();
   m_tframes.clear();
   m_tgraphs.clear();
   const DetBasePtrCont& readouts = plane->get_parts();
   
   m_canvas->Clear(); //cd(0);
//   m_canvas->Divide(1,1);
   m_canvas->Divide(2, static_cast<unsigned>(readouts.size()));
   double fontsize = 0.045;
   unsigned ii = 0;
   BOOST_FOREACH(boost::shared_ptr<CDetBase> base, readouts) {
      std::vector<double> x;
      std::vector<double> t;
      std::stringstream ss;
      CDetReadoutBase* rd = dynamic_cast<CDetReadoutBase*>(base.get());
      std::pair<size_t, size_t> srange = rd->get_full_strip_range();

      if (srange.first != srange.second) {
         ss << "All1D_qhist_" << rd->get_name();
         m_canvas->cd(ii+1);
         TH1F* qh = new TH1F(ss.str().c_str(), ss.str().c_str(),
                             (int)srange.second - (int)srange.first, srange.first, srange.second+1);
         qh->SetDirectory(0);
         qh->GetXaxis()->SetLabelSize(fontsize);
         qh->GetYaxis()->SetLabelSize(fontsize);

         const CDetReadoutBase::EventShortData& data = rd->get_short_data();
         BOOST_FOREACH(CDetReadoutBase::StripShortData sd, data) {
            qh->Fill(sd.strip_no, sd.charge);
            x.push_back(sd.strip_no);
            t.push_back(sd.time);
            
         }
         qh->SetFillColor(kOrange+2);

         qh->Draw();
         
         m_qhists.push_back( boost::shared_ptr<TH1F>(qh));
      }
      else {
         //TODO make text: bad range
      }
      
      
      //std::pair<double, double> trange = rd->get_full_time_range(); wrong time range
  
      std::pair<double, double> trange = rd->get_time_range(); //Francesco
      //std::cout<<"GuiTabPlaneAll1d::get_time_range  first "<<trange.first<<"   second time "<<trange.second<<std::endl;
       
      if (trange.first != trange.second && !t.empty()) {
         ss.str("");
         ss << "All1D_time" << rd->get_name();
         
         TH2F* tf = new TH2F( ss.str().c_str(), ss.str().c_str(), 
                             (int)srange.second - (int)srange.first, srange.first, srange.second+1,
                             (int)trange.second - (int)trange.first, trange.first, trange.second);
         tf->SetDirectory(0);
         tf->GetXaxis()->SetLabelSize(fontsize);
         tf->GetYaxis()->SetLabelSize(fontsize);
         m_tframes.push_back( boost::shared_ptr<TH2F>(tf));
         
         TGraph* tg = new TGraph((unsigned)(x.size()), &x[0], &t[0]);
         m_tgraphs.push_back( boost::shared_ptr<TGraph>(tg));

         m_canvas->cd(ii+2);
         tf->Draw();
         draw_info(rd);
         draw_footer();
         
         if (!x.empty()) {
            tg->Draw("*same");
         }
      }
      else {
         //TODO make text: bad range
      }
           
      ii+=2;
   }
   
   m_canvas->Update();
   
   
}



