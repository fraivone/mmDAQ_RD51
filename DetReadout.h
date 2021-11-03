//
//  DetReadout.h
//  browser
//
//  Created by Marcin Byszewski on 12/18/11.
//  Copyright 2011 CERN - PH/ADE. All rights reserved.
//

#ifndef browser_DetReadout_h
#define browser_DetReadout_h

#include "DetBase.h"

#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/shared_ptr.hpp>



#include <map>
#include <utility>

class TH1F;
namespace browser {
   class CDetStrip;
   
   class CDetReadoutBase : public CDetBase
   {
      
   public:
      enum ReadoutType {readoutUnknown = -1, readoutX = 0, readoutY = 1, readoutU = 2, readoutV = 3};
      
      struct StripShortData {
         StripShortData() : strip_no(0), charge(0.0), time(0.0) {};
         StripShortData(size_t no, double q, double t) : strip_no(no), charge(q), time(t) {};
         size_t strip_no;
         double charge;
         double time;
         
         struct FindNumber : public std::unary_function<StripShortData, bool> {
            size_t num_;
            FindNumber(size_t num) : num_(num) {}
            bool operator() (const StripShortData& rhs) const {
               return num_ == rhs.strip_no;
            }
         };
         
      };
      
      struct StripLongData {
         StripLongData(size_t no, const std::vector<double>& q, double timebin_ns)
         : strip_no(no), charge(q), timebin_width(timebin_ns) {};
         size_t strip_no;
         std::vector<double> charge;
         double timebin_width;
      };
      
      

      
      
      typedef std::vector<StripShortData> EventShortData;
      typedef std::vector<StripLongData> EventLongData;
      
      CDetReadoutBase(CDetDetector* det, CDetBase* parent, const std::string& name, 
                      size_t id_number, const TVector3& size, const TVector3& pos, const TVector3& rot, 
                      double phi_deg,  double pitch_mm,  std::pair<long, long> striprange);
      virtual ~CDetReadoutBase() = 0;
      virtual void extract_readouts(std::vector<CDetReadoutBase*>& vec);
      virtual void extract_readouts(std::vector<const CDetReadoutBase*>& vec) const;
      virtual void print() const;
      std::pair<size_t,size_t> get_strip_range() const;
      std::pair<size_t,size_t> get_full_strip_range() const;
      std::pair<double,double> get_full_time_range() const;
      std::pair<double,double> get_full_charge_range() const;
      std::pair<boost::posix_time::ptime,boost::posix_time::ptime> get_run_time_range() const;
      std::pair<double,double> get_time_range() const;
      
//      void add_strip_number(size_t number);
      void add_strip_to_stats(CDetStrip* strip);

//      void add_strip_to_stats(size_t strip_number, const std::vector<short>& qvec);
      void add_entry_to_stats(long time_s, int time_us);
      void add_strip_to_pedestal(unsigned strip_number, double pedmean, double pedstdev);

      void reset_event_stats();
      
      virtual void gui_populate_tree(TGListTree* tree, TGListTreeItem* parent_item) const;
      virtual void load_event_data(DetBasePtrCont& parent_strips, DetBasePtrCont& overlay_strips);
      virtual void add_loaded_event_to_stats();

      virtual void load_pedestal_data(DetBasePtrCont& pedestal_strips);

      virtual void clear_event_data();

      virtual bool is_my_strip(const CDetStrip* strip) const;
//      virtual bool strip_has_my_name(DetBasePtr strip) const;
//      virtual bool strip_has_my_id(DetBasePtr strip) const;


      const StripShortData get_short_data(size_t strip_number) const;
      const EventShortData& get_short_data() const;
      const EventLongData&  get_long_data() const;
      const std::map<size_t, int>& get_stats_hitmap() const;

      const std::map<size_t, int>& get_stats_nstrips() const;
      const TH1F* get_stats_qmax_hist() const;
      const TH1F* get_stats_maxqmax_hist() const;
      const TH1F* get_stats_tmax_hist() const;
      const TH1F* get_stats_maxtmax_hist() const;
      const EventShortData& get_overlay_short_data() const;
      const EventShortData& get_pedestal_data() const;



      std::map<long, int>   get_stats_map_tmax() const;
      std::map<long, int>   get_stats_map_maxtmax() const;
      
