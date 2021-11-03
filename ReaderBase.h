//
//  ReaderBase.h
//  eventbrowser
//
//  Created by Marcin Byszewski on 1/18/13.
//  Copyright (c) 2013 CERN - PH/ADE. All rights reserved.
//

#ifndef __eventbrowser__ReaderBase__
#define __eventbrowser__ReaderBase__

#include "DetTypes.h"

#include <iostream>
#include <vector>
#include <map>
#include <string>

class TEnv;

namespace browser {
   
   class CStripsTree;
   class CDetDetector;
   class CGuiMainFrame;
   
   
   class CReaderBase
   {
      
   public:
      CReaderBase() {};
      
      virtual ~CReaderBase() {  }

      void open_file(const std::string& filename) {
         do_open_file(filename);
      }
      
      void close_file() {
         do_close_file();
      }
      
//      CStripsTree* strips_tree();
      
      TEnv*  get_env() {
         return do_get_env();
      }
      
      
      const std::string& get_daq_config_xml() const {
         return do_get_daq_config_xml();
      }
      const std::string& get_srs_config_xml() const {
         return do_get_srs_config_xml();
      }
      
      const std::string& get_det_config_xml() const {
         return do_get_det_config_xml();
      }
      
      const std::map<std::string, std::string>& get_det_config_xml_chambers() const {
         return do_get_det_config_xml_chambers();
      }
      
      std::string load_xml_config(const std::string& name) {
         return do_load_xml_config(name);
      }
      
      void set_tree_current_entry(size_t entry) {
         do_set_tree_current_entry(entry);
      }
      
      std::vector<size_t> get_tree_current_entries() const {
         return do_get_tree_current_entries();
      }
      
      size_t get_tree_max_entry() const {
         return do_get_tree_max_entry();
      }
      
      size_t get_current_event_number() const {
         return do_get_current_event_number();
      }

      
      
      DetBasePtrCont load_event_data_raw(CDetDetector* det) {
         return do_load_event_data_raw(det);
      }
      
      DetBasePtrCont load_event_data_overlay(CDetDetector* det) {
         return do_load_event_data_overlay(det);
      }

      DetBasePtrCont load_event_data_pedestal(CDetDetector* det) {
         return do_load_event_data_pedestal(det);
      }
      
      void parse_trees(CDetDetector* det, CGuiMainFrame*  gui) {
         do_parse_trees(det, gui);
      }
      
      
   private:
      virtual void do_open_file(const std::string& filename) = 0;
      virtual void do_close_file() = 0;
      virtual TEnv* do_get_env() = 0;
      
      virtual const std::string& do_get_daq_config_xml() const = 0;
      virtual const std::string& do_get_srs_config_xml() const = 0;
      virtual const std::string& do_get_det_config_xml() const = 0;
      virtual const std::map<std::string, std::string>& do_get_det_config_xml_chambers() const = 0;
      virtual std::string do_load_xml_config(const std::string& name) = 0;
      
      virtual void do_set_tree_current_entry(size_t entry) = 0;
      virtual std::vector<size_t> do_get_tree_current_entries() const = 0;
      virtual size_t do_get_tree_max_entry() const = 0;
      virtual size_t do_get_current_event_number() const = 0;

      
      virtual DetBasePtrCont do_load_event_data_raw(CDetDetector* det) = 0;
      virtual DetBasePtrCont do_load_event_data_overlay(CDetDetector* det) = 0;
      virtual DetBasePtrCont do_load_event_data_pedestal(CDetDetector* det) = 0;
      virtual void do_parse_trees(CDetDetector* det, CGuiMainFrame*  gui) = 0;
      
            
   };
}

#endif /* defined(__eventbrowser__ReaderBase__) */
