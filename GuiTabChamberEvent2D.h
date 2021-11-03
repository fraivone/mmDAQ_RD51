//
//  GuiTabChamberEvent2D.h
//  browser
//
//  Created by Francesco Cirotto  on 29.7.14.
//  Copyright 2012 CERN - PH/ADE. All rights reserved.
//

#ifndef browser_GuiTabChamberEvent2D_h
#define browser_GuiTabChamberEvent2D_h

#include "GuiTab.h"

#include <boost/shared_ptr.hpp>


class TRootEmbeddedCanvas;
class TCanvas;
class TH1F;
class TH2F;
class TGraph;
class TRandom;

namespace browser {
   
   class CGuiTabChamberEvent2D : public CGuiTab
   {
      
   public:
      CGuiTabChamberEvent2D(CGuiBase* gui_parent, CConfiguration* config, CEventBrowser* eb, TGTab* main_tabs, const std::string& name );
      virtual ~CGuiTabChamberEvent2D();
      virtual void do_construct_tab();
      virtual void do_update(const CDetBase* entry);
      virtual void do_clear_tab();
      
   private:
      TRootEmbeddedCanvas*	m_re_canvas; 
      TCanvas*             m_canvas;
      double x = -1;
      double y = -1;
      std::map<std::string, std::vector<double> > RecHit_Collection;
     std::vector<boost::shared_ptr<TH2F> >  m_beamProfile_container;
      
   };


   
} // namespace

#endif