   protected:
      size_t m_min_strip;
      size_t m_max_strip;
      
      double m_phi;              ///< inclination angle of the strips from the X axis (to Y) in RHS
      double m_pitch;            ///< strip pitch normal to strip direction
      double m_vpitch;           ///< strip pich projecton to Y axis
      long   m_min_strip_number; ///< min possible strip number f(angle, pitch, size), read form file
      long   m_max_strip_number; ///< max possible strip number f(angle, pitch, size), read form file
      double m_mid_strip_number; ///< strip(double) that goes through centre of plane
      double m_min_time;         ///< min drift time (ns)
      double m_max_time;         ///< max drift time (ns)
      double m_max_range;        ///<max time range according to the timebins setting
      double m_max_charge;
      double m_max_event_charge;
      double m_time_max_event_charge; // time of the max charge signal
      boost::posix_time::ptime m_min_event_time;   ///< first event time
      boost::posix_time::ptime m_max_event_time;   ///< last event time
      
      ReadoutType             m_readout_type;     ///< Readout type corresponds to X,Y, U,V 
      EventShortData          m_event_short_data;
      EventLongData           m_event_long_data;
      std::map<size_t, int>   m_stats_hitmap; ///< strip number and number of hits
//      std::map<int, int>      m_stats_qmax; ///< int qvalue  and number of hits
//      std::map<int, int>      m_stats_maxqmax; ///< int qvalue  and number of hits
      boost::shared_ptr<TH1F> m_stats_qmax;  /// stats for the float time distribution, try with rebinning of hist
      boost::shared_ptr<TH1F> m_stats_maxqmax;  /// stats for the float time distribution, try with rebinning of hist
      boost::shared_ptr<TH1F> m_stats_tmax;  /// stats for the float time distribution, try with rebinning of hist
      boost::shared_ptr<TH1F> m_stats_maxtmax;  /// stats for the float time distribution, try with rebinning of hist
      
      std::map<long, int>   m_stats_map_tmax;    ///< tmax histogram, dt = 1ns
      std::map<long, int>   m_stats_map_maxtmax; ///< maxtmax histgoram, dt = 1ns
      
      std::map<size_t, int>   m_stats_nstrips; ///< tbqmax histgram
      size_t                  m_stats_strip_counter;
      EventShortData          m_overlay_short_data;
      EventShortData          m_pedestal_data;
      
   private:
      virtual int do_update_my_gui_tab(std::vector<CGuiTab*>& tabs) const;
   };
   
   
      
   /**
    Readout with an arbitrary angle. Must not be vertical or horizontal.
    */
   
   class CDetReadout : public CDetReadoutBase {
   public:
      CDetReadout(CDetDetector* det, CDetBase* parent, const std::string& name, 
                  size_t id_number, const TVector3& size, const TVector3& pos, const TVector3& rot,
                  double phi_deg,  double pitch_mm,  std::pair<long, long> striprange);
      
      virtual ~CDetReadout() {};
      
      virtual CDetReadout* clone() const { return new CDetReadout(*this); }
      virtual void print() const;
      
   };
   
   
   /**
    Readout with horizontal strips (along x axis)
    */ 
   
   class CDetReadoutH : public CDetReadoutBase {
   public:
      CDetReadoutH(CDetDetector* det, CDetBase* parent, const std::string& name, 
                   size_t id_number, const TVector3& size, const TVector3& pos, const TVector3& rot,
                   double pitch_mm,  std::pair<long, long> striprange);
      
      virtual ~CDetReadoutH() {};      
      virtual CDetReadoutH* clone() const { return new CDetReadoutH(*this); }
      virtual void print() const;
      
   };
   
   
   /**
    Readout with vertical strips (along y axis)
    */ 
   
   class CDetReadoutV : public CDetReadoutBase {
   public:
      CDetReadoutV(CDetDetector* det, CDetBase* parent, const std::string& name, 
                   size_t id_number, const TVector3& size, const TVector3& pos, const TVector3& rot,
                   double pitch_mm,  std::pair<long, long> striprange);
      
      virtual ~CDetReadoutV() {};      
      virtual CDetReadoutV* clone() const { return new CDetReadoutV(*this); }
      virtual void print() const;
      
   };
   
   typedef boost::shared_ptr<CDetReadoutBase> DetRdPtr;
   
} // namespace

#endif
