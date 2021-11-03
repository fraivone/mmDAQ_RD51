//
//  ShmemReader.h
//  eventbrowser
//
//  Created by Marcin Byszewski on 1/18/13.
//  Copyright (c) 2013 CERN - PH/ADE. All rights reserved.
//

#ifndef __eventbrowser__ShmemReader__
#define __eventbrowser__ShmemReader__

#include "ReaderBase.h"
#include "DetTypes.h"

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

#include <iostream>
#include <vector>
#include <map>

namespace browser {
   
   class CConfiguration;
   class CDetDetector;
   struct SPublisherIpcData;
   
   /// Reader of shared memory by DAQ server
   /// 
   class CShmemReader : public CReaderBase
   {
      
   public:
      CShmemReader(CConfiguration const& config);
      virtual ~CShmemReader();
      
      void restart();
      
      bool get_flag_new_event_data() const;
      static unsigned n_tbins; 
      static std::string run_name;
      static std::string run_filename;


      std::map<uint32_t, std::vector<int16_t> > const& get_event_udp_raw_data() const;

//      //to remove?
//      std::vector<size_t> get_tree_current_entries() const;
//      size_t get_max_entry() const;
//      size_t get_current_event_number() const;
      

   private:
      
      virtual void do_open_file(const std::string& filename);
      virtual void do_close_file();
      virtual TEnv* do_get_env();
      
      virtual const std::string& do_get_daq_config_xml() const;
      virtual const std::string& do_get_srs_config_xml() const;
      virtual const std::string& do_get_det_config_xml() const;
      virtual const std::map<std::string, std::string>& do_get_det_config_xml_chambers() const;
      virtual std::string do_load_xml_config(const std::string& name);
      
      
      virtual void do_set_tree_current_entry(size_t entry);
      virtual std::vector<size_t> do_get_tree_current_entries() const;
      virtual size_t do_get_tree_max_entry() const;
      virtual size_t do_get_current_event_number() const;
      virtual size_t do_get_timebin_number() const;
      
      virtual DetBasePtrCont do_load_event_data_raw(CDetDetector* det);
      virtual DetBasePtrCont do_load_event_data_overlay(CDetDetector* det);
      virtual DetBasePtrCont do_load_event_data_pedestal(CDetDetector* det);
      virtual void do_parse_trees(CDetDetector* det, CGuiMainFrame*  gui);

      void clear();
      void init();
      void connect_shared_memory();
      
      void read_shared_configuration_data();
      void read_shared_receiver_data();
      void read_shared_network_device_data();
      void read_shared_writer_data();
      void read_shared_event_data(DetBasePtrCont& raw_strips);
      void read_shared_event_udp_raw_data( std::map<uint32_t, std::vector<int16_t> >& raw_udp);
      void read_shared_event_number();
      void read_timebin_number();
      void read_shared_filename();

      /// shared memory for raw and event data 
      boost::scoped_ptr<boost::interprocess::managed_shared_memory> m_shm_manager;

      
      
      /// memory shared by DAQ server publisher 
      typedef boost::interprocess::shared_memory_object IpcShmemObjectType;
      boost::scoped_ptr<IpcShmemObjectType> m_shm_object;
      
      /// memory shared by DAQ server
      typedef boost::interprocess::mapped_region IpcShmemMappedRegion;
      boost::scoped_ptr<IpcShmemMappedRegion> m_shm_mapped_region;
      
      /// data structure shared by DAQ server
      SPublisherIpcData* m_shm_shared_data;
      
      /// The io_service used to perform asynchronous operations.
      boost::asio::io_service m_io_service;
      
      ///keep service from terminating with idle work for service
      boost::shared_ptr<boost::asio::io_service::work> m_idle_work;
      
      ///worker thread
      boost::thread m_thread;
      
      boost::mutex m_access_mutex;
      
      CConfiguration const& m_config;
      std::string m_daq_config_xml;
      std::string m_srs_config_xml;
      std::string m_det_config_xml;
      std::map<std::string,std::string> m_det_config_xml_chambers;
      
      bool m_stop_reading;
      
      // count failed attemts to read shared memory
      int m_shmem_error_counter;
      size_t       m_current_event_number;
      size_t m_n_tbins;
      bool m_flag_new_event_data;
      
      ///event udp data from (APV25) chips
      std::map<uint32_t, std::vector<int16_t> > m_event_raw_udp;
   };
   
   
}
#endif /* defined(__eventbrowser__ShmemReader__) */
