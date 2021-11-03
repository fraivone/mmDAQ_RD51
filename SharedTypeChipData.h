//
//  SharedChipDataType.h
//  mmdaq3-server
//
//  Created by Marcin Byszewski on 2/25/13.
//  Copyright (c) 2013 CERN - PH/ADE. All rights reserved.
//

#ifndef mmdaq3_server_SharedTypeChipData_h
#define mmdaq3_server_SharedTypeChipData_h

#include "SharedTypes.h"


namespace mmdaq3 {
   namespace server {
      
      //////////////////////////////////////////////////////////////////////////
      // structure for monitored chip values and its raw data
      struct ShmemChipData
      {
         
         ShmemChipData(ShmemSegmentManagerType* ssm)
         : name(ssm), chip_id(0), event_buffer_size(0), eb_evid_written(0),
           m_error_count_late_event(0),
           m_error_count_repeated_event_id(0),
           m_error_count_missing_event(0),
           m_error_count_unordered_events(0)
         {}
         

         ShmemChipData(ShmemCharString const& myname,
        		 uint32_t chipid,
        		 size_t ebuff_size,
        		 uint64_t ewrt,
        		 ShmemSegmentManagerType* ssm)
         : name(myname, ssm), chip_id(chipid), event_buffer_size(ebuff_size),
           eb_evid_written(ewrt),
           m_error_count_late_event(0),
           m_error_count_repeated_event_id(0),
           m_error_count_missing_event(0),
           m_error_count_unordered_events(0)
         { }

         
         ///chip name
         ShmemCharString name;
         
         ///chip id
         uint32_t chip_id;
         
         
         //mapping channel-strip, strip-channel (const?)
         //         boost::shared_ptr<CDetConnector> m_connector;
         
         ///pedestals (not all chips)
         //         ApvPedestalMap m_pedestals_map
         
         
         ///event numbers of events currently in chip's buffer
         size_t event_buffer_size;
         
         ///////event data:
         
         ///event number written by EB/RW
         uint64_t eb_evid_written;
         
         ///late event - desynchronised data streams, data loss
         size_t m_error_count_late_event;
         
         ///repeated event id - error in config or time stamp too early, data loss
         size_t m_error_count_repeated_event_id;
         
         ///missing event - when asembling to write to file, data lost
         size_t m_error_count_missing_event;
         
         ///unordered events - the event to write is not at front(), data loss
         size_t m_error_count_unordered_events;
         
         /// reserved for specific information depending on chip type
         
         
      };
      
      //ShmemChipData vector -> map
      typedef bipc::allocator<ShmemChipData, ShmemSegmentManagerType>  ShmemChipDataAllocator;
      typedef bipc::deleter<ShmemChipData, ShmemSegmentManagerType>  ShmemChipDataDeleter;
      typedef bipc::shared_ptr<ShmemChipData, ShmemVoidAllocator, ShmemChipDataDeleter> ShmemChipDataSharedPtr;
      typedef bipc::allocator<ShmemChipDataSharedPtr, ShmemSegmentManagerType>  ShmemChipDataPtrAllocator;
      //      typedef bipc::vector<ShmemChipDataSharedPtr>  ShmemChipDataVector;
      
      //map container for uint-> shared_ptr<chipdata>
      typedef uint32_t  ShmemChipDataMapKeyType;
      typedef ShmemChipDataSharedPtr ShmemChipDataMapMappedType;
      typedef std::pair<const ShmemChipDataMapKeyType, ShmemChipDataMapMappedType>   ShmemChipDataMapValueType;
      typedef bipc::allocator<ShmemChipDataMapValueType, ShmemSegmentManagerType> ShmemChipDataMapAllocator;
      typedef bipc::map<ShmemChipDataMapKeyType, ShmemChipDataMapMappedType, std::less<ShmemChipDataMapKeyType>, ShmemChipDataMapAllocator> ShmemChipDataMap;
      
      
   }
}

#endif
