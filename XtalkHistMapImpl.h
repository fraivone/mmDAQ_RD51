//
//  XtalkHistMapImpl.h
//  eventbrowser
//
//  Created by Marcin Byszewski on 5/30/13.
//  Copyright (c) 2013 CERN - PH/ADE. All rights reserved.
//

#ifndef __eventbrowser__XtalkHistMapImpl__
#define __eventbrowser__XtalkHistMapImpl__

#include <iostream>
#include <map>
#include <boost/shared_ptr.hpp>

class TH2F;

namespace browser {

   class XtalkHistMapImpl {
      
   public:
      std::map<int, boost::shared_ptr<TH2F> > data; /// ch vs all channels
      
   };
}


#endif /* defined(__eventbrowser__XtalkHistMapImpl__) */
