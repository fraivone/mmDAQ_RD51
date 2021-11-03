//
//  ParserApvRawTree.h
//  browser
//
//  Created by Marcin Byszewski on 12/17/11.
//  Copyright 2011 CERN - PH/ADE. All rights reserved.
//

#ifndef browser_ParserApvRawTree_h
#define browser_ParserApvRawTree_h

#include <cstddef>

class TEnv;

namespace browser {

   class CApvRawTree;
   class CDetDetector;
   class CGuiMainFrame;
   
   /**
    parse raw apv tree and reconstruct detector config
    used when working with older files without detconfig info
    return TEnv* with detconfig
    */
   class CParserApvRawTree 
   {
      
   public:
      CParserApvRawTree() : m_rawtree(0), m_env(0), m_detector(0), m_gui(0), n_entries(0) {};
      CParserApvRawTree(CApvRawTree* rawtree, CDetDetector* det, CGuiMainFrame* gui);
      virtual ~CParserApvRawTree();
      
      TEnv* get_det_config() const { return m_env;}
      CDetDetector* get_detector() { return m_detector;}
      
   private:
      CApvRawTree*  m_rawtree;
      TEnv*         m_env;
      CDetDetector* m_detector;
      CGuiMainFrame* m_gui;
      size_t        n_entries;
      
      void parse_entry(CDetDetector* detector);
      void progress(double val); 
   };
   
} // namespace

#endif
