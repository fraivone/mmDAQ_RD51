//
//  GuiTabDetectorCrosstalk.cpp
//  eventbrowser
//
//  Created by Marcin Byszewski on 10.5.12.
//  Copyright (c) 2012 CERN - PH/ADE. All rights reserved.
//

#include "GuiTabDetectorCrosstalk.h"
#include "DetDetector.h"
#include "XtalkHistMapImpl.h"


#include <TGTab.h>
#include <TH1.h>
#include <TH2.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TGFrame.h>
#include <TRootEmbeddedCanvas.h>
#include <TGButton.h>

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>

#include <iostream>
#include <iomanip>

using namespace browser;

CGuiTabDetectorCrosstalk::CGuiTabDetectorCrosstalk(CGuiBase* gui_parent, CConfiguration* config, CEventBrowser* eb, TGTab* main_tabs, const std::string& name) :
CGuiTab(gui_parent, config, eb, main_tabs,name),
m_re_canvas(0),
m_canvas(0),
m_gui_xtalk_create_map_file(0),
m_xtalk_all( new XtalkHistMapImpl() ),
m_xtalk_32( new XtalkHistMapImpl() ),
m_local_all_chips_xtalk()
{
    construct_tab();
}


CGuiTabDetectorCrosstalk::~CGuiTabDetectorCrosstalk()
{
   delete m_xtalk_32;
   delete m_xtalk_all;
}

void CGuiTabDetectorCrosstalk::do_construct_tab()
{
   m_tab_frame->SetLayoutManager(new TGVerticalLayout(m_tab_frame));
 
   
   {
      //clear thresholds button
      m_gui_xtalk_create_map_file = new TGTextButton(m_tab_frame, "&Make Map");
      m_gui_xtalk_create_map_file->Resize(110, 22);
      m_gui_xtalk_create_map_file->Connect("Clicked()", "browser::CGuiTabDetectorCrosstalk", this, "gui_handle_create_xtalk_map_file()");
      m_tab_frame->AddFrame(m_gui_xtalk_create_map_file, new TGLayoutHints( kLHintsTop, 1, 1, 1, 1));
   }
   
   
   m_re_canvas = new TRootEmbeddedCanvas("CGuiTabDetectorCrosstalk", m_tab_frame, 100, 100);
   m_canvas = m_re_canvas->GetCanvas();
//   m_canvas->Divide(2,2);
   m_tab_frame->AddFrame(m_re_canvas, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY));
}

void CGuiTabDetectorCrosstalk::do_clear_tab()
{ 
   m_xtalk_all->data.clear();
   m_xtalk_32->data.clear();
   m_local_all_chips_xtalk.clear();

}

