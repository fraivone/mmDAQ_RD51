//
//  GuiTabDetectorPedestal1D.h
//  eventbrowser
//
//  Created by Nikos Tsirintanis on 22/07/2014.
//  Copyright 2014 CERN - PH/ADE. All rights reserved.
//

#ifndef eventbrowser_GuiTabDetectorPedestal1D_h
#define eventbrowser_GuiTabDetectorPedestal1D_h

#include "GuiTab.h"

#include <boost/shared_ptr.hpp>

#include <map>

class TRootEmbeddedCanvas;
class TCanvas;
class TH1F;
class TH2F;
class TGraph;

namespace browser {
   
   
   class CGuiTabDetectorPedestal1D : public CGuiTab
   {
      
   public:
      CGuiTabDetectorPedestal1D(CGuiBase* gui_parent, CConfiguration* config, CEventBrowser* eb, TGTab* main_tabs, const std::string& name);
      virtual ~CGuiTabDetectorPedestal1D();
      
      virtual void do_construct_tab();
      virtual void do_update(const CDetBase* entry);
      virtual void do_clear_tab();
      
   protected:
      TRootEmbeddedCanvas*	m_re_canvas;
      TCanvas*             m_canvas;
      
      std::vector<boost::shared_ptr<TH2F> >   m_qdisp;
      std::vector<boost::shared_ptr<TH2F> >   m_tdisp;
   };
   
} // namespace

#endif
