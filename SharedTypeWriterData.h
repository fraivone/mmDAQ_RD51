
//
//  SharedTypeWriterData.h
//  mmdaq3-server
//
//  Created by Marcin Byszewski on 2/26/13.
//  Copyright (c) 2013 CERN - PH/ADE. All rights reserved.
//

#ifndef mmdaq3_server_SharedTypeWriterData_h
#define mmdaq3_server_SharedTypeWriterData_h

#include "SharedTypes.h"


namespace mmdaq3 {
   namespace server {
 
      struct ShmemWriterData
      {
         ShmemWriterData(ShmemSegmentManagerType* ssm)
         : write_data(false), filename(ssm), evids_to_write(0), last_evid(0),
         run_event_counter(0), megabytes_written(0), run_rate_event(0.0),
         run_rate_bytes_write(0.0)
         { }
         
         ///dry run or write data flag
         bool write_data;
         
         ///name of the file
         ShmemCharString filename;
         
         ///number of events ready to write
         size_t evids_to_write;
         
         ///last evid written
         uint64_t last_evid;
         
         ///total number of events
         size_t run_event_counter;
         
         ///total megabytes written to file
         float megabytes_written;
         
         ///write rate events/s
         float run_rate_event;
         
         ///write rate MB/s
         float run_rate_bytes_write;
         
      };
      
      // vector
      typedef bipc::allocator<ShmemWriterData, ShmemSegmentManagerType>  ShmemWriterDataAllocator;
      typedef bipc::vector<ShmemWriterData, ShmemWriterDataAllocator>    ShmemWriterDataVector;
      

      
   }
}

#endif
