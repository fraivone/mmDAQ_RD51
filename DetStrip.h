//
//  DetStrip.h
//  browser
//
//  Created by Marcin Byszewski on 7.4.12.
//  Copyright 2012 CERN - PH/ADE. All rights reserved.
//

#ifndef browser_DetStrip_h
#define browser_DetStrip_h

#include "DetBase.h"
#include "SrsChipId.h"
#include "SrsChannelId.h"
#include "DetStripId.h"

#include <boost/shared_ptr.hpp>

namespace browser {
   
   class CSrsChannelId;
   class CDetStripId;
   class CStripsTreeEntry;
   
   class CDetStrip : public CDetBase
   {

      
   public:
      CDetStrip(CDetDetector* det, CDetBase* parent, const std::string& name, size_t id_number, const TVector3& size, const TVector3& pos);
      
      CDetStrip(CDetDetector* det,
                const CSrsChannelId& channel_id,
                const CDetStripId& strip_id,
                const std::vector<double>& qvec,
                double timebin_width_ns);
      
      CDetStrip(CDetDetector* det,
                const CSrsChannelId& channel_id,
                const CDetStripId& strip_id,
                double charge,
                double time);
      
      
      virtual ~CDetStrip();
      virtual void print() const{};
      virtual void gui_populate_tree(TGListTree* tree, TGListTreeItem* parent_item) const;

      virtual bool is_my_strip(const CDetStrip* ) const { return true; }

//      virtual bool strip_has_my_name(DetBasePtr ) const { return true; };
//      virtual bool strip_has_my_id(DetBasePtr ) const { return true; };
      
      void set_strip_tree_entry(boost::shared_ptr<CStripsTreeEntry> entry);

      const CSrsChannelId& get_channel_id() const { return m_srs_channel_id;}
      const CDetStripId& get_strip_id() const { return m_strip_id;}
     
//      size_t get_chamber_id() const {return m_chamber_id;}
//      size_t get_multilayer_id() const {return m_multilayer_id;}
//      size_t get_plane_id() const {return m_plane_id;}
//      size_t get_readout_id() const {return m_readout_id;}
//      
//      const std::string& get_chamber_name() const {return m_chamber_name;}
//      const std::string& get_multilayer_name() const {return m_multilayer_name;}
//      const std::string& get_plane_name() const {return m_plane_name;}
//      const std::string& get_readout_name() const {return m_readout_name;}
      
      
      const std::vector<double>& get_raw_charge() const { return m_raw_charge;}
      double get_charge() const;
      double get_time() const;
      const CStripsTreeEntry* get_strip_tree_entry() const;
      double get_timebin_width() const { return m_timebin_width;}
      int get_timebin() const; 
       
//      class HasEqualName;
//      class HasEqualChamberName;
//      class HasEqualMultiLayerName;
//      class HasEqualPlaneName;
//      class HasEqualReadoutName;
      class CompSrsChipId;
      class LessChargePtr;
      
   private:
      virtual int do_update_my_gui_tab(std::vector<CGuiTab*>& tabs) const;
      
      CSrsChannelId m_srs_channel_id;
      CDetStripId   m_strip_id;
      
//      std::string m_chamber_name;
//      std::string m_multilayer_name;
//      std::string m_plane_name;
//      std::string m_readout_name;
//     
//      size_t m_chamber_id;
//      size_t m_multilayer_id;
//      size_t m_plane_id;
//      size_t m_readout_id;
      
      //      size_t m_strip_number;
      std::vector<double> m_raw_charge;
      
      double m_charge;
      double m_time;
      boost::shared_ptr<CStripsTreeEntry> m_strip_tree_entry; //fir results, times (tbqmax, tfit, tcfd, ...), charge
      double m_timebin_width;
      int m_timebin; 
   };
   
   
   class CDetStrip::CompSrsChipId : public std::unary_function<CDetStrip, bool>{
      CSrsChipId m_chipid;
   public:
      CompSrsChipId(CSrsChipId cid) : m_chipid(cid) {}
      bool operator() (const CDetStrip* rhs) const {
         return rhs->get_channel_id().chip_id() == m_chipid;
      }
   };
   
   class CDetStrip::LessChargePtr : public std::binary_function<DetStripPtr, DetStripPtr, bool> {
   public:
      bool operator() (const DetStripPtr lhs, const DetStripPtr rhs) const {
         return (lhs)->get_charge() < (rhs)->get_charge();
      }
   };
    
} // namespace


#endif
