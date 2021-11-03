//
//  GuiTabDetectorCrosstalk.h
//  eventbrowser
//
//  Created by Marcin Byszewski on 10.5.12.
//  Copyright 2012 CERN - PH/ADE. All rights reserved.
//

#ifndef eventbrowser_GuiTabPedestalCrosstalk_h
#define eventbrowser_GuiTabPedestalCrosstalk_h

#include "GuiTab.h"

//#include <boost/shared_ptr.hpp>
#include <map>

class TRootEmbeddedCanvas;
class TCanvas;
class TH1F;
class TH2F;
class TGraph;
class TGTextButton;


namespace browser {
   
   class XtalkHistMapImpl;
   
   class CGuiTabPedestalCrosstalk  : public CGuiTab
   {
      
   public:
      CGuiTabPedestalCrosstalk(CGuiBase* gui_parent, CConfiguration* config, CEventBrowser* eb, TGTab* main_tabs, const std::string& name);
      virtual ~CGuiTabPedestalCrosstalk();
      virtual void do_construct_tab();
      virtual void do_update(const CDetBase* entry);
      virtual void do_clear_tab();
      
      //gui
      
      void gui_handle_create_xtalk_map_file();
      
   private:
      TRootEmbeddedCanvas*	m_re_canvas; 
      TCanvas*             m_canvas;
      TGTextButton*        m_gui_xtalk_create_map_file;
      
      XtalkHistMapImpl* m_xtalk_all;
      XtalkHistMapImpl* m_xtalk_32;
      std::multimap<int, int> m_local_all_chips_xtalk;

      
//      std::map<int, boost::shared_ptr<TH2F> >   m_xtalk_all; /// ch vs all channels
//      std::map<int, boost::shared_ptr<TH2F> >   m_xtalk_32;  // ch vs channel +/- 32
      
      
   };
   
} // namespace

#endif
