//
//  ParserMmdaq3BnlMiniRawTree.h
//  eventbrowser
//
//  Created by Marcin Byszewski on 07/08/2012.
//  Copyright 2012 CERN - PH/ADE. All rights reserved.
//

#ifndef eventbrowser_ParserMmdaq3BnlMiniRawTree_h
#define eventbrowser_ParserMmdaq3BnlMiniRawTree_h

#include <cstddef>
#include <map>

class TEnv;


namespace browser {

   class CSrsChannelId;
   class CDetStripId;
   class CConfiguration;
   class CTreeRawBnlMini;
   class CDetDetector;
   class CGuiMainFrame;
   
   class CParserMmdaq3BnlMiniRawTree
   {
      
   public:
      CParserMmdaq3BnlMiniRawTree(CTreeRawBnlMini* rawtree, CConfiguration* config, CDetDetector* det = 0, CGuiMainFrame* gui = 0);

      virtual ~CParserMmdaq3BnlMiniRawTree();
      
      TEnv* get_det_config() const { return m_env;}
      CDetDetector* get_detector() { return m_detector;}
      
     
      
   private:
      CConfiguration*   m_config;
      CTreeRawBnlMini*  m_rawtree;
      TEnv*             m_env;
      CDetDetector*     m_detector;
      CGuiMainFrame*    m_gui;
      size_t            n_entries;
      

      void parse_entry(CDetDetector* detector);
      void progress(double val); 
      
   };
   
} // namespace

#endif
