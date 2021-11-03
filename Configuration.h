//
//  Configuration.h
//  browser
//
//  Created by Marcin Byszewski on 11/15/11.
//  Copyright 2011 CERN - PH/ADE. All rights reserved.
//

#ifndef browser_Configuration_h
#define browser_Configuration_h

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>

#include <string>
#include <vector>

#include "SrsChannelId.h"
#include "VMM1Parameters.h"

class TFile;

namespace browser {
   
   class CSrsChannelId;
   class CDetStripId;
   struct VMM1Parameters;
   
   class CConfiguration
   {
      enum BrowserStateType {
         stateInvalid,
         stateOffline,
         stateOnline
      }
      ;
      
      typedef std::map<CSrsChannelId, VMM1Parameters > VMM1ParsMap;
      typedef std::map<CSrsChannelId, CDetStripId> StripMapType;

   public:
      CConfiguration(boost::program_options::variables_map& options_varmap);
      virtual ~CConfiguration();
      void clear();

      //observers
      const boost::filesystem::path& get_config_file() const { return m_config_file; }
      const boost::filesystem::path& get_working_path() const { return m_working_path; }
      const boost::filesystem::path& get_current_file_path() const { return m_current_file; }
      std::string get_current_file_name() const;
      std::string get_current_file_dirname() const; 
      const std::string& get_raw_tree_name() const;
      bool get_overlay_fitting();
      
      void set_working_path(const std::string& path);
      void set_current_file_path(const std::string& path);
      void set_raw_tree_name(const std::string& name);
      void set_overlay_fitting(bool val);

      void set_vmm1_offsets(const VMM1ParsMap& vmm_pars);
      //  std::map<CSrsChannelId, VMM1Parameters > vmm_tac_map;
      bool get_vmm1_parameters(const CSrsChannelId& chid, VMM1Parameters& pars);

      void add_mapped_strips(const StripMapType& m_strip_map);
      const std::map<CSrsChannelId, CDetStripId>& get_strip_map() const;
      bool get_mapped_strip(const CSrsChannelId& chid, CDetStripId& stripid) const;

      
   private:
//      std::vector<std::string>              m_args;
//      boost::program_options::options_description m_options_description;
      boost::program_options::variables_map m_options_varmap;
      boost::property_tree::ptree           m_ptree;
      boost::filesystem::path  m_config_file;
      boost::filesystem::path  m_initial_path;
      boost::filesystem::path  m_working_path;
      boost::filesystem::path  m_current_file;
      TFile*                   m_rootfile;


      std::string m_raw_tree_name;
      bool        m_overlay_fitting; ///< show fitting results on top of raw data
      BrowserStateType m_browser_state;
      VMM1ParsMap m_vmm_parameters;
      
      StripMapType m_strip_map;
      
      void init();
      void handle_options();
      void read_config_file(const std::string& filename);
      void print();
      void reset();
      
   
      //
   };
   
} // namespace

#endif
