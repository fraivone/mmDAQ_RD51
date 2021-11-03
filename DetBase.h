//
//  DetBase.h
//  browser
//
//  Created by Marcin Byszewski on 12/16/11.
//  Copyright 2011 CERN - PH/ADE. All rights reserved.
//

#ifndef browser_DetBase_h
#define browser_DetBase_h
#include "DetTypes.h"

#include <TVector3.h>

#include <boost/shared_ptr.hpp>

#include <vector>
#include <string>
#include <list>

class TGListTree;
class TGListTreeItem;


namespace browser {
   
   class CDetBase;
   class CDetDetector;
   class CDetChamber;
   class CDetMultiLayer;
   class CDetPlane;
   class CDetReadoutBase;
   class CGuiTab;
   class CDetStrip;
   
   
   class CDetBase
   {
   public:
      //typedef std::list<boost::shared_ptr<CDetBase> > DetBasePtrCont;

      CDetBase(CDetDetector* det, CDetBase* parent, const std::string& name, size_t id_number, const TVector3& size, const TVector3& pos, const TVector3& rot);
      virtual ~CDetBase() = 0;
      void add_part(CDetBase* part);
      void add_part(boost::shared_ptr<CDetBase> part);
      bool add_part_if_unique(CDetBase* part);
      CDetBase* get_part(const std::string& name);
      CDetBase* get_part_by_id(size_t idnum); 
      DetBasePtrCont& get_parts() {return m_parts; }
      const DetBasePtrCont& get_parts() const {return m_parts; }
   
      const std::string& get_name() const { return m_name;}
      std::string get_full_name() const;
      const size_t& get_number() const { return m_idnumber;}
      void set_detector(CDetDetector* det) { m_detector = det;}
      CDetDetector* get_detector() { return m_detector;}

      void set_parent(CDetBase* parent) { m_parent = parent;}
      
      
      
      virtual void extract_chambers(std::vector<CDetChamber*>& vec);
      virtual void extract_multilayers(std::vector<CDetMultiLayer*>& vec);
      virtual void extract_planes(std::vector<CDetPlane*>& vec);
      virtual void extract_readouts(std::vector<CDetReadoutBase*>& vec);
      virtual void extract_readouts(std::vector<const CDetReadoutBase*>& vec) const;
      virtual void print() const ;
      virtual void gui_populate_tree(TGListTree* tree, TGListTreeItem* parent_item) const;
      virtual void load_event_data(DetBasePtrCont& parent_strips, DetBasePtrCont& overlay_strips);
      virtual void add_loaded_event_to_stats();
      
      virtual void load_pedestal_data(DetBasePtrCont& pedestal_strips);
      int update_my_gui_tab(std::vector<CGuiTab*>& tabs)  {
         if(m_new_data_to_draw) {
            return do_update_my_gui_tab(tabs);
            m_new_data_to_draw = false;
         }
         return -1;
      }
      virtual void clear_event_data();
      virtual bool is_my_strip(const CDetStrip* strip) const = 0;
      
      void set_name(const std::string& name) { m_name = name;};
      void set_size(const TVector3& vec) { m_size = vec;};
      void set_position(const TVector3& vec)  { m_position = vec;};
      void set_rotation(const TVector3& vec) { m_rotation = vec;};
      void set(size_t idnumber, const std::string& name, const TVector3& size, const TVector3& pos, const TVector3& rot);

   protected:
      

      
      CDetDetector*  m_detector;
      CDetBase*      m_parent;
      DetBasePtrCont m_parts;
      std::string    m_name;
      size_t         m_idnumber;
      TVector3       m_size;
      TVector3       m_position;
      TVector3       m_rotation;
      bool           m_new_data_to_draw;
      virtual void clear();
      class PartPtrCompare;
      class PartNameCompare;
      class PartIdCompare;
      class IdNumberLess;
      class HasEqualName;
      
      class IsMyStrip : public std::unary_function<boost::shared_ptr<CDetBase>, bool>{
         const CDetBase* m_detelement;
      public:
         IsMyStrip(const CDetBase* ptr): m_detelement(ptr) {}
         bool operator() (const boost::shared_ptr<CDetBase> rhs) const;
      };
      
      
   private:
      virtual int do_update_my_gui_tab(std::vector<CGuiTab*>& tabs) const = 0;
      
      //      class StripHasMyName;
//      class StripHasMyId;
   };
   
   
      
   class CDetBase::PartPtrCompare : public std::unary_function<boost::shared_ptr<CDetBase>, bool>{
      const CDetBase* m_ptr;
   public:
      PartPtrCompare(const CDetBase* ptr): m_ptr(ptr) {}
      bool operator() (const boost::shared_ptr<CDetBase> rhs) const {
         return rhs.get() == m_ptr;
      }
   };
   
   class CDetBase::PartNameCompare : public std::unary_function<boost::shared_ptr<CDetBase>, bool>{
      std::string m_name;
   public:
      PartNameCompare(const std::string& name): m_name(name) {}
      bool operator() (const boost::shared_ptr<CDetBase> rhs) const {
         return rhs->get_name() == m_name;
      }
   };
   
   class CDetBase::PartIdCompare : public std::unary_function<boost::shared_ptr<CDetBase>, bool>{
      size_t m_id;
   public:
      PartIdCompare(size_t num): m_id(num) {}
      bool operator() (const boost::shared_ptr<CDetBase> rhs) const {
         return rhs->get_number() == m_id;
      }
   };
   
   
   
   class CDetBase::IdNumberLess : public std::binary_function<boost::shared_ptr<CDetBase>, boost::shared_ptr<CDetBase>, bool>{
      std::string m_name;
   public:
      bool operator() (const boost::shared_ptr<CDetBase>& lhs, const boost::shared_ptr<CDetBase>& rhs) const {
         return lhs->m_idnumber < rhs->m_idnumber;
      }
   };
   
   
   class CDetBase::HasEqualName : public std::unary_function<boost::shared_ptr<CDetBase>, bool>{
      std::string m_name;
   public:
      HasEqualName(const std::string& name): m_name(name) {}
      bool operator() (const boost::shared_ptr<CDetBase>& rhs) const {
         return rhs->get_name() == m_name;
      }
   };
   
   
} // namespace

#endif
