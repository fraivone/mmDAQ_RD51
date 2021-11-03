//
//  PublisherIpcData.h
//  mmdaq3-server
//
//  Created by Marcin Byszewski on 1/17/13.
//  Copyright (c) 2013 CERN - PH/ADE. All rights reserved.
//

#ifndef mmdaq3_server_PublisherIpcData_h
#define mmdaq3_server_PublisherIpcData_h

//#include "SharedTypes.h"

#include <boost/interprocess/sync/interprocess_semaphore.hpp>



namespace browser {
   struct SPublisherIpcData {
      
      
      boost::interprocess::interprocess_semaphore sem_server_stored;
      boost::interprocess::interprocess_semaphore sem_client_finished;
      boost::interprocess::interprocess_semaphore sem_mutex;

      //add int hearbeat, pulse counter
      bool flag_new_config;
      bool flag_new_event_data;
      int server_state;
      
      
      SPublisherIpcData() :
      sem_server_stored(0),
      sem_client_finished(1),
      sem_mutex(1),
      flag_new_config(false),
      flag_new_event_data(false),
      server_state(0)
      {}
   };
   

}


#endif
