//
//  GuiMainFrameOnline.h
//  eventbrowser
//
//  Created by Marcin Byszewski on 1/17/13.
//  Copyright (c) 2013 CERN - PH/ADE. All rights reserved.
//

#ifndef __eventbrowser__GuiMainFrameOnline__
#define __eventbrowser__GuiMainFrameOnline__

#include "GuiMainFrame.h"
#include <iostream>

class TGStatusBar;
class TGHProgressBar;
class TGCanvas;
class TGViewPort;
class TGListTree;
class TGNumberEntry;
class TRootEmbeddedCanvas;
class TCanvas;
class TGTab;
class TGListBox;
class TTimer;
class TGCheckButton;
class TGPictureButton;
class TGLabel;
class TGNumberEntryField;
class TGTextEntry;
class TGComboBox;



namespace browser {
   
   class CEventBrowser;
   class CEventBrowserOnline;
   class CDetDetector;
   class CGuiTab;
   
   
   class CGuiMainFrameOnline : public CGuiMainFrame
   {
      
   public:
      CGuiMainFrameOnline();
      CGuiMainFrameOnline(CEventBrowser* eb, CConfiguration* config);
      virtual ~CGuiMainFrameOnline();
      
      void gui_handle_run_type_select(Int_t sel);
      void gui_handle_run_start();
       void gui_handle_run_start_effective();
       void gui_handle_run_stop();
      void gui_handle_run_configure();
      void gui_handle_daq_load_config_file();
      void gui_handle_daq_load_pedestals_file();
      void gui_handle_daq_set_write_enable();
       void gui_handle_daq_mysql();
       void gui_handle_write_enabled();
       void gui_handle_daq_open_configuration_form();
       void gui_database_config();
       void save_database_settings();
       void save_database_settings_to_file();
       void save_gui_info_run();

       void gui_handle_co_pilot_selected();

       void gui_handle_max_events();
       void gui_set_zerofactor();
       
       void write_to_database();
       void write_to_file();
       
       void gui_dummy_det_config();

       void CloseWindow_transient();
       void CloseWindow_transient_database();
       void CloseWindow_summary();
      void update_timed() {
         do_gui_handle_current_entry_set(0);
      }
      bool m_is_stopping;
       TGLabel* m_gui_daq_configuration_label;
       
       
       //database
       void save_new_database();
       void create_new_database();


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

      void update_tree(CDetDetector* det);
      void update_tabs(TGListTreeItem* entry);
      void update_run_control();
       

      void clear_tabs();
      void clean_tree();
      void gui_reset_colors();
      void gui_update_status_bar();

       //co pilot
       void manage_co_pilot_run();
       


      
      TGListTreeItem* find_item_by_fullname(std::string const& fullname);
      TGListTreeItem* find_item_by_fullname_descendants(std::string const& fullname, TGListTreeItem* child);
      
      CEventBrowserOnline* m_eb;
      //GUI-related
      TGMainFrame*       m_mainframe;
      TGStatusBar*       m_gui_statusbar;
      TGHProgressBar*    m_gui_progressbar;
      TGHorizontalFrame* m_gui_top_frame;
      TGHorizontalFrame* m_gui_bottom_frame;
      //GUI - top frame elements
      TGListBox*		    m_gui_run_type;
      TGTextButton*      m_gui_run_start;
      TGTextButton*      m_gui_run_stop;
      TGTextButton*      m_gui_run_conf;
//      TGNumberEntry*     m_gui_current_entry;
      TGTextButton*      m_gui_daq_load_config_file;
      TGTextButton*      m_gui_daq_load_pedestal_file;
       TGTextButton*  m_gui_daq_configuration;

      TGCheckButton*     m_gui_daq_write_enabled;
       TGCheckButton*     m_gui_daq_mysql;

       TGCheckButton* m_gui_daq_max_event_enabled;
      TGLabel*   m_gui_daq_label_config_file;
      TGLabel*   m_gui_daq_label_pedestal_file;
      TGCheckButton* m_gui_view_pause_display;
       TGCheckButton* m_gui_co_pilot;

       TGNumberEntryField* m_gui_evt_counter;
       TGNumberEntry* m_gui_options_zerofactor;
       TGTextEntry* m_gui_comment_edit;
       TGComboBox* m_gui_type_physics;
       TGTransientFrame* fMain;
       TGTransientFrame* fMain_database;
       TGTransientFrame* fMain_new_database;

       TGTransientFrame* fMain_summary;

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
      
       std::vector<std::string> command_date_vec;
       std::vector<std::string> command_time_vec;
       std::vector<std::string> command_events_vec;

       
      //GUI - tabs
      TGTab*                 m_gui_main_tabs;
      std::vector<CGuiTab*>  m_tabs;
      
      TTimer* m_timer_update;
	  
	  IDList IDs;

      ClassDef(browser::CGuiMainFrameOnline, 0)
       
       //mysql configuration
       TString overload_mysql_name;
       TString overload_mysql_comment;
       TString date_start;
       TString date_stop;
       TString time_start;
       TString time_stop;
       TString pedestal_number;
       bool stop_clicked;
       bool run_is_ongoing;
       bool start_from_user;
       //database configuration
       
       TGTextEntry* database_username;
       TGTextEntry* database_password;
       TGTextEntry* database_ip;
       TGTextEntry* database_name;
       TGTextEntry* configuration_table_name;
       TGTextEntry* run_table_name;
       TGPictureButton* m_gui_mysql_impo;
       
       //Summary
       TGTextButton* start_run;
       TGTextButton* exit;
       
       //New database information
       TGTextEntry* m_database_new_config;
       TGTextEntry* m_database_new_runs;

      
   };
   
}



#endif /* defined(__eventbrowser__GuiMainFrameOnline__) */
