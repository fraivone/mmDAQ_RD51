//
//  SharedTypeChannelStripMap.h
//  mmdaq3-server
//
//  Created by Marcin Byszewski on 3/10/13.
//  Copyright (c) 2013 CERN - PH/ADE. All rights reserved.
//

#ifndef mmdaq3_server_SharedTypeChannelStripMap_h
#define mmdaq3_server_SharedTypeChannelStripMap_h

#include "SharedTypes.h"


#include <boost/interprocess/managed_shared_memory.hpp> //channel-strip map:
#include <boost/interprocess/allocators/allocator.hpp> //channel-strip map:
#include <boost/unordered_map.hpp>     //channel-strip map: boost::unordered_map
#include <functional>                  //channel-strip map: std::equal_to
#include <boost/functional/hash.hpp>   //channel-strip map: boost::hash
#include <boost/lexical_cast.hpp>


namespace mmdaq3 {
   namespace server {
      
      struct ShmemStripId
      {
        
         
      public:
         ShmemStripId(ShmemSegmentManagerType* ssm) :
         chamber_name(ssm), multi_layer_id(0), layer_id(0), readout_name_char(0), strip_no(0)
         {};
         
         ShmemStripId(const std::string& chamber, unsigned mlay, unsigned lay,
                      char rd, unsigned strip,
                      ShmemSegmentManagerType* ssm) :
         chamber_name(ssm), multi_layer_id(mlay), layer_id(lay),
         readout_name_char(boost::lexical_cast<unsigned>(rd)), strip_no(strip)
         {
            chamber_name = chamber.c_str();
         };
         
         
         
      public:
         ShmemCharString chamber_name;
         unsigned multi_layer_id;
         unsigned layer_id;
         unsigned readout_name_char;
         unsigned strip_no;
      
      };
      
      //Note that unordered_map<Key, MappedType>'s value_type is std::pair<const Key, MappedType>,
      //so the allocator must allocate that pair.
      typedef uint32_t ShmemStripMapKeyType;
      typedef ShmemStripId ShmemStripMapMappedType;
      typedef std::pair<const ShmemStripMapKeyType, ShmemStripMapMappedType> ShmemStripMapValueType;
      
      //Typedef the allocator
      typedef bipc::allocator<ShmemStripMapValueType, ShmemSegmentManagerType> ShmemStripMapAllocator;
      
      //Alias an unordered_map  that uses the previous STL-like allocator.
      typedef boost::unordered_map
      < ShmemStripMapKeyType , ShmemStripMapMappedType
      , boost::hash<ShmemStripMapKeyType>  ,std::equal_to<ShmemStripMapKeyType>
      , ShmemStripMapAllocator>
      ShmemStripMap;
      
//      //Construct a shared memory hash map.
//      //Note that the first parameter is the initial bucket count and
//      //after that, the hash function, the equality function and the allocator
//      MyHashMap *myhashmap = segment.construct<MyHashMap>("MyHashMap")  //object name
//      ( 3, boost::hash<int>(), std::equal_to<int>()                  //
//       , segment.get_allocator<ValueType>());                         //allocator instance

      
   }
} //namespace




#endif
