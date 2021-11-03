//
//  RootReader.h
//  browser
//
//  Created by Marcin Byszewski on 12/16/11.
//  Copyright 2011 CERN - PH/ADE. All rights reserved.
//

#ifndef browser_RootReader_h
#define browser_RootReader_h

#include "ReaderBase.h"
#include "DetTypes.h"

#include <boost/shared_ptr.hpp>
#include <boost/property_tree/ptree.hpp>
#include <vector>
#include <string>

class TFile;
class TEnv;

namespace browser {
   
   class CApvRawTree;
   class CStripsTree; // all strips, time and q , fitted and cfd
   class CConfiguration;
   class CStripsTreeEntry;
   class CTreeBase;
   class CDetDetector;
   class CGuiMainFrame;
   
   class CRootReader : public CReaderBase
   {
   public:
      CRootReader(CConfiguration& config);
      virtual ~CRootReader();
      
////      CTreeBase* raw_tree() { return m_rawtree;}
//      CStripsTree* strips_tree() { return m_stripstree;}
//      void   open_file(const std::string& filename);
//      void   close_file();
//      TEnv*  get_env() {return m_env_file;}
//      //new mmdaq configs
//      const std::string& get_daq_config_xml() const { return m_daq_config_xml;};
//      const std::string& get_srs_config_xml() const { return m_srs_config_xml;};
//      const std::string& get_det_config_xml() const { return m_det_config_xml;};
//      void   set_tree_current_entry(size_t entry);
//      std::vector<size_t> get_tree_current_entries() const;
//      size_t get_tree_max_entry() const;
//      DetBasePtrCont load_event_data_raw(CDetDetector* det);
//      DetBasePtrCont load_event_data_overlay(CDetDetector* det);
//      void parse_trees(CDetDetector* det, CGuiMainFrame*  gui);
//      
      size_t get_current_event_number() const;
//      std::string load_xml_config(const std::string& name);

      
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
      
      virtual DetBasePtrCont do_load_event_data_raw(CDetDetector* det);
      virtual DetBasePtrCont do_load_event_data_overlay(CDetDetector* det);
      virtual DetBasePtrCont do_load_event_data_pedestal(CDetDetector* det);
      virtual void do_parse_trees(CDetDetector* det, CGuiMainFrame*  gui);

      
      
      CConfiguration& m_config;
      TFile*       m_rootfile;
      std::vector<boost::shared_ptr<CTreeBase> > m_rawtrees;
      std::vector<boost::shared_ptr<CTreeBase> > m_overlaytrees;
      std::vector<boost::shared_ptr<CTreeBase> > m_pedtrees;
      CStripsTree* m_stripstree;
      TEnv*        m_env_file;
      
      //new mmdaq configs
      std::string m_daq_config_xml;
      std::string m_srs_config_xml;
      std::string m_det_config_xml;
//      std::vector<std::string> m_det_chambers_config_xml;
      std::map<std::string,std::string> m_det_config_xml_chambers;


//      size_t       m_tree_current_entry;
      size_t       m_tree_max_entry;
      size_t       m_current_event_number;
      
      void init_trees();
//      void init_trees_new();
//      void   init_trees_old_apv();
      TEnv*  load_det_config();

      size_t tree_max_entry();
      boost::shared_ptr<CStripsTreeEntry> get_striptree_entry(unsigned strip_number) const;

   };
   
} // namespace

#endif
