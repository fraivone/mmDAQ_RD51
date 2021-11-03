//
//  DetReadout.cpp
//  browser
//
//  Created by Marcin Byszewski on 12/18/11.
//  Copyright (c) 2011 CERN - PH/ADE. All rights reserved.
//

#include "DetReadout.h"
#include "DetStrip.h"
#include "MBtools.h"
#include "GuiTabReadout.h"

#include <TMath.h>
#include <TGListTree.h>
#include <TH1F.h>

#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/date_time/posix_time/conversion.hpp>
#include <iostream>
#include <stdexcept>

using namespace browser;
namespace bpt = boost::posix_time;


CDetReadoutBase::CDetReadoutBase(CDetDetector* det, CDetBase* parent, const std::string& name, 
                                 size_t id_number, const TVector3& size, const TVector3& pos, const TVector3& rot, 
                                 double phi_deg,  double pitch_mm,  std::pair<long, long> striprange) :
CDetBase(det, parent, name, id_number, size, pos, rot),
m_min_strip(striprange.first), 
m_max_strip(striprange.second),
m_phi(phi_deg * TMath::DegToRad()), 
m_pitch(pitch_mm), 
m_vpitch(m_pitch),
m_min_strip_number(striprange.first),
m_max_strip_number(striprange.second),
m_mid_strip_number(  0.5*(double)(m_max_strip_number + m_min_strip_number) ), 
m_min_time(0),
m_max_time(1),
m_max_range(1),
m_max_charge(0),
m_max_event_charge(0.0),
m_time_max_event_charge(0.0),
m_min_event_time(bpt::pos_infin),
m_max_event_time(bpt::neg_infin),
m_readout_type(readoutUnknown),
m_event_short_data(),
m_event_long_data(),
m_stats_hitmap(),
m_stats_qmax(new TH1F(std::string(get_full_name() + "_qmax").c_str(),
                      std::string(get_full_name() + "_qmax").c_str(),
                      50, 0, 2.0) ),
m_stats_maxqmax(new TH1F(std::string(get_full_name() + "_maxqmax").c_str(),
                         std::string(get_full_name() + "_maxqmax").c_str(),
                         50, 0, 2.0) ),
m_stats_tmax(new TH1F(std::string(get_full_name() + "_tmax").c_str(),
                     std::string(get_full_name() + "_tmax").c_str(),
                     50, 0, 2.0) ),
m_stats_maxtmax(new TH1F(std::string(get_full_name() + "_maxtmax").c_str(),
                         std::string(get_full_name() + "_maxtmax").c_str(),
                         50, 0, 2.0) ),

m_stats_map_tmax(),
m_stats_map_maxtmax(),
m_stats_nstrips(),
m_stats_strip_counter(0),
m_overlay_short_data(),
m_pedestal_data()
{
   if (m_name == "X") {
      m_readout_type = readoutX;
   }
   else if(m_name == "Y") {
      m_readout_type = readoutY;
   }
   else if(m_name == "U") {
      m_readout_type = readoutU;
   }
   else if(m_name == "V") {
      m_readout_type = readoutV;
   }
  
   m_stats_qmax->SetCanExtend(TH1::kXaxis);
   m_stats_maxqmax->SetCanExtend(TH1::kXaxis);
   m_stats_tmax->SetCanExtend(TH1::kXaxis);
   m_stats_maxtmax->SetCanExtend(TH1::kXaxis);
   
   m_stats_qmax->SetDirectory(0);
   m_stats_maxqmax->SetDirectory(0);
   m_stats_tmax->SetDirectory(0);
   m_stats_maxtmax->SetDirectory(0);
}


CDetReadoutBase::~CDetReadoutBase()
{
   
}

void CDetReadoutBase::extract_readouts(std::vector<CDetReadoutBase*>& vec)
{
   void  (CDetBase::*exrd) (std::vector<CDetReadoutBase*> &vec) = &CDetBase::extract_readouts;
   if (std::find(vec.begin(), vec.end(), this) == vec.end()) {
      vec.push_back(this);
   }
   std::for_each(m_parts.begin(), m_parts.end(), boost::bind(exrd, _1 , boost::ref(vec)));
}

void CDetReadoutBase::extract_readouts(std::vector<const CDetReadoutBase*>& vec) const
{
   void  (CDetBase::*exrd) (std::vector<const CDetReadoutBase*> &vec) const = &CDetBase::extract_readouts;
   if (std::find(vec.begin(), vec.end(), this) == vec.end()) {
      vec.push_back(this);
   }
   std::for_each(m_parts.begin(), m_parts.end(), boost::bind(exrd, _1 , boost::ref(vec)));
}

