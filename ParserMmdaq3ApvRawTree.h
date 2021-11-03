//
//  ParserMmdaq3ApvRawTree.h
//  eventbrowser
//
//  Created by Marcin Byszewski on 01/08/2012.
//  Copyright 2012 CERN - PH/ADE. All rights reserved.
//

#ifndef eventbrowser_ParserMmdaq3ApvRawTree_h
#define eventbrowser_ParserMmdaq3ApvRawTree_h

#include <cstddef>

class TEnv;

namespace browser {
   
   class CMmdaq3TreeApvRaw;
   class CDetDetector;
   class CGuiMainFrame;
   
   class CParserMmdaq3ApvRawTree
   {
      
   public:
//      CParserMmdaq3ApvRawTree();
      CParserMmdaq3ApvRawTree(CMmdaq3TreeApvRaw* rawtree, CDetDetector* det = 0, CGuiMainFrame* gui = 0);

      virtual ~CParserMmdaq3ApvRawTree();
      
      
      TEnv* get_det_config() const { return m_env;}
      CDetDetector* get_detector() { return m_detector;}

      
   private:
      CMmdaq3TreeApvRaw*  m_rawtree;
      TEnv*         m_env;
      CDetDetector* m_detector;
      CGuiMainFrame* m_gui;
      size_t        n_entries;
      
      void parse_entry(CDetDetector* detector);
      void progress(double val); 
   };
   
} // namespace

#endif
