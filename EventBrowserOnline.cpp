//
//  EventBrowserOnline.cpp
//  eventbrowser
//
//  Created by Marcin Byszewski on 1/17/13.
//  Copyright (c) 2013 CERN - PH/ADE. All rights reserved.
//

#include "EventBrowserOnline.h"
#include "GuiMainFrameOnline.h"
#include "RootThread.h"

#include "DetDetector.h"
#include "DetectorBuilder.h"
#include "AsioDeadlineTimer.h"
#include "DaqClientIpc.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>

#include <iostream>
#include <stdexcept>

using namespace browser;
using namespace MBTools;

CEventBrowserOnline::CEventBrowserOnline(boost::program_options::variables_map& options_varmap) :
CEventBrowser(),
m_config(options_varmap),
m_read_timer(new AsioDeadlineTimer(330,boost::bind(&CEventBrowserOnline::read_new_online_data, this))),
m_shm_reader(m_config), //shared memory reader
m_r_reader(m_config), //root file reader, placed here for pedestal
m_daq_client(new CDaqClientIpc() ),
m_detector(),
m_gui(0),
m_event_raw(),
m_event_strips(),
m_overlay_strips(),
m_pedestal_strips(m_r_reader.load_event_data_pedestal(0)),
m_mutex(),
m_new_event_data(false),
m_pedestal_file()
//new_pedestal(false)

{
   init();
   std::cout << "CEventBrowserOnline() timer run \n";
   m_gui = new CGuiMainFrameOnline(this, &m_config);
   m_read_timer->run(); // restart the timer, shared data will be read

}


CEventBrowserOnline::~CEventBrowserOnline()
{
   delete m_gui;
}

void CEventBrowserOnline::init()
{
//   std::cout << "----- CEventBrowserOnline::init() -----\n";


   //TODO: we need to lock here (or stop use of detecto altogether before) !!!!
   
   //build detector from shared xml
   //can be called many refresh of config at daq server
   //TODO: clear statistics too !
   
//   std::cout << "CEventBrowserOnline::init() timer stop \n";
//   try {
//      m_read_timer->stop(); //stop timer that drives the readout
//
//   } catch(std::exception &ex) {
//      std::cout << "CEventBrowserOnline::init() except: " << ex.what();
//
//   }
   
//   std::cout << "CEventBrowserOnline::init() m_shm_reader.restart \n";
   m_shm_reader.restart(); //reconect to shared memory and restart reading (on condition notification)
   m_new_pedestal = false;
//   std::cout << "CEventBrowserOnline::init() builder \n";
   CDetectorBuilder builder(&m_shm_reader, (CGuiMainFrame*)0 ); // make a new detector from config files

//   std::cout << "CEventBrowserOnline::init() builder.get_detector() \n";

   m_detector = builder.get_detector();

//   std::cout << "CEventBrowserOnline::init() timer run \n";
//   m_read_timer->run(); // restart the timer, shared data will be read
   
//   std::cout << "----- CEventBrowserOnline::init() ok ---\n";

}


///unused
void CEventBrowserOnline::read_new_online_data()
{
//   std::cout << "----- timed: CEventBrowserOnline::read_new_online_data() -----\n";
//   set_current_entry(0);//always zero , event number will be read by shmemreader
   
   do_load_event_data();
   
}


