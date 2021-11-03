//
//  ShmemReader.cpp
//  eventbrowser
//
//  Created by Marcin Byszewski on 1/18/13.
//  Copyright (c) 2013 CERN - PH/ADE. All rights reserved.
//
//  Updated by N. Tsirintanis, E. Farina, F. Cirotto
 
#include "ShmemReader.h"
#include "PublisherIpcData.h"
#include "SharedTypes.h"
#include "SharedTypeChannelData.h"
#include "SharedTypeReceiverData.h"
#include "SharedTypeFecData.h"
#include "SharedTypeWriterData.h"
#include "SharedTypeChannelStripMap.h"

#include "SrsChipId.h"
#include "DetStripId.h"
#include "DetStrip.h"
#include "DetDetector.h"

#include <boost/foreach.hpp>
#include <boost/interprocess/exceptions.hpp>
#include <boost/bind.hpp>

#include <algorithm>
#include <ostream>
#include <iostream>



struct ChannelData {
   ChannelData(mmdaq3::server::ShmemChannelData const& shdata) :
   m_time(shdata.m_time), m_charge(shdata.m_charge)
//   ,
//   m_raw(shdata.m_raw.begin(),shdata.m_raw.end())
   
   {
   
   }
   
   double m_time;
   double m_charge;
//   std::vector<int16_t> m_raw;
};

using namespace browser;

namespace bptm = boost::posix_time;
namespace bipc = boost::interprocess;
namespace bas = boost::asio;

unsigned CShmemReader::n_tbins = 1;
std::string CShmemReader::run_name = "0";
std::string CShmemReader::run_filename = "";

CShmemReader::CShmemReader(CConfiguration const& config) :
m_shm_manager(),
m_shm_object(),
m_shm_mapped_region(),
m_shm_shared_data(0),
m_io_service(),
m_idle_work(new bas::io_service::work(m_io_service)),
m_thread(boost::bind(&boost::asio::io_service::run, &m_io_service)),
m_access_mutex(),

m_config(config),
m_daq_config_xml(),
m_srs_config_xml(),
m_det_config_xml(),
m_det_config_xml_chambers(),
m_stop_reading(false),
m_shmem_error_counter(0),
m_current_event_number(0),
m_flag_new_event_data(0),
m_event_raw_udp(),
m_n_tbins(1)
{
   
   //TODO: ignore new-config-flag on start up - we need any info
   restart();
}

CShmemReader::~CShmemReader()
{
   
   m_shm_object.reset();
   m_shm_manager.reset();
   m_idle_work.reset();
//   m_thread.interrupt();
}

void CShmemReader::clear()
{
   m_daq_config_xml.clear();
   m_srs_config_xml.clear();
   m_det_config_xml.clear();
   m_det_config_xml_chambers.clear();
   m_event_raw_udp.clear();
   m_current_event_number = 0;
   m_shmem_error_counter = 0;
   m_flag_new_event_data = 0;
   
}

/// reconnect to shared memory (aka open file?)
void CShmemReader::restart()
{
   std::cout << "----- CShmemReader::restart() -----\n";

   // stop the reading_loop and lock
   m_stop_reading = true;

   m_current_event_number = 0;
   m_shmem_error_counter = 0;
   m_flag_new_event_data = 0;
   
   boost::lock_guard<boost::mutex> access_lock(m_access_mutex);

   init();
}

void CShmemReader::init()
{
   std::cout << "----- CShmemReader::init() -----\n";

   //can this be just rerun if m_shm_object is still valid?
   //TODO:
   // -> stop service thread if running
   clear();
   connect_shared_memory();
}

