//
//  EventBrowser.h
//  browser
//
//  Created by Marcin Byszewski on 11/15/11.
//  Copyright 2011 CERN - PH/ADE. All rights reserved.
//

#ifndef browser_EventBrowser_h
#define browser_EventBrowser_h

#include "Configuration.h"

#include "DetTypes.h"

#include <boost/scoped_ptr.hpp>

#include <string>
#include <vector>


namespace browser {
   
   class CGuiMainFrame;
   class CDetDetector;
   class DetBase;
   class CRootReader;
   
   class CEventBrowser
   {
   public:
      CEventBrowser() {};
      virtual ~CEventBrowser() {};
      
      void open_file();
      void load_event_data();
      
      void set_current_entry(size_t entry);
      std::vector<size_t> get_current_entry() const;
      size_t get_max_entry() const;
      size_t get_current_event_number() const;

      CDetDetector* get_detector();


      
   private:
      
      /// handle open_file ( or reconnection for shared memory)
      virtual void do_open_file() = 0;

      /// load event data from file or shared memory
      virtual void do_load_event_data() = 0;
      
      /// set selection 
      virtual void do_set_current_entry(size_t entry) = 0;
      virtual std::vector<size_t> do_get_current_entry() const = 0;
      virtual size_t do_get_max_entry() const = 0;
      virtual size_t do_get_current_event_number() const = 0;
      virtual CDetDetector* do_get_detector() = 0;

   };
   
   
   
   
   inline CDetDetector* CEventBrowser::get_detector()
   {
      return do_get_detector();
   }

      
   inline void CEventBrowser::open_file()
   {
      do_open_file();
   }
   
   inline void CEventBrowser::set_current_entry(size_t entry)
   {
      do_set_current_entry(entry);
   }
   
   
   inline std::vector<size_t> CEventBrowser::get_current_entry() const
   {
      return do_get_current_entry();
   }
   
   inline size_t CEventBrowser::get_max_entry() const
   {
      return do_get_max_entry();
   }
   
   
   inline size_t CEventBrowser::get_current_event_number() const
   {
      return do_get_current_event_number();
   }
   
   /**
    parse root file trees and create all data to be passed to the detector
    */
   inline void CEventBrowser::load_event_data()
   {
      do_load_event_data();
   }
   
   
} // namespace

#endif
