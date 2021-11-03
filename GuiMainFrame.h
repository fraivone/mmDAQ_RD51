//
//  GuiMainFrame.h
//  browser
//
//  Created by Marcin Byszewski on 4/5/12.
//  Copyright 2012 CERN - PH/ADE. All rights reserved.
//

#ifndef browser_GuiMainFrame_h
#define browser_GuiMainFrame_h

#include "GuiBase.h"
#include <TGFrame.h>
#include <vector>

class TGListTreeItem;



//class TGTextButton;
//class TGStatusBar;
//class TGCanvas;
//class TGViewPort;
//class TGListTree;
//class TGListTreeItem;
//class TGPicture;
//class TGTab;
//class TGNumberEntry;
//class TRootEmbeddedCanvas;
//class TCanvas;
//class TGHProgressBar;


namespace browser {
   class CDetBase;
   
//   class CEventBrowser;
//   class CGuiTab;
//   class CDetDetector;
//   class CDetBase;
   
   class CGuiMainFrame : public TGMainFrame, public CGuiBase
   {

   public:
      CGuiMainFrame(CGuiBase* gui_parent, CConfiguration* config) : CGuiBase(gui_parent, config) {} ;
      virtual ~CGuiMainFrame() {};
      
      void CloseWindow();

      CGuiMainFrame* get_main_frame() {
         return do_get_gui_main_frame();
      }
      
      TGMainFrame*  main_frame() {
         return do_get_tg_main_frame();
      }
      
      CDetBase* get_tree_selected_user_data() {
         return do_get_tree_selected_user_data();
      }
      
      int get_tree_checked_items(std::vector<TGListTreeItem*>& vec) {
         return do_get_tree_checked_items(vec);
      }
      
      int get_item_checked_descendants(TGListTreeItem* first_item, std::vector<TGListTreeItem*>& vec) {
         return do_get_item_checked_descendants(first_item, vec);
      }
      
      //gui callbacks /slots
      void gui_handle_load_file() {
         do_gui_handle_load_file();
      }
      
      void gui_handle_current_entry_set(Long_t val) {
         do_gui_handle_current_entry_set(val);
      }
      
      void gui_handle_tree_click(TGListTreeItem* entry, int) {
         do_gui_handle_tree_click(entry, 0);
      }
      
      void gui_handle_tree_check(TObject* obj, Bool_t check) {
         do_gui_handle_tree_check(obj, check);
      }
      
      void update_progress(double val) {
         do_update_progress(val);
      }

      void update() {
         do_update();
      }
      
   private:
   
      virtual CGuiMainFrame* do_get_gui_main_frame() = 0;
      virtual TGMainFrame*  do_get_tg_main_frame() = 0;
      
      virtual CDetBase* do_get_tree_selected_user_data() = 0;
      virtual int do_get_tree_checked_items(std::vector<TGListTreeItem*>& vec) = 0;
      virtual int do_get_item_checked_descendants(TGListTreeItem* first_item, std::vector<TGListTreeItem*>& vec) = 0;
      //gui callbacks /slots
      virtual void do_gui_handle_load_file() = 0;
      virtual void do_gui_handle_current_entry_set(Long_t val) = 0;
      virtual void do_gui_handle_tree_click(TGListTreeItem* entry, int) = 0;
      virtual void do_gui_handle_tree_check(TObject* obj, Bool_t check) = 0;
      virtual void do_update_progress(double val) = 0;
      virtual void do_update() = 0;
      
      
   };
}


#endif