void CShmemReader::connect_shared_memory()
{
   std::cout << "----- CShmemReader::connect_shared_memory() -----\n";

   try{
      
      
      /// shared memory manager for event data
      boost::scoped_ptr<bipc::managed_shared_memory> smng (new bipc::managed_shared_memory(bipc::open_only, "mmDaqSharedMemory"));
      m_shm_manager.swap(smng);
      
      
      /// shared object / mapped memory for sync
      boost::scoped_ptr<IpcShmemObjectType> obj(new
                                                IpcShmemObjectType(bipc::open_only,
                                                                   "IPCPublisherSharedMemory" ,
                                                                   bipc::read_write));
      m_shm_object.swap(obj);
      
      
      //Map the whole shared memory in this process
      boost::scoped_ptr<IpcShmemMappedRegion> reg(new bipc::mapped_region(*m_shm_object,
                                                                          bipc::read_write ));
      
      m_shm_mapped_region.swap(reg);
      
      
      //Get the address of the mapped region
      void * addr = m_shm_mapped_region->get_address();
      
      //Construct the shared structure in memory
      m_shm_shared_data = static_cast<SPublisherIpcData*>(addr);
      
      
      //reset semaphores
      while(m_shm_shared_data->sem_client_finished.try_wait()) {
         std::cout << "CPublisherIpc::connect_shared_memory() sem_client_finished try_wait() " << std::endl;
      }; //reset to 0
      while(m_shm_shared_data->sem_mutex.try_wait()) {
         std::cout << "CPublisherIpc::connect_shared_memory() sem_mutex try_wait() " << std::endl;
      }; //reset to 0
      
      read_shared_configuration_data();
      
      m_shm_shared_data->flag_new_config = false;
      m_shm_shared_data->sem_mutex.post();
      m_shm_shared_data->sem_client_finished.post();
//      std::cout << "CShmemReader() connect_shared_memory() --> m_shm_shared_data->flag_new_config = false;" << std::endl;
      

   }
   
   catch(...) {
      std::cout << "connect_shared_memory(): mmdaq3 not detected ..." << std::endl;
      m_shm_shared_data = 0;
      m_shm_object.reset();
//      boost::this_thread::sleep(boost::posix_time::milliseconds(++m_shmem_error_counter));

   }
   m_shmem_error_counter = 0;
}


//restart
void CShmemReader::do_open_file(const std::string&)
{
   std::cout << "----- CShmemReader::do_open_file() -----\n";

   restart();
}

void CShmemReader::do_close_file()
{
   
}

TEnv* CShmemReader::do_get_env()
{
   return 0;
}

const std::string& CShmemReader::do_get_daq_config_xml() const
{
   return m_daq_config_xml;
}

const std::string& CShmemReader::do_get_srs_config_xml() const
{
   return m_srs_config_xml;
}

const std::string& CShmemReader::do_get_det_config_xml() const
{
   return m_det_config_xml;
}

const std::map<std::string, std::string>& CShmemReader::do_get_det_config_xml_chambers() const
{
   return m_det_config_xml_chambers;
}

std::string CShmemReader::do_load_xml_config(const std::string& name)
{
//   std::cout << "unimlemented CShmemReader::do_load_xml_config(" << name << ")\n"; //chamber xml
   //make [xmlname]->xmldata map
   return m_det_config_xml_chambers[name];
}



void CShmemReader::do_set_tree_current_entry(size_t )
{
   // rootreader here calls GetEntry()
   // we could set event number now
}

std::vector<size_t> CShmemReader::do_get_tree_current_entries() const
{
   return std::vector<size_t>(1, 8888);
}

size_t CShmemReader::do_get_tree_max_entry() const
{
   return 9999;
}

size_t CShmemReader::do_get_current_event_number() const
{
   return m_current_event_number;
}

