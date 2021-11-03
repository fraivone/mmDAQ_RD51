//
//  RootReader.cpp
//  browser
//
//  Created by Marcin Byszewski on 12/16/11.
//  Copyright (c) 2011 CERN - PH/ADE. All rights reserved.
//
//  Modified by Nikos Tsirintanis - Nikolaos.Tsirintanis@Cern.ch

#include "RootReader.h"
#include "CApvRawTree.h"
#include "StripsTree.h"
//#include "ParserApvRawTree.h"
#include "SrsChannelId.h"
#include "DetStrip.h"
#include "Configuration.h"
#include "Mmdaq3TreeApvRaw.h"
#include "TreeRawBnlMini.h"
#include "TreeRawBnlTrigger.h"
#include "TreeRawApvPedestals.h"
#include "CSrsPedestalsRoot.h"
#include "DetDetector.h"

#include <TEnv.h>
#include <TFile.h>
#include <TDirectory.h>
#include <TObjString.h>
#include <TString.h>

#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/property_tree/xml_parser.hpp>


#include <cassert>
#include <iostream>
#include <stdexcept>
#include <sstream>

using namespace browser;
namespace bfs = boost::filesystem;

CRootReader::CRootReader(CConfiguration& config) :
m_config(config),
m_rootfile(0),
m_rawtrees(),
m_overlaytrees(),
m_pedtrees(),
m_stripstree(0),
m_env_file(0),
m_daq_config_xml(),
m_srs_config_xml(),
m_det_config_xml(),
m_det_config_xml_chambers(),
m_tree_max_entry(0),
m_current_event_number(0)
{
   
}


CRootReader::~CRootReader()
{
   close_file();
}


TEnv* CRootReader::do_get_env()
{
   return m_env_file;
}

const std::string& CRootReader::do_get_daq_config_xml() const
{
   return m_daq_config_xml;
}

const std::string& CRootReader::do_get_srs_config_xml() const
{
   return m_srs_config_xml;
}

const std::string& CRootReader::do_get_det_config_xml() const
{
   return m_det_config_xml;
}

const std::map<std::string, std::string>& CRootReader::do_get_det_config_xml_chambers() const
{
   return m_det_config_xml_chambers;
}


void CRootReader::do_open_file(const std::string &filename)
{
   std::cout << "CRootReader::open_file "<< filename << std::endl;
   if (!bfs::exists(filename) || !bfs::is_regular(filename)) {
      throw std::runtime_error("ERROR: CRootReader::open_file(): file does not exist: '" + filename + "'");
   }
   
   if (m_rootfile) {
      close_file();
   }
   
   m_rootfile = new TFile(filename.c_str(), "READ");
   if (!m_rootfile || !m_rootfile->IsOpen()) {
      m_rootfile = 0;
      throw std::runtime_error("CRootReader::open_file(): failed to open root file");
   }
   if (m_rootfile->IsZombie()) {
      delete m_rootfile;
      m_rootfile = 0;
      throw std::runtime_error("CRootReader::open_file(): zombie root file");
   }
   
   m_rawtrees.clear();
   m_overlaytrees.clear();
   m_pedtrees.clear();
   
   init_trees();
   
   
   
   m_tree_max_entry = tree_max_entry();
   m_env_file = load_det_config();
   
   
   m_daq_config_xml = load_xml_config("daq_config_xml");
   m_srs_config_xml = load_xml_config("srs_config_xml");
   m_det_config_xml = load_xml_config("det_config_xml");
   
//   std::cout << "DAQXML" << m_daq_config_xml << std::endl;
//   std::cout << "SRSXML" << m_srs_config_xml << std::endl;
//   std::cout << "DETXML" << m_det_config_xml << std::endl;


   
}


