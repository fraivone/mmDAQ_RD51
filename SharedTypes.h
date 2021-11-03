//
//  SharedTypes.h
//  mmdaq3-server
//
//  Created by Marcin Byszewski on 06/07/2012.
//  Copyright (c) 2012 CERN - PH/ADE. All rights reserved.
//

#ifndef mmdaq3_server_SharedTypes_h
#define mmdaq3_server_SharedTypes_h

#include "SrsChannelId.h"
//#include "SharedSrsChannelData.h"

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/smart_ptr/shared_ptr.hpp>
#include <boost/interprocess/smart_ptr/deleter.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/named_condition.hpp>
#include <boost/interprocess/sync/named_condition_any.hpp>


//#include <string>
#include <cstdlib> //std::system
#include <stddef.h> //size_t
//#include <vector>


namespace mmdaq3 {
   namespace server {
      
      //      class CSrsChannelId;
      //      class CSharedSrsChannelData;
      
      
      //      typedef enum {
      //         shChipTypeInvalid,
      //         shChipTypeAPV25,
      //         shChipTypeAPV25Raw,
      //         shChipTypeAPV25ZsRaw,
      //         shChipTypeAPV25ZsPeak,
      //         shChipTypeVMM1,
      //         shChipTypeVMM1BNLMiniDaq
      //      } SharedChipType;
      
      
      namespace bipc = boost::interprocess;
      typedef bipc::managed_shared_memory::segment_manager ShmemSegmentManagerType;
      
      //1) Define an STL compatible allocator of ints that allocates from the managed_shared_memory.
      //This allocator will allow placing containers in the segment
      
      //2) Alias a vector that uses the previous STL-like allocator so that allocates
      //its values from the segment
      
      
      //Typedefs of allocators and containers
      typedef bipc::allocator<void, ShmemSegmentManagerType>                        ShmemVoidAllocator;
      
      
      //string, vector of string
      typedef bipc::allocator<char, ShmemSegmentManagerType>                        ShmemCharAllocator;
      typedef bipc::basic_string<char, std::char_traits<char>, ShmemCharAllocator>  ShmemCharString;
      
      typedef bipc::allocator<ShmemCharString, ShmemSegmentManagerType>             ShmemCharVectorAllocator;
      typedef bipc::vector<ShmemCharString, ShmemCharVectorAllocator>               ShmemCharStringVector;
      
      
      //uint32
      typedef bipc::allocator<uint32_t, ShmemSegmentManagerType>  ShmemUint32Allocator;
      typedef bipc::vector<uint32_t, ShmemUint32Allocator>        ShmemUint32Vector;
      
      //uint16
      typedef bipc::allocator<int16_t, ShmemSegmentManagerType>  ShmemInt16Allocator;
      typedef bipc::vector<int16_t, ShmemInt16Allocator>        ShmemInt16Vector;
      
      //uint16
      typedef bipc::allocator<unsigned long, ShmemSegmentManagerType>  ShmemULongAllocator;
      typedef bipc::vector<unsigned long, ShmemULongAllocator>        ShmemULongVector;
      
      
      //double vector
      typedef bipc::allocator<double, ShmemSegmentManagerType>    ShmemDblAllocator;
      typedef bipc::vector<double, ShmemDblAllocator>             ShmemDblVector;
      
      //vector of vectors double
      typedef bipc::allocator<ShmemDblVector, ShmemSegmentManagerType>  ShmemDblVectorAllocator;
      typedef bipc::vector<ShmemDblVector, ShmemDblVectorAllocator>     ShmemDblVectorVector;
      
      
      typedef boost::interprocess::interprocess_mutex ShmemMutexType;
      typedef boost::interprocess::scoped_lock< ShmemMutexType > ShmemScopedLock;
      
      typedef boost::interprocess::named_mutex ShmemNamedMutexType;
      typedef boost::interprocess::scoped_lock< ShmemNamedMutexType > ShmemNamedScopedLock;
      
      typedef boost::interprocess::named_condition_any ShmemNamedCondition;
      
      
      //all this to have map of vectors ShmemRawDataMap
      
      //Note that map<Key, MappedType>'s value_type is std::pair<const Key, MappedType>,
      //so the allocator must allocate that pair.
      //map is [chipid] = (raw vector int16)
      typedef uint32_t  ShmemRawMapKeyType;
      typedef ShmemInt16Vector ShmemRawMapMappedType;
      typedef std::pair<const ShmemRawMapKeyType, ShmemRawMapMappedType> ShmemRawMapValueType;
      
      //Alias an STL compatible allocator of for the map.
      //This allocator will allow to place containers
      //in managed shared memory segments
      typedef bipc::allocator<ShmemRawMapValueType, ShmemSegmentManagerType> ShmemRawMapAllocator;
      
      //Alias a map of ints that uses the previous STL-like allocator.
      //Note that the third parameter argument is the ordering function
      //of the map, just like with std::map, used to compare the keys.
      typedef bipc::map<ShmemRawMapKeyType, ShmemRawMapMappedType, std::less<ShmemRawMapKeyType>, ShmemRawMapAllocator> ShmemRawMap;
      
      
      // Shared Map for strip data
      
      
      
      
      
      
      
      
   }
}

#endif