size_t CShmemReader::do_get_timebin_number() const //Francesco
{
   return m_n_tbins;
}
//read new data from mem, into detector
/// read raw trees, create strips and put them into strip container
DetBasePtrCont CShmemReader::do_load_event_data_raw(CDetDetector* det)
{

    
//   std::cout << "----- CShmemReader::do_load_event_data_raw() -----\n";
   
   if (!m_shm_manager) {
//      boost::this_thread::sleep(boost::posix_time::milliseconds(++m_shmem_error_counter));
      std::cout << "CShmemReader::do_load_event_data_raw(): no shared memory\n";
      throw std::runtime_error("CShmemReader::do_load_event_data_raw(): no shared memory");
   }
   
   // TODO: lock  this map to copy data
   if (!m_shm_shared_data) {
      std::cout << "----- CShmemReader::do_load_event_data_raw() shared memory not set -----\n";
      throw std::runtime_error("CShmemReader::do_load_event_data_raw(): m_shm_shared_data = NULL");
   }
   
   if(m_shm_shared_data->flag_new_config) {
      std::cout << "----- CShmemReader::do_load_event_data_raw() new config found -----\n";
//      restart();
      throw std::runtime_error("CShmemReader::do_load_event_data_raw(): new config flag");
   }

    // std::cout << "CShmemReader::read_timebin_number() = " << m_n_tbins << "" << std::endl;
   
   //check for new server data
//   if(!m_shm_shared_data->sem_server_stored.timed_wait(bptm::second_clock::universal_time() + bptm::milliseconds(1000)))
//   {
//      std::cout << "----- CShmemReader::do_load_event_data_raw() sem_server_stored timeout = no new data-----\n";
//      //timeout, no new data from server
//      m_shm_shared_data->sem_client_finished.post(); // client has finished
//      return raw_strips;
//   }
   
   
   DetBasePtrCont raw_strips;

   if(!m_shm_shared_data->flag_new_event_data) {
//       std::cout << "----- CShmemReader::do_load_event_data_raw() NO new data -----\n";
      return raw_strips;
   }
   
   m_shm_shared_data->sem_server_stored.wait();
   
   
   //lock data
   bptm::ptime timeout(bptm::microsec_clock::universal_time() + bptm::milliseconds(200));
   if(!m_shm_shared_data->sem_mutex.timed_wait(timeout)) {
      //timeout on semaphore
      return raw_strips;
   }
   //we are now processing the data -> client not finished
   
//   while(m_shm_shared_data->sem_client_finished.try_wait()) {
//      std::cout << "CPublisherIpc::update_published(MonitoredDataEvent) sem_client_finished try_wait() " << std::endl;
//   }; //reset to 0
   read_shared_filename();

   m_shm_shared_data->flag_new_event_data = false;
   read_shared_receiver_data(); // move out ?
   read_shared_network_device_data(); // move out?
   read_shared_writer_data();
   read_shared_event_number();
   read_timebin_number();
   if(m_flag_new_event_data) {
      read_shared_event_data(raw_strips);      
      read_shared_event_udp_raw_data(m_event_raw_udp);
   }
   
   m_shm_shared_data->sem_mutex.post();//unlock
   m_shm_shared_data->sem_client_finished.post(); // client has finished
   return raw_strips;
}

std::map<uint32_t, std::vector<int16_t> > const& CShmemReader::get_event_udp_raw_data() const
{
   return m_event_raw_udp;
}


void CShmemReader::read_shared_configuration_data()
{
   std::cout << "CShmemReader::read_shared_configuration_data()"<< std::endl;
   
   std::pair<mmdaq3::server::ShmemCharStringVector*, bipc::managed_shared_memory::size_type> res;
   res = m_shm_manager->find<mmdaq3::server::ShmemCharStringVector> ("mmDaqSharedConfigXml");
   
   if(res.second == 1)
   {
      mmdaq3::server::ShmemCharStringVector* shm_config_strings = res.first;
      if (res.first->size() >= 3) {
//         mmdaq3::server::ShmemCharString const& str = (*shm_config_strings)[0];
//
         m_daq_config_xml = std::string((*shm_config_strings)[0].c_str(),
                                        (*shm_config_strings)[0].size());
         m_srs_config_xml = std::string((*shm_config_strings)[1].c_str(),
                                        (*shm_config_strings)[1].size());
         m_det_config_xml = std::string((*shm_config_strings)[2].c_str(),
                                        (*shm_config_strings)[2].size());
         
         
         for (size_t ii = 3; ii < res.first->size()-1; ii+=2) {
            std::string name((*shm_config_strings)[ii].c_str(),
                             (*shm_config_strings)[ii].size());
            std::string xmldata((*shm_config_strings)[ii+1].c_str(),
                                (*shm_config_strings)[ii+1].size());
            m_det_config_xml_chambers[name] = xmldata;
         }
         
      }
//      else {
//         std::cout << "CShmemReader::read_shared_configuration_data() error vec : shm_config_strings size != 3" << res.first->size() << std::endl;
//      }
   }
//   else {
//      std::cout << "CShmemReader::read_shared_configuration_data() no shared config" << std::endl;
//   }
}