void CRootReader::do_close_file()
{
//   if (m_rawtree) {
//      delete m_rawtree;
//      m_rawtree = 0;
//   }
   if (m_stripstree) {
      delete m_stripstree;
      m_stripstree = 0;
   }
   
   
   m_env_file = 0;
   if(m_rootfile && m_rootfile->IsOpen()) {
      delete m_rootfile;
      m_rootfile = 0;
   }
//   m_tree_current_entry = 0;
   m_tree_max_entry = 0;
}


void CRootReader::init_trees()
{
      
   assert(m_rootfile);
   // try to open all available trees
   
   try {
      m_rawtrees.push_back(boost::shared_ptr<CTreeBase> (new CMmdaq3TreeApvRaw(m_rootfile, "apv_raw")));
   } catch (std::runtime_error& re) { /**/ }
  
   
   try {
      m_rawtrees.push_back(boost::shared_ptr<CTreeBase> (new CTreeRawBnlMini(&m_config, m_rootfile, "bnl_raw")));
   } catch (std::runtime_error& re) {
      std::cout << " CRootReader::init_trees() error in CTreeRawBnlMini() :" << re.what() << std::endl;
   }
  
   try {
      m_overlaytrees.push_back(boost::shared_ptr<CTreeBase> (new CTreeRawBnlTrigger(&m_config, m_rootfile, "bnl_trg")));
   } catch (std::runtime_error& re) {
      std::cout << " CRootReader::init_trees() error in CTreeRawBnlTrigger() :" << re.what() << std::endl;
   }
   
   try {
      m_rawtrees.push_back(boost::shared_ptr<CTreeBase> (new CApvRawTree(m_rootfile, "raw")));
   } catch (std::runtime_error& re) { /**/ }
   
   try {
      m_rawtrees.push_back(boost::shared_ptr<CTreeBase> (new CApvRawTree(m_rootfile, "raw_processed")));
   } catch (std::runtime_error& re) { /**/ }
   
   // it must have the detector altrady built
   try {
      m_pedtrees.push_back(boost::shared_ptr<CTreeBase> (new CTreeRawApvPedestals(&m_config, m_rootfile, "pedestals")));
   } catch (std::runtime_error& re) { /**/ }
   try {
      m_pedtrees.push_back(boost::shared_ptr<CTreeBase>(new CSrsPedestalsRoot(&m_config, m_rootfile, "apv_raw_ped")));
   } catch (std::runtime_error& re) { /**/ }
   
   
   std::cout << " CRootReader::init_trees() raw ntrees = " << m_rawtrees.size() << std::endl;
   std::cout << " CRootReader::init_trees() pedestal ntrees = " << m_pedtrees.size() << std::endl;
   
   BOOST_FOREACH(TreeBasePtr tr , m_rawtrees) {
      std::cout << "tr = " << tr.get() << " " << tr->GetName() << std::endl;
   }
   
   
//   const std::string& raw_tree_name = m_config.get_raw_tree_name();
//   if (raw_tree_name == "apv_raw") {
//      //new tree format
//      init_trees_new();
//   }
//   else {
//      init_trees_old_apv();
//   }
}

size_t CRootReader::tree_max_entry() 
{
   std::vector<size_t> max_entries;
   BOOST_FOREACH(TreeBasePtr tr, m_rawtrees) {
       long n = tr->GetEntries();
       if(n>=0) {
           max_entries.push_back(n);
       }

   }
   
//   if (m_rawtree) {
//      max_entries.push_back(m_rawtree->GetEntries());
//   }
//   if (m_stripstree) {
//      max_entries.push_back(m_stripstree->GetEntries());
//   }
   
   
   if (max_entries.empty()) {
      return 0;
   }
   return *std::min_element(max_entries.begin(), max_entries.end());
}


