//
//  EventBrowserOnline.h
//  eventbrowser
//
//  Created by Marcin Byszewski on 1/17/13.
//  Copyright (c) 2013 CERN - PH/ADE. All rights reserved.
//

#ifndef __eventbrowser__EventBrowserOnline__
#define __eventbrowser__EventBrowserOnline__

#include "EventBrowser.h"
#include "ShmemReader.h"
#include "RootReader.h"

#include "Configuration.h"


#include "DetTypes.h"

#include <boost/asio.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread/mutex.hpp>


#include <string>
#include <vector>
#include <iostream>

namespace MBTools {
  class AsioDeadlineTimer; 
};

namespace browser {
   
   class CGuiMainFrame;
   class CDetDetector;
   class DetBase;
   class CDaqClient;
   
   class CEventBrowserOnline : public CEventBrowser
   {
   public:
      CEventBrowserOnline(boost::program_options::variables_map& options_varmap);
      virtual ~CEventBrowserOnline();

      //public for timer
      void read_new_online_data();
      CDaqClient* get_daq_client() {
         return m_daq_client.get();
      }
      void set_pedestal_filename(std::string val) {m_pedestal_file = val;}
      void set_new_pedestal_flag(bool b) {m_new_pedestal = b;}

      
   private:
      ///
      CConfiguration m_config;
      
      boost::scoped_ptr<MBTools::AsioDeadlineTimer> m_read_timer;
      /// shared memory reader, with own thread (use conditions )
      CShmemReader m_shm_reader;
      CRootReader m_r_reader;

      
      boost::scoped_ptr<CDaqClient> m_daq_client;
      
      boost::shared_ptr<CDetDetector>  m_detector;
      CGuiMainFrame*  m_gui;
      DetBasePtrCont m_event_raw;
      DetBasePtrCont m_event_strips; ///< detector strips of the current event
      DetBasePtrCont m_overlay_strips; ///< detector strips of the current event
      DetBasePtrCont m_pedestal_strips; ///< pedestal strips of the current event
      boost::timed_mutex m_mutex;
      
      bool m_new_event_data; ///< new data read but not displayed;
      bool m_new_pedestal;
      std::string m_pedestal_file;
      
      void init();
      virtual void do_open_file();
      virtual void do_load_event_data();
      virtual void do_set_current_entry(size_t entry);
      virtual std::vector<size_t> do_get_current_entry() const;
      virtual size_t do_get_max_entry() const;
      virtual size_t do_get_current_event_number() const ;
      virtual  CDetDetector* do_get_detector() ;

      

   };
   
}; // namespace


#endif /* defined(__eventbrowser__EventBrowserOnline__) */
