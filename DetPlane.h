//
//  DetPlane.h
//  browser
//
//  Created by Marcin Byszewski on 12/18/11.
//  Copyright 2011 CERN - PH/ADE. All rights reserved.
//

#ifndef browser_DetPlane_h
#define browser_DetPlane_h

#include "DetBase.h"


namespace browser {
   
   class CDetPlane : public CDetBase
   {
   protected:
      

      
   public:
      CDetPlane(CDetDetector* det, CDetBase* parent, const std::string& name, size_t id_number, const TVector3& size, const TVector3& pos, const TVector3& rot);
      virtual ~CDetPlane();
      virtual void extract_planes(std::vector<CDetPlane*>& vec);

      
      virtual bool is_my_strip(const CDetStrip* strip) const;

//      virtual bool strip_has_my_name(DetBasePtr strip) const;
//      virtual bool strip_has_my_id(DetBasePtr strip) const;

   private:
      virtual int do_update_my_gui_tab(std::vector<CGuiTab*>& tabs) const;
   
   };
   
} // namespace

#endif