void CShmemReader::read_shared_receiver_data()
{
 
   std::pair<mmdaq3::server::ShmemReceiverDataVector*, bipc::managed_shared_memory::size_type> res = m_shm_manager->find<mmdaq3::server::ShmemReceiverDataVector> ("mmDaqSharedReceiverData");
   if(res.second == 1)
   {
      mmdaq3::server::ShmemReceiverDataVector* shvec = res.first;
      
      std::cout << "ShmemReceiverDataVector vec : " << shvec->size() << std::endl;
      
      for (mmdaq3::server::ShmemReceiverDataVector::const_iterator it = shvec->begin();
           it != shvec->end(); ++it) {
         //fill local data
         std::cout << "Recv: " << boost::asio::ip::address_v4(it->m_local_ip).to_string();
         
         /// run state (error, idle, running)..
         std::cout << " s=" << it->m_run_state;
         
         /// frame counter
         std::cout << " f=" << it->m_run_frame_counter;
         
         /// total number of bytes received
         std::cout << " kB=" << it->m_run_bytes_total/1024.0;
         
         /// rate in frames/s
         std::cout << " f/s==" << it->m_frame_rate ;
         
         /// rate in B/s
         std::cout << " kB/s=" << it->m_data_rate/1024.0;
         std::cout << "\n";
      }
   }
   else {
      
   }
   
}

void CShmemReader::read_shared_filename()
{
    std::pair<mmdaq3::server::ShmemCharString *, bipc::managed_shared_memory::size_type> res = m_shm_manager->find<mmdaq3::server::ShmemCharString> ("daq_run_filename");
    
    if(res.second == 1)
    {

        mmdaq3::server::ShmemCharString* run_file_name = res.first;
        std::stringstream ss;
        ss << run_file_name->c_str();
        run_filename = ss.str();
        std::cout << run_filename << std::endl;
    }
    else {
    
    }

}


void CShmemReader::read_shared_network_device_data()
{
 
   std::cout << "CShmemReader::read_shared_network_device_data() \n";
   std::pair<mmdaq3::server::ShmemFecDataVector *, bipc::managed_shared_memory::size_type> res = m_shm_manager->find<mmdaq3::server::ShmemFecDataVector> ("mmDaqSharedFecData");
   if(res.second == 1)
   {
      mmdaq3::server::ShmemFecDataVector* shvec = res.first;
      
      //std::cout << "ShmemFecDataVector vec : " << shvec->size() << std::endl;
      
      
      for (mmdaq3::server::ShmemFecDataVector::const_iterator it = shvec->begin();
           it != shvec->end(); ++it) {
         //fill local data
         std::cout << "SrsNetDev: n=" << it->name.c_str();
         std::cout << " ip=(" <<  (it->remote_ip?"":boost::asio::ip::address_v4(it->remote_ip).to_string());
         std::cout << ") buf=" << it->buffer_size;
         std::cout << " fr=" << it->frame_counter;
         std::cout << " le=" << it->last_evid ;
         if (it->reserved.size() == 4) {
            std::cout << " fafa=" << it->reserved[0] ;
            std::cout << " e_hdr=" << it->reserved[1] ;
            std::cout << " e_udp=" << it->reserved[2] ;
            std::cout << " e_evt=" << it->reserved[3] ;
         }
         std::cout << "\n";


//         //debug printout chips
//         BOOST_FOREACH(mmdaq3::server::ShmemChipDataMap::value_type c, it->chips) {
//            std::cout << "  - chip=" << c.second->name;
//            std::cout << " id=" << c.second->chip_id << "'"<< CSrsChipId(c.second->chip_id).get_string() << "' ";
//            //mapping channel-strip, strip-channel (const?)
//            //         boost::shared_ptr<CDetConnector> m_connector;
//            ///pedestals (not all chips)
//            //         ApvPedestalMap m_pedestals_map
//            ///event number written by EB/RW
//            std::cout << "eb_evid=" << c.second->eb_evid_written;
//            std::cout << " buff_events_size=" <<  c.second->event_buffer_size;
//            std::cout << " er_in_late=" << c.second->m_error_count_late_event;
//            std::cout << " er_in_rptd=" << c.second->m_error_count_repeated_event_id;
//            std::cout << " er_out_mis=" << c.second->m_error_count_missing_event;
//            std::cout << " er_out_ord=" << c.second->m_error_count_unordered_events;
//            std::cout << ")\n";
//         }
         
         
//         ///vector of chip-related data
//         ShmemChipDataMap chips;


         
//         /// reserved for specific information depending on chip type
//         ShmemUint32Vector reserved; //bipc::vector<unsigned long>
//         ifec->reserved[0] = data->fec_fafa_event_number;
//         ifec->reserved[1] = data->fec_error_decode_header;
//         ifec->reserved[2] = data->fec_error_decode_udp;
//         ifec->reserved[3] = data->fec_error_create_event;
         
         
         
      }
   }
   else {
      
   }
   
}