std::pair<size_t,size_t> CDetReadoutBase::get_strip_range() const
{
   size_t min = 0, max = 0;
   min = std::min_element(m_parts.begin(), m_parts.end(), CDetBase::IdNumberLess())->get()->get_number();
   max = std::max_element(m_parts.begin(), m_parts.end(), CDetBase::IdNumberLess())->get()->get_number();
   return std::pair<size_t,size_t>(min, max);
}

std::pair<size_t,size_t> CDetReadoutBase::get_full_strip_range() const
{
   return std::pair<size_t, size_t> (m_min_strip, m_max_strip);
}

std::pair<double,double> CDetReadoutBase::get_full_time_range() const
{
   return std::pair<double, double> (m_min_time, m_max_time);
}

std::pair<double,double> CDetReadoutBase::get_time_range() const
{
   return std::pair<double, double> (0.0, m_max_range);
}

std::pair<double,double> CDetReadoutBase::get_full_charge_range() const
{
   return std::pair<double, double> (0.0, m_max_charge);
}

std::pair<bpt::ptime,bpt::ptime> CDetReadoutBase::get_run_time_range() const
{
   return std::pair<bpt::ptime,bpt::ptime>(m_min_event_time, m_max_event_time);
}

void CDetReadoutBase::print() const
{
   
   CDetBase::print();
   std::cout << "pitch=" << m_pitch << " strips min=" << m_min_strip << " max=" <<m_max_strip << "number of strips ="<< m_parts.size() << std::endl;
}

//void CDetReadoutBase::add_strip_number(size_t number /*max t,q*/)
//{
//   m_min_strip = m_min_strip ? std::min(m_min_strip, number) : number;
//   m_max_strip = std::max(m_max_strip, number);
//   m_mid_strip_number =  0.5*(double)(m_max_strip_number + m_min_strip_number);
//   
//   //TODO min max time
//}


void CDetReadoutBase::add_loaded_event_to_stats()
{
   BOOST_FOREACH(DetBasePtr ptr, m_parts) {
      CDetStrip* strip = dynamic_cast<CDetStrip*>(ptr.get());
      add_strip_to_stats(strip);
   }
   
   reset_event_stats();
}

void CDetReadoutBase::add_strip_to_stats(CDetStrip* strip /*size_t strip_number, const std::vector<short>& qvec*/ )
{
   if (!strip) {
      std::cout << "null strip\n"; 
      return;
   }
    
   size_t strip_number = strip->get_number();
   double strip_charge = strip->get_charge();
   double strip_time = strip->get_time();

   
//   const std::vector<double>& qvec = strip->get_raw_charge();
   m_min_strip = m_min_strip ? std::min(m_min_strip, strip_number) : strip_number;
   m_max_strip = std::max(m_max_strip, strip_number);
   m_mid_strip_number =  0.5*(double)(m_max_strip_number + m_min_strip_number);

//   std::vector<double>::const_iterator iqmax = std::max_element(qvec.begin(), qvec.end());   
   
     
   m_max_charge = std::max(m_max_charge, strip_charge);
//   m_max_charge = std::max(m_max_charge, *iqmax);
   
   
   if (strip_charge > m_max_event_charge) {
      m_max_event_charge = strip_charge;
      m_time_max_event_charge = strip_time;
   }
   m_max_event_charge = std::max(m_max_event_charge, strip_charge);
   
   //time
   
   int tbin = strip->get_timebin(); //number of timebin
   m_max_range = tbin*25.0; //maximum of time window
   
   m_min_time = std::min(m_min_time, strip_time);
   m_max_time = std::max(m_max_time, strip_time);  
   
   
//   size_t tbqmax = std::distance(qvec.begin(), iqmax);

   //stats
   m_stats_hitmap[strip_number] += 1;
//   m_stats_qmax[std::ceil(strip_charge) ] += 1;
//   m_stats_tbqmax[std::ceil(strip_time)] += 1;
   m_stats_strip_counter++;
   
   m_stats_qmax->Fill(strip_charge);
   m_stats_tmax->Fill(strip_time);
   m_stats_map_tmax[(long)(std::ceil(strip_time))] += 1;
   
//   if ((long)(std::ceil(strip_time)) > 1000  || (long)(std::ceil(strip_time)) < -1000) {
//      std::cout << "CDetReadoutBase::add_strip_to_stats bad time ceil from " << strip_time << std::endl;
//   }
//   std::cout << "ceil: " << (int)(std::ceil(strip_time)) << " <- " << strip_time << std::endl;
}


