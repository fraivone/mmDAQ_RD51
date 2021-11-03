//
//  GuiMainFrameOffline.h
//  eventbrowser
//
//  Created by Marcin Byszewski on 1/17/13.
//  Copyright (c) 2013 CERN - PH/ADE. All rights reserved.
//

#ifndef __eventbrowser__GuiMainFrameOffline__
#define __eventbrowser__GuiMainFrameOffline__

#include "GuiMainFrame.h"
#include <iostream>

class TGStatusBar;
class TGHProgressBar;
class TGCanvas;
class TGViewPort;
class TGListTree;
class TGNumberEntry;
class TGTextView;
class TRootEmbeddedCanvas;
class TCanvas;
class TGTab;
class TGNumberEntryField;



namespace browser {

   class CEventBrowser;
   class CDetDetector;
   class CGuiTab;

   
   class CGuiMainFrameOffline : public CGuiMainFrame
   {
      
   public:
      CGuiMainFrameOffline();
      CGuiMainFrameOffline(CEventBrowser* eb, CConfiguration* config);
      virtual ~CGuiMainFrameOffline();
      
   private:
      
      virtual CGuiMainFrame* do_get_gui_main_frame();
      virtual TGMainFrame*  do_get_tg_main_frame();
      
      virtual CDetBase* do_get_tree_selected_user_data();
      virtual int do_get_tree_checked_items(std::vector<TGListTreeItem*>& vec);
      virtual int do_get_item_checked_descendants(TGListTreeItem* first_item, std::vector<TGListTreeItem*>& vec);
      
      //gui callbacks /slots
      virtual void do_gui_handle_load_file();
      virtual void do_gui_handle_current_entry_set(Long_t val);
      virtual void do_gui_handle_tree_click(TGListTreeItem* entry, int);
      virtual void do_gui_handle_tree_check(TObject* obj, Bool_t check);
      virtual void do_update_progress(double val);
      virtual void do_update();
      
      //other
      void construct_window();

      void update_tree( CDetDetector* det);
      void update_tabs(TGListTreeItem* entry);
      void clear_tabs();
      void clean_tree();
      
      
      CEventBrowser*     m_eb;
      //GUI-related
      TGMainFrame*       m_mainframe;
      TGStatusBar*       m_gui_statusbar;
      TGHProgressBar*    m_gui_progressbar;
      TGHorizontalFrame* m_gui_top_frame;
      TGHorizontalFrame* m_gui_bottom_frame;
      //GUI - top frame elements
      TGTextButton*      m_gui_load_file;
      TGNumberEntry*     m_gui_current_entry;
TGNumberEntryField* m_gui_current_event;
      //GUI - tree
      TGCanvas*        m_gui_tree_canvas;
      TGViewPort*      m_gui_tree_viewport;
      TGListTree*      m_gui_tree_list;
      TGListTreeItem*  m_gui_tree_item;
      const TGPicture* m_gui_pic_open;       //used for list tree items
      const TGPicture* m_gui_pic_close;      //used for list tree items
      
      //GUI -dropdown
      TRootEmbeddedCanvas* m_gui_dnd_rootcanvas;
      TCanvas*        m_gui_dnd_canvas;
      
      
      //GUI - tabs
      TGTab*                 m_gui_main_tabs;
      std::vector<CGuiTab*>  m_tabs;
      
      
      
      ClassDef(browser::CGuiMainFrameOffline, 0)
      
   };
   
   
}

#endif /* defined(__eventbrowser__GuiMainFrameOffline__) */
