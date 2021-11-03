//
//  GuiTabPedestal.h
//  eventbrowser
//
//  Created by Marcin Byszewski on 5.5.12.
//  Copyright 2012 CERN - PH/ADE. All rights reserved.
//

#ifndef eventbrowser_GuiXmlCreator_h
#define eventbrowser_GuiXmlCreator_h

#include "GuiTab.h"
#include <vector>
#include "GuiMainFrame.h"

class TGRadioButton;
class TGCheckButton;
class TGVButtonGroup;
class TGTextButton;
class TGNumberEntry;
class CDetConfig;
namespace browser {
   
    class CDetBase;
   	class CGuiTabFec;
    class CEventBrowser;
    class CEventBrowserOnline;
	
	
   class CGuiTabXml : public CGuiTab
   {
      
	  
	  
   public:
      CGuiTabXml() : CGuiTab(), m_last_detbase(0) {};
      CGuiTabXml(CGuiBase* gui_parent, CConfiguration* config, CEventBrowser* m_eb, TGTab* main_tabs, const std::string& name);
      virtual ~CGuiTabXml();
      //CEventBrowserOnline* m_eb;
	  virtual void do_construct_tab();
      virtual void do_update(const CDetBase* entry);
      virtual void do_clear_tab();

    
	 
      //handlers
      void update_right_tab();

	  
   private:
      const CDetBase* m_last_detbase;
	  void save_xml_file(const std::string& filename);
       TGTextButton*                 save_button;
	   TGTextButton*                 fec_button;
	    TGMainFrame*       m_mainframe;
		
	   
   };
   
} // namespace

#endif