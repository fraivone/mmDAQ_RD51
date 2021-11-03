//
//  GuiTabStrip.cpp
//  browser
//
//  Created by Marcin Byszewski on 7.4.12.
//  Copyright (c) 2012 CERN - PH/ADE. All rights reserved.
//

#include "GuiTabStrip.h"
#include "EventBrowser.h"
#include "DetStrip.h"
#include "Configuration.h"
#include "RootReader.h"
#include "StripsTree.h"

#include <TCanvas.h>
#include <TGFrame.h>
#include <TRootEmbeddedCanvas.h>
#include <TH1.h>
#include <TGListTree.h>
#include <TGraph.h>
#include <TMath.h>
#include <TF1.h>
#include <TPaveText.h>
#include <TText.h>

#include <boost/foreach.hpp>
#include <boost/bind.hpp>
//#include <boost/date_time/gregorian/gregorian.hpp>
//#include <boost/date_time/gregorian/greg_month.hpp>
//#include <boost/date_time/posix_time/posix_time.hpp>

#include <iostream>
#include <string>

using namespace browser;

CGuiTabStrip::CGuiTabStrip(CGuiBase* gui_parent,
                           CConfiguration* config, 
                           CEventBrowser*   eb,
                           TGTab* main_tabs, 
                           const std::string& name) :
CGuiTab(gui_parent, config, eb, main_tabs,name),
m_re_canvas(0), 
m_canvas(0),
m_hist(0),
m_fit(0),
m_fitfun(0)
//m_info(0)
{
   construct_tab();
}


CGuiTabStrip::~CGuiTabStrip()
{
   
}

void CGuiTabStrip::do_construct_tab()
{
   m_tab_frame->SetLayoutManager(new TGVerticalLayout(m_tab_frame));
   
   m_re_canvas = new TRootEmbeddedCanvas("CGuiTabStrip_rec", m_tab_frame, 100, 100);
   m_canvas = m_re_canvas->GetCanvas();
   m_tab_frame->AddFrame(m_re_canvas, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY));
   
   m_hist = new TH1F("CGuiTabStrip_hist", "CGuiTabStrip_hist", 30, 0, 30);
   m_hist->SetFillColor(kYellow);
//   m_info = new TPaveText(0.1, 0.8, 0.3, 0.9, "NDC");
//   m_info->AddText("This is CGuiTabStrip");
//   m_info->SetTextSize(0.02);

}

inline double fitFunctionFinal(double *x, double *par)
{
//   return *x;
   //par[0]->a, par[1]->g, par[2]->σ, par[3]->d, par[4]->k
   double r= (par[0]*TMath::Power(x[0] - par[1], par[3]) 
              * TMath::Exp(-(x[0] - par[1])/(2*par[2]*par[2])));
   
   if (r!=r) {
      return 0.0;
   }
   return r;
	//return (par[0]*TMath::Power(x[0]-par[1],par[3])*TMath::Exp(-TMath::Power((x[0]-par[1]),par[4])/(2*par[2]*par[2])));
}


void CGuiTabStrip::do_update(const CDetBase* detbase)
{
   m_hist->Reset();
   const CDetStrip* strip = dynamic_cast<const CDetStrip*>(detbase);
   const std::vector<double>& qvec = strip->get_raw_charge();
   m_hist->SetBins(static_cast<unsigned>(qvec.size()),
                   0, static_cast<unsigned>(qvec.size()));
   for (size_t ii = 0; ii < qvec.size(); ++ii) {
      m_hist->Fill(ii, qvec[ii]);
   }
   
   if (m_config->get_overlay_fitting()) {
      const CStripsTreeEntry* ste = strip->get_strip_tree_entry();
      
      if (ste && !ste->fitpars.empty()) {
         std::stringstream ss;
         std::string funname("CGuiTabStrip_hist_fitfnc");
         if (m_fitfun) {
            delete m_fitfun;
         }
         m_fitfun = new TF1(funname.c_str(), &fitFunctionFinal, 0, qvec.size(), 4);
         m_fitfun->SetParameter(0, ste->fitpars[0]);
         m_fitfun->SetParameter(1, ste->fitpars[1]);
         m_fitfun->SetParameter(2, ste->fitpars[2]);
         m_fitfun->SetParameter(3, ste->fitpars[3]);
         
//         std::cout << "fit pars:" << std::endl;
//         BOOST_FOREACH(double p, ste->fitpars) {
//            std::cout << "par = " << p << std::endl;
//         }
//         
//         for (double ii = 0; ii < qvec.size(); ++ii) {
//            std::cout << "fitFunctionFinal ("<<ii<<")" << fitFunctionFinal(&ii, const_cast<double*>(&(ste->fitpars)[0])) << std::endl;
//         }
         
      }
   }
   
   m_canvas->cd();
   
   

//   boost::posix_time::ptime time_stamp(boost::posix_time::second_clock::local_time() );
//   ss.str("");
//   ss << m_config->get_current_file_name() 
//   << " (evt " << m_eb->get_current_entry() << ") " 
//   << boost::posix_time::to_simple_string(time_stamp) << " " 
//   << " ";
   
   m_hist->Draw();
   m_info->Draw();
   
   if (m_fitfun) {
      m_fitfun->Draw("same");
   }
   draw_footer();
   draw_info(strip);
   m_canvas->Update();
}

void CGuiTabStrip::do_clear_tab()
{
   m_hist->Reset();
   m_canvas->Modified();
   m_canvas->Update();
}