TEnv* CRootReader::load_det_config()
{
//   std::cout << "CRootReader::load_det_config " << std::endl;
   
   //look for det config TEnv
   std::stringstream ss;
   ss << m_rootfile->GetName() << ":" << "config";
   TDirectory* dir1 = m_rootfile->GetDirectory(ss.str().c_str());
   
   ss.str("");
   ss << m_rootfile->GetName() << ":" << "DetConfig";
   TDirectory* dir2 = m_rootfile->GetDirectory(ss.str().c_str());
   
   ss.str("");
   ss << m_rootfile->GetName() << ":" << "DetConfig";
   m_rootfile->cd();
   TEnv* ptrenv = 0;
   m_rootfile->GetDirectory("")->GetObject("DetConfig", ptrenv);
   
   //    if (!ptrenv && !dir1 && !dir2) {
   //        return 0;
   //    }
   
   if (ptrenv) {
      //
   }
   else if (dir1) {
      m_rootfile->cd(dir1->GetName());
      dir1->GetObject("TEnv", ptrenv);
   }
   else if (dir2) {
      m_rootfile->cd(dir2->GetName());
      dir2->GetObject("TEnv", ptrenv);
   }
   
   return ptrenv ? static_cast<TEnv*>(ptrenv->Clone()) : 0;
}



std::string CRootReader::do_load_xml_config(const std::string& name)
{
   
   std::stringstream ss;
   ss << m_rootfile->GetName() << ":" << "config";
   TDirectory* dir1 = m_rootfile->GetDirectory(ss.str().c_str());
   
   ss.str("");
   ss << m_rootfile->GetName() << ":" << "DetConfig";
   TDirectory* dir2 = m_rootfile->GetDirectory(ss.str().c_str());
   
   TObjString* str = 0;
   
   boost::property_tree::ptree pt;
   if (dir1) {
      m_rootfile->cd(dir1->GetName());
      dir1->GetObject(name.c_str(), str);
      std::stringstream ss;
      if (str) {
         ss << str->GetString().Data();
         return std::string(ss.str());
      }
      std::cout << "no object in root file named:'" << name <<  "'"  << std::endl;
   }
   else if (dir2) {
      m_rootfile->cd(dir2->GetName());
      dir2->GetObject(name.c_str(), str);
      std::stringstream ss;
      if (str) {
         ss << str->GetString().Data();
         return std::string(ss.str());
      }
      std::cout << "no object in root file named:'" << name <<  "'" << std::endl;
   }
   else {
      std::cout << "no config directory in root file :" << std::endl;
   }


   return std::string();

}



void CRootReader::do_set_tree_current_entry(size_t entry)
{

   /*
    TTree::GetEntry()
    If entry does not exist the function returns 0.
    If an I/O error occurs, the function returns -1.
    */
   
   BOOST_FOREACH(TreeBasePtr tr, m_rawtrees) {
      int rc = tr->GetEntry(entry);
      
//      if (rc <= 0) {
//         std::cout << "CRootReader::set_tree_current_entry() no entry" << std::endl;
////         m_tree_current_entry = 0;
//         return;
//      }
   }
   BOOST_FOREACH(TreeBasePtr tr, m_overlaytrees) {
      int rc = tr->GetEntry(entry);
      if (rc <= 0) {
         std::cout << "CRootReader::set_tree_current_entry() overlay tree has no such entry" << std::endl;
      }
   }
   
    BOOST_FOREACH(TreeBasePtr tr, m_pedtrees) {
      int rc = tr->GetEntry(entry);
      if (rc <= 0) {
         std::cout << "CRootReader::set_tree_current_entry() pedestal tree has no such entry" << std::endl;
      }
   }
   
   
   
//   if (!m_rawtree) {
//      std::cout << "CRootReader::set_tree_current_entry() no raw tree '" 
//      << m_config.get_raw_tree_name() << "'" << std::endl;
//      m_tree_current_entry = 0;
//      return;
//   }
//   int rc = m_rawtree->GetEntry(entry);
//   if (rc > 0) {
//      m_tree_current_entry = entry;
//   }
//   else {
//      std::cout << "CRootReader::set_tree_current_entry() no entry" << std::endl;
//      m_tree_current_entry = 0;
//      return;
//   }
   
   if(m_stripstree) {
      int rc = m_stripstree->GetEntry(entry);
      if (rc <= 0) {
         std::cerr << "m_strips tree has no entry or error" << std::endl;
      }
   }
}


