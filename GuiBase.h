//
//  GuiBase.h
//  browser
//
//  Created by Marcin Byszewski on 4/5/12.
//  Copyright 2012 CERN - PH/ADE. All rights reserved.
//

#ifndef browser_GuiBase_h
#define browser_GuiBase_h

namespace browser {
   class CConfiguration;
   
   class IDList {	
   private:
      //static 
      int nID;   // creates unique widget's IDs
   public:
      IDList():nID(0) {}
      ~IDList() {}
      int GetUniqueID(void) { return ++nID; }
   };
   
   class CGuiBase
   {
      
   public:
      CGuiBase() : m_id(), m_gui_parent(0), m_config(0) { };
      CGuiBase(CGuiBase* gui_parent, CConfiguration* config);
      virtual ~CGuiBase();
      virtual CGuiBase* get_main_frame() {
    	  return (m_gui_parent?m_gui_parent->get_main_frame():0) ;
      }
      
      CConfiguration* get_config() { return m_config;}
      
   protected:
      IDList    m_id;          ///< Widget IDs generator	
      CGuiBase* m_gui_parent;  ///< main gui frame
      CConfiguration* m_config;
      
   private:   
      
      
   };
   
} // namespace

#endif
