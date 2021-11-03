//
//  SharedTypeFecData.h
//  mmdaq3-server
//
//  Created by Marcin Byszewski on 2/25/13.
//  Copyright (c) 2013 CERN - PH/ADE. All rights reserved.
//

#ifndef mmdaq3_server_SharedTypeFecData_h
#define mmdaq3_server_SharedTypeFecData_h


#include "SharedTypes.h"
#include "SharedTypeChipData.h"


namespace mmdaq3 {
   namespace server {
      
      
      
      
      //////////////////////////////////////////////////////////////////////////
      ///structure of monitored values in CSrsNetworkDevice
      /// a FEC has only one IP address (make multiple instances of this for multiple-ip netdev)
      struct ShmemFecData
      {
         ShmemFecData(ShmemSegmentManagerType* ssm) :
         name(ssm), remote_ip(0), buffer_size(0),
         frame_counter(0), last_evid(0), chips(std::less<ShmemChipDataMapKeyType>(), ssm), reserved(ssm)
         {  };
         
         ShmemFecData(ShmemCharString devname,
                      unsigned long remip,
                      size_t bufsz,
                      size_t fr_cnt,
                      uint64_t lastev,
                      ShmemSegmentManagerType* ssm)
         : name(devname, ssm), remote_ip(remip), buffer_size(bufsz),
         frame_counter(fr_cnt), last_evid(lastev), chips(std::less<ShmemChipDataMapKeyType>(), ssm), reserved(ssm)
         {
            //
         }
         
         ShmemFecData(ShmemCharString devname,
                      unsigned long remip,
                      size_t bufsz,
                      size_t fr_cnt,
                      uint64_t lastev,
                      ShmemChipDataMap const& local_chips,
                      const ShmemUint32Vector& other,
                      ShmemSegmentManagerType* ssm)
         : name(devname, ssm), remote_ip(remip), buffer_size(bufsz),
         frame_counter(fr_cnt), last_evid(lastev), chips(local_chips), reserved(ssm)
         {
            reserved.assign(other.begin(), other.end());
         }
         
         ShmemCharString name;
         
         ///remote ip addresses of the fec
         unsigned long remote_ip;
         
         ///input buffer size
         size_t buffer_size;
         
         ///data frame counter, this device only
         size_t frame_counter;
         
         ///last event id build
         uint64_t last_evid;
         
         ///vector of chip-related data
         ShmemChipDataMap chips;
         
         /// reserved for specific information depending on chip type
         ShmemUint32Vector reserved; //bipc::vector<unsigned long>
         
         
         
      };
      
      //ShmemFecData vector
      typedef bipc::allocator<ShmemFecData, ShmemSegmentManagerType>  ShmemFecDataAllocator;
      typedef bipc::vector<ShmemFecData, ShmemFecDataAllocator>  ShmemFecDataVector;
      
      
   }
}


#endif