std::vector<size_t> CRootReader::do_get_tree_current_entries() const
{
   std::vector<size_t> vec;   
   BOOST_FOREACH(TreeBasePtr tr, m_rawtrees) {
      vec.push_back(tr->GetCurrentEntryNumber());
//      std::cout << "CRootReader::get_tree_current_entries() " << vec.size() << std::endl;
   }
   return vec;
}


size_t CRootReader::do_get_tree_max_entry() const
{
   return m_tree_max_entry;
}

/// read raw trees, create strips and put them into strip container
DetBasePtrCont CRootReader::do_load_event_data_raw(CDetDetector* det)
{
   DetBasePtrCont raw_strips;
   BOOST_FOREACH(TreeBasePtr tr, m_rawtrees) {
      tr->LoadEventData(raw_strips, det);
       m_current_event_number = tr->GetEventNumb();
   }
//   std::cout << "CRootReader::load_event_data_raw()  total raw size sz= " << raw_strips.size() << std::endl;
   return raw_strips;
}

DetBasePtrCont CRootReader::do_load_event_data_overlay(CDetDetector* det)
{
   DetBasePtrCont overlay_strips;
   BOOST_FOREACH(TreeBasePtr tr, m_overlaytrees) {
      tr->LoadEventData(overlay_strips, det);
   }
//   std::cout << "CRootReader::load_event_data_raw()  total overlay size sz= " << overlay_strips.size() << std::endl;
   return overlay_strips;
}

DetBasePtrCont CRootReader::do_load_event_data_pedestal(CDetDetector* det)
{
   DetBasePtrCont pedestal_strips;
   BOOST_FOREACH(TreeBasePtr tr, m_pedtrees) {
      tr->LoadEventData(pedestal_strips, det);
   }
 //  std::cout << "CRootReader::load_event_data_pedestal()  total pedestal size sz= " << pedestal_strips.size() << std::endl;
   return pedestal_strips;
}


void CRootReader::do_parse_trees(CDetDetector* det, CGuiMainFrame*  gui)
{
   set_tree_current_entry(0);
   BOOST_FOREACH(TreeBasePtr tr, m_rawtrees) {
      tr->parse_tree(det, gui);
   }
  // BOOST_FOREACH(TreeBasePtr tr2, m_pedtrees) {
  //    tr2->parse_tree(det, gui);
 //  }
   
//   
//   ////////////////////////////////////////////
//   
//   CTreeBase* base =  m_reader->raw_tree();
//   //loop raw tree, get chambers, planes, rd, strip ranges
//   
//   //switch on tree type:
//   if (!base) {
//      return 0;
//   } else if (CApvRawTree* apvtree = dynamic_cast<CApvRawTree*>(m_reader->raw_tree())) {
//      return parse_apv_raw_tree(det, apvtree);
//   } else if (CMmdaq3TreeApvRaw* apv3tree = dynamic_cast<CMmdaq3TreeApvRaw*>(m_reader->raw_tree())) {
//      return parse_mmdaq3_apv_raw_tree(det, apv3tree);
//   }
//   
//   return 0;
   
   
   
}

boost::shared_ptr<CStripsTreeEntry> CRootReader::get_striptree_entry(unsigned strip_number) const
{
   if (!m_stripstree) {
      return boost::shared_ptr<CStripsTreeEntry> (new CStripsTreeEntry());
   }
   long index = m_stripstree->get_index_by_number(strip_number);
   if (index < 0) {
      return boost::shared_ptr<CStripsTreeEntry> (new CStripsTreeEntry());
   }
   return boost::shared_ptr<CStripsTreeEntry> (new CStripsTreeEntry(m_stripstree, index));
}
size_t CRootReader::get_current_event_number() const
{
    return do_get_current_event_number();
}

size_t CRootReader::do_get_current_event_number() const
{
   return m_current_event_number;
}


