//
//  Configuration.cpp
//  browser
//
//  Created by Marcin Byszewski on 11/15/11.
//  Copyright (c) 2011 CERN - PH/ADE. All rights reserved.
//

#include "Configuration.h"
#include "DetDetector.h"
#include "DetectorBuilder.h"
#include "DetStripId.h"
#include "SrsChannelId.h"
#include <TFile.h>

#include <boost/property_tree/xml_parser.hpp>

#include <iostream>
#include <stdexcept>

using namespace browser;
using std::vector;
using std::string;
using std::cout;
using std::endl;

namespace bpo = boost::program_options;
namespace bfs = boost::filesystem;

const double VMM1Parameters::VMM1TdoVoltsCoef = 0.000201416015625;
const double VMM1Parameters::VMM1PdoVoltsCoef = 0.000201416015625;


CConfiguration::CConfiguration(boost::program_options::variables_map& options_varmap) :
//m_args(argv, argv+argc),
//m_options_description("event-browser options", 256),
m_options_varmap(options_varmap),
m_ptree(),
m_config_file(""),
m_initial_path(""),
m_working_path(""),
m_current_file(""),
m_rootfile(0),
m_raw_tree_name("apv_raw"),
m_overlay_fitting(false),
m_browser_state(stateOffline),
m_vmm_parameters(),
m_strip_map()
{
   
//   m_options_description.add_options()
//   ("help", "display help message")
//   ("config-file,c", bpo::value<string>(), "load configuration from file")
//   ("data-dir,d", bpo::value< string>(),"data directory")
//   ("data-file,f", bpo::value< string>(),"data file")
//   ("online" ,"online configuration")
//   ;
//   
//   
//   bpo::positional_options_description popt;
//   popt.add("data-file", -1);
//   
//   
//   bpo::store(bpo::command_line_parser(argc, argv).
//              options(m_options_description).positional(popt).run(), m_options_varmap);
//   bpo::notify(m_options_varmap);
   
   
   //
   //    bpo::store(bpo::parse_command_line(argc, argv, m_options_description), m_options_varmap);
   //    bpo::notify(m_options_varmap);
   
   
   
   init();
}


CConfiguration::~CConfiguration()
{  }


void CConfiguration::init()
{
   bfs::path path = bfs::initial_path();
//   m_args.push_back(path.string());
   m_initial_path = path;
   m_working_path = m_initial_path;
   
   
   
   handle_options();
   print();
}


void CConfiguration::clear()
{
   m_strip_map.clear();
   m_current_file.clear();
   m_raw_tree_name.clear();
}

void CConfiguration::handle_options()
{
   m_browser_state = stateOffline;
   if (m_options_varmap.count("online")) {
      m_browser_state = stateOnline;
   }
   
   if (m_options_varmap.count("config-file")) {
      m_config_file = m_options_varmap["config-file"].as<std::string>();
      m_config_file = bfs::absolute(m_config_file);
      if (!bfs::exists(m_config_file) || !bfs::is_regular(m_config_file)) {
         throw std::runtime_error("ERROR: CConfiguration::handle_options(): bad config file: " + m_config_file.string());
      }
      
      //read config file
      try {
         read_config_file(m_options_varmap["config-file"].as<std::string>());
      } catch (std::exception &e) {
         std::cerr << "read_config_file(): " << e.what() << std::endl;
      }
   }
   
   if (m_options_varmap.count("data-dir")) {
      try {
         m_working_path = m_options_varmap["data-dir"].as<std::string>() ;
         m_working_path = bfs::absolute(m_working_path);
         if (!bfs::exists(m_working_path) || !bfs::is_directory(m_working_path)) {
            throw std::runtime_error("ERROR: CConfiguration::handle_options(): bad path: " + m_working_path.string());
         }
      } catch (const bfs::filesystem_error& ex) {
         throw;
      }
   }
   
   if (m_options_varmap.count("data-file")) {
      try {
         m_current_file = m_options_varmap["data-file"].as<std::string>() ;
         if (!m_current_file.is_absolute()) {
            bfs::path tmp(m_working_path);
            tmp /= m_current_file;
            m_current_file = tmp;
         }
         
         m_current_file = bfs::absolute(m_current_file);
         if (!bfs::exists(m_current_file) || !bfs::is_regular(m_current_file)) {
            throw std::runtime_error("ERROR: CConfiguration::handle_options(): bad input file: " + m_current_file.string());
         }
      } catch (const bfs::filesystem_error& ex) {
         throw;
      }
   }
}


