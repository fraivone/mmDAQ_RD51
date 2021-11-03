//
//  RootThread.h
//  mmdaq3-server
//
//  Created by Marcin Byszewski on 04/07/2012.
//  Copyright 2012 CERN - PH/ADE. All rights reserved.
//

#ifndef mmdaq3_server_RootThread_h
#define mmdaq3_server_RootThread_h

#include <TThread.h>
#include <iostream>

//#include <boost/thread/mutex.hpp>
//#include <boost/thread.hpp>

//#include <boost/thread/condition.hpp>

namespace browser {

      
//      class CRootThread
//      {
//         
//
//      public:
//         CRootThread();
//         virtual ~CRootThread();
//
//         
//      };
      
      class CRootThreadScopedLock
      {
//         boost::lock_guard<boost::mutex> lock;
      public:
         CRootThreadScopedLock()
//         : lock(RootGlobalMutex)
         {
            std::cout <<"TThread::Lock()\n"; 
            status = TThread::Lock();
            
         }
         virtual ~CRootThreadScopedLock() {
            std::cout <<"TThread::UnLock()\n";
            TThread::UnLock();
         }
         int status;
         
//         static boost::mutex RootGlobalMutex;
         
      };
      
      
   
} // namespace

#endif
