//
//  GuiTabReadoutStats.cpp
//  browser
//
//  Created by Marcin Byszewski on 11.4.12.
//  Copyright (c) 2012 CERN - PH/ADE. All rights reserved.
//

#include "GuiTabReadoutStats.h"
#include "EventBrowser.h"
#include "DetReadout.h"
#include "MBtools.h"

#include <TH1.h>
#include <TH2.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TGFrame.h>
#include <TRootEmbeddedCanvas.h>
#include <TStyle.h>
#include <TROOT.h>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time.hpp>
#include <boost/foreach.hpp>


#include <iostream>
#include <utility>

using namespace browser;
namespace bpt = boost::posix_time;


CGuiTabReadoutStats::CGuiTabReadoutStats(CGuiBase* gui_parent, CConfiguration* config, CEventBrowser* eb, TGTab* main_tabs, const std::string& name ) :
CGuiTab(gui_parent, config, eb, main_tabs,name),
m_re_canvas(0),
m_canvas(0),
m_hitmap(0),
m_qmax(0),
m_maxqmax(0),
m_tbqmax(0),
m_tbmaxqmax(0),
m_nstrips(0),
m_lumi(0)
{
   construct_tab();
}


CGuiTabReadoutStats::~CGuiTabReadoutStats()
{
   delete m_hitmap;
   delete m_qmax;
   delete m_maxqmax;
   delete m_tbqmax;
   delete m_nstrips;
   delete m_lumi;
}



void CGuiTabReadoutStats::do_construct_tab()
{
   double fontsize= 0.040;
   m_tab_frame->SetLayoutManager(new TGVerticalLayout(m_tab_frame));
   
   m_re_canvas = new TRootEmbeddedCanvas("CGuiTabReadoutStats_rec", m_tab_frame, 100, 100);
   m_canvas = m_re_canvas->GetCanvas();
   m_canvas->Divide(3,2);
   m_tab_frame->AddFrame(m_re_canvas, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY));
   
   m_hitmap = new TH1F("CGuiTabReadoutStats_hitmap", "hitmap", 10, 0, 10);
   m_hitmap->SetFillColor(kAzure-2);
   m_hitmap->GetXaxis()->SetLabelSize(fontsize);
   m_hitmap->GetYaxis()->SetLabelSize(fontsize);

   m_qmax = new TH1F("CGuiTabReadoutStats_qmax", "charge all_strips", 10, 0, 10);
   m_qmax->SetFillColor(kRed-2);
   m_qmax->GetXaxis()->SetLabelSize(fontsize);
   m_qmax->GetYaxis()->SetLabelSize(fontsize);
   m_qmax->SetDirectory(0);
   
   m_maxqmax = new TH1F("CGuiTabReadoutStats_maxqmax", "charge maxQ_strips", 10, 0, 10);
   m_maxqmax->SetFillColor(kRed-3);
   m_maxqmax->GetXaxis()->SetLabelSize(fontsize);
   m_maxqmax->GetYaxis()->SetLabelSize(fontsize);
   
   m_tbqmax = new TH1F("CGuiTabReadoutStats_tbqmax", "time all_strips", 10, 0, 10);
   m_tbqmax->SetFillColor(kRed-2);
   m_tbqmax->GetXaxis()->SetLabelSize(fontsize);
   m_tbqmax->GetYaxis()->SetLabelSize(fontsize);
   
   m_tbmaxqmax = new TH1F("CGuiTabReadoutStats_tbmaxqmax", "time maxQ_strips", 10, 0, 10);
   m_tbmaxqmax->SetFillColor(kRed-3);
   m_tbmaxqmax->GetXaxis()->SetLabelSize(fontsize);
   m_tbmaxqmax->GetYaxis()->SetLabelSize(fontsize);

   m_nstrips = new TH1F("CGuiTabReadoutStats_nstrips", "#strips", 10, 0, 10);
   m_nstrips->SetFillColor(kAzure-2);
   m_nstrips->GetXaxis()->SetLabelSize(fontsize);
   m_nstrips->GetYaxis()->SetLabelSize(fontsize);
   
   m_lumi = new TH1F("CGuiTabReadoutStats_lumi", "lumi", 10, 0, 10);
   m_lumi->SetFillColor(kAzure-2);
   m_lumi->GetXaxis()->SetLabelSize(fontsize);
   m_lumi->GetYaxis()->SetLabelSize(fontsize);
   m_lumi->SetBarWidth(1.0);
   m_lumi->GetXaxis()->SetTimeDisplay(1);
   //~ timeSH[ii]->GetXaxis()->SetTimeFormat("%d\/%m\/%y%F2000-02-28 13:00:01");
   m_lumi->GetXaxis()->SetTimeFormat("%H:%M:%S");
}


