//
//  DetMultiLayer.h
//  mmdaq3-server
//
//  Created by Marcin Byszewski on 06/07/2012.
//  Copyright 2012 CERN - PH/ADE. All rights reserved.
//

#ifndef mmdaq3_server_DetMultiLayer_h
#define mmdaq3_server_DetMultiLayer_h

#include "DetBase.h"

namespace browser {
   
   class CDetMultiLayer : public CDetBase
   {
      
   public:
      CDetMultiLayer(CDetDetector* det, CDetBase* parent, const std::string& name,
                     size_t id_number, const TVector3& size, const TVector3& pos, const TVector3& rot);
      
      
      virtual ~CDetMultiLayer();
      virtual void extract_multilayers(std::vector<CDetMultiLayer*>& vec);
      
      virtual bool is_my_strip(const CDetStrip* strip) const;

//      virtual bool strip_has_my_name(DetBasePtr strip) const;
//      virtual bool strip_has_my_id(DetBasePtr strip) const;
   private:
      virtual int do_update_my_gui_tab(std::vector<CGuiTab*>& tabs) const;

      
   };
   
} // namespace

#endif
