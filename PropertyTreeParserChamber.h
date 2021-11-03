//
//  PropertyTreeParserChamber.h
//  mmdaq3-server
//
//  Created by Marcin Byszewski on 06/07/2012.
//  Copyright 2012 CERN - PH/ADE. All rights reserved.
//

#ifndef mmdaq3_server_PropertyTreeParserChamber_h
#define mmdaq3_server_PropertyTreeParserChamber_h

#include "PropertyTreeParser.h"
//#include "DetTypes.h"
//#include "SrsTypes.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/shared_ptr.hpp>

namespace browser {
   class CDetBase;
   class CDetChamber;
   class CDetConnector;
   
   typedef boost::shared_ptr<CDetBase> DetBasePtr;
   typedef boost::shared_ptr<CDetChamber> DetChamPtr;
   typedef boost::shared_ptr<CDetConnector> DetConnPtr;

   
   class CPropertyTreeParserChamber : public CPropertyTreeParser
   {
      
   public:
      CPropertyTreeParserChamber(const boost::property_tree::ptree& m_det_cfg);
      virtual ~CPropertyTreeParserChamber();
      
      void configure(CDetChamber* chamber, bool has_multilayers, bool has_layers);
   protected:
      
      void parse_ptree_chamber_node( const boost::property_tree::ptree& mainnode, CDetChamber* chamber,  bool has_multilayers, bool has_layers);
      DetBasePtr parse_ptree_chamber_multilayer_node(const boost::property_tree::ptree::value_type& node, CDetBase* chamber);
      DetBasePtr parse_ptree_chamber_layer_node(const boost::property_tree::ptree::value_type& node, CDetBase* multilayer);
      DetBasePtr parse_ptree_chamber_readout_node(const boost::property_tree::ptree::value_type& node, CDetBase* layer);
//      DetConnPtr parse_ptree_chamber_connector_node(const boost::property_tree::ptree::value_type& node, DetChamPtr chamber);
      
      
      //         std::vector<DetBasePtr> m_multilayers;
      //         std::vector<DetConnPtr> m_connectors;
      
   };
}

#endif