void CGuiTabReadoutStats::do_update(const CDetBase* detbase)
{
   const CDetReadoutBase* rd = dynamic_cast<const CDetReadoutBase*>(detbase);
   if (!rd) {
      return;
   }
   int optstat = gStyle->GetOptStat();
   gStyle->SetOptStat(1111111);

   //retrieve timebin window
   std::pair<double,double> tr = rd->get_time_range();
   int tbins =  (tr.second-tr.first) /25;
   
   {
      m_hitmap->Reset();
      std::pair<size_t, size_t> strip_range = rd->get_full_strip_range();
      strip_range.second += 1;
      
      if (strip_range.first !=  strip_range.second ) {
         m_hitmap->SetBins((int)strip_range.second - (int)strip_range.first , strip_range.first, strip_range.second);
         
         const std::map<size_t, int>& hitmap_data = rd->get_stats_hitmap();
         for (std::map<size_t, int>::const_iterator it = hitmap_data.begin(); it != hitmap_data.end(); ++it) {
            m_hitmap->Fill(it->first, it->second);
         }
         m_canvas->cd(1);
         m_hitmap->Draw();
         draw_footer();
         draw_info(rd);
      }
      else {
         if (TVirtualPad* p =  m_canvas->GetPad(1)) {
            p->Clear();
         }
      }
   }
   
   
   
   {
      
      const TH1F* rdth = rd->get_stats_qmax_hist();
      delete m_qmax;
      m_qmax = (TH1F*) rdth->Clone();
      m_qmax->SetDirectory(0);
      m_qmax->SetFillColor(kRed-2);
      m_canvas->cd(2);
      m_qmax->Draw();
      draw_footer();
      draw_info(rd);
      
      
//      m_qmax->Reset();
//      std::pair<double, double> qrange = rd->get_full_charge_range();
//      
//      if (qrange.first != qrange.second) {
//         m_qmax->SetBins(100, qrange.first, qrange.second);
//         const std::map<int, int>& qmax_data = rd->get_stats_qmax();
//         size_t ii = 0;
//         for (std::map<int, int>::const_iterator it = qmax_data.begin(); it != qmax_data.end(); ++it, ++ii) {
//            m_qmax->Fill(it->first, it->second);
//         }
//         
//         m_canvas->cd(2);
//         m_qmax->Draw();
//         draw_footer();
//         draw_info(rd);
//      }
//      else {
//         if (TVirtualPad* p =  m_canvas->GetPad(2)) {
//            p->Clear();
//         }
//      }
   }
   
   
   {

      // using auto rebinning hist form readout
      const TH1F* rdth = rd->get_stats_maxqmax_hist();
      delete m_maxqmax;
      m_maxqmax = (TH1F*) rdth->Clone();
      m_maxqmax->SetDirectory(0);
      m_maxqmax->SetFillColor(kRed-3);
      m_canvas->cd(5);
      m_maxqmax->Draw();
      draw_footer();
      draw_info(rd);

//      m_maxqmax->Reset();
//      std::pair<double, double> qrange = rd->get_full_charge_range();   
//      if (qrange.first != qrange.second) {
//         m_maxqmax->SetBins(100, qrange.first, qrange.second);
//         const std::map<int, int>& maxqmax_data = rd->get_stats_maxqmax();
//         size_t ii = 0;
//         for (std::map<int, int>::const_iterator it = maxqmax_data.begin(); it != maxqmax_data.end(); ++it, ++ii) {
//            m_maxqmax->Fill(it->first, it->second);
//         }
//         
//         m_canvas->cd(5);
//         m_maxqmax->Draw(); 
//         draw_footer();
//         draw_info(rd);
//      }
//      else {
//         if (TVirtualPad* p =  m_canvas->GetPad(5)) {
//            p->Clear();
//         }
//      }
   }
   
   
   
   {
      // number of strips in events
      m_nstrips->Reset("ICES");
      
      const std::map<size_t, int>& nstrips_data = rd->get_stats_nstrips();      
      if (!nstrips_data.empty()) {
         size_t maxn = std::max_element(nstrips_data.begin(), 
                                        nstrips_data.end(), 
                                        FirstLess<std::pair<size_t, int> >())->first;
         
         m_nstrips->SetBins((int)maxn+1, 0, (int)maxn+1);
         for (std::map<size_t, int>::const_iterator it = nstrips_data.begin(); it != nstrips_data.end(); ++it) {
            m_nstrips->Fill(it->first, it->second);
         }
         m_canvas->cd(4);
         m_nstrips->Draw();
         draw_footer();
         draw_info(rd);
      }
      else {
         if (TVirtualPad* p =  m_canvas->GetPad(4)) {
            p->Clear();
         }
      }
   }
   
   { //draw tmax
      
      m_tbqmax->Reset("ICES");
      typedef std::map<long, int> TbmaxDataType;
      const TbmaxDataType& tbqmax_data = rd->get_stats_map_tmax();
      TbmaxDataType::key_type tbqmax_max = std::max_element(tbqmax_data.begin(), tbqmax_data.end(), FirstLess<TbmaxDataType::value_type>() )->first;
      TbmaxDataType::key_type tbqmax_min = std::min_element(tbqmax_data.begin(), tbqmax_data.end(), FirstLess<TbmaxDataType::value_type>() )->first;
      

      if (!tbqmax_data.empty()) {
         size_t tbins = tbqmax_data.size();
         if (tbins > 100) {
            tbins = 100;
         }
         if (tbins == 0) {
            tbins = 1;
         }
         m_tbqmax->SetCanExtend(TH1::kXaxis);
         m_tbqmax->SetBins(tbins  , tr.first, tr.second);
//         std::cout << "tbqmax_min=" << tbqmax_min << ", tbqmax_max =" <<tbqmax_max
//            << " std::ceil(double(tbqmax_max)/double(tbins))=" << std::ceil(double(tbqmax_max)/double(tbins)) << std::endl;
         size_t ii = 0;
         for (TbmaxDataType::const_iterator it = tbqmax_data.begin(); it != tbqmax_data.end(); ++it, ++ii) {

            m_tbqmax->Fill(it->first, it->second);
         }
         
         m_canvas->cd(3);
         m_tbqmax->Draw();
         draw_footer();
         draw_info(rd);
         
            
      }
      else {
         if (TVirtualPad* p =  m_canvas->GetPad(3)) {
            p->Clear();
         }
      }
   }
   
   //      get_stats_map_tmax();
   //      get_stats_map_maxtmax();

   
   
   { //draw maxtmax

      
      
//      // using auto rebinning hist form readout
//      const TH1F* rdth = rd->get_stats_maxqmax_hist();
//      delete m_maxqmax;
//      m_maxqmax = (TH1F*) rdth->Clone();
//      m_maxqmax->SetDirectory(0);
//      m_maxqmax->SetFillColor(kRed-3);
//      m_canvas->cd(5);
//      m_maxqmax->Draw();
//      draw_footer();
//      draw_info(rd);

      
      
      m_tbmaxqmax->Reset("ICES");
      typedef std::map<long, int> TbmaxDataType;
      const TbmaxDataType& tbqmax_data = rd->get_stats_map_maxtmax();
      TbmaxDataType::key_type tbqmax_max = std::max_element(tbqmax_data.begin(), tbqmax_data.end(), FirstLess<TbmaxDataType::value_type>() )->first;
      TbmaxDataType::key_type tbqmax_min = std::min_element(tbqmax_data.begin(), tbqmax_data.end(), FirstLess<TbmaxDataType::value_type>() )->first;

      if(!tbqmax_data.empty()) {
         size_t tbins = tbqmax_data.size();
         if (tbins > 100) {
            tbins = 100;
         }
         m_tbmaxqmax->SetBins(tbins  , tr.first, tr.second);
         size_t ii = 0;
         for (TbmaxDataType::const_iterator it = tbqmax_data.begin(); it != tbqmax_data.end(); ++it, ++ii) {
            m_tbmaxqmax->Fill(it->first, it->second);
         }
         
         m_canvas->cd(6);
         m_tbmaxqmax->Draw();
         draw_footer();
         draw_info(rd);
      }
      else {
         if (TVirtualPad* p =  m_canvas->GetPad(6)) {
            p->Clear();
         }
      }
   }
   
   
   {
      //lumi
      /*
      m_lumi->Reset();
      std::pair<bpt::ptime, bpt::ptime> timerange = rd->get_run_time_range();
      
      
      if (timerange.first != timerange.second) {
         std::string left = bpt::to_iso_extended_string(timerange.first - bpt::seconds(1));
         std::string right = bpt::to_iso_extended_string(timerange.second + bpt::seconds(1));
         left[10]  = ' ';
         right[10] = ' ';
         left = left.substr(0, 19);
         right = right.substr(0, 19);
         std::cout << "left :" << left << std::endl;
         std::cout << "right:" << right << std::endl;
         //      std::string timeformat("%d/%m/%y%F" + left);
         //      m_lumi->GetXaxis()->SetTimeFormat("%d/%m/%y%F2000-02-28 13:00:01");
         //      m_lumi->GetXaxis()->SetTimeFormat(timeformat.c_str());      
         
         //time in seconds
         bpt::ptime time_t_epoch(boost::gregorian::date(1970,1,1)); 
         bpt::time_duration left_epoch =  timerange.first - time_t_epoch;
         std::cout << "Seconds diff: " << left_epoch.total_seconds() << std::endl;
         bpt::time_duration right_epoch =  timerange.second - time_t_epoch;
         std::cout << "right_epoch : " << right_epoch.total_seconds() << std::endl;
         
         int nbins = (right_epoch.total_seconds()-left_epoch.total_seconds())/60 ;//1 min
         
         if (nbins > 60) {
            nbins = (right_epoch.total_seconds()-left_epoch.total_seconds())/600; //10 min
         }
         else if (nbins > 6000) {
            nbins = (right_epoch.total_seconds()-left_epoch.total_seconds())/3000; //10 min
         }
         
         m_lumi->SetBins((right_epoch.total_seconds()-left_epoch.total_seconds())/6000,
                         left_epoch.total_seconds(),
                         right_epoch.total_seconds());
         
         
         //      m_lumi->GetXaxis()->SetTimeFormat(timeformat.c_str());      
         
         //      if (trange.first != trange.second) {
         //         const std::map<size_t, int>& tbqmax_data = rd->get_stats_tbmaxqmax();
         //         size_t ii = 0;
         //         for (std::map<size_t, int>::const_iterator it = tbqmax_data.begin(); it != tbqmax_data.end(); ++it, ++ii) {
         //            m_tbmaxqmax->Fill(it->first, it->second);
         //         }
         //         
         m_canvas->cd(7);
         m_lumi->Draw();
         //         draw_footer();
         //         draw_info(rd);
         //      }
      }
      else {
         if (TVirtualPad* p =  m_canvas->GetPad(7)) {
            p->Clear();
         }
      }
       */
   }

   
   m_canvas->cd();
   m_canvas->Update();
   
   gStyle->SetOptStat(optstat);
   
}

