//
//  DetectorBuilder.h
//  browser
//
//  Created by Marcin Byszewski on 7.4.12.
//  Copyright 2012 CERN - PH/ADE. All rights reserved.
//

#ifndef browser_DetectorBuilder_h
#define browser_DetectorBuilder_h

#include <boost/shared_ptr.hpp>


class TEnv;

namespace browser {
   
   class CReaderBase;
   class CRootReader;
   class CShmemReader;
   class CDetDetector;
   class CGuiMainFrame;
   
   class CApvRawTree;
   class CMmdaq3TreeApvRaw;
   
   class CDetectorBuilder
   {
      
   public:
      CDetectorBuilder(CRootReader* reader, CGuiMainFrame*  gui);
      CDetectorBuilder(CShmemReader* reader, CGuiMainFrame*  gui);
      virtual ~CDetectorBuilder();
      boost::shared_ptr<CDetDetector> get_detector();
      
   private:
      CReaderBase*  m_reader;
      boost::shared_ptr<CDetDetector> m_detector;
      CGuiMainFrame*  m_gui;
      
      void load_det_config();
      void parse_raw_trees(CDetDetector* det);
      boost::shared_ptr<CDetDetector> parse_tenv(TEnv* env);
      boost::shared_ptr<CDetDetector> parse_xml(const std::string& daq_config_xml,
                                                const std::string& srs_config_xml,
                                                const std::string& det_config_xml);
//      CDetDetector* parse_apv_raw_tree(CDetDetector* det, CApvRawTree* apvtree);
//      CDetDetector* parse_mmdaq3_apv_raw_tree(CDetDetector* det, CMmdaq3TreeApvRaw* apvtree3);

      bool m_has_multilayers;
      bool m_has_layers;
   };
   
} // namespace

#endif
