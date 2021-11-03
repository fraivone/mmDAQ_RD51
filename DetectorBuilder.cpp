//
//  DetectorBuilder.cpp
//  browser
//
//  Created by Marcin Byszewski on 7.4.12.
//  Copyright (c) 2012 CERN - PH/ADE. All rights reserved.
//

#include "DetectorBuilder.h"
#include "ReaderBase.h"
#include "RootReader.h"
#include "ShmemReader.h"
#include "ParserApvRawTree.h"
#include "ParserMmdaq3ApvRawTree.h"
#include "ParserTEnv.h"
#include "ParserXml.h"
#include "DetDetector.h"
#include "CApvRawTree.h"
#include "Mmdaq3TreeApvRaw.h"

#include <TFile.h>
#include <TVector3.h>
#include <TEnv.h>

#include <boost/make_shared.hpp>

//#include <boost/property_tree/ptree.hpp>

#include <iostream>
#include <stdexcept>



using namespace browser;

namespace bprt = boost::property_tree;

CDetectorBuilder::CDetectorBuilder(CRootReader* reader, CGuiMainFrame* gui) :
m_reader(reader), m_detector(), m_gui(gui),
m_has_multilayers(false), m_has_layers(false)
{
   
   TEnv* env = m_reader->get_env();
   const std::string& daq_config_xml = m_reader->get_daq_config_xml();
   const std::string& srs_config_xml = m_reader->get_srs_config_xml();
   const std::string& det_config_xml = m_reader->get_det_config_xml();

   if (!daq_config_xml.empty() &&
       !srs_config_xml.empty() &&
       !det_config_xml.empty() ) {
      //build from xml config
      m_detector = parse_xml(daq_config_xml, srs_config_xml, det_config_xml);
      parse_raw_trees(m_detector.get()); // needed?

   }
   else if (env) {
      //build from config
      m_detector = parse_tenv(env);
      parse_raw_trees(m_detector.get());
   }
   else {
      //build from raw tree
      m_detector = boost::shared_ptr<CDetDetector>(new CDetDetector(0, 0, "Detector", TVector3(), TVector3(), TVector3()));
      parse_raw_trees(m_detector.get());
   }
   
   if (!m_detector) {
      m_detector = boost::shared_ptr<CDetDetector>(new CDetDetector(0, 0, "Empty", TVector3(), TVector3(), TVector3()));
   }
}


CDetectorBuilder::CDetectorBuilder(CShmemReader* reader, CGuiMainFrame* gui) :
m_reader(reader), m_detector(), m_gui(gui),
m_has_multilayers(false), m_has_layers(false)
{
   const std::string& daq_config_xml = m_reader->get_daq_config_xml();
   const std::string& srs_config_xml = m_reader->get_srs_config_xml();
   const std::string& det_config_xml = m_reader->get_det_config_xml();
   
   if (!daq_config_xml.empty() &&
       !srs_config_xml.empty() &&
       !det_config_xml.empty() ) {
      //build from xml config
      m_detector = parse_xml(daq_config_xml, srs_config_xml, det_config_xml);
   } else {
      m_detector = boost::make_shared<CDetDetector>((CReaderBase*)(0), (CConfiguration*)(0), "Empty", TVector3(), TVector3(), TVector3());
   }
}




CDetectorBuilder::~CDetectorBuilder()
{
   
}

boost::shared_ptr<CDetDetector> CDetectorBuilder::get_detector()
{
    return m_detector;
}


void CDetectorBuilder::parse_raw_trees(CDetDetector* det)
{
//   std::cout << "CRootReader::parse_raw_trees " << std::endl;
   m_reader->parse_trees(det, m_gui);
   return;
   
  
 
}



boost::shared_ptr<CDetDetector> CDetectorBuilder::parse_tenv(TEnv* env)
{
   std::cout << "CRootReader::parse_tenv " << std::endl;
   if(!env) {
      return boost::shared_ptr<CDetDetector>();
   }
   try {
      CParserTEnv parser(env);
      return parser.get_detector();
   } catch (std::runtime_error &e) {
      std::cout << "ERROR: CRootReader::parse_tenv() " << e.what() << std::endl;
   }
   return boost::shared_ptr<CDetDetector>();
}


boost::shared_ptr<CDetDetector> CDetectorBuilder::parse_xml(const std::string& daq_xml,
                                                            const std::string& srs_xml,
                                                            const std::string& det_xml)
{
//   std::cout << "CDetectorBuilder::parse_xml " << std::endl;
   if(daq_xml.empty() || srs_xml.empty() || det_xml.empty() ) {
      return boost::shared_ptr<CDetDetector>();
   }
   try {
      CParserXml parser(daq_xml, srs_xml, det_xml);
      parser.read_chamber_configs(m_reader,  m_has_multilayers, m_has_layers);
      return parser.get_detector();
   } catch (std::runtime_error &e) {
      std::cout << "ERROR: CDetectorBuilder::parse_xml() " << e.what() << std::endl;
   }
   return boost::shared_ptr<CDetDetector>();
}
