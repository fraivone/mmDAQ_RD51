//
//  DetChamber.h
//  browser
//
//  Created by Marcin Byszewski on 12/18/11.
//  Copyright 2011 CERN - PH/ADE. All rights reserved.
//

#ifndef browser_DetChamber_h
#define browser_DetChamber_h

#include "DetBase.h"

#include <string>

class TVector3;

namespace browser {

   class CDetDetector;
   class CDetChamber : public CDetBase
   {
      
   public:
      CDetChamber(CDetDetector* det, CDetBase* parent, const std::string& name, size_t id_number, const TVector3& size, const TVector3& pos, const TVector3& rot);
      virtual ~CDetChamber();
      virtual void extract_chambers(std::vector<CDetChamber *>& vec);
      
      virtual bool is_my_strip(const CDetStrip* strip) const;
      void set_config_file(const std::string& config_file);
      const std::string& get_config_file() const { return m_config_file;}
      
   private:
      virtual int do_update_my_gui_tab(std::vector<CGuiTab*>& tabs) const;

      
      std::string m_config_file; // config file extarcted from det_config - to load the right xml from root
   };
   
} // namespace

#endif
