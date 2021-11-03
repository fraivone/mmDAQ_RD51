//
//  ParserTEnv.h
//  browser
//
//  Created by Marcin Byszewski on 8.4.12.
//  Copyright 2012 CERN - PH/ADE. All rights reserved.
//

#ifndef browser_ParserTEnv_h
#define browser_ParserTEnv_h

#include <boost/shared_ptr.hpp>

class TEnv;

namespace browser {
   
   class CDetDetector;
   
   class CParserTEnv
   {
      
   public:
      CParserTEnv(TEnv* env);
      virtual ~CParserTEnv();
      boost::shared_ptr<CDetDetector> get_detector() { return m_detector;}

      
   private:
      TEnv*         m_env;
      boost::shared_ptr<CDetDetector>  m_detector;
      
      void parse(TEnv* env, CDetDetector* detector);
   };
   
} // namespace

#endif