void CDetReadoutBase::add_entry_to_stats(long time_s, int time_us)
{
   std::time_t stdtime ( time_s ); 
   bpt::ptime event_time = bpt::from_time_t( stdtime );
   if (time_us > 500000) {
      event_time += bpt::seconds(1);
   }
   
   if (event_time < m_min_event_time) {
      m_min_event_time = event_time;
   }
   if (event_time > m_max_event_time) {
      m_max_event_time = event_time;
   }
}

void CDetReadoutBase::add_strip_to_pedestal(unsigned strip_number, double pedmean, double pedstdev)
{
//   std::cout << "CDetReadoutBase::add_strip_to_pedestal "  << m_name << " " << strip_number << std::endl;
   m_pedestal_data.push_back(StripShortData(strip_number, pedmean, pedstdev));
}

void CDetReadoutBase::reset_event_stats()
{
   m_stats_maxqmax->Fill(m_max_event_charge);
   m_stats_maxtmax->Fill(m_time_max_event_charge);
   

   m_stats_map_maxtmax[(int)(std::ceil(m_time_max_event_charge))] += 1;
//   m_stats_maxqmax[std::ceil(m_max_event_charge) ] += 1;
//   m_stats_tbmaxqmax[std::ceil(m_time_max_event_charge)] += 1;
   m_stats_nstrips[m_stats_strip_counter] += 1;
   m_max_event_charge = 0.0;
   m_time_max_event_charge = 0.0;
   m_stats_strip_counter = 0;
}

void CDetReadoutBase::load_event_data(DetBasePtrCont& parent_strips,
                                      DetBasePtrCont& overlay_strips)
{
   m_new_data_to_draw = true;
   DetBasePtrCont ostrips;
   ostrips.splice(ostrips.end(), overlay_strips);
   
   m_parts.clear();
   m_overlay_short_data.clear();
   m_event_short_data.clear();
   m_event_long_data.clear();
   
//   CDetStrip::HasEqualReadoutName equalname(m_name);
//   DetBasePtrCont::iterator firstbad = std::partition(parent_strips.begin(), 
//                                                      parent_strips.end(), 
//                                                      equalname);

   DetBasePtrCont::iterator firstbad = std::partition(parent_strips.begin(),
                                                      parent_strips.end(),
                                                      IsMyStrip(this));
   m_parts.splice(m_parts.begin(), parent_strips, parent_strips.begin(), firstbad);
   
//   std::stringstream ss;
//   ss << m_idnumber;
//   CDetStrip::HasEqualReadoutName equalid(ss.str());
//   firstbad = std::partition(parent_strips.begin(), 
//                             parent_strips.end(), 
//                             equalid);
//   m_parts.splice(m_parts.begin(), parent_strips, parent_strips.begin(), firstbad);

   m_parts.sort(CDetBase::IdNumberLess());
   
   BOOST_FOREACH(boost::shared_ptr<CDetBase> baseptr, m_parts) {
      CDetStrip* strip = dynamic_cast<CDetStrip*>(baseptr.get());
      strip->set_detector(m_detector);
      strip->set_parent(this);
      m_event_short_data.push_back(StripShortData(strip->get_number(), 
                                                  strip->get_charge(), 
                                                  strip->get_time() ));
      
      m_event_long_data.push_back( StripLongData(strip->get_number(), 
                                                 strip->get_raw_charge(),
                                                 strip->get_timebin_width()) );
   }
  
   
   //overlay strips
   BOOST_FOREACH(boost::shared_ptr<CDetBase> basestrip, ostrips) {
      CDetStrip* strip = dynamic_cast<CDetStrip*>(basestrip.get());
      
         strip->set_detector(m_detector);
         strip->set_parent(this);
         m_overlay_short_data.push_back(StripShortData(strip->get_number(),
                                                       strip->get_charge(),
                                                       strip->get_time() ));
      

   }
}

void CDetReadoutBase::clear_event_data()
{
   m_parts.clear();
   m_event_short_data.clear();
   m_event_long_data.clear();
}


