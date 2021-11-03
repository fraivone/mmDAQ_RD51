//
//  GuiTabDetectorStats.h
//  browser
//
//  Created by Francesco Cirotto  on 29.7.14.
//  Copyright 2012 CERN - PH/ADE. All rights reserved.
//

#ifndef browser_GuiTabDetectorStats_h
#define browser_GuiTabDetectorStats_h

#include "GuiTab.h"

#include <boost/shared_ptr.hpp>


class TRootEmbeddedCanvas;
class TCanvas;
class TH1F;
class TH2F;
class TGraph;

namespace browser {
   
   class CGuiTabDetectorStats : public CGuiTab
   {
      
   public:
      CGuiTabDetectorStats(CGuiBase* gui_parent, CConfiguration* config, CEventBrowser* eb, TGTab* main_tabs, const std::string& name );
      virtual ~CGuiTabDetectorStats();
      virtual void do_construct_tab();
      virtual void do_update(const CDetBase* entry);
      virtual void do_clear_tab();
      
   private:
      TRootEmbeddedCanvas*	m_re_canvas; 
      TCanvas*             m_canvas;
      //  TH1F*                m_hitmap;
      //TH1F*                m_qmax;
      //TH1F*                m_maxqmax;
      //TH1F*                m_tbqmax;
      //TH1F*                m_tbmaxqmax;
      //TH1F*                m_nstrips;
      //TH1F*                m_lumi;
 
     std::vector<boost::shared_ptr<TH1F> >   m_hitmap_container;
     std::vector<boost::shared_ptr<TH1F> >  m_tbqmax_container;
      
   };


   
} // namespace

#endif
