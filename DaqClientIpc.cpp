//
//  DaqClientIpc.cpp
//  eventbrowser
//
//  Created by Marcin Byszewski on 3/18/13.
//  Copyright (c) 2013 CERN - PH/ADE. All rights reserved.
//

#include "DaqClientIpc.h"

#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>


size_t const max_msg_size = 0x1000; //4096 bytes max


using namespace browser;

namespace bip = boost::interprocess;
//namespace bfs = boost::filesystem;

CDaqClientIpc::CDaqClientIpc()
: CDaqClient(),
m_runmode(runModeInvalid),
m_zerosuppression_apv(1.0)
{
   
}


CDaqClientIpc::~CDaqClientIpc()
{
   
}



bool CDaqClientIpc::do_is_running() const
{
   try {
      bip::message_queue ipc_queue(bip::open_only, "mmdaq3_srv_command");
      bip::message_queue msg_queue_resp(bip::open_only, "mmdaq3_srv_command_resp");
   } catch (bip::interprocess_exception & e) {
      return false;
   };
   return true;
}



bool CDaqClientIpc::do_send_start()
{
   try {
      m_messages.clear();

      
      std::string opt;
      switch (m_runmode) {
         case runModePedestal:
            opt="pedestal";
            break;
         case runModePhysics:
            opt="physics";
            break;
         default:
            break;
      }
      m_messages.push_back(std::string("run=") + opt);
      unsigned int rc = send_commands();
      return (rc == m_messages.size());
   } catch (bip::interprocess_exception & e) {
      return false;
   };
   return true;
}

bool CDaqClientIpc::do_send_stop()
{
   try {
      m_messages.clear();
      m_messages.push_back(std::string("stop=") );
      unsigned int rc = send_commands();
      return (rc == m_messages.size());
   } catch (bip::interprocess_exception & e) {
      return false;
   };
   return true;
}

bool CDaqClientIpc::do_send_configure(std::string const& filename)
{
   try {
      m_messages.clear();
      std::stringstream ss;
      ss << "configure=" << (filename.empty() ? "" : filename);
      m_messages.push_back(ss.str());
      unsigned int rc = send_commands();
      return (rc == m_messages.size());
   } catch (bip::interprocess_exception & e) {
      return false;
   };
   return true;
}

bool CDaqClientIpc::do_send_pedestals(std::string const& filename)
{
   try {
      m_messages.clear();
      std::stringstream ss;
      ss << "pedestal=" << (filename.empty() ? "" : filename);
      m_messages.push_back(ss.str());
      unsigned int rc = send_commands();
      return (rc == m_messages.size());
   } catch (bip::interprocess_exception & e) {
      return false;
   };
   return true;
}

bool CDaqClientIpc::do_send_write(bool val)
{
   try {
      m_messages.clear();
      std::string opt(val?"false":"true");
      m_messages.push_back(std::string("nosave=") + opt );
      
      unsigned int rc = send_commands();
      return (rc == m_messages.size());
   } catch (bip::interprocess_exception & e) {
      return false;
   };
   return true;
}


void CDaqClientIpc::do_set_runmode(RunModeType mode)
{
   m_runmode = mode;
   
}


bool CDaqClientIpc::do_set_zero_suppression_apv(double val)
{
   m_zerosuppression_apv = val;
   
   try {
      m_messages.clear();

      
      std::string opt =boost::lexical_cast<std::string>(val);
	  
      m_messages.push_back(std::string("zsapv=") + opt);
      unsigned int rc = send_commands();
      return (rc == m_messages.size());
   } catch (bip::interprocess_exception & e) {
      return false;
   };
   return true;
}
bool CDaqClientIpc::do_send_evt_counter(double val)
{
   try {
      m_messages.clear();

      
      std::string opt =boost::lexical_cast<std::string>(val);
	  
      m_messages.push_back(std::string("max_events=") + opt);
      unsigned int rc = send_commands();
      return (rc == m_messages.size());
   } catch (bip::interprocess_exception & e) {
      return false;
   };
   return true;
}
double CDaqClientIpc::do_get_zero_suppression_apv()
{
   return m_zerosuppression_apv;
}

