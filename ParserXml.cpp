//
//  ParserXml.cpp
//  eventbrowser
//
//  Created by Marcin Byszewski on 09/10/2012.
//  Copyright (c) 2012 CERN - PH/ADE. All rights reserved.
//

#include "ParserXml.h"
#include "PropertyTreeParserChamber.h"
#include "DetDetector.h"
#include "DetChamber.h"

#include "ReaderBase.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>

#include <iostream>
#include <sstream>

using namespace browser;
namespace bprt = boost::property_tree;
////

CParserXml::CParserXml(const std::string& daq_xml,
                       const std::string& srs_xml,
                       const std::string& det_xml) :
m_daq_xml(daq_xml), m_srs_xml(srs_xml), m_det_xml(det_xml),
m_daq_config(),
m_srs_config(),
m_det_config(),
m_chamber_configs(),
m_detector()
{
   try {
      std::istringstream ssdaq(daq_xml);
      read_xml(ssdaq, m_daq_config);
      std::istringstream sssrs(srs_xml);
      read_xml(sssrs, m_srs_config);
      std::istringstream ssdet(det_xml);
      read_xml(ssdet, m_det_config);
   } catch (bprt::xml_parser_error &perr) {
      std::cout << "ERROR: CParserXml : " << perr.what() << std::endl;
      throw std::runtime_error("CParserXml: xml parser error");
   }
   
   if (m_daq_config.empty() || m_srs_config.empty() || m_det_config.empty()) {
      throw std::runtime_error("CParserXml: xml config empty");
   }
   
   //try?
   if (!m_detector) {
      m_detector = boost::shared_ptr<CDetDetector>(new CDetDetector(0, 0, "DetectorXml", TVector3(), TVector3(), TVector3()));
   }
   
   parse(m_detector.get());
//   m_detector->print();
   
}


CParserXml::~CParserXml()
{
   
}


void CParserXml::parse(CDetDetector* det)
{
   parse_srs(m_srs_config);
   parse_det(m_det_config, det);
}

void CParserXml::parse_srs(const bprt::ptree& pt)
{
   
}


void CParserXml::parse_det(const bprt::ptree& pt, CDetDetector* det)
{
   
   parse_ptree_det_detector_node(pt.get_child("detector"), det);
   
   
}


void CParserXml::parse_ptree_det_detector_node(const bprt::ptree& mainnode, CDetDetector* det)
{
   std::string name;
   TVector3 pos;
   TVector3 rot;
   TVector3 size;
   
   BOOST_FOREACH(const bprt::ptree::value_type &v, mainnode) {
      if (v.first == "position") {
         double x = v.second.get<double>("x", 0.0);
         double y = v.second.get<double>("y", 0.0);
         double z = v.second.get<double>("z", 0.0);
         pos = TVector3(x,y,z);
      }
      else if (v.first == "size") {
         double x = v.second.get<double>("x", 0.0);
         double y = v.second.get<double>("y", 0.0);
         double z = v.second.get<double>("z", 0.0);
         size = TVector3(x,y,z);
      }
      else if (v.first == "rotation") {
         double x = v.second.get<double>("x", 0.0);
         double y = v.second.get<double>("y", 0.0);
         double z = v.second.get<double>("z", 0.0);
         rot = TVector3(x,y,z);
      }
      else if (v.first == "name") {
         name = v.second.data();
      }
      else if (v.first == "<xmlcomment>") {
         //         std::cout << "comment in file: " << v.second.data() << std::endl;
      }
      else if (v.first == "<xmlattr>") {
         //         std::cout << "<xmlattr>=" << v.second.data() << std::endl;
         name = v.second.get<std::string>("name");
         //         std::cout << "read name=" << name << std::endl;
      }
   
   }
   det->set(0, name, size, pos, rot);
//   boost::shared_ptr<CDetDetector> detector(new CDetDetector(0, 0, name, size, pos , rot ));
   
   BOOST_FOREACH(const bprt::ptree::value_type &v, mainnode) {
      if (v.first == "chamber") {
         try {
            boost::shared_ptr<CDetBase> chamber = parse_ptree_det_chamber_node(v, det);
            det->add_part(chamber);
         } catch (std::exception& e) {
            std::cout << "ERROR creating chamber in " << name << " :\n";
            std::cout << e.what() << std::endl;
         }
      }
   }   
   
}


boost::shared_ptr<CDetBase> CParserXml::parse_ptree_det_chamber_node(const boost::property_tree::ptree::value_type& node,
                                                                     CDetDetector* det)
{
 
   boost::shared_ptr<CDetChamber> chamber(new CDetChamber(det, det, "", 0, TVector3(), TVector3(), TVector3() ) );
   
//   std::vector<DetConnPtr> connectors;
   std::string chamber_name;
   std::string config_file;
   
   BOOST_FOREACH(const bprt::ptree::value_type& v, node.second) {
      if (v.first == "position") {
         double x = v.second.get<double>("x", 0.0);
         double y = v.second.get<double>("y", 0.0);
         double z = v.second.get<double>("z", 0.0);
         chamber->set_position(TVector3(x,y,z));
      }
      else if (v.first == "size") {
         double x = v.second.get<double>("x", 0.0);
         double y = v.second.get<double>("y", 0.0);
         double z = v.second.get<double>("z", 0.0);
         chamber->set_size(TVector3(x,y,z));
      }
      else if (v.first == "rotation") {
         double x = v.second.get<double>("x", 0.0);
         double y = v.second.get<double>("y", 0.0);
         double z = v.second.get<double>("z", 0.0);
         chamber->set_rotation(TVector3(x,y,z));
      }
      else if (v.first == "name") {
         chamber_name = v.second.data();
         chamber->set_name(chamber_name);
      }
      else if (v.first == "config_file") {
         config_file = v.second.data();
         chamber->set_config_file(config_file);
      }
      else if (v.first == "<xmlcomment>") {
         //
      }
      else if (v.first == "<xmlattr>") {         
         chamber_name = v.second.get<std::string>("name");
         chamber->set_name(chamber_name);         

         //         std::cout << "read name=" << name << std::endl;
      }
   }
   
   BOOST_FOREACH(const bprt::ptree::value_type& v, node.second) {
      if (v.first == "connector") {
         //ignore connectors and maps
      }
   }
   return chamber;
}



void CParserXml::read_chamber_configs(CReaderBase* reader,  bool has_multilayers, bool has_layers)
{
   //iterate over chambers and read config files from root file
   //get all chambers from detector
//   DetBasePtrCont& children = m_detector->get_parts();
   
   std::vector<CDetChamber *> vec;
   m_detector->extract_chambers(vec);
   for (std::vector<CDetChamber *>::iterator it = vec.begin(); it != vec.end(); ++it) {
      
      // change to path
      
      std::string name = (*it)->get_config_file(); //(*it)->get_name();
      std::string ch_xml = reader->load_xml_config(name);
   
      std::istringstream sscham(ch_xml);
      boost::property_tree::ptree cham_config;
      read_xml(sscham, cham_config);
      parse_chamber_config(cham_config, *it, has_multilayers, has_layers);
   }
}

void CParserXml::parse_chamber_config(const boost::property_tree::ptree& pt, CDetChamber* chamber, bool has_multilayers, bool has_layers)
{
   CPropertyTreeParserChamber parser(pt);
   parser.configure(chamber, has_multilayers, has_layers);
}