void CDetReadoutBase::load_pedestal_data(DetBasePtrCont& pedestal_strips)
{
   DetBasePtrCont pstrips(m_parts);
   pstrips.clear();

   DetBasePtrCont::iterator firstbad = std::partition(pedestal_strips.begin(),
                                                      pedestal_strips.end(),
                                                      IsMyStrip(this));
   pstrips.splice(pstrips.begin(), pedestal_strips, pedestal_strips.begin(), firstbad);

   pstrips.sort(CDetBase::IdNumberLess());
  // std::cout << "CDetReadoutBase::load_pedestal_data into " << m_name << " (from sz=" << pstrips.size() << ")\n";
   m_pedestal_data.clear();

   BOOST_FOREACH(boost::shared_ptr<CDetBase> basestrip, pstrips) {
      CDetStrip* strip = dynamic_cast<CDetStrip*>(basestrip.get());
      strip->set_detector(m_detector);
      strip->set_parent(this);
      m_pedestal_data.push_back(StripShortData(strip->get_number(),
                                               strip->get_charge(), //ped mean
                                               strip->get_time() ));//ped stdev
   }
}



////////////////////////////////////////////////////////////////////////////////
// GUI related
////////////////////////////////////////////////////////////////////////////////


void CDetReadoutBase::gui_populate_tree(TGListTree* tree, TGListTreeItem* parent_item) const
{
   
   TGListTreeItem * current_item = tree->FindChildByName(parent_item, m_name.c_str());
   if (current_item) {
      tree->DeleteChildren(current_item);
   }
 
   
   CDetBase::gui_populate_tree(tree, parent_item);

//   TGListTreeItem *current_item = tree->AddItem(parent_item, m_name.c_str());
//   current_item->SetUserData( static_cast<void*>(  (CDetBase*)this )  );
//   current_item->SetOpen(false);
//   current_item->SetCheckBox(true);
//   std::for_each(m_parts.begin(), m_parts.end(), boost::bind(&CDetBase::gui_populate_tree, _1 , tree, current_item));
   
   current_item = tree->FindChildByName(parent_item, m_name.c_str());

   current_item->SetOpen(false);
//   std::stringstream ss;
//   ss << current_item->GetText() << " (" << m_parts.size() << ")";
//   current_item->SetText(ss.str().c_str());
}




int CDetReadoutBase::do_update_my_gui_tab(std::vector<CGuiTab*>& tabs ) const
{
   int ii = 0;
   for (std::vector<CGuiTab*>::iterator it = tabs.begin(); it != tabs.end(); ++it, ++ii) {
      if (dynamic_cast<CGuiTabReadout*>(*it)) {
         (*it)->update(this);
         return ii;
      }
   }
   return -1;
}


const CDetReadoutBase::StripShortData CDetReadoutBase::get_short_data(size_t strip_number) const
{
   //      typedef std::vector<StripShortData> EventShortData;

   EventShortData::const_iterator found = std::find_if(m_event_short_data.begin(), 
                                                       m_event_short_data.end(), 
                                                       StripShortData::FindNumber(strip_number)
                                                       );
   if (found == m_event_short_data.end()) {
      return StripShortData();
   }
   return *found;   
}


const CDetReadoutBase::EventShortData& CDetReadoutBase::get_short_data() const
{
   return m_event_short_data;   
}

const CDetReadoutBase::EventLongData& CDetReadoutBase::get_long_data() const
{
   return m_event_long_data;   
}


const std::map<size_t, int>& CDetReadoutBase::get_stats_hitmap() const
{
   return m_stats_hitmap;
}

const TH1F* CDetReadoutBase::get_stats_qmax_hist() const
{
   return m_stats_qmax.get();
}

const TH1F* CDetReadoutBase::get_stats_maxqmax_hist() const
{
   return m_stats_maxqmax.get();
}

//const std::map<size_t, int>& CDetReadoutBase::get_stats_tbqmax() const
//{
//   return m_stats_tbqmax;
//}



const TH1F* CDetReadoutBase::get_stats_tmax_hist() const
{
   return m_stats_tmax.get();
}

const TH1F* CDetReadoutBase::get_stats_maxtmax_hist() const
{
   return m_stats_maxtmax.get();
}


//const std::map<size_t, int>& CDetReadoutBase::get_stats_tbmaxqmax() const
//{
//   return m_stats_tbmaxqmax;
//}

const std::map<size_t, int>& CDetReadoutBase::get_stats_nstrips() const
{
   return m_stats_nstrips;
}


/** checks if strip belongs to this readout
   there are 2 ways of decoding readout in root files 
   as id numbers ( alphabetically sorted names assigned 0,1,2...)
   or as char names of the readouts (X,Y ...) 
   here we have to check for both
 */
