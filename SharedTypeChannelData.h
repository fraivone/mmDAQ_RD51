//
//  SharedTypeChannelData.h
//  mmdaq3-server
//
//  Created by Marcin Byszewski on 2/25/13.
//  Copyright (c) 2013 CERN - PH/ADE. All rights reserved.
//

#ifndef mmdaq3_server_SharedTypeChannelData_h
#define mmdaq3_server_SharedTypeChannelData_h

#include "SharedTypes.h"

namespace mmdaq3 {
   namespace server {
      
      
      // shared strip data
      
      
      struct ShmemChannelData {
         ShmemChannelData(std::vector<int16_t> raw, ShmemSegmentManagerType* segmanager) :
         m_time(0.0), m_charge(0.0)
         //         , m_raw(segmanager)
         {
            //            m_raw.assign(raw.begin(), raw.end());
         }
         
         ShmemChannelData(double time, double charge, ShmemSegmentManagerType* segmanager) :
         m_time(time), m_charge(charge)
         //         , m_raw(segmanager)
         {
            
         }
         
         double m_time;
         double m_charge;
         //         ShmemInt16Vector m_raw;
      };
      
      
      
      ///// Shared Map for strip data: Key is the channelId
      typedef uint32_t  ShmemChannelMapKeyType;
      
      ///// Shared Map for strip data: Mapped type is vector of Channels (pointer to?)
      typedef ShmemChannelData ShmemChannelMapMappedType;
      
      // Shared Map for strip data type
      typedef std::pair<const ShmemChannelMapKeyType, ShmemChannelMapMappedType>   ShmemChannelMapValueType;
      typedef bipc::allocator<ShmemChannelMapValueType, ShmemSegmentManagerType> ShmemChannelMapAllocator;
      typedef bipc::map<ShmemChannelMapKeyType, ShmemChannelMapMappedType, std::less<ShmemChannelMapKeyType>, ShmemChannelMapAllocator> ShmemChannelMap;
      

      
      

      
   }
}


#endif