int CDaqClientIpc::send_commands()
{
      
   int ok_counter = 0;
   
   try {
      bip::message_queue ipc_queue(bip::open_only, "mmdaq3_srv_command");
      bip::message_queue msg_queue_resp(bip::open_only, "mmdaq3_srv_command_resp");

      BOOST_FOREACH (const std::string& msg, m_messages) {
         ipc_queue.send(msg.data(), msg.size(), 0);
//         counter++;
         std::cout << "CDaqClientIpc::send_commands() --> sent:'" << msg << "'" <<  std::endl;
         
         std::string resp;
         resp.resize(max_msg_size);
         size_t recvd_size = 0;
         unsigned int priority = 0;
         //std::cout << "receiving response (add timeout) ... : " ;
         msg_queue_resp.receive(&resp[0], resp.size(), recvd_size, priority);
         resp.resize(recvd_size);
       //  std::cout <<  resp << "" <<  std::endl;
         if(resp.substr(0, 2) == "OK")
            ++ok_counter;
      }
   }
   catch (bip::interprocess_exception & e) {
      return -1;
   };
   
   return ok_counter;
}


bool CDaqClientIpc::do_read_daq_configuration_file_name(std::string& filename)
{
   
   try {
      std::string q("GET=configuration_filename");
      std::string response;
      bool rc = send_question(q, response);
      
      std::string str_value;
      if(rc && analyse_response(str_value, "configuration_filename", response)) {
         filename = str_value;
         return true;
      }
      
   } catch (bip::interprocess_exception & e) {
      return false;
   };
   return false;

}


bool CDaqClientIpc::do_read_daq_pedestals_file_name(std::string& filename)
{
   try {
      std::string q("GET=pedestal_filename");
      std::string response;
      bool rc = send_question(q, response);
      
      //analyse resp
      std::string str_value;
      if(rc && analyse_response(str_value, "pedestal_filename", response)) {
         filename = str_value;
         return true;
      }
      
   } catch (bip::interprocess_exception & e) {
      return false;
   };
   return false;
}


bool CDaqClientIpc::do_read_daq_write_enable(bool& value)
{
   try {
      std::string q("GET=write_value");
      std::string response;
      bool rc = send_question(q, response);
      
      //analyse resp
      std::string str_value;
      if(rc && analyse_response(str_value, "write_value", response)) {
         value = boost::lexical_cast<bool>(str_value);
      }
      
   } catch (bip::interprocess_exception & e) {
      return false;
   };
   return true;
}


bool CDaqClientIpc::send_question(std::string const& question, std::string& response)
{
   
   try {
      bip::message_queue ipc_queue(bip::open_only, "mmdaq3_srv_command");
      bip::message_queue msg_queue_resp(bip::open_only, "mmdaq3_srv_command_resp");
      
      ipc_queue.send(question.data(), question.size(), 0);
      std::cout << "CDaqClientIpc::send_question() --> sent:'" << question << "'" <<  std::endl;
      
      std::string resp;
      resp.resize(max_msg_size);
      size_t recvd_size = 0;
      unsigned int priority = 0;
      std::cout << "CDaqClientIpc::send_question() receiving (add timeout) ... : " ;
      msg_queue_resp.receive(&resp[0], resp.size(), recvd_size, priority);
      resp.resize(recvd_size);
      std::cout << "CDaqClientIpc::send_question() --> resp: " <<  resp << "" <<  std::endl;
      response = resp;
   }
   catch (bip::interprocess_exception & e) {
      return false;
   };
   
   return true;
}


bool CDaqClientIpc::analyse_response(std::string& value, std::string const& keyword, std::string const& answer)
{
   std::cout << "CDaqClientIpc::analyse_response() in: '" << answer << "'" <<   std::endl;
   size_t pos = answer.find(keyword);
   if(pos == std::string::npos) {
      std::cout << "CDaqClientIpc::analyse_response() --> npos: keyword =" <<  keyword <<   std::endl;
      return false;
   }
   std::cout << "CDaqClientIpc::analyse_response() --> pos: " <<  pos << " of " << keyword << " in '" << answer << "'" <<   std::endl;

   value = answer.substr(answer.find("=", pos + keyword.length())+1);
//   value = answer.substr(pos + keyword.length() + 1);
   
   std::cout << "CDaqClientIpc::analyse_response() --> pos: " <<  pos << " value = '" << value << "'" <<  std::endl;
   return true;
}




