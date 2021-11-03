//
//  GuiTabPlaneAll1D.h
//  browser
//
//  Created by Marcin Byszewski on 12.4.12.
//  Copyright 2012 CERN - PH/ADE. All rights reserved.
//

#ifndef browser_GuiTabPlaneAll1D_h
#define browser_GuiTabPlaneAll1D_h

#include "GuiTab.h"

#include <boost/shared_ptr.hpp>

class TRootEmbeddedCanvas;
class TCanvas;
class TH1F;
class TH2F;
class TGraph;

namespace browser {
   
   class CGuiTabPlaneAll1D : public CGuiTab
   {
      
   public:
      CGuiTabPlaneAll1D(CGuiBase* gui_parent, CConfiguration* config, CEventBrowser* eb, TGTab* main_tabs, const std::string& name);
      virtual ~CGuiTabPlaneAll1D();
      virtual void do_construct_tab();
      virtual void do_update(const CDetBase* entry);
      
   private:
      TRootEmbeddedCanvas*	m_re_canvas; 
      TCanvas*             m_canvas;
      
      std::vector<boost::shared_ptr<TH1F> >   m_qhists;
      std::vector<boost::shared_ptr<TH2F> >   m_tframes;
      std::vector<boost::shared_ptr<TGraph> > m_tgraphs;
      
   };
   
} // namespace

#endif
