//
//  GuiTabDetectorRawUdp.h
//  eventbrowser
//
//  Created by Marcin Byszewski on 4/8/13.
//  Copyright (c) 2013 CERN - PH/ADE. All rights reserved.
//

#ifndef __eventbrowser__GuiTabDetectorRawUdp__
#define __eventbrowser__GuiTabDetectorRawUdp__


#include "GuiTab.h"

#include <boost/shared_ptr.hpp>

#include <map>
#include <stdint.h>

class TRootEmbeddedCanvas;
class TCanvas;
class TH1F;
class TH2F;
class TGraph;

namespace browser {
   
   
   class CGuiTabDetectorEventRawUdp : public CGuiTab
   {
      
   public:
      CGuiTabDetectorEventRawUdp(CGuiBase* gui_parent, CConfiguration* config, CEventBrowser* eb, TGTab* main_tabs, const std::string& name);
      virtual ~CGuiTabDetectorEventRawUdp();
      
      virtual void do_construct_tab();
      virtual void do_update(const CDetBase* entry);
      virtual void do_clear_tab();
      
   protected:
      TRootEmbeddedCanvas*	m_re_canvas;
      TCanvas*             m_canvas;
      typedef std::map<uint32_t, boost::shared_ptr<TGraph> > ChipHistMap;
      ChipHistMap m_rawhist;
   };
   
} // namespace


#endif /* defined(__eventbrowser__GuiTabDetectorRawUdp__) */
