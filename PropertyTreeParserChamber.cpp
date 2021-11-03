//
//  PropertyTreeParserChamber.cpp
//  mmdaq3-server
//
//  Created by Marcin Byszewski on 06/07/2012.
//  Copyright (c) 2012 CERN - PH/ADE. All rights reserved.
//

#include "PropertyTreeParserChamber.h"

#include "DetDetector.h"
#include "DetChamber.h"
#include "DetMultiLayer.h"
#include "DetPlane.h"
#include "DetReadout.h"
//#include "DetConnector.h"

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include <iostream>

using namespace browser;

namespace bprt = boost::property_tree;

CPropertyTreeParserChamber::CPropertyTreeParserChamber(const bprt::ptree& chamber_cfg) :
CPropertyTreeParser(chamber_cfg)
{
   
}


CPropertyTreeParserChamber::~CPropertyTreeParserChamber()
{
   
}

void CPropertyTreeParserChamber::configure(CDetChamber* chamber, bool has_multilayers, bool has_layers)
{
   ///TODO: \todo apply read config to this
   
//   std::vector<DetBasePtr> local_multilayers;
//   std::vector<DetConnPtr> local_connectors;
   
   try {
      parse_ptree_chamber_node(ptree_.get_child("chamber"), chamber, has_multilayers, has_layers);
      
//      BOOST_FOREACH(DetConnPtr conn, m_connectors) {
//         chamber->add_connector(conn);
//      }
//      BOOST_FOREACH(DetBasePtr ml, m_multilayers) {
//         chamber->add_child(ml);
//      }
      
   } catch (std::runtime_error &re) {
      std::cout << "ERROR: in chamber configuration: CPropertyTreeParserChamber::configure failed:\n";
      std::cout << re.what() << std::endl;
      throw;
   }
   
}

void CPropertyTreeParserChamber::parse_ptree_chamber_node(const boost::property_tree::ptree& mainnode, CDetChamber* chamber,
                                                          bool has_multilayers, bool has_layers)
{
   std::string idstr;
   std::string name;
   TVector3 size;
      // no position and rotation of the chamber in the chamber file
   
   BOOST_FOREACH(const bprt::ptree::value_type &v, mainnode) {
      if (v.first == "size") {
         double x = v.second.get<double>("x", 0.0);
         double y = v.second.get<double>("y", 0.0);
         double z = v.second.get<double>("z", 0.0);
         size = TVector3(x,y,z);
      }
      else if (v.first == "name") {
         name = v.second.data();
      }
      else if (v.first == "id") {
         idstr = v.second.data();
      }
      else if (v.first == "<xmlcomment>") {
         //         std::cout << "comment in file: " << v.second.data() << std::endl;
      }
      else if (v.first == "<xmlattr>") {
         //         std::cout << "<xmlattr>=" << v.second.data() << std::endl;
         std::string idstr2 = v.second.get("id", "");
         std::string namestr2 = v.second.get("name", "");
         if (!idstr2.empty()) {  
            idstr = idstr2;
         }
         if (!namestr2.empty()) {
            name = namestr2;
         }
      }
   }
   
   
   BOOST_FOREACH(const bprt::ptree::value_type &v, mainnode) {
      if (v.first == "multilayer") {
         try {
            DetBasePtr ml = parse_ptree_chamber_multilayer_node(v, chamber);
            chamber->add_part(ml);
            //            m_multilayers.push_back(ml);
         } catch (std::exception& e) {
            std::cout << "ERROR creating multilayer in " << name << " :\n";
            std::cout << e.what() << std::endl;
         }
      }
      else if (v.first == "layer"){
         DetBasePtr ml = parse_ptree_chamber_layer_node(v, chamber);
         chamber->add_part(ml);
      }
      else if (v.first == "connector") {
         try {
//            DetConnPtr conn = parse_ptree_chamber_connector_node(v, chamber);
            
         } catch (std::exception& e) {
            std::cout << "ERROR creating chamber in " << name << " :\n";
            std::cout << e.what() << std::endl;
         }
      }
   }
   
    
}

