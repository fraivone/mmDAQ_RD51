//
//  EventBrowserOffline.h
//  eventbrowser
//
//  Created by Marcin Byszewski on 1/17/13.
//  Copyright (c) 2013 CERN - PH/ADE. All rights reserved.
//

#ifndef __eventbrowser__EventBrowserOffline__
#define __eventbrowser__EventBrowserOffline__

#include "EventBrowser.h"

#include "Configuration.h"
#include "RootReader.h"

#include "DetTypes.h"

#include <boost/scoped_ptr.hpp>

#include <string>
#include <vector>
#include <iostream>


namespace browser {
   
   class CGuiMainFrame;
   class CDetDetector;
   class DetBase;
   
   
   class CEventBrowserOffline : public CEventBrowser
   {
   public:
      CEventBrowserOffline(boost::program_options::variables_map& options_varmap);
      virtual ~CEventBrowserOffline();
      
      
   private:
      CConfiguration m_config;
      CRootReader    m_reader;
      boost::shared_ptr<CDetDetector>  m_detector;
      CGuiMainFrame*  m_gui;
      DetBasePtrCont m_event_strips; // detector strips of the current event
      DetBasePtrCont m_overlay_strips; // detector strips of the current event
      DetBasePtrCont m_pedestal_strips;
      
      void init();
      virtual void do_open_file();
      virtual void do_load_event_data();
      virtual void do_set_current_entry(size_t entry);
      virtual std::vector<size_t> do_get_current_entry() const;
      virtual size_t do_get_max_entry() const;
      virtual size_t do_get_current_event_number() const ;
      virtual CDetDetector* do_get_detector();
      
      
      
   };
}


#endif /* defined(__eventbrowser__EventBrowserOffline__) */
