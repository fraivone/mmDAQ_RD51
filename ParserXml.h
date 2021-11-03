//
//  ParserXml.h
//  eventbrowser
//
//  Created by Marcin Byszewski on 09/10/2012.
//  Copyright 2012 CERN - PH/ADE. All rights reserved.
//

#ifndef eventbrowser_ParserXml_h
#define eventbrowser_ParserXml_h

#include <boost/property_tree/ptree.hpp>
#include <boost/shared_ptr.hpp>

#include <string>
#include <map>


namespace browser {

   class CDetBase;
   class CDetDetector;
   class CDetChamber;
   class CReaderBase;
   
   
   class CParserXml
   {
      
   public:
      CParserXml(const std::string& daq_xml,
                 const std::string& srs_xml,
                 const std::string& det_xml);
      virtual ~CParserXml();

      void read_chamber_configs(CReaderBase* m_reader, bool has_multilayers, bool has_layers);
      boost::shared_ptr<CDetDetector> get_detector() { return m_detector;}

      
   protected:
      const std::string m_daq_xml;
      const std::string m_srs_xml;
      const std::string m_det_xml;
      boost::property_tree::ptree m_daq_config;
      boost::property_tree::ptree m_srs_config;
      boost::property_tree::ptree m_det_config;
      std::map<std::string, boost::property_tree::ptree> m_chamber_configs;
      boost::shared_ptr<CDetDetector> m_detector;
      
      void parse(CDetDetector* det);
      void parse_srs(const boost::property_tree::ptree& pt);
      void parse_det(const boost::property_tree::ptree& pt, CDetDetector* det);
      void parse_ptree_det_detector_node(const boost::property_tree::ptree& mainnode, CDetDetector* det);
      boost::shared_ptr<CDetBase> parse_ptree_det_chamber_node(const boost::property_tree::ptree::value_type& node,
                                        CDetDetector* det);
      void parse_chamber_config(const boost::property_tree::ptree& pt, CDetChamber* chamber,  bool has_multilayers, bool has_layers);

      
      

   };
   
} // namespace

#endif