void CShmemReader::read_shared_writer_data()
{
   
   
   std::pair<mmdaq3::server::ShmemWriterData*, bipc::managed_shared_memory::size_type> res = m_shm_manager->find<mmdaq3::server::ShmemWriterData> ("mmDaqSharedWriterData");
   
   if(res.second == 1)
   {
      mmdaq3::server::ShmemWriterData* wrt = res.first;
      
      std::cout
      << "Writer: "
      << " file=" << wrt->filename
      << "\nWriter: "
      << " save=" << wrt->write_data
      << " queue=" << wrt->evids_to_write
      << " evid=" << wrt->last_evid
      << " ev_cnt=" << wrt->run_event_counter
      << " ev/s=" << wrt->run_rate_event
      << " wr_MB=" << wrt->megabytes_written
      << " MB/s=" << wrt->run_rate_bytes_write
      << "\n";
//Writer:  save=1 queue=0 evid=49999 ev_cnt=23058 ev/s=2080433110 kB/s=0.00515307
   }
   else {
      std::cout << "CShmemReader::read_shared_writer_data() data not found " << std::endl;
   }
   

}


void CShmemReader::read_shared_event_number() //Francesco & Edo
{
   typedef uint64_t EBEventIdType;
    
   //std::pair<EBEventIdType*, bipc::managed_shared_memory::size_type> res = m_shm_manager->find<EBEventIdType> ("mmDaqSharedEventNumber");
   
    std::pair<mmdaq3::server::ShmemWriterData*, bipc::managed_shared_memory::size_type> res = m_shm_manager->find<mmdaq3::server::ShmemWriterData> ("mmDaqSharedWriterData");
    
    if(res.second == 1)
      {
    
	mmdaq3::server::ShmemWriterData* wrt = res.first;

	
	std::stringstream temp_run_string;
	temp_run_string << wrt->filename;
	std::string temp_run_name = temp_run_string.str();
	if(temp_run_name != "")
	  {
	    std::size_t pos = temp_run_name.find("run");
	    std::string sub_run_name = temp_run_name.substr(pos+3);
	    pos = sub_run_name.find(".root");
	    sub_run_name.resize(pos);
	    CShmemReader::run_name = sub_run_name;
	    
	    //CShmemReader::run_name = temp_run_name;
	    //	EBEventIdType val = *res.first;
	  }
	else   CShmemReader::run_name = "no writing";

	    if(m_current_event_number != wrt->run_event_counter) {
	      m_current_event_number = wrt->run_event_counter;
	      m_flag_new_event_data = true;
	    }
      else {
         m_flag_new_event_data = false;
      }
   }
   
   

}

void CShmemReader::read_timebin_number()//Francesco
{
  std::pair<unsigned*, bipc::managed_shared_memory::size_type> results =  m_shm_manager->find<unsigned> ("mmDaqTimeBins");
  m_n_tbins = *results.first;
  // std::cout << "CShmemReader::read_timebin_number() = " << m_n_tbins << "" << std::endl;
  CShmemReader::n_tbins = m_n_tbins;
}

