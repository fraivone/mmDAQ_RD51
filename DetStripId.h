//
//  DetStripId.h
//  mmdaq3-server
//
//  Created by Marcin Byszewski on 06/07/2012.
//  Copyright 2012 CERN - PH/ADE. All rights reserved.
//

#ifndef mmdaq3_server_DetStripId_h
#define mmdaq3_server_DetStripId_h

#include <cstddef>
#include <string>
#include <sstream>
#include <ostream>
#include <iostream>

#include <boost/lexical_cast.hpp>

namespace browser { 
   
   class CDetStripId
   {

      
   public:
      explicit CDetStripId();
      CDetStripId(const std::string& m_chamber_name,
                  size_t multi_layer_id, 
                  size_t layer_id,
                  char readout_name,
                  size_t strip_id);
      virtual ~CDetStripId();
      
      friend std::ostream& operator<< (std::ostream & out, const CDetStripId & sid);

      std::string multilayer_id_to_name() const { return id_to_name(m_multi_layer_id);}
      std::string layer_id_to_name() const { return id_to_name(m_layer_id); }
      std::string readout_id_to_name() const { return id_to_name(m_readout_name); }
      std::string strip_id_to_name() const { return id_to_name(m_strip_no); }
      size_t chamber_name_to_id() const { return name_to_id(m_chamber_name);} // will return 0 for most chamber names

     std::string chamber_name() const { return m_chamber_name;}
     size_t multilayer_id() const { return m_multi_layer_id;}
     size_t layer_id() const { return m_layer_id;}
     char readout_id() const { return m_readout_name;}
     size_t strip_no() const { return m_strip_no;}
      
      static std::string id_to_name(size_t num) {
         std::stringstream ss;
         ss << num;
         return ss.str();
      }
      
      static size_t name_to_id(const std::string& name) {
         try {
            size_t idnum = boost::lexical_cast<size_t>(name);
            return idnum;
         } catch (boost::bad_lexical_cast &bc) {
            std::cerr << "CDetStripId: bad conversion of name to id\n";
            return 0;
         }
      }
      
   protected:
      
      std::string m_chamber_name;
      size_t m_multi_layer_id;
      size_t m_layer_id;
      char   m_readout_name;
      size_t m_strip_no;
      
      
   };
      
      std::ostream& operator<< (std::ostream & out, const CDetStripId & sid);
   
} // namespace

#endif
