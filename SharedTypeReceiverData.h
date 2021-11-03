//
//  SharedTypeReceiverData.h
//  mmdaq3-server
//
//  Created by Marcin Byszewski on 2/25/13.
//  Copyright (c) 2013 CERN - PH/ADE. All rights reserved.
//

#ifndef mmdaq3_server_SharedTypeReceiverData_h
#define mmdaq3_server_SharedTypeReceiverData_h

#include "SharedTypes.h"

namespace mmdaq3 {
   namespace server {
      
      
      //////////////////////////////////////////////////////////////////////////
      /// data structure to hold monitored UDPReceiver parameters
      struct ShmemReceiverData
      {
         ShmemReceiverData(unsigned long ip, int state, size_t run_frame_cnt,
                           size_t run_bytes_cnt, double fr_rate, double data_rate) :
         m_local_ip(ip), m_run_state(state), m_run_frame_counter(run_frame_cnt),
         m_run_bytes_total(run_bytes_cnt), m_frame_rate(fr_rate), m_data_rate(data_rate) {
            
         }
         
         /// IP address of UDPReceiever, v4
         unsigned long m_local_ip;
         
         /// run state (error, idle, running)..
         int    m_run_state;
         
         /// frame counter
         size_t m_run_frame_counter;
         /// total number of bytes received
         size_t m_run_bytes_total;
         
         /// rate in frames/s
         double m_frame_rate;
         
         /// rate in B/s
         double m_data_rate;
      };
      
      //ShmemReceiverData vector
      typedef bipc::allocator<ShmemReceiverData, ShmemSegmentManagerType>  ShmemReceiverDataAllocator;
      typedef bipc::vector<ShmemReceiverData, ShmemReceiverDataAllocator>  ShmemReceiverDataVector;

      
      
   }
}

#endif