void CShmemReader::read_shared_event_data(DetBasePtrCont& raw_strips)
{
   
   
   /// copy shared data to local strip map (why?)
   typedef std::map<uint32_t, ChannelData> ChannelMapType;
   ChannelMapType stripmap;
   // map: uint32_t channelId -> channel data
   std::pair<mmdaq3::server::ShmemChannelMap*, bipc::managed_shared_memory::size_type> res;
   res = m_shm_manager->find<mmdaq3::server::ShmemChannelMap> ("mmDaqSharedChannelData");
   //map uint32_t channelId -> CStripId
   std::pair<mmdaq3::server::ShmemStripMap*,   bipc::managed_shared_memory::size_type> res2;
   res2 = m_shm_manager->find<mmdaq3::server::ShmemStripMap> ("mmDaqSharedConfigStripMap");
   
   if(res.second == 1 && res2.second == 1)
   {
      mmdaq3::server::ShmemChannelMap* shmap = res.first;
      size_t ii = 0;
      for (mmdaq3::server::ShmemChannelMap::const_iterator it = shmap->begin(); it != shmap->end(); ++it, ++ii) {
         stripmap.insert(ChannelMapType::value_type(it->first, ChannelData(it->second)) );
      }
   }
   else {
      std::cout << "ERR: mmDaqSharedEventData resvec1.second=" << res.second << std::endl;
      std::cout << "ERR: mmDaqSharedEventData resvec2.second=" << res2.second << std::endl;
//      boost::this_thread::sleep(boost::posix_time::milliseconds(++m_shmem_error_counter));
      throw std::runtime_error("CShmemReader::read_shared_event_data(): no event data shared");
   }
   
   m_shmem_error_counter = 0;
   
   //loop local strip map
   //make strips, use map from shared channel.strip map
   mmdaq3::server::ShmemStripMap* stmap = res2.first;
   BOOST_FOREACH(ChannelMapType::value_type const& pc, stripmap) {
      mmdaq3::server::ShmemStripMap::const_iterator stmit = stmap->find(pc.first);
      if(stmit != stmap->end()) {
         const mmdaq3::server::ShmemStripId& sstrid =stmit->second;
         CDetStripId stripid(sstrid.chamber_name.c_str(),
                             sstrid.multi_layer_id,
                             sstrid.layer_id,
                             sstrid.readout_name_char,
                             sstrid.strip_no);
         DetStripPtr strptr(new CDetStrip(NULL, //detector=0
                                          CSrsChannelId(pc.first),
                                          stripid,
                                          pc.second.m_charge,
                                          pc.second.m_time));
         raw_strips.push_back(strptr);
      }
   }
   
//   std::cout << "   CShmemReader::read_shared_event_data()  raw_strips sz= " << raw_strips.size() << std::endl;
}


void CShmemReader::read_shared_event_udp_raw_data(std::map<uint32_t, std::vector<int16_t> >& raw_udp)
{
   std::pair<mmdaq3::server::ShmemRawMap*, bipc::managed_shared_memory::size_type> res  = m_shm_manager->find<mmdaq3::server::ShmemRawMap> ("mmDaqSharedRawData");

   if(res.second == 1) {
      mmdaq3::server::ShmemRawMap* shraw = res.first;
      //ShmemRawMapValueType == pair<chipid, rawvec >
      BOOST_FOREACH(mmdaq3::server::ShmemRawMapValueType const& p, *shraw) {
//         uint32_t chipid = p.first;
//         std::vector<int16_t> rvec(p.second.begin(), p.second.end());

         raw_udp[p.first] = std::vector<int16_t>(p.second.begin(), p.second.end() );
      }
      
      
   }
   else {
      std::cout << "ERR: read_shared_event_udp_raw_data() res.second=" << res.second << std::endl;
//      boost::this_thread::sleep(boost::posix_time::milliseconds(++m_shmem_error_counter));
      throw std::runtime_error("CShmemReader::read_shared_event_udp_raw_data(): no event raw data ");
   }
   
}


DetBasePtrCont CShmemReader::do_load_event_data_overlay(CDetDetector*)
{
   //n/a here
   return DetBasePtrCont();
}

DetBasePtrCont CShmemReader::do_load_event_data_pedestal(CDetDetector*)
{
   //n/a here
   return DetBasePtrCont();
}

void CShmemReader::do_parse_trees(CDetDetector* det, CGuiMainFrame*  gui)
{
   
}




bool CShmemReader::get_flag_new_event_data() const
{
   return m_flag_new_event_data;
}

