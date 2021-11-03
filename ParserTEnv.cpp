//
//  ParserTEnv.cpp
//  browser
//
//  Created by Marcin Byszewski on 8.4.12.
//  Copyright (c) 2012 CERN - PH/ADE. All rights reserved.
//

#include "ParserTEnv.h"
#include "DetDetector.h"
#include "DetChamber.h"
#include "DetPlane.h"
#include "DetReadout.h"

#include "MBtools.h"

#include <TEnv.h>

#include <boost/foreach.hpp>

#include <iostream>
#include <stdexcept>

using namespace browser;


#define ORIGIN_POS  (TVector3(0.0,0.0,0.0))
#define ORIGIN_ROT  (TVector3(0.0,0.0,0.0))

#define DET_SIZE  (TVector3(120,120,1800))
#define CHAMBER_SIZE  (TVector3(90,90,20))
#define PLANE_SIZE  (TVector3(90,90,20))
#define READOUT_SIZE  (TVector3(90,90,5))


CParserTEnv::CParserTEnv(TEnv* env) :
m_env(env), m_detector()
{
   
   if (m_env == 0) {
      throw std::runtime_error("CParserTEnv: NULL TEnv*");  
   }
   
   //try?
   if (!m_detector) {
      m_detector = boost::shared_ptr<CDetDetector>(new CDetDetector(0, 0, "Detector", TVector3(), TVector3(), TVector3()));
   }
   
   
   parse(m_env, m_detector.get());
//   m_detector->print();
   
}


CParserTEnv::~CParserTEnv()
{
   
}

void CParserTEnv::parse(TEnv* env, CDetDetector* detector)
{
   // add chambers
   std::string line = env->GetValue("mmdaq.Chambers", "");
   std::vector<std::string> chamber_names = CSVToVectorFiller<std::string>(",")(line);
   BOOST_FOREACH(std::string chamber_name, chamber_names) {
      CDetChamber* chamber = new CDetChamber(detector, detector, chamber_name, 
                                             detector->get_parts().size(), TVector3(), TVector3(), TVector3());
      detector->add_part(chamber);
      //add planes, readouts to chambers
      std::stringstream ss;
      ss << "mmdaq.Chamber." << chamber_name << ".Planes";
      line = env->GetValue(ss.str().c_str(), "");
      std::vector<std::string> plane_names = CSVToVectorFiller<std::string>(",")(line);
      bool has_planes = !plane_names.empty();
      if (plane_names.empty()) {
         plane_names.push_back("0");
      }
      //add planes
      BOOST_FOREACH(std::string plane_name, plane_names) {
         CDetPlane* plane = new CDetPlane(detector, chamber, plane_name, 
                                              chamber->get_parts().size(), TVector3(), TVector3(), TVector3());
         chamber->add_part(plane);
         
         ss.str("");
         if (has_planes) {
            ss << "mmdaq.Chamber." << chamber_name << "."<< plane_name << "." << "Strips";
         }
         else {
            ss << "mmdaq.Chamber." << chamber_name << ".Strips" ;
         }
         
         std::stringstream plane_prefix;
         plane_prefix.str("");
         plane_prefix << ss.str();
         line = env->GetValue(ss.str().c_str(), "");
         std::vector<std::string> readout_names = CSVToVectorFiller<std::string>(",")(line);
         MakeElementsUnique(readout_names);
         //add readouts
         BOOST_FOREACH(std::string readout_name, readout_names) {
            std::stringstream prefix;
            prefix << plane_prefix.str() << "." << readout_name << ".";
            ss.str(""); ss << prefix.str() << "Angle";
            double angle = env->GetValue(ss.str().c_str(), 0.0);
            ss.str(""); ss << prefix.str() << "Pitch";
            double pitch = env->GetValue(ss.str().c_str(), 1.2345);
            ss.str(""); ss << prefix.str() << "Min";
            size_t min_strip = env->GetValue(ss.str().c_str(), 1);
            ss.str(""); ss << prefix.str() << "Max";
            size_t max_strip = env->GetValue(ss.str().c_str(), 1);

            size_t id_number = plane->get_parts().size();
            //continue as for normal only for the range
            CDetReadoutBase* readout = 0;
            if (std::fabs(angle) < EPS) {
               readout = new CDetReadoutH(detector, plane, readout_name, id_number, 
                                          READOUT_SIZE, ORIGIN_POS, ORIGIN_ROT, pitch,
                                          std::pair<long, long> (min_strip, max_strip));
            }
            else if (std::fabs(angle - 90.0) < EPS ) {
               readout = new CDetReadoutV(detector, plane, readout_name, id_number,
                                          READOUT_SIZE, ORIGIN_POS, ORIGIN_ROT, pitch,
                                          std::pair<long, long> (min_strip, max_strip));
            }
            else {
               
               readout = new CDetReadout(detector, plane, readout_name, id_number, 
                                         READOUT_SIZE, ORIGIN_POS, ORIGIN_ROT, angle,  pitch,
                                         std::pair<long, long> (min_strip, max_strip));
            }
            plane->add_part(readout);
         } //foreach readout
      }//foreach plane
   }//foreach chamber
   
}


