//
//  GuiTabDetectorStats.cpp
//  browser
//
//  Created by Francesco Cirotto on 29.7.14.
//  Copyright (c) 2012 CERN - PH/ADE. All rights reserved.
//

#include "GuiTabDetectorStats.h"
#include "EventBrowser.h"
#include "DetReadout.h"
#include "MBtools.h"
#include "GuiMainFrame.h"
#include "DetDetector.h"


#include <TGListTree.h>
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


CGuiTabDetectorStats::CGuiTabDetectorStats(CGuiBase* gui_parent, CConfiguration* config, CEventBrowser* eb, TGTab* main_tabs, const std::string& name ) :
CGuiTab(gui_parent, config, eb, main_tabs,name),
m_re_canvas(0),
m_canvas(0),
//m_hitmap(0),
// m_qmax(0),
// m_maxqmax(0),
// m_tbqmax(0),
// m_tbmaxqmax(0),
// m_nstrips(0),
// m_lumi(0),
m_hitmap_container(),
m_tbqmax_container()
{
   construct_tab();
}


CGuiTabDetectorStats::~CGuiTabDetectorStats()
{
   // delete m_hitmap;
   // delete m_qmax;
   // delete m_maxqmax;
   // delete m_tbqmax;
   // delete m_nstrips;
   // delete m_lumi;
}



void CGuiTabDetectorStats::do_construct_tab()
{
   double fontsize= 0.040;
   m_tab_frame->SetLayoutManager(new TGVerticalLayout(m_tab_frame));
   
   m_re_canvas = new TRootEmbeddedCanvas("CGuiTabDetectorStats_rec", m_tab_frame, 100, 100);
   m_canvas = m_re_canvas->GetCanvas();
   m_canvas->Divide(3,2);
   m_tab_frame->AddFrame(m_re_canvas, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY));
   
  }


void CGuiTabDetectorStats::do_update(const CDetBase* detbase)
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
      //int optstat = gStyle->GetOptStat();
      // gStyle->SetOptStat(1111111);

   BOOST_FOREACH(const CDetReadoutBase* rd, all_readouts) {
    
     if (rd->get_name() == "E") {
       continue;
     }

       
     std::string name = rd->get_full_name();
   
   

       std::pair<size_t, size_t> strip_range = rd->get_full_strip_range();
       strip_range.second += 1;

       TH1::AddDirectory(kFALSE);
       
       //create hitmap
       m_hitmap_container.insert(m_hitmap_container.end(),
                     boost::shared_ptr<TH1F> (new TH1F(std::string(name +"_hitmap").c_str(),
                                                       name.c_str(),
						       10,0,10 ) ) ); 

       // create tbqmax
       m_tbqmax_container.insert(m_tbqmax_container.end(),
				 boost::shared_ptr<TH1F> (new TH1F(std::string(name +"_tbqmax").c_str(),
								   name.c_str(),
								   10,0,10 ) ) );


       TH1::AddDirectory(kTRUE);
       
       m_hitmap_container.back()->GetXaxis()->SetLabelSize(fontsize);
       m_hitmap_container.back()->GetYaxis()->SetLabelSize(fontsize);
       m_hitmap_container.back()->SetFillColor(kAzure-2);
       
       m_tbqmax_container.back()->GetXaxis()->SetLabelSize(fontsize);
       m_tbqmax_container.back()->GetYaxis()->SetLabelSize(fontsize);
       m_tbqmax_container.back()->SetFillColor(kRed-2);
	      

       //Fill hitmap
       if (strip_range.first !=  strip_range.second ) {
         m_hitmap_container.back()->SetBins((int)strip_range.second - (int)strip_range.first , strip_range.first, strip_range.second);
	 
         const std::map<size_t, int>& hitmap_data = rd->get_stats_hitmap();
         for (std::map<size_t, int>::const_iterator it = hitmap_data.begin(); it != hitmap_data.end(); ++it) {
	   m_hitmap_container.back()->Fill(it->first, it->second);
	   // std::cout<<"hitmap " <<it->first<<"  "<<it->second<<std::endl;
	   
	 }
      }
       else {
         if (TVirtualPad* p =  m_canvas->GetPad(1)) {
	   p->Clear();
         }
       }

     
       //Fill tbqmax     
       std::pair<double,double> tr = rd->get_time_range();
       int tbins =  (tr.second-tr.first) /25;
       typedef std::map<long, int> TbmaxDataType;
       const TbmaxDataType& tbqmax_data = rd->get_stats_map_tmax();
       TbmaxDataType::key_type tbqmax_max = std::max_element(tbqmax_data.begin(), tbqmax_data.end(), FirstLess<TbmaxDataType::value_type>() )->first;
       TbmaxDataType::key_type tbqmax_min = std::min_element(tbqmax_data.begin(), tbqmax_data.end(), FirstLess<TbmaxDataType::value_type>() )->first;
       

       
       if (!tbqmax_data.empty()) {
	 /*
	 size_t tbins = tbqmax_data.size();
	 if (tbins > 100) {
	   tbins = 100;
	 }
	 if (tbins == 0) {
	   tbins = 1;
	 }
	 */
	 m_tbqmax_container.back()->SetCanExtend(TH1::kXaxis);
	 m_tbqmax_container.back()->SetBins(tbins  , tr.first, tr.second);
	 
	 //         std::cout << "tbqmax_min=" << tbqmax_min << ", tbqmax_max =" <<tbqmax_max
	 //            << " std::ceil(double(tbqmax_max)/double(tbins))=" << std::ceil(double(tbqmax_max)/double(tbins)) << std::endl;
         size_t ii = 0;
         for (TbmaxDataType::const_iterator it = tbqmax_data.begin(); it != tbqmax_data.end(); ++it, ++ii) {
	   
	   m_tbqmax_container.back()->Fill(it->first, it->second);
         }
       }
       else {
	 if (TVirtualPad* p =  m_canvas->GetPad(3)) {
	   p->Clear();
	 }
     }
       
   }

   //////////////////////////////////////////////////////
   
   
   m_canvas->Clear();
   m_canvas->Divide(2, (int)(m_tbqmax_container.size()) );
   
   //draw
   for (unsigned ii = 0; ii < m_tbqmax_container.size()*2 ; ii+=2) {
  
      //draw hitmap
      m_canvas->cd(ii+1);//left
      m_hitmap_container[ii/2]->Draw();

      //draw tbqmax
      m_canvas->cd(ii+2);//right
      m_tbqmax_container[ii/2]->Draw();
   }
  
   m_canvas->Update();
   //gStyle->SetOptStat(optstat);
   
 
}

void CGuiTabDetectorStats::do_clear_tab()
{
   m_tbqmax_container.clear();
   m_hitmap_container.clear();
}