void CGuiTabDetectorCrosstalk::do_update(const CDetBase* detbase)
{
//   return;
   
   const CDetDetector* det = dynamic_cast<const CDetDetector*>(detbase);
   if (!det) {
      return;
   }
   
   clear_tab();
   const std::map<int, CDetDetector::xtalk_matrix_type>& xmapall = det->get_xtalk_map_all();
   
   typedef std::pair<int, CDetDetector::xtalk_matrix_type> xtalk_pair_type;
   //loop chip id
   
//   m_canvas->Divide(1,1);
//   m_canvas->Clear();
//   m_xtalk_all[0].reset(new TH2F("xtalk_all", "xtalk_all", 128, 0, 128, 128, 0, 128));
//   BOOST_FOREACH(xtalk_pair_type p, xmapall) {
//   
//      //traverse matrix CDetDetector::xtalk_matrix_type
//      for (size_t ii = 0; ii < p.second.size1(); ++ii) {
//         for (size_t jj = 0; jj < p.second.size2(); ++jj) {
//            if (p.second(ii,jj) > 0) {
//               m_xtalk_all[0]->Fill(ii,jj, p.second(ii,jj));
//            }
//            
//         }
//      }
//
//   }
   
   
//   std::multimap<int, int> local_all_xtalk;
   m_local_all_chips_xtalk.clear();
   BOOST_FOREACH(xtalk_pair_type p, xmapall) {

      if (!m_xtalk_all->data[p.first].get()) {
         std::string name = CSrsChipId(p.first).get_string() + "_xtalk";
         m_xtalk_all->data[p.first].reset(new TH2F(name.c_str(), name.c_str(), 128, 0, 128, 128, 0, 128));
         m_xtalk_all->data[p.first]->SetDirectory(0);
         m_canvas->Divide(1,1);
         m_canvas->Clear();
         m_canvas->Divide(4, m_xtalk_all->data.size()/4);
      }
      
      //traverse matrix CDetDetector::xtalk_matrix_type
      for (size_t ii = 0; ii < p.second.size1(); ++ii) {
         typedef boost::numeric::ublas::mapped_matrix<double> xtalk_matrix_type;
         xtalk_matrix_type& m = p.second;
         
         boost::numeric::ublas::matrix_row<xtalk_matrix_type > aRow (m, ii);
         
         boost::numeric::ublas::matrix_row<xtalk_matrix_type >::iterator imax = std::max_element(aRow.begin(), aRow.end());
         if(imax != aRow.end()) {
            m_local_all_chips_xtalk.insert(std::pair<int, int>(ii, imax.index()));
//            std::cout << p.first << ": " << ii << "->" << imax.index() << "(" << *imax << ")\n";
            m_xtalk_all->data[p.first]->Fill(ii, imax.index(), *imax);
         }
         
                
//         for (size_t jj = 0; jj < p.second.size2(); ++jj) {
//            if (p.second(ii,jj) > 0) {
//               m_xtalk_all->data[p.first]->Fill(ii,jj, p.second(ii,jj));
//               std::cout << p.first <<  ": " << ii << "->" << jj << "("  << p.second(ii,jj)<< ")\n";
//            }
//         }
      }
   }
   
   
   
//   for (std::multimap<int, int>::iterator it = local_all_xtalk.begin(); it != local_all_xtalk.end(); ++it) {
//      std::cout <<  "m: " << it->first << "->" << it->second << "\n";
//   }
   
   for (int ii = 0; ii < 128; ++ii) {
      std::pair<std::multimap<int, int>::iterator, std::multimap<int, int>::iterator> rng;
      rng = m_local_all_chips_xtalk.equal_range(ii);
      std::map<int, int> count; // count same ghost channel nuzmbers
      std::map<int, int>::iterator maxitr = count.end(); // most frequent
      for (std::multimap<int, int>::iterator itr = rng.first; itr != rng.second; ++itr) {
         count[itr->second] += 1;
         if (maxitr == count.end()) {
            maxitr = count.begin();
         }
         if(maxitr->second < count[itr->second]) {
            maxitr = count.find(itr->second);
         }
      }
      m_local_all_chips_xtalk.erase(rng.first, rng.second);
      m_local_all_chips_xtalk.insert(std::pair<int,int>(ii, maxitr->first));
   }
   


   
   typedef std::pair<int, boost::shared_ptr<TH2F> > xhist_pair_type;
   BOOST_FOREACH(xhist_pair_type ph, m_xtalk_all->data) {
      m_canvas->cd((ph.first & 0xF) + 1);
      m_xtalk_all->data[ph.first]->DrawCopy("*");
                   
   }
   
//   //do not reset it is constant
//   m_xtalk_all()
//   m_xtalk_32()

   m_canvas->Update();
   
}

void CGuiTabDetectorCrosstalk::gui_handle_create_xtalk_map_file()
{
// print out

   std::cout <<  "APVCh_0 \t APVCH_Ghost \t Correction \n";
  for (std::multimap<int, int>::iterator it = m_local_all_chips_xtalk.begin(); it != m_local_all_chips_xtalk.end(); ++it) {
     std::cout << std::setw(4) << it->first << " \t " <<  std::setw(4) <<  it->second <<   "\t" << std::setw(4) << " xx " << "\n";
  }
   
   
}