//called by timed gui to display data in reader
void CEventBrowserOnline::do_set_current_entry(size_t entry)
{
   //   std::cout << "----- CEventBrowserOnline::do_set_current_entry() -----\n";
   //   m_shm_reader.set_tree_current_entry(entry); //no-op
//   load_event_data();
  
   
   if(!m_detector) {
      std::cout << "CEventBrowserOnline::do_set_current_entry() no detector \n";
      return;
   }
   

   try {
      if (!m_shm_reader.get_flag_new_event_data()) {
         return;
      }
      
//      if(m_event_strips.empty()) {
//         return;
//      }
      
      boost::unique_lock<boost::timed_mutex> lockgrd(m_mutex, boost::defer_lock);
      if(!lockgrd.timed_lock(boost::posix_time::milliseconds(100)) ) {
         std::cout << "CEventBrowserOnline::do_set_current_entry() failed lock \n";
         return;
      }
      
      if(m_new_event_data) {
//      if(m_shm_reader.get_flag_new_event_data()) {
         m_detector->lock();
         m_detector->clear_event_data();
         std::for_each(m_event_strips.begin(), m_event_strips.end(), boost::bind(&CDetBase::set_detector, _1, m_detector.get()));
         //TODO: \todo CEventBrowserOnline add: rd->add_entry_to_stats(evt_time)
         m_detector->load_event_data(m_event_strips, m_overlay_strips);
         m_detector->set_event_raw_udp_data(m_shm_reader.get_event_udp_raw_data());
         m_detector->add_loaded_event_to_stats();
         m_new_event_data = false;
         m_detector->unlock();
      }
      
      
   } catch (std::runtime_error& re) {
      m_detector->unlock();
      // if no shared data try reopenig shared memory open file
      std::cout << "CEventBrowserOnline::do_set_current_entry() caught error  '" << re.what() << "' ->open_file() \n";
      open_file();
   }
   
   

}

/**
 parse shared data and create all data to be passed to the detector
 TODO: add to statistics too !
 */
void CEventBrowserOnline::do_load_event_data()
{
//   std::cout << "----- CEventBrowserOnline::do_load_event_data() -----\n";

   if(!m_detector) {
      std::cout << "CEventBrowserOnline::do_load_event_data() no detector -> open_file \n";
      open_file();
      return;
   }
   
   if(m_new_pedestal){
    m_r_reader.open_file(m_pedestal_file);
    m_pedestal_strips = m_r_reader.load_event_data_pedestal(m_detector.get());    
    m_new_pedestal = false;
    std::cout<<" CEventBrowserOnline::do_load_event_data() New pedestal has been loaded: "<<m_pedestal_file<<std::endl;            
    }
    
    m_detector->load_pedestal_data(m_pedestal_strips);
   
   try {
      boost::unique_lock<boost::timed_mutex> lockgrd(m_mutex, boost::defer_lock);
      
      if(lockgrd.timed_lock(boost::posix_time::time_duration(boost::posix_time::milliseconds(100)))) {
         m_event_strips.clear();
         m_overlay_strips.clear();
         DetBasePtrCont strips(m_shm_reader.load_event_data_raw(0));
         if(!strips.empty()) {
            m_event_strips = strips;
//          std::cout << "CEventBrowserOnline::do_load_event_data() strips "<< m_event_strips.size() << "\n";
            m_new_event_data = true;
         }

         
      }
      
   } catch (std::runtime_error& re) {
//      m_detector->unlock();
      // if no shared data try reopenig shared memory open file
      std::cout << "CEventBrowserOnline::do_load_event_data() caught error  '" << re.what() << "' ->open_file() \n";
      open_file();
   }
   
   
}




 CDetDetector* CEventBrowserOnline::do_get_detector() 
{
   return m_detector.get();
}


//called on change of run with new / reloded shared config
//aka file change
void CEventBrowserOnline::do_open_file()
{
//   std::cout << "----- CEventBrowserOnline::do_open_file() -----\n";
   boost::lock_guard<boost::timed_mutex> lockgrd(m_mutex);

   try {

      init();
      

      if(m_gui) {
         m_gui->gui_handle_load_file();
      }
      else {
         std::cerr << "ERROR: CEventBrowserOnline::do_open_file() no gui" << std::endl;

      }
      
   } catch (std::runtime_error& re) {
      std::cerr << "ERROR: " << re.what() << std::endl;
      //reset cuurent file in config,
   }
}





std::vector<size_t> CEventBrowserOnline::do_get_current_entry() const
{
   return m_shm_reader.get_tree_current_entries();
}


size_t CEventBrowserOnline::do_get_max_entry() const
{
   return 0;
}


size_t CEventBrowserOnline::do_get_current_event_number() const
{
   return m_shm_reader.get_current_event_number();
}