DetBasePtr CPropertyTreeParserChamber::parse_ptree_chamber_multilayer_node(const bprt::ptree::value_type& node, CDetBase* parent)
{
   std::string idstr;
   std::string name;
   TVector3 pos;
   TVector3 rot;
   TVector3 size;

   
//   DetBasePtr multilayer(new CDetMultiLayer(parent->get_detector(), parent));
   
   BOOST_FOREACH(const bprt::ptree::value_type &v, node.second) {
      if (v.first == "position") {
         double x = v.second.get<double>("x", 0.0);
         double y = v.second.get<double>("y", 0.0);
         double z = v.second.get<double>("z", 0.0);
         pos = TVector3(x,y,z);
      }
      else if (v.first == "size") {
         double x = v.second.get<double>("x", 0.0);
         double y = v.second.get<double>("y", 0.0);
         double z = v.second.get<double>("z", 0.0);
         size = TVector3(x,y,z);
      }
      else if (v.first == "rotation") {
         double x = v.second.get<double>("x", 0.0);
         double y = v.second.get<double>("y", 0.0);
         double z = v.second.get<double>("z", 0.0);
         rot = TVector3(x,y,z);
      }
      else if (v.first == "name") {
         name = v.second.data();
      }
      else if (v.first == "id") {
         idstr = v.second.data();
      }
      
      else if (v.first == "<xmlcomment>") {
         //         std::cout << "comment in file: " << v.second.data() << std::endl;
      }
      else if (v.first == "<xmlattr>") {
         //         std::cout << "<xmlattr>=" << v.second.data() << std::endl;
         std::string idstr2 = v.second.get<std::string>("id","");
         std::string namestr2 = v.second.get<std::string>("name","");
         if (!idstr2.empty()) {  
            idstr = idstr2;
         }
         if (!namestr2.empty()) {
            name = namestr2;
         }
         //         std::cout << "read name=" << name << std::endl;
      }
      
   }
   size_t idnum = boost::lexical_cast<size_t>(idstr);
   
//   std::cout << "parse_ptree_chamber_multilayer_node : id= " << idnum << std::endl;
//   std::cout << "parse_ptree_chamber_multilayer_node : id= " << idnum << " name="<< name << std::endl;

   std::stringstream name_hack_id;
   name_hack_id << idnum;
   
   DetBasePtr multilayer(new CDetMultiLayer(parent->get_detector(), parent, name_hack_id.str(), idnum, size, pos, rot ));
   
   BOOST_FOREACH(const bprt::ptree::value_type &v, node.second) {
      if (v.first == "layer") {
         try {
            DetBasePtr layer = parse_ptree_chamber_layer_node(v, multilayer.get());
            multilayer->add_part(layer);
         } catch (std::exception& e) {
            std::cout << "ERROR creating layer in " << node.first << " " << name << ":\n";
            throw;
         }
      }
   }
   
   return multilayer;
}

DetBasePtr CPropertyTreeParserChamber::parse_ptree_chamber_layer_node(const bprt::ptree::value_type& node, CDetBase* parent)
{
   std::string idstr;
   std::string name;
   TVector3 pos;
   TVector3 rot;
   TVector3 size;
   
//   std::cout << "parse_ptree_chamber_layer_node : node: " << node.first << std::endl;

   
   BOOST_FOREACH(const bprt::ptree::value_type &v, node.second) {
      if (v.first == "position") {
         double x = v.second.get<double>("x", 0.0);
         double y = v.second.get<double>("y", 0.0);
         double z = v.second.get<double>("z", 0.0);
         pos = TVector3(x,y,z);
      }
      else if (v.first == "size") {
         double x = v.second.get<double>("x", 0.0);
         double y = v.second.get<double>("y", 0.0);
         double z = v.second.get<double>("z", 0.0);
         size = TVector3(x,y,z);
      }
      else if (v.first == "rotation") {
         double x = v.second.get<double>("x", 0.0);
         double y = v.second.get<double>("y", 0.0);
         double z = v.second.get<double>("z", 0.0);
         rot = TVector3(x,y,z);
      }
      else if (v.first == "name") {
         name = v.second.data();
      }
      else if (v.first == "id") {
         idstr = v.second.data();
      }
      else if (v.first == "<xmlcomment>") {
         //         std::cout << "comment in file: " << v.second.data() << std::endl;
      }
      else if (v.first == "<xmlattr>") {
         //         std::cout << "<xmlattr>=" << v.second.data() << std::endl;
         std::string idstr2 = v.second.get<std::string>("id","");
         std::string namestr2 = v.second.get<std::string>("name","");
         if (!idstr2.empty()) {  
            idstr = idstr2;
         }
         if (!namestr2.empty()) {
            name = namestr2;
         }
         //         std::cout << "read name=" << name << std::endl;
      }
      
   }
   size_t idnum = boost::lexical_cast<size_t>(idstr);
//   std::cout << "parse_ptree_chamber_layer_node : id= " << idnum << " name="<< name << std::endl;

   std::stringstream name_hack_id;
   name_hack_id << idnum;

   DetBasePtr layer(new CDetPlane(parent->get_detector(), parent, name_hack_id.str(), idnum,  size, pos, rot ));

   BOOST_FOREACH(const bprt::ptree::value_type &v, node.second) {
      if (v.first == "readout") {
         try {
            DetBasePtr rd = parse_ptree_chamber_readout_node(v, layer.get());
            layer->add_part(rd);
         } catch (std::exception& e) {
            std::cout << "ERROR creating readout in " << node.first << " " << name << ":\n";
            throw;
         }
      }
   }
   
   return layer;
}