/// read config file for the Event Browser from file to property tree
void CConfiguration::read_config_file(const std::string& filename)
{
   // Load the XML file into the property tree. If reading fails
   // (cannot open file, parse error), an exception is thrown.
   read_xml(filename, m_ptree);
   
   // Get the filename and store it in the m_file variable.
   // Note that we construct the path to the value by separating
   // the individual keys with dots. If dots appear in the keys,
   // a path type with a different separator can be used.
   // If the debug.filename key is not found, an exception is thrown.
   //   m_server_ip = m_ptree.get<std::string>("server.ip");
   
   
   // Get the debug level and store it in the m_level variable.
   // This is another version of the get method: if the value is
   // not found, the default value (specified by the second
   // parameter) is returned instead. The type of the value
   // extracted is determined by the type of the second parameter,
   // so we can simply write get(...) instead of get<int>(...).
   //    m_receiver_ip = m_ptree.get("receiver.ip", std::string("127.0.0.1"));
   
   // Iterate over the debug.modules section and store all found
   // modules in the m_modules set. The get_child() function
   // returns a reference to the child at the specified path; if
   // there is no such child, it throws. Property tree iterators
   // are models of BidirectionalIterator.
   //    BOOST_FOREACH(boost::property_tree::ptree::value_type &v,
   //                  m_ptree.get_child("srs.fec_cards"))
   //        m_fecs.push_back(v.second.data());
   
}


void CConfiguration::print()
{
   std::cout << "CConfiguration::print() : " << std::endl;
   std::cout << "  m_config_file  = " << m_config_file  << std::endl;
   std::cout << "  m_working_path = " << m_working_path << std::endl;
   std::cout << "  m_current_file = " << m_current_file << std::endl;
   
}

std::string CConfiguration::get_current_file_name() const
{
   return m_current_file.filename().string() ;//leaf().string();
}
std::string CConfiguration::get_current_file_dirname() const
{
   return m_current_file.parent_path().string();
}

void CConfiguration::set_working_path(const std::string& path)
{
   m_working_path = bfs::path(path);
}

void CConfiguration::set_current_file_path(const std::string& path)
{
   m_current_file = bfs::path(path);
}

void CConfiguration::set_raw_tree_name(const std::string& name)
{
   m_raw_tree_name = name;
}

const std::string& CConfiguration::get_raw_tree_name() const
{
   return m_raw_tree_name;
}

void CConfiguration::set_overlay_fitting(bool val)
{
   m_overlay_fitting = val;
}

bool CConfiguration::get_overlay_fitting()
{
   return m_overlay_fitting;
}


void CConfiguration::set_vmm1_offsets(const VMM1ParsMap& vmm_pars)
{
   m_vmm_parameters = vmm_pars;
}

bool CConfiguration::get_vmm1_parameters(const CSrsChannelId& chid, VMM1Parameters& pars)
{
   VMM1ParsMap::iterator it =  m_vmm_parameters.find(chid) ;
   if (it != m_vmm_parameters.end() ) {
      pars = it->second;
      return true;
   }
   return false;
}


void CConfiguration::add_mapped_strips(const StripMapType& map)
{
   m_strip_map.insert(map.begin(), map.end());
}


const std::map<CSrsChannelId, CDetStripId>& CConfiguration::get_strip_map() const
{
   return m_strip_map;
}

bool CConfiguration::get_mapped_strip(const CSrsChannelId& chid, CDetStripId& stripid) const
{
   
   StripMapType::const_iterator it = m_strip_map.find(chid);
   if(it != m_strip_map.end() ) {
      stripid = it->second;
      //         std::cout << "CConfiguration::get_mapped_strip " << chid.to_string() << "->" << stripid << std::endl;
      return true;
   }
   //      std::cout << "CConfiguration::get_mapped_strip " << chid.to_string() << " unmapped"<< std::endl;
   return false;
}



