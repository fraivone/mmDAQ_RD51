//
//  DaqClientIpc.h
//  eventbrowser
//
//  Created by Marcin Byszewski on 3/18/13.
//  Copyright (c) 2013 CERN - PH/ADE. All rights reserved.
//

#ifndef __eventbrowser__DaqClientIpc__
#define __eventbrowser__DaqClientIpc__

#include <iostream>
#include <string>
#include <vector>

namespace browser {
   
   /// global run mode of the daq
   typedef enum {
      runModeInvalid,
      runModePedestal,     /// calculate means...
      runModePhysics,      /// do ZS etc
      runModePhysicsRaw,    /// do not treat data
      runModeDryPedestal,  /// pedestals, do not save
      runModeDryPhysics    /// physics, do not save data
   } RunModeType;
   
   ///dummy class for future client types (net)
   class CDaqClient
   {
   public:
      virtual ~CDaqClient() {};
      bool is_running()
      {
         return do_is_running();
      }
      
      bool send_start()
      {
         return do_send_start();
      }
      
      bool send_stop()
      {
         return do_send_stop();
      }
      
      bool send_configure(std::string const& filename)
      {
         return do_send_configure(filename);
      }
      
      bool send_pedestals(std::string const& filename)
      {
         return do_send_pedestals(filename);
      }

      bool send_write(bool val)
      {
         return do_send_write(val);

      }
      
      void set_run_type(RunModeType mode)
      {
         do_set_runmode(mode);
      }

      void set_zero_suppression_apv(double val)
      {
         do_set_zero_suppression_apv(val);
      }
	  void send_evt_counter(double val)
      {
         do_send_evt_counter(val);
      }
	   double get_zero_suppression_apv()
      {
         return do_get_zero_suppression_apv();
      }
      bool read_daq_configuration_file_name(std::string& filename)
      {
         return do_read_daq_configuration_file_name(filename);
      }
      
      bool read_daq_pedestals_file_name(std::string& filename)
      {
         return do_read_daq_pedestals_file_name(filename);
      }
      
      bool read_daq_write_enable(bool& value)
      {
         return do_read_daq_write_enable(value);
      }
      
   private:
      virtual bool do_is_running() const = 0;
      virtual bool do_send_start() = 0;
      virtual bool do_send_stop() = 0;
      virtual bool do_send_configure(std::string const& filename) = 0;
      virtual bool do_send_pedestals(std::string const& filename) = 0;
      virtual bool do_send_write(bool) = 0;
      virtual void do_set_runmode(RunModeType mode) = 0;
      virtual bool do_set_zero_suppression_apv(double val) = 0;
      virtual bool do_send_evt_counter(double val) = 0;
      virtual double do_get_zero_suppression_apv() = 0;

      virtual bool do_read_daq_configuration_file_name(std::string& filename) = 0;
      virtual bool do_read_daq_pedestals_file_name(std::string& filename) = 0;
      virtual bool do_read_daq_write_enable(bool& value) = 0;
      
   };
   
   class CDaqClientIpc : public CDaqClient
   {
      
   public:
      CDaqClientIpc();
      virtual ~CDaqClientIpc();
           
   private:
      virtual bool do_is_running() const;
      virtual bool do_send_start() ;
      virtual bool do_send_stop() ;
      virtual bool do_send_configure(std::string const& filename) ;
      virtual bool do_send_pedestals(std::string const& filename) ;
      virtual bool do_send_write(bool) ;
      virtual void do_set_runmode(RunModeType mode) ;
      virtual bool do_set_zero_suppression_apv(double val);
      virtual bool do_send_evt_counter(double val);
      virtual double do_get_zero_suppression_apv();


      virtual bool do_read_daq_configuration_file_name(std::string& filename);
      virtual bool do_read_daq_pedestals_file_name(std::string& filename);
      virtual bool do_read_daq_write_enable(bool& value);

      
      int send_commands();
      bool send_question(std::string const& question, std::string& resp);
      bool analyse_response(std::string& value, std::string const& keyword, std::string const& answer);
      
      RunModeType m_runmode;
      double m_zerosuppression_apv;
      std::vector<std::string> m_messages;
      
   };
   
   
} //namespace

#endif /* defined(__eventbrowser__DaqClientIpc__) */