DetBasePtr CPropertyTreeParserChamber::parse_ptree_chamber_readout_node(const bprt::ptree::value_type& node, CDetBase* parent)
{
   std::string idstr;
   std::string readout_name;
   TVector3 pos;
   TVector3 rot;
   TVector3 size;
   double pitch;
   std::pair<int,int> strip_range;
   

   BOOST_FOREACH(const bprt::ptree::value_type &v, node.second) {
      if (v.first == "position") {
         double x = v.second.get<double>("x", 0.0);
         double y = v.second.get<double>("y", 0.0);
         double z = v.second.get<double>("z", 0.0);
         pos = TVector3(x,y,z);
      }
      else if (v.first == "size") {
         double x = v.second.get<double>("x", 0.0);
         double y = v.second.get<double>("y", 0.0);
         double z = v.second.get<double>("z", 0.0);
         size = TVector3(x,y,z);
      }
      else if (v.first == "rotation") {
         double x = v.second.get<double>("x", 0.0);
         double y = v.second.get<double>("y", 0.0);
         double z = v.second.get<double>("z", 0.0);
         rot = TVector3(x,y,z);
      }
      else if (v.first == "name") {
         readout_name = v.second.data();
      }
      else if (v.first == "id") {
         idstr = v.second.data();
      }
      else if (v.first == "strip_range") {
         double x1 = v.second.get<double>("min", 0.0);
         double x2 = v.second.get<double>("max", 0.0);
         if (x1 == 0 || x2 == 0) {
            throw std::runtime_error("Config error strip range out of bounds");
         }
         strip_range = std::pair<int, int>(x1, x2);
      }
      else if (v.first == "pitch") {
         pitch = v.second.get_value<double>(); //?
//         std::cout << "is OK ? pitch is " << pitch << std::endl;
      }
      
      else if (v.first == "<xmlcomment>") {
         //         std::cout << "comment in file: " << v.second.data() << std::endl;
      }
      else if (v.first == "<xmlattr>") {
         //         std::cout << "<xmlattr>=" << v.second.data() << std::endl;
         std::string idstr2 = v.second.get<std::string>("id","");
         std::string namestr2 = v.second.get<std::string>("name","");
         if (!idstr2.empty()) {  
            idstr = idstr2;
         }
         if (!namestr2.empty()) {
            readout_name = namestr2;
         }
         //         std::cout << "read name=" << name << std::endl;
      }
      
   }
   
   
   ////////
//   //   CDetDetector* det, CDetBase* parent, const std::string& name,
//   size_t id_number, const TVector3& size, const TVector3& pos, const TVector3& rot,
//   double phi_deg,  double pitch_mm,  std::pair<long, long> striprange);
   
   size_t id_number = boost::lexical_cast<size_t>(idstr);
   
   ///
   DetBasePtr readout;
   if (std::fabs(rot.z()) < 0.0001) {
      readout.reset( new CDetReadoutH(parent->get_detector(), parent, readout_name, id_number,
                                      size, pos, rot, pitch, strip_range) );
   }
   else if (std::fabs(rot.z() - 90.0) < 0.0001 ) {
      readout.reset( new CDetReadoutV(parent->get_detector(), parent, readout_name, id_number,
                                 size, pos, rot,  pitch, strip_range) );
   }
   else {
      readout.reset( new CDetReadout(parent->get_detector(), parent, readout_name, id_number,
                                size, pos, rot, rot.z(), pitch, strip_range) );
   }

   return readout;
}


/*
DetConnPtr CPropertyTreeParserChamber::parse_ptree_chamber_connector_node(const bprt::ptree::value_type& node, DetChamPtr chamber)
{   
   std::string name;
   std::string idstr;
   std::string map_file;
   
   BOOST_FOREACH(const bprt::ptree::value_type &v, node.second) {
      if (v.first == "map_file") {
         map_file =  v.second.data();
      }
      else if (v.first == "name") {
         name =  v.second.data();
      }
      else if (v.first == "id") {
          idstr =  v.second.data();
      }
      else if (v.first == "<xmlcomment>") {
         //         std::cout << "comment in file: " << v.second.data() << std::endl;
      }
      else if (v.first == "<xmlattr>") {
         std::string idstr2 = v.second.get("id", "");
         std::string namestr2 = v.second.get("name", "");
         if (!idstr2.empty()) {  
            idstr = idstr2;
         }
         if (!namestr2.empty()) {
            name = namestr2;
         }
      }
      else {
         std::stringstream ss;
         ss << "bad data in chamber config: unknown key '" << v.first << "'" << "' in node " <<  node.first;
         std::cout << ss.str() << std::endl;
         
      }
   }

   
   //set correct path to the map file
   boost::filesystem::path map_path(map_file);
   //make absolute paths for included config files
   if (!map_path.has_parent_path()) {
      map_path = daqconfig_.get_config_path().parent_path() / map_path;
   }
   
   boost::shared_ptr<CDetConnector> conn(new CDetConnector(chamber->get_detector(), chamber, 0,name));
   conn->read_strip_map_file(map_path.string());
   chamber->add_connector(conn);
   return conn;
}
*/


