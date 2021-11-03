//
//  GuiTabChamberEvent1D.h
//  eventbrowser
//
//  Created by Marcin Byszewski on 31/10/2012.
//  Copyright 2012 CERN - PH/ADE. All rights reserved.
//

#ifndef eventbrowser_GuiTabChamberEvent1D_h
#define eventbrowser_GuiTabChamberEvent1D_h

#include "GuiTab.h"

#include <boost/shared_ptr.hpp>

#include <map>

class TRootEmbeddedCanvas;
class TCanvas;
class TH1F;
class TH2F;
class TGraph;

namespace browser {
   
   
   class CGuiTabChamberEvent1D : public CGuiTab
   {
      
   public:
      CGuiTabChamberEvent1D(CGuiBase* gui_parent, CConfiguration* config, CEventBrowser* eb, TGTab* main_tabs, const std::string& name);
      virtual ~CGuiTabChamberEvent1D();
      
      virtual void do_construct_tab();
      virtual void do_update(const CDetBase* entry);
      virtual void do_clear_tab();
      
   protected:
      TRootEmbeddedCanvas*	m_re_canvas;
      TCanvas*             m_canvas;
      
      std::vector<boost::shared_ptr<TH1F> >   m_qdisp;
      std::vector<boost::shared_ptr<TH2F> >   m_tdisp;
   };
   
} // namespace

#endif
