//
//  DetDetector.h
//  browser
//
//  Created by Marcin Byszewski on 12/16/11.
//  Copyright 2011 CERN - PH/ADE. All rights reserved.
//

#ifndef browser_DetDetector_h
#define browser_DetDetector_h

#include "DetBase.h"
#include "SrsChipId.h"

#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/thread/mutex.hpp>

#include <string>
#include <map>

class TVector3;
class TEnv;

namespace browser {
   
   class CReaderBase;
   class CConfiguration;
   
   class CDetDetector : public CDetBase
   {
   public:
      typedef boost::numeric::ublas::mapped_matrix<double> xtalk_matrix_type;
      typedef std::map<uint32_t, std::vector<int16_t> > RawUdpMap;

      
      CDetDetector(const CReaderBase* reader, const CConfiguration* config,  const std::string& name, const TVector3& size, const TVector3& pos, const TVector3& rot);
      virtual ~CDetDetector();
      
      TEnv* make_tenv();
      virtual void load_event_data(DetBasePtrCont& source_strips, DetBasePtrCont& overlay_strips);
      virtual void load_pedestal_data(DetBasePtrCont& pedestal_strips);

      virtual bool is_my_strip(const CDetStrip* ) const { return true; }

//      virtual bool strip_has_my_name(DetBasePtr ) const { return true; }
//      virtual bool strip_has_my_id(DetBasePtr ) const { return true; }

      
      void add_srs_chip(const CSrsChipId& cid);
      void add_xtalk_map(const std::map<CSrsChipId, std::vector<DetStripPtr> >& xtalk_map);
      
      const std::map<int, xtalk_matrix_type>& get_xtalk_map_all() const;

      void lock();
      void unlock();
      
      void set_event_raw_udp_data(RawUdpMap const& event_raw_data);
      RawUdpMap const& get_event_raw_udp_data() const;
      
   private:
      virtual int do_update_my_gui_tab(std::vector<CGuiTab*>& tabs) const;
      void build();

      
      const CReaderBase*    m_reader;
      const CConfiguration* m_config;

      std::vector<CSrsChipId> m_srs_chips;
      
      std::map<int, xtalk_matrix_type> m_xtalkmap_all;
      std::map<int, xtalk_matrix_type> m_xtalkmap_32;
      
      xtalk_matrix_type m_xtalk_all;
      xtalk_matrix_type m_xtalk_32;
      
      RawUdpMap m_event_raw_udp;

      
      boost::mutex m_mutex;
      
      
   };
   
} // namespace

#endif