bool CDetReadoutBase::is_my_strip(const CDetStrip* strip) const
{
   if (!strip) {
      std::cout << "CDetReadoutBase::is_my_strip(NULL)\n";
      return false;
   }
   int stripreadoutid = -1;
   try {
      stripreadoutid = boost::lexical_cast<int>(strip->get_strip_id().readout_id());
   } catch (boost::bad_lexical_cast& bc) {
//      std::cout << " CDetReadoutBase::is_my_strip bad cast of '"
//      << strip->get_strip_id().readout_id() << "'  : " << bc.what() << "\n";
      stripreadoutid = strip->get_strip_id().readout_id();
   }
   
   bool retval1 = (strip->get_strip_id().readout_id() == m_name);
   bool retval2 = (stripreadoutid == int(m_idnumber));
   
//   std::cout << "CDetReadoutBase::is_my_strip " << retval1 << retval2
//   << " : " << strip->get_full_name()
//   << " "  << strip->get_strip_id().readout_id() << "(" << stripreadoutid << ")"
//   << " -->  my id: " << (int)m_idnumber
//   << " ,  my name: " << get_full_name()
//   << std::endl;
   return retval1 || retval2;
}

const CDetReadoutBase::EventShortData& CDetReadoutBase::get_overlay_short_data() const
{
   return m_overlay_short_data;
}

std::map<long, int>   CDetReadoutBase::get_stats_map_tmax() const
{
   return m_stats_map_tmax;
}

std::map<long, int>   CDetReadoutBase::get_stats_map_maxtmax() const
{
   return m_stats_map_maxtmax;
}


const CDetReadoutBase::EventShortData& CDetReadoutBase::get_pedestal_data() const
{
   return m_pedestal_data;
}


////////////////////////////////////////////////////////////////////////////////
// derived classes
////////////////////////////////////////////////////////////////////////////////


CDetReadout::CDetReadout(CDetDetector* det, CDetBase* parent, const std::string& name, 
                         size_t id_number, const TVector3& size, const TVector3& pos, const TVector3& rot,
                         double phi_deg,  double pitch_mm,  std::pair<long, long> striprange)
: CDetReadoutBase(det, parent, name,  id_number, size, pos, rot, phi_deg, pitch_mm, striprange)
{ 
   if (std::fabs(m_phi - TMath::Pi()) < EPS) {
      std::cerr << "CReadout should be CReadoutV " << std::endl;
      m_vpitch = m_pitch; // ? minus ?
      
   }
   else if (std::fabs(m_phi) < EPS) {
      std::cerr << "CReadout should be CReadoutH " << std::endl;
      m_vpitch = -  m_pitch; // ? minus ?
   }
   m_vpitch = -  m_pitch/std::cos(m_phi); // ? minus ?
}




//strip counting up along axis Y, --> same as Readout at angle 180deg
CDetReadoutH::CDetReadoutH(CDetDetector* det, CDetBase* parent, const std::string& name, 
                           size_t id_number, const TVector3& size, const TVector3& pos, const TVector3& rot,
                           double pitch_mm,  std::pair<long, long> striprange)
: CDetReadoutBase(det, parent, name,  id_number, size, pos, rot, 0.0, pitch_mm, striprange)
{
   m_vpitch = m_pitch; // no minus -> in internal plane/readout coordinatete system 
   // (mirror the displayed histogram for right orientation !)
   //   m_parent_plane_origin_line.Set( 0.5*(m_size.Y()) , m_phi); //offset, angle
}




//strip counting up along axis X, --> same as Readout at angle 90deg
CDetReadoutV::CDetReadoutV(CDetDetector* det, CDetBase* parent, const std::string& name, 
                           size_t id_number, const TVector3& size, const TVector3& pos, const TVector3& rot,
                           double pitch_mm,  std::pair<long, long> striprange)
: CDetReadoutBase(det, parent, name,  id_number, size, pos, rot, 90.0, pitch_mm, striprange)
{
   m_vpitch = m_pitch; //TODO: shodn't be * -1 <-- counting left right->against Y axis
   //   m_parent_plane_origin_line.Set( 0.5*(m_size.X()) , m_phi); //offset, angle
}


void CDetReadout::print() const
{
   
   CDetReadoutBase::print();
   std::cout << "readout m_phi="<< m_phi << " strips min = " << m_min_strip << " max=" <<m_max_strip << std::endl;
}

void CDetReadoutH::print() const
{
   
   CDetReadoutBase::print();
   std::cout << "H readout strips min = " << m_min_strip << " max=" <<m_max_strip << std::endl;
}

void CDetReadoutV::print() const
{
   
   CDetReadoutBase::print();
   std::cout << "V readout strips min = " << m_min_strip << " max=" <<m_max_strip << std::endl;
}


