//
//  DetTypes.h
//  browser
//
//  Created by Marcin Byszewski on 10.4.12.
//  Copyright (c) 2012 CERN - PH/ADE. All rights reserved.
//

#ifndef browser_DetTypes_h
#define browser_DetTypes_h

#include <boost/shared_ptr.hpp>
#include <list>
#include <vector>

namespace browser {
   
   class CDetBase;
   class CDetStrip;
   typedef boost::shared_ptr<CDetBase> DetBasePtr;
   typedef std::list<DetBasePtr> DetBasePtrCont;
   typedef boost::shared_ptr<CDetStrip> DetStripPtr;
   typedef std::vector<DetStripPtr> DetStripPtrVec;

}


#endif
