 //
//  GuiMainFrameOnline.cpp
//  eventbrowser
//
//  Created by Marcin Byszewski on 1/17/13.
//  Copyright (c) 2013 CERN - PH/ADE. All rights reserved.
//

#include "GuiMainFrameOnline.h"
#include "GuiTabStrip.h"
#include "GuiTabReadout.h"
#include "GuiTabPlane.h"
#include "GuiTabChamber.h"
#include "GuiTabDetector.h"
#include "GuiTabMainOptions.h"
#include "GuiTabDaqMonitor.h"
#include "GuiTabXmlCreator.h"
#include "ConfigRun.h"
#include "GuiConfig.h"
#include "ConfigDatabase.h"


#include "EventBrowser.h"
#include "EventBrowserOnline.h"
#include "DetDetector.h"

#include "ShmemReader.h"

#include "RootThread.h"
#include "DaqClientIpc.h"


#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/date_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>


#include <TGNumberEntry.h>
#include <TGButton.h>
#include <TGLabel.h>
#include <TGCanvas.h>
#include <TGListTree.h>
#include <TGTab.h>
#include <TGStatusBar.h>
#include <TGProgressBar.h>
#include <TGFileDialog.h>
#include <TGPicture.h>
#include <TGListBox.h>
#include <TGComboBox.h>
#include <TTimer.h>
#include <TGTextEntry.h>
#include <TGMsgBox.h>



#include <sstream>

#include <TSQLServer.h>
#include <TSQLStatement.h>

using namespace browser;
namespace bfs = boost::filesystem;

CGuiMainFrameOnline::CGuiMainFrameOnline() :
CGuiMainFrame(this, 0),
m_eb(0),
m_mainframe(0),
m_gui_statusbar(0),
m_gui_progressbar(0),
m_gui_top_frame(0),
m_gui_bottom_frame(0),
m_gui_run_type(0),
m_gui_run_start(0),
m_gui_run_stop(0),
m_gui_run_conf(0),
m_gui_comment_edit(0),
//m_gui_current_entry(0),
m_gui_daq_load_config_file(0),
m_gui_daq_load_pedestal_file(0),
m_gui_daq_write_enabled(0),
m_gui_daq_label_config_file(0),
m_gui_daq_label_pedestal_file(0),
m_gui_view_pause_display(0),
m_gui_tree_canvas(0),
m_gui_tree_viewport(0),
m_gui_tree_list(0),
m_gui_tree_item(0),
m_gui_pic_open(0),
m_gui_pic_close(0),
m_gui_dnd_rootcanvas(0),
m_gui_dnd_canvas(0),
m_gui_main_tabs(0),
m_tabs(),
m_is_stopping(false),
m_timer_update(new TTimer()),
stop_clicked(0),
run_is_ongoing(0),
IDs()
{

   m_timer_update->Connect("Timeout()", "browser::CGuiMainFrameOnline", this, "update_timed()");
   m_timer_update->Start(300, kFALSE);
   
}


CGuiMainFrameOnline::CGuiMainFrameOnline(CEventBrowser* eb, CConfiguration* config) :
CGuiMainFrame(this, config),
m_eb(dynamic_cast<CEventBrowserOnline*>(eb)),
m_mainframe(0),
m_gui_statusbar(0),
m_gui_progressbar(0),
m_gui_top_frame(0),
m_gui_bottom_frame(0),
m_gui_run_type(0),
m_gui_run_start(0),
m_gui_run_stop(0),
m_gui_run_conf(0),
//m_gui_current_entry(0),
m_gui_daq_load_config_file(0),
m_gui_daq_load_pedestal_file(0),
m_gui_daq_write_enabled(0),
m_gui_daq_label_config_file(0),
m_gui_daq_label_pedestal_file(0),
m_gui_view_pause_display(0),
m_gui_evt_counter(0),
m_gui_tree_canvas(0),
m_gui_tree_viewport(0),
m_gui_tree_list(0),
m_gui_tree_item(0),
m_gui_pic_open(0),
m_gui_pic_close(0),
m_gui_dnd_rootcanvas(0),
m_gui_dnd_canvas(0),
m_gui_main_tabs(0),
run_is_ongoing(0),
m_tabs(),
m_is_stopping(false),
m_timer_update(new TTimer())
{
   SetCleanup(kDeepCleanup);
   construct_window();
  
    //ConfigRun::GetInstance()->setSetup(0);
    
   m_timer_update->Connect("Timeout()", "browser::CGuiMainFrameOnline", this, "update_timed()");
   m_timer_update->Start(220, kFALSE);
   
}


CGuiMainFrameOnline::~CGuiMainFrameOnline()
{
   Cleanup();
}

CGuiMainFrame* CGuiMainFrameOnline::do_get_gui_main_frame()
{
   return this;
}

void CloseWindow();

TGMainFrame*  CGuiMainFrameOnline::do_get_tg_main_frame()
{
   return m_mainframe;
}

void CGuiMainFrameOnline::CloseWindow_transient()


{
    fMain->CloseWindow();
}
/**
 construct all gui elements
 */
void CGuiMainFrameOnline::construct_window()
{
   m_mainframe = new TGMainFrame(gClient->GetRoot(), 10, 10, kMainFrame | kVerticalFrame);
   ////////////////////////////////////////////////////////////////////////////
   
   {
      //top control vertical frame
      m_gui_top_frame = new TGHorizontalFrame(m_mainframe, 650, 100, kHorizontalFrame);

      
      {
         TGGroupFrame* group_opt = new TGGroupFrame(m_gui_top_frame, "Run Type", kVerticalFrame);
         {
            //run type
//            TGGroupFrame* group1frame = new TGGroupFrame(group_opt, "", kVerticalFrame);
            m_gui_run_type = new TGListBox(group_opt, -1, kHorizontalFrame | kSunkenFrame | kDoubleBorder | kOwnBackground);
            m_gui_run_type->AddEntry("Physics", 0);
            m_gui_run_type->AddEntry("Pedestals", 1);
            m_gui_run_type->Resize(122, 42);
            m_gui_run_type->Select(0);
            
            m_gui_run_type->Connect("Selected(Int_t)", "browser::CGuiMainFrameOnline", this, "gui_handle_run_type_select(Int_t)");
            group_opt->AddFrame(m_gui_run_type, new TGLayoutHints(kLHintsLeft | kLHintsTop, -10, 0, 0, 0));
            

//            group_opt->AddFrame(group1frame, new TGLayoutHints(kLHintsLeft, 0, 0, 0, 0));
         }
         
         {
            m_gui_daq_write_enabled = new TGCheckButton(group_opt, "Write File");
            m_gui_daq_write_enabled->Connect("Clicked()", "browser::CGuiMainFrameOnline", this, "gui_handle_write_enabled()");
            m_gui_daq_write_enabled->Resize(122, 22);
            group_opt->AddFrame(m_gui_daq_write_enabled, new TGLayoutHints(kLHintsLeft, -10, -10, 0, 0));
         }

          {
              
            TGHorizontalFrame* mysql_frame= new TGHorizontalFrame(group_opt);
              
              m_gui_daq_mysql = new TGCheckButton(mysql_frame, "Enable MySQL");
             m_gui_daq_mysql->Connect("Clicked()", "browser::CGuiMainFrameOnline", this, "gui_handle_daq_mysql()");
//              m_gui_daq_mysql->	SetDisabledAndSelected(false);
              m_gui_daq_mysql->SetState(kButtonUp);
              m_gui_daq_mysql->SetEnabled(false);
              m_gui_daq_mysql->Resize(122, 22);
              
              mysql_frame->AddFrame(m_gui_daq_mysql, new TGLayoutHints(kLHintsLeft, 0, 0, 0, 0));
              
              
             m_gui_mysql_impo = new TGPictureButton(mysql_frame,gClient->GetPicture("arrow_right.xpm"));
             m_gui_mysql_impo->Resize(15, 15);
              m_gui_mysql_impo->SetEnabled(false);

              m_gui_mysql_impo->Connect("Clicked()", "browser::CGuiMainFrameOnline", this, "gui_database_config()");
              mysql_frame->AddFrame(m_gui_mysql_impo, new TGLayoutHints( kLHintsTop , 0, 0, 0, 0));
              
              

              group_opt->AddFrame(mysql_frame, new TGLayoutHints(kLHintsLeft, -10, -10, 0, 0));
          }

         m_gui_top_frame->AddFrame(group_opt, new TGLayoutHints( kLHintsTop, 0, 0, 0, 0));
     
          
      }
      
      
      
      {
         TGGroupFrame* group_run = new TGGroupFrame(m_gui_top_frame, "Run", kVerticalFrame);
         {
            //start run button
            m_gui_run_start = new TGTextButton(group_run, "&Start");
            m_gui_run_start->Resize(110, 22);
            m_gui_run_start->Connect("Clicked()", "browser::CGuiMainFrameOnline", this, "gui_handle_run_start()");
            group_run->AddFrame(m_gui_run_start, new TGLayoutHints( kLHintsTop | kLHintsExpandX, 10, 10, 1, 1));
         }
         
         {
            //stop run button
            m_gui_run_stop = new TGTextButton(group_run, "&Stop");
            m_gui_run_stop->Resize(110, 22);
            m_gui_run_stop->Connect("Clicked()", "browser::CGuiMainFrameOnline", this, "gui_handle_run_stop()");
            group_run->AddFrame(m_gui_run_stop, new TGLayoutHints( kLHintsTop | kLHintsExpandX, 10, 10, 0, 0));
         }
	 {
            //max event counter
		   m_gui_evt_counter = new TGNumberEntryField(group_run, IDs.GetUniqueID(), 0, TGNumberEntryField::kNESInteger, TGNumberEntryField::kNEANonNegative, TGNumberEntryField::kNELLimitMin, 0.0,9e9 );//
            m_gui_evt_counter->Resize(65, 20);
            //m_gui_evt_counter->Connect("ValueSet(Long_t)", "browser::CGuiMainFrameOnline", this, "gui_handle_max_events()");
            group_run->AddFrame(m_gui_evt_counter, new TGLayoutHints( kLHintsRight, 10, 10, 0, 0));
         }

      
	 {
	   m_gui_daq_max_event_enabled = new TGCheckButton (group_run,"");
	   m_gui_daq_max_event_enabled->Resize(122,22);
	    m_gui_daq_max_event_enabled->Connect("Clicked()", "browser::CGuiMainFrameOnline", this, "gui_handle_max_events()");
	   group_run->AddFrame(m_gui_daq_max_event_enabled, new TGLayoutHints ( kLHintsTop, -10,0,-17,4));
	 }
              m_gui_top_frame->AddFrame(group_run, new TGLayoutHints( kLHintsTop, 0, 0, 0, 0 ));
      }

      {
         TGGroupFrame* group_other = new TGGroupFrame(m_gui_top_frame, "Other", kVerticalFrame);
         
         {
            //configure run button
            m_gui_run_conf = new TGTextButton(group_other, "&Configure");
            m_gui_run_conf->Resize(110, 22);
            m_gui_run_conf->Connect("Clicked()", "browser::CGuiMainFrameOnline", this, "gui_handle_run_configure()");
            group_other->AddFrame(m_gui_run_conf, new TGLayoutHints( kLHintsTop, 1, 1, 1, 1));
         }
         
         {
            //m_gui_view_pause_display
            m_gui_view_pause_display = new TGCheckButton(group_other, "Pause Display");
            //         m_gui_view_pause_display->Connect("Clicked()", "browser::CGuiMainFrameOnline", this, "gui_handle_daq_set_write_enable()");
            m_gui_view_pause_display->Resize(122, 22);
            group_other->AddFrame(m_gui_view_pause_display, new TGLayoutHints(kLHintsLeft, 0, 0, 0, 1));
         }
          
        {
              //m_gui_view_pause_display
            m_gui_co_pilot = new TGCheckButton(group_other, "Co-Pilot");
            m_gui_co_pilot->Connect("Clicked()", "browser::CGuiMainFrameOnline", this, "gui_handle_co_pilot_selected()");
            m_gui_co_pilot->Resize(122, 22);
            group_other->AddFrame(m_gui_co_pilot, new TGLayoutHints(kLHintsLeft, 0, 0, 0, 10));
        }
         m_gui_top_frame->AddFrame(group_other, new TGLayoutHints( kLHintsTop, 0, 0, 0, 0));

      }
      
      
      {
         TGGroupFrame* group_files = new TGGroupFrame(m_gui_top_frame, "External Files", kVerticalFrame);
       
         
         {//group config file
            TGHorizontalFrame* group_config_file = new TGHorizontalFrame(group_files);
            {
               //Set Main Configuration file
               m_gui_daq_load_config_file = new TGTextButton(group_config_file, "&Daq Config File");
               m_gui_daq_load_config_file->Resize(110, 22);
               m_gui_daq_load_config_file->Connect("Clicked()", "browser::CGuiMainFrameOnline", this, "gui_handle_daq_load_config_file()");
               group_config_file->AddFrame(m_gui_daq_load_config_file, new TGLayoutHints( kLHintsLeft | kLHintsTop, 1, 1, 0, 1));
               m_gui_daq_label_config_file = new TGLabel(group_config_file, "");
               m_gui_daq_label_config_file->SetTextJustify(kTextLeft | kTextCenterY);
               group_config_file->AddFrame(m_gui_daq_label_config_file,  new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX, 0, 0, 0, 0));
            }
            group_files->AddFrame(group_config_file, new TGLayoutHints( kLHintsLeft| kLHintsTop | kLHintsExpandX, 0, 0, 0, 0));
         }//group config file
         
         {//group pedestal file
            TGHorizontalFrame* group_pedestal_file = new TGHorizontalFrame(group_files);
            {
               //Set Pedestal file
               m_gui_daq_load_pedestal_file = new TGTextButton(group_pedestal_file, "&Pedestal File");
               m_gui_daq_load_pedestal_file->Resize(110, 22);
               m_gui_daq_load_pedestal_file->Connect("Clicked()", "browser::CGuiMainFrameOnline", this, "gui_handle_daq_load_pedestals_file()");
               group_pedestal_file->AddFrame(m_gui_daq_load_pedestal_file, new TGLayoutHints( kLHintsTop, 1, 1, 0, 1));
               m_gui_daq_label_pedestal_file = new TGLabel(group_pedestal_file, "");
               m_gui_daq_label_pedestal_file->SetTextJustify(kTextLeft | kTextCenterY);
               group_pedestal_file->AddFrame(m_gui_daq_label_pedestal_file,  new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX, 0, 0, 0, 0));
            }
            group_files->AddFrame(group_pedestal_file,  new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX, 0, 0, 0, 0));
        
             {//group configuration
                 TGHorizontalFrame* group_configuration = new TGHorizontalFrame(group_files);
                 {
                     //Set configuration
                     m_gui_daq_configuration = new TGTextButton(group_configuration, "&Configuration");
                     m_gui_daq_configuration->Resize(110, 22);
                     m_gui_daq_configuration->Connect("Clicked()", "browser::CGuiMainFrameOnline", this, "gui_handle_daq_open_configuration_form()");
                     group_configuration->AddFrame(m_gui_daq_configuration, new TGLayoutHints( kLHintsTop, 1, 1, 0,0 ));
                     m_gui_daq_configuration->SetEnabled(false);
                     m_gui_daq_configuration_label = new TGLabel(group_configuration, "");
                     m_gui_daq_configuration_label->SetTextJustify(kTextLeft | kTextCenterY);
                     group_configuration->AddFrame(m_gui_daq_configuration_label,  new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX, 0, 0, 0, 0));
                 }
                 group_files->AddFrame(group_configuration,  new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX, 0, 0, 0, 1));
             }
         
         }//group pedestal file
         m_gui_top_frame->AddFrame(group_files, new TGLayoutHints( kLHintsTop | kLHintsExpandX, 0, 0, 0, 0));
    
      
       TGGroupFrame* groupzerosopp = new TGGroupFrame(m_gui_top_frame, "Zero Suppression Factor", kVerticalFrame);
       {
           
               //set zero factor value
               m_gui_options_zerofactor = new TGNumberEntry(groupzerosopp, 1, 5, IDs.GetUniqueID(), TGNumberEntry::kNESReal, TGNumberEntry::kNEAPositive, TGNumberEntry::kNELLimitMin, 0.0 );//
               m_gui_options_zerofactor->Resize(110, 22);
               m_gui_options_zerofactor->Connect("ValueSet(Long_t)", "browser::CGuiMainFrameOnline", this, "gui_set_zerofactor()");
               groupzerosopp->AddFrame(m_gui_options_zerofactor, new TGLayoutHints( kLHintsRight, 0, 0, 0, 44));
           
       }
          m_gui_top_frame->AddFrame(groupzerosopp, new TGLayoutHints( kLHintsTop | kLHintsRight ,  0, 0, 0, 0));

          
      }

//      {
//         // next, prev, quit, curr
//         TGLabel* label_centry  = new TGLabel(m_gui_top_frame, "Current entry:");
//         m_gui_top_frame->AddFrame(label_centry, new TGLayoutHints(kLHintsLeft | kLHintsTop , 5, 5, 5, 5));
//         
//         m_gui_current_entry = new TGNumberEntry(m_gui_top_frame, 0, 7,
//                                                 m_id.GetUniqueID(),
//                                                 TGNumberEntry::kNESInteger,
//                                                 TGNumberFormat::kNEANonNegative);
//         m_gui_current_entry->Connect("ValueSet(Long_t)",
//                                      "browser::CGuiMainFrame",
//                                      this,
//                                      "gui_handle_current_entry_set(Long_t)");
//         
//         m_gui_top_frame->AddFrame(m_gui_current_entry,
//                                   new TGLayoutHints(kLHintsLeft | kLHintsTop , 0, 0, 0, 0));
//      }
      
      m_mainframe->AddFrame(m_gui_top_frame, new TGLayoutHints(kLHintsExpandX , 1, 1, 1, 1));
   }
    
    {
        
    TGHorizontalFrame* hor_frame = new TGHorizontalFrame(m_mainframe);
        
        TGGroupFrame* comments_frame = new TGGroupFrame(hor_frame, "Comments", kVerticalFrame);
   {
       m_gui_comment_edit = new TGTextEntry( comments_frame);
       m_gui_comment_edit->Resize(250,20);
       comments_frame->AddFrame(m_gui_comment_edit, new TGLayoutHints(kLHintsExpandX , 0,0,0,0));
    
   }
        hor_frame->AddFrame(comments_frame, new TGLayoutHints(kLHintsExpandX , 0,0,0,0));

        
    TGGroupFrame* physics_frame = new TGGroupFrame(hor_frame, "Physics Type", kVerticalFrame);
    {
        
        m_gui_type_physics= new TGComboBox(physics_frame);
        m_gui_type_physics->Resize(100,20);
        
        m_gui_type_physics->AddEntry("", 0);

        m_gui_type_physics->AddEntry("Beam", 1);
        m_gui_type_physics->AddEntry("Cosmics", 2);
        m_gui_type_physics->AddEntry("X-Rays", 3);
        m_gui_type_physics->AddEntry("Random", 4);
        m_gui_type_physics->Select(0);

        physics_frame->AddFrame(m_gui_type_physics, new TGLayoutHints(kLHintsRight, 0,0,0,0));
        
    }
    hor_frame->AddFrame(physics_frame, new TGLayoutHints(kLHintsRight, 0,0,0,0));
        
    m_mainframe->AddFrame(hor_frame, new TGLayoutHints(kLHintsExpandX , 0,0,0,0));

        
        
    }

   {
      //bottom main frame (split)
      m_gui_bottom_frame = new TGHorizontalFrame(m_mainframe, 650, 200, kHorizontalFrame);
      
      {
         {
            //det tree
            //tree widget
            m_gui_tree_canvas = new TGCanvas(m_gui_bottom_frame, 180, 152);
            m_gui_tree_viewport = m_gui_tree_canvas->GetViewPort();
            // list tree
            m_gui_tree_list = new TGListTree(m_gui_tree_canvas, kHorizontalFrame);
            m_gui_tree_list->SetName("m_treelistbox");
            m_gui_tree_list->SetCheckMode(TGListTree::kRecursive);
            m_gui_tree_list->Connect("Clicked(TGListTreeItem*,Int_t)",
                                     "browser::CGuiMainFrame",
                                     this,
                                     "gui_handle_tree_click(TGListTreeItem*,Int_t)");
            m_gui_tree_list->Connect("Checked(TObject*, Bool_t)",
                                     "browser::CGuiMainFrame",
                                     this,
                                     "gui_handle_tree_check(TObject*, Bool_t)");
            
            m_gui_tree_viewport->AddFrame(m_gui_tree_list);
            m_gui_tree_list->SetLayoutManager(new TGHorizontalLayout(m_gui_tree_list));
            m_gui_tree_list->MapSubwindows();
            m_gui_tree_canvas->SetContainer(m_gui_tree_list);
            m_gui_tree_canvas->MapSubwindows();
            m_gui_bottom_frame->AddFrame(m_gui_tree_canvas,
                                         new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandY, 2, 2, 2, 2));
         }
         
         //         {
         //            //dropdown area
         //            m_gui_dnd_rootcanvas = new TRootEmbeddedCanvas("dnd", m_gui_bottom_frame, 100, 100);
         //            m_gui_dnd_canvas = m_gui_dnd_rootcanvas->GetCanvas();
         //            m_gui_bottom_frame->AddFrame(m_gui_dnd_rootcanvas,
         //                                         new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY, 2, 2, 2, 2));
         //         }
         
         
         {
            //tabs
            // tab widget
            m_gui_main_tabs = new TGTab(m_gui_bottom_frame, 484, 316);
            m_tabs.push_back(new CGuiTabDaqMonitor(this,   m_config, m_eb, m_gui_main_tabs, "DAQ"));
            m_tabs.push_back(new CGuiTabStrip(this,   m_config, m_eb, m_gui_main_tabs, "Strip"));
            m_tabs.push_back(new CGuiTabReadout(this, m_config, m_eb, m_gui_main_tabs, "Readout"));
            m_tabs.push_back(new CGuiTabPlane(this,   m_config, m_eb, m_gui_main_tabs, "Layer"));
            m_tabs.push_back(new CGuiTabChamber(this, m_config, m_eb, m_gui_main_tabs, "Chamber"));
            m_tabs.push_back(new CGuiTabDetector(this, m_config, m_eb, m_gui_main_tabs, "Detector"));
            m_tabs.push_back(new CGuiTabMainOptions(this, m_config, m_eb, m_gui_main_tabs, "Main Options"));
            m_tabs.push_back(new CGuiTabXml(this, m_config, m_eb, m_gui_main_tabs, "Xml config"));
//             m_tabs.push_back(new CGuiTabConfig(this, m_config, m_eb, m_gui_main_tabs, "Config"));
//             m_tabs.push_back(new CGuiTabHVConfig(this, m_config, m_eb, m_gui_main_tabs, "HV Config"));

             //                m_tabs.push_back(new CGuiTabDetConfig(this, m_gui_main_tabs, "Det Config", m_daq));
            //                m_tabs.push_back(new CGuiTabDaqConfig(this, m_gui_main_tabs, "DAQ Config", m_daq));
            
            
            //tabs - finish up
            m_gui_main_tabs->SetTab("Main Options");
            m_gui_main_tabs->Resize(m_gui_bottom_frame->GetDefaultSize());
            m_gui_bottom_frame->AddFrame(m_gui_main_tabs, new TGLayoutHints( kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY));
            //end tabs
            
         }
         
         
      }
      m_mainframe->AddFrame(m_gui_bottom_frame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 1, 1, 1, 1));
   }
   
   {
      // status bar
     Int_t parts[] = {20, 20, 20, 40};
      m_gui_statusbar = new TGStatusBar(m_mainframe, 50, 10, kHorizontalFrame);
      m_gui_statusbar->SetParts(parts, 4);
      m_gui_statusbar->SetText("filename", 0);
      m_gui_statusbar->SetText("current event =", 1);
      
      {
         TGCompositeFrame *part = m_gui_statusbar->GetBarPart(2); // 3rd part
         m_gui_progressbar = new TGHProgressBar(part,100);
         m_gui_progressbar->SetMax(100.0);
         part->SetLayoutBroken(kTRUE);
         m_gui_progressbar->Resize(200, 14);
         //         part->SetLayoutBroken(kFALSE);
         m_gui_progressbar->MoveResize(2,2,200,10);
         part->AddFrame(m_gui_progressbar, new TGLayoutHints(kLHintsCenterY | kLHintsCenterX | kLHintsExpandX , 0, 0, 0, 0));
         part->MapSubwindows();
      }
      
      m_mainframe->AddFrame(m_gui_statusbar, new TGLayoutHints(kLHintsBottom | kLHintsLeft | kLHintsExpandX, 0, 0, 2, 0));
   }
   
   ////////////////////////////////////////////////////////////////////////////
   // main frame - finish up
   m_mainframe->SetMWMHints(kMWMDecorAll, kMWMFuncAll, kMWMInputModeless);
   m_mainframe->SetWindowName("mmDAQ Online Event Browser");
   m_mainframe->MapSubwindows();
   m_mainframe->Resize(m_mainframe->GetDefaultSize());
   m_mainframe->MapWindow();
   m_mainframe->Resize(750, 650);
   m_mainframe->MoveResize(100, 0, (unsigned)(gClient->GetDisplayWidth()*0.7), (unsigned)(gClient->GetDisplayHeight()*0.75));
   m_mainframe->Connect("CloseWindow()", "browser::CGuiMainFrame", this, "CloseWindow()");
   
   update();
//   for (int ii = 0; ii < m_gui_main_tabs->GetNumberOfTabs()-1; ++ii) {
//      //m_gui_main_tabs->GetTabTab(ii)->SetEnabled(false);
//   }
   
    
    //in case of crush recover the info from the file
    std::stringstream file_path;
    file_path << m_config->get_working_path().string() << "/gui_run_info.txt";
    std::ifstream gui_info(file_path.str().c_str());
    if (gui_info)
    {
        
        double run_type_id, evt_number_id, zero_suppr_id,physics_type_id;
        int write_enabled_id, mysql_enabled_id, max_event_active_id;
        std::string config_file_path_id, info_setup_id, pedestal_file_id, comment_id;
        gui_info >> run_type_id;
        m_gui_run_type->Select(run_type_id);
        
        gui_info >> write_enabled_id;
        
        if (write_enabled_id==1)
            m_gui_daq_write_enabled->SetState(kButtonDown);
        else
            m_gui_daq_write_enabled->SetState(kButtonUp);

        gui_info >> mysql_enabled_id;

        
        gui_handle_write_enabled();
        
        if (mysql_enabled_id==1){
            m_gui_daq_mysql->SetState(kButtonDown);
        gui_handle_daq_mysql();}
        else{
            m_gui_daq_mysql->SetState(kButtonUp);
        

    }
    
        
        gui_info >> max_event_active_id;
        
        if (max_event_active_id==1)
            m_gui_daq_max_event_enabled->SetState(kButtonDown);
        else
            m_gui_daq_max_event_enabled->SetState(kButtonUp);
        

        
        gui_info >> evt_number_id;
        m_gui_evt_counter->SetNumber(evt_number_id);
        //Co pilot?
        // gui_info << m_gui_co_pilot->IsOn()  << std::endl;
        
        gui_info >> config_file_path_id;
        m_gui_daq_label_config_file->SetText(TString(config_file_path_id));

        gui_info >> pedestal_file_id;
        m_gui_daq_label_pedestal_file->SetText(TString(pedestal_file_id));
	 pedestal_number = boost::filesystem::basename (pedestal_file_id);
        
        gui_info >> info_setup_id;

        std::stringstream setup_config;
        setup_config << "Setup Configuration " << TString(info_setup_id);
        m_gui_daq_configuration_label->SetText(setup_config.str().c_str());
        
        
        if (m_gui_daq_mysql->IsOn()){
        ConfigRun::GetInstance()->setSetup(atof(info_setup_id.c_str()));
        ConfigRun::GetInstance()->check_complete();
        }

        gui_info >> zero_suppr_id;
        m_gui_options_zerofactor->SetNumber(zero_suppr_id);
        
        std::getline(gui_info,comment_id);
	 std::string temp;
 	 std::stringstream s2;

	 while(gui_info >> temp) {
		s2 << temp << " ";
	     if (gui_info.peek() == '\n')
  	         break;

	 }
	 comment_id = s2.str();
        m_gui_comment_edit->SetText(TString(comment_id));
        
        gui_info >> physics_type_id;
        m_gui_type_physics->Select(physics_type_id);
        
        gui_info >> date_start;

        gui_info >> time_start;

        gui_info >> overload_mysql_name;
        gui_info >> overload_mysql_comment;
        run_is_ongoing = true;
        gui_info.close();
    }
    
   // load the information of the database by reading the file
   ConfigDatabase* create_database = ConfigDatabase::GetInstance();
    
}


void CGuiMainFrameOnline::do_update()
{
    if (m_gui_co_pilot->IsOn() && !run_is_ongoing && command_events_vec.size()>0)
    {
        manage_co_pilot_run();
    }
    
    
   //update run control
   update_run_control();
   
   
   if(m_eb->get_detector()) {
      m_eb->get_detector()->lock();
      update_tree(m_eb->get_detector());
   }
   else {
	   update_tree(0);
   }
   
//   clean_tree();
   gui_update_status_bar();


//   update_tabs(0);

   if(m_eb->get_detector()) {
      m_eb->get_detector()->unlock();
   }
   if(ConfigRun::GetInstance()->IsComplete()){
    std::stringstream ss3;
    ss3 << "Setup Configuration:  " << ConfigRun::GetInstance()->GetSetup();
    m_gui_daq_configuration_label->SetText(ss3.str().c_str());
   }

   
}


void CGuiMainFrameOnline::manage_co_pilot_run()
{
    boost::posix_time::ptime m_run_time = boost::posix_time::second_clock::local_time();
    std::string date_now = boost::gregorian::to_iso_string(m_run_time.date());
    std::string time_now = boost::posix_time::to_iso_string(m_run_time.time_of_day());

    if (*(command_date_vec.begin())==date_now && *(command_time_vec.begin())==time_now) {
        
        if (*command_events_vec.begin()=="-")
            m_gui_daq_max_event_enabled->SetState(kButtonUp);
        else{
            m_gui_evt_counter->SetNumber(atof(command_events_vec.begin()->c_str()));
            m_gui_daq_max_event_enabled->SetState(kButtonDown);
        }
        start_from_user=false;
        gui_handle_run_start_effective();
        
        command_events_vec.erase(command_events_vec.begin());
        command_date_vec.erase(command_date_vec.begin());
        command_time_vec.erase(command_time_vec.begin());

     if (command_events_vec.size()==0){

       m_gui_co_pilot->SetState(kButtonUp);
        gui_handle_co_pilot_selected();
     } 
    }
    else if (*(command_date_vec.begin())=="-" && *(command_time_vec.begin())=="-") {
      sleep(5);
      if (*command_events_vec.begin()=="-")
        m_gui_daq_max_event_enabled->SetState(kButtonUp);
      else{
         m_gui_evt_counter->SetNumber(atof(command_events_vec.begin()->c_str()));
         m_gui_daq_max_event_enabled->SetState(kButtonDown);
      }
      start_from_user=false;
      gui_handle_run_start_effective();

      command_events_vec.erase(command_events_vec.begin());
      command_date_vec.erase(command_date_vec.begin());
      command_time_vec.erase(command_time_vec.begin());

     if (command_events_vec.size()==0){

       m_gui_co_pilot->SetState(kButtonUp);
        gui_handle_co_pilot_selected();
     } 
   }


       
        


            
    
    
}

void CGuiMainFrameOnline::gui_update_status_bar()
{
   std::stringstream ss;
   //std::stringstream ss2;
   ss.str("");
   //ss2.str("");

   size_t curr_entry = m_eb->get_current_event_number();
   size_t max_evt = m_gui_evt_counter->GetNumber();
   
   ss << " Run number: " << CShmemReader::run_name <<"   Current entry = " << curr_entry;

   m_gui_statusbar->SetText(ss.str().c_str(), 3);

   if(max_evt && m_gui_daq_max_event_enabled->IsOn())
   {
    do_update_progress((double) curr_entry / (double) max_evt);
 
    if(curr_entry >= max_evt && !m_is_stopping ) 
    {
    gui_handle_run_stop(); 
    m_is_stopping = true;
     ULong_t color;
     gClient->GetColorByName("green", color);
     m_gui_run_stop->TGWindow::SetBackgroundColor(color);
     std::stringstream ss2;
     ss2.str("");
     ss2 << " Data acquisition has finished ";
     m_gui_statusbar->SetText(ss2.str().c_str(), 1);
    }
   }

}

void CGuiMainFrameOnline::gui_handle_co_pilot_selected()
{
    if (m_gui_co_pilot->IsOn()){
        TGFileInfo file_info;

        const char *filetypes[] = {
            "TXT",   "*.txt",
            "All files",     "*",
            0,               0 };
        
        int ftype = 0; // default selection
        
        size_t sz = m_config->get_working_path().string().size();
        char* cwd = new char[sz+1];
        strncpy(cwd, m_config->get_working_path().string().c_str(), sz);
        cwd[sz] = '\0';
        
        file_info.fFileTypes = filetypes;
        file_info.fFileTypeIdx = ftype;
        file_info.fIniDir    = cwd;
        new TGFileDialog(gClient->GetRoot(), m_mainframe, kFDOpen, &file_info); // do not delete
        
        
        if (!file_info.fFilename) {
            m_gui_daq_label_config_file->SetText("");
            m_gui_top_frame->Layout();
            return;
        }
        
        if (!bfs::exists(file_info.fIniDir) || !bfs::is_directory(file_info.fIniDir)) {
            std::cout << "WARNING: bad input path: " + std::string(file_info.fIniDir) << std::endl;
            m_gui_daq_label_config_file->SetText("");
            m_gui_top_frame->Layout();
            return;
        }
        if (!bfs::exists(file_info.fFilename) || !bfs::is_regular(file_info.fFilename)) {
            std::cout << "WARNING: bad input file: " + std::string(file_info.fFilename) << std::endl;
            m_gui_daq_label_config_file->SetText("");
            m_gui_top_frame->Layout();
            return;
        }
        
        std::ifstream command_file;
        command_file.open(file_info.fFilename);
        
        if (command_file.is_open()) {
            while (!command_file.eof()) {
                
                std::string date, time, events;

                command_file >> date >> time >> events;
                 if (date!=""){

                   command_date_vec.push_back(date);
                  command_time_vec.push_back(time);
                   command_events_vec.push_back(events);
                 }
            }

        //    for (int k=0; k<command_date.size(); k++)
             //  std::cout << command_date_vec[k] << command_time_vec[k] << command_events_vec[k] <<std::endl;
         
        }
        command_file.close();
        
        
        
   /*     m_gui_run_start->SetEnabled(false);
        m_gui_run_stop->SetEnabled(false);
        m_gui_run_conf->SetEnabled(false);
        m_gui_daq_load_config_file->SetEnabled(false);
        m_gui_daq_load_pedestal_file->SetEnabled(false);
        m_gui_daq_configuration->SetEnabled(false);
        m_gui_daq_write_enabled->SetEnabled(false);
        m_gui_daq_mysql->SetEnabled(false);
        m_gui_daq_max_event_enabled->SetEnabled(false);
        m_gui_type_physics->SetEnabled(false); */

        
    }
    else {
    
        command_date_vec.clear();
        command_time_vec.clear();
        command_events_vec.clear();
        
     /*   m_gui_run_start->SetEnabled(true);
        m_gui_run_stop->SetEnabled(true);
        m_gui_run_conf->SetEnabled(true);
        m_gui_daq_load_config_file->SetEnabled(true);
        m_gui_daq_load_pedestal_file->SetEnabled(true);
        m_gui_daq_configuration->SetEnabled(true);
        m_gui_daq_write_enabled->SetEnabled(true);
        m_gui_daq_mysql->SetEnabled(true);
        m_gui_daq_max_event_enabled->SetEnabled(true);
        m_gui_type_physics->SetEnabled(true); */

        
    }

}


void CGuiMainFrameOnline::update_tree( CDetDetector* det)
{
//      std::cout << "----- CGuiMainFrameOnline::update_tree() -----\n";
   {
      if (det && m_gui_tree_list) {
         det->gui_populate_tree(m_gui_tree_list, (TGListTreeItem*)0);
      }
      else {
         clean_tree();
         std::cout << "CGuiMainFrameOnline::update_tree (det = 0)\n";
      }
   }
}

void CGuiMainFrameOnline::clear_tabs()
{
//   std::cout << "----- CGuiMainFrameOnline::clear_tabs() -----\n";
   std::for_each(m_tabs.begin(), m_tabs.end(), boost::bind(&CGuiTab::clear_tab, _1) );
}


void CGuiMainFrameOnline::gui_handle_daq_mysql()
{
    
        FILE* pipe = popen("which mysql", "r");
        if (!pipe)
            std::cout << "ERROR" << std::endl;
        char buffer[128];
        std::string result = "";
        while(!feof(pipe)) {
            if(fgets(buffer, 128, pipe) != NULL)
                result += buffer;
        }
        pclose(pipe);
 
    
    if(result.empty())
    {
         ConfigRun::GetInstance()->set_check_online(false);
        TGMsgBox* message_box = new TGMsgBox (gClient->GetRoot(), 0, "Error in MySql", "MySql is not correctly installed in your computer!",0, kMBClose);
        m_gui_daq_mysql->SetState(kButtonUp);
        
    }
    
    
    
    else {
    
        try{


            std::stringstream cmd;
            cmd <<"mysql://"<<ConfigDatabase::GetInstance()->GetIp()<<"/"<<ConfigDatabase::GetInstance()->GetDatabaseName();

            TSQLServer* serv = TSQLServer::Connect(cmd.str().c_str(), ConfigDatabase::GetInstance()->GetUsername().c_str(), ConfigDatabase::GetInstance()->GetPassword().c_str());
            
            
            
            if (! serv) {std::cout<<"------- Error connecting to the database. Contact the Server administrator ------- "<<std::endl;
                m_gui_daq_configuration->SetEnabled(false);
                ConfigRun::GetInstance()->set_check_online(false);
                m_gui_daq_mysql->SetState(kButtonUp);
                

            }
            else {
        
                
                m_gui_daq_configuration->SetEnabled(true);

                if(ConfigRun::GetInstance()->get_check_online()){
                    ConfigRun::GetInstance()->set_check_online(false);
                    m_gui_daq_configuration->SetEnabled(false);}
                else
                    ConfigRun::GetInstance()->set_check_online(true);
            }
        }
        
        catch (int e)
        {
            std::cout << "An exception occurred. Exception Nr. " << e << '\n';
        }
        
    
    }
    
}



/**
 call detector element from selected tree item and update its gui tab
 */
void CGuiMainFrameOnline::update_tabs(TGListTreeItem* )
{
   
//   std::cout << "----- CGuiMainFrameOnline::update_tabs() -----\n";

   if(!m_gui_main_tabs) {
      std::cout << "----- CGuiMainFrameOnline::update_tabs() m_gui_main_tabs NULL-----\n";
      return;
   }
   
   for (int ii = 0; ii < m_gui_main_tabs->GetNumberOfTabs()-1; ++ii) {
      //m_gui_main_tabs->GetTabTab(ii)->SetEnabled(false);
   }
   
   bool do_switch_tab = true;
   //check if DAQ on options is enabled
   // then update those and return
   int curr = m_gui_main_tabs->GetCurrent();
   if(CGuiTabMainOptions* tab = dynamic_cast<CGuiTabMainOptions*>(m_tabs[curr])) {
      tab->update(0);
      do_switch_tab= false;
   }
   else if(CGuiTabDaqMonitor* tab = dynamic_cast<CGuiTabDaqMonitor*>(m_tabs[curr])) {
      tab->update(0);
      do_switch_tab= false;
   }
   

   

   
   
   //update all other tabs
   //update gui tabs and set the current as active
   if (m_gui_tree_item) {
      CDetBase* detbase = static_cast<CDetBase*>(m_gui_tree_item->GetUserData());
      if (detbase) {
         detbase->get_detector()->lock();
         int active = detbase->update_my_gui_tab(m_tabs);
//         std::cout << "----- CGuiMainFrameOnline::update_tabs() active = "<< active << " \n";

         if (1/*active >= 0*/) {
            m_gui_main_tabs->SetEnabled(active);
            if(do_switch_tab)
               m_gui_main_tabs->SetTab(active);
         }
         detbase->get_detector()->unlock();
      }
   }

   
   
   
   
   
     
   
}




void CGuiMainFrameOnline::gui_handle_write_enabled()
{
  
    if (m_gui_daq_write_enabled->IsOn()){
        m_gui_daq_mysql->SetEnabled(true);
        m_gui_mysql_impo->SetEnabled(true);
    }
    else {
        m_gui_daq_mysql->SetState(kButtonUp);
        m_gui_daq_mysql->SetEnabled(false);
        m_gui_mysql_impo->SetEnabled(false);

//        m_gui_daq_mysql->SetDisabledAndSelected(false);
        ConfigRun::GetInstance()->set_check_online(false);
    }
}



void CGuiMainFrameOnline::do_update_progress(double val)
{
   m_gui_progressbar->SetPosition(val*100.0);
    double percent=val*100;
   std::stringstream ss;
   ss << percent <<" %";
   m_gui_statusbar->SetText(ss.str().c_str(), 0);
}


void CGuiMainFrameOnline::gui_handle_daq_open_configuration_form()
{
    CGuiConfig* frame = new CGuiConfig(m_mainframe, 200, 200);

    
}
    

void CGuiMainFrameOnline::update_run_control()
{
   bool enabled = m_eb->get_daq_client()->is_running();
    if (enabled ==false)
    {
   m_gui_run_start->SetEnabled(enabled);
   m_gui_run_stop->SetEnabled(enabled);
   m_gui_run_conf->SetEnabled(enabled);
    
    }
    
}

void CGuiMainFrameOnline::clean_tree()
{
//   std::cout << "----- CGuiMainFrameOnline::clean_tree() -----\n";
   //clean tree50
   TGListTreeItem* FirstItem = m_gui_tree_list->GetFirstItem();
   while (FirstItem) {
      m_gui_tree_list->DeleteChildren(FirstItem);
      m_gui_tree_list->DeleteItem(FirstItem);
      FirstItem = m_gui_tree_list->GetFirstItem();
   }
   m_gui_tree_list->ClearViewPort();

   //?
   TList* itemlist = m_gui_tree_list->GetList();
   //clear tree
   TObjLink *lnk = itemlist->FirstLink();
   while (lnk) {
      TObjLink* nextlnk = lnk->Next();
      lnk->GetObject()->Delete();
      lnk = nextlnk;
   }
}


void CGuiMainFrameOnline::gui_reset_colors()
{
   ULong_t color;
   gClient->GetColorByName("gray", color);
//   ULong_t col = m_mainframe->GetDefaultFrameBackground();
   m_gui_run_start->TGWindow::SetBackgroundColor(color);
   m_gui_run_stop->TGWindow::SetBackgroundColor(color);
   m_gui_daq_label_config_file->TGWindow::SetBackgroundColor(color);
   m_gui_daq_label_pedestal_file->TGWindow::SetBackgroundColor(color);
   m_gui_daq_write_enabled->TGWindow::SetBackgroundColor(color);
   gClient->NeedRedraw(m_gui_run_start);
   gClient->NeedRedraw(m_gui_run_stop);
   gClient->NeedRedraw(m_gui_daq_label_config_file);
   gClient->NeedRedraw(m_gui_daq_label_pedestal_file);
   gClient->NeedRedraw(m_gui_daq_write_enabled);
}

void CGuiMainFrameOnline::gui_handle_run_type_select(Int_t sel)
{
   
   switch (sel) {
      case 0:
         m_eb->get_daq_client()->set_run_type(runModePhysics);
           m_gui_type_physics->SetEnabled(true);
         break;
      case 1:
         m_eb->get_daq_client()->set_run_type(runModePedestal);
           m_gui_type_physics->SetEnabled(false);
           m_gui_type_physics->Select(0);

         break;
         
      default:
         break;
   }
}

void CGuiMainFrameOnline::gui_handle_max_events()
{
  
   if ( m_eb )
     {	
       if(m_gui_daq_max_event_enabled->IsOn())
	 {
		m_gui_evt_counter->SetEnabled(false);

	 }
       else 
	 {
	   m_gui_evt_counter->SetEnabled(true);
	 }
     }
  else
		std::cout << "not available in offline mode" << std::endl;
}
   

void CGuiMainFrameOnline::gui_handle_run_start()
{
    
    if (ConfigRun::GetInstance()->IsComplete()==false){
        
        
      
        
            fMain = new TGTransientFrame(gClient->GetRoot(), m_mainframe, 10, 10, kMainFrame | kVerticalFrame);
            
            fMain->CenterOnParent();

            fMain->Connect("CloseWindow()", "browser::CGuiMainFrameOnline", this, "CloseWindow_transient()");
            //fMain->DontCallClose();
            fMain->SetCleanup(kDeepCleanup);
            
        
        TGVerticalFrame* vertical = new TGVerticalFrame(fMain); {
            
            TGLabel* allert  = new TGLabel(vertical, "No detector setup has been chosen yet. \n Please configure one or set a dummy detector. Then PRESS START again");
            
            
            vertical->AddFrame(allert,  new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX, 10, 10, 10, 11));
            
            
            TGHorizontalFrame* group2frame = new TGHorizontalFrame(vertical, 130, 30, kHorizontalFrame);
                {
        
                TGTextButton* dummy_det = new TGTextButton(group2frame,"Dummy detector");
                group2frame->AddFrame(dummy_det, new TGLayoutHints(kLHintsCenterX,3,3,3,3));
                
                dummy_det->Connect("Clicked()", "browser::CGuiMainFrameOnline", this, "gui_dummy_det_config()");
                
            }
            
            vertical->AddFrame(group2frame,  new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX, 0, 0, 0, 0));
            

            }
            fMain->AddFrame(vertical);
            
            fMain->SetWindowName("Error");
            fMain->MapSubwindows();
            
            fMain->MapWindow();
            fMain->Resize(450, 80);
            
            
        }

    else {
        
        std::stringstream message2;
        message2 << "Run is starting with setup ID: " << ConfigRun::GetInstance()->GetSetup();

        fMain_summary = new TGTransientFrame(gClient->GetRoot(), m_mainframe, 10, 10, kMainFrame | kVerticalFrame);
        fMain_summary->CenterOnParent();
        
        fMain_summary->Connect("CloseWindow()", "browser::CGuiMainFrameOnline", this, "CloseWindow_summary()");
        fMain_summary->DontCallClose();
        fMain_summary->SetCleanup(kDeepCleanup);
        
        
        TGVerticalFrame* vertical = new TGVerticalFrame(fMain_summary); {
            
            TGLabel* allert  = new TGLabel(vertical, message2.str().c_str());
            
            
            vertical->AddFrame(allert,  new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX, 10, 10, 10, 11));
            
            
            TGHorizontalFrame* group2frame = new TGHorizontalFrame(vertical, 130, 30, kHorizontalFrame);
            {
                
                
                start_run = new TGTextButton(group2frame,"START");
                group2frame->AddFrame(start_run, new TGLayoutHints(kLHintsCenterX,3,3,3,3));
                
                start_run->Connect("Clicked()", "browser::CGuiMainFrameOnline", this, "gui_handle_run_start_effective()");
                
                exit = new TGTextButton(group2frame,"STOP");
                group2frame->AddFrame(exit, new TGLayoutHints(kLHintsCenterX,3,3,3,3));
                
                exit->Connect("Clicked()", "browser::CGuiMainFrameOnline", this, "CloseWindow_summary()");
                
            }
            
            vertical->AddFrame(group2frame,  new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX, 10, 10, 10, 1));
            
            
        }
        fMain_summary->AddFrame(vertical);
        
        fMain_summary->SetWindowName("Setup ID");
        fMain_summary->MapSubwindows();
        
        fMain_summary->MapWindow();
        fMain_summary->Resize(200, 80);
        start_from_user=true;
    
    }
   }

void CGuiMainFrameOnline::gui_handle_run_start_effective()
{

    if(start_from_user)
        CloseWindow_summary();
    run_is_ongoing = true;
    
    

    //retrieve the working dir
   size_t sz = m_config->get_working_path().string().size();
   char* cwd = new char[sz+1];
   strncpy(cwd, m_config->get_working_path().string().c_str(), sz);
   cwd[sz] = '\0';

   TGFileInfo file_info;


   file_info.fIniDir    = cwd;


    std::stringstream ss3;
    ss3 << "Setup Configuration:  " << ConfigRun::GetInstance()->GetSetup();
    m_gui_daq_configuration_label->SetText(ss3.str().c_str());
    
    boost::posix_time::ptime m_run_time = boost::posix_time::second_clock::local_time();
    date_start = boost::gregorian::to_iso_string(m_run_time.date());
    time_start = boost::posix_time::to_iso_string(m_run_time.time_of_day());
    




    std::stringstream ss2;
    ss2.str("");
    m_gui_statusbar->SetText(ss2.str().c_str(), 1);
    m_is_stopping = false;
    gui_reset_colors();
    
    m_gui_run_start->SetEnabled(false);
    m_gui_daq_load_config_file->SetEnabled(false);
    m_gui_daq_load_pedestal_file->SetEnabled(false);
    
    ULong_t color;
    gClient->GetColorByName("red", color);
    
    
    //send run + runtype
    gui_handle_run_type_select(m_gui_run_type->GetSelected());
    
    bool error = false;
    
    
    if(!m_eb->get_daq_client()->send_write (m_gui_daq_write_enabled->IsOn())) {
        //error failed set write
        error = true;
        m_gui_daq_write_enabled->TGWindow::SetBackgroundColor(color);
    }
    else if(!m_eb->get_daq_client()->send_start()) {
        //error failed start
        error = true;
        m_gui_run_start->TGWindow::SetBackgroundColor(color);
    }
    
    if(error) {
        //error failed start
        m_gui_run_start->TGWindow::SetBackgroundColor(color);
        m_gui_run_start->SetEnabled(true);
        m_gui_daq_load_config_file->SetEnabled(true);
        m_gui_daq_load_pedestal_file->SetEnabled(true);
        run_is_ongoing = false;
    }
    else {
        ULong_t green;
        gClient->GetColorByName("green", green);
        m_gui_run_start->TGWindow::SetBackgroundColor(green);
        stop_clicked=true;
        run_is_ongoing = true;
        if (m_gui_daq_write_enabled->IsOn()){
            gui_handle_daq_set_write_enable();
            if(m_gui_daq_mysql->IsOn() &&  CShmemReader::run_name!= "no writing" )
            {
                // I should check wheter is present or not in the database
                overload_mysql_comment="";
                overload_mysql_name="";
                
                
                
                //     TSQLServer* serv = TSQLServer::Connect("mysql://137.138.179.76/MMdatabase", "root", "root");
                
                sleep(0.1);
                std::stringstream cmd;
                cmd <<"mysql://"<<ConfigDatabase::GetInstance()->GetIp()<<"/"<<ConfigDatabase::GetInstance()->GetDatabaseName();
                
                TSQLServer* serv = TSQLServer::Connect(cmd.str().c_str(), ConfigDatabase::GetInstance()->GetUsername().c_str(), ConfigDatabase::GetInstance()->GetPassword().c_str());
                if (! serv) std::cout<<"------- Error connecting to the database. Contact the Server administrator ------- "<<std::endl;
                else {
                    std::stringstream ss;
                    ss << "SELECT * FROM "<<ConfigDatabase::GetInstance()->GetRunTableName().c_str() << " WHERE RunNumber = " << CShmemReader::run_filename;
                    
                    usleep(5000);
                    
                    TSQLStatement* stmt = serv->Statement(ss.str().c_str(), 4000);
                    
                    if (stmt->Process()) {
                        stmt->StoreResult();
                        if (stmt->GetNumFields()>0){
                            while (stmt->NextResultRow()) {
                                int id1 = stmt->GetDouble(0);
                                std::cout << "id1:  " << id1<< std::endl;
                                // I found another with the same run number
                                std::stringstream stream_name;
                                boost::posix_time::ptime m_run_time = boost::posix_time::second_clock::local_time();
                                stream_name << CShmemReader::run_name << boost::posix_time::to_iso_string(m_run_time.time_of_day());
                                
                                overload_mysql_name = stream_name.str();
                                
                                std::stringstream stream_comment;
                                
                                FILE* pipe = popen("uname -n", "r");
                                if (!pipe)
                                    std::cout << "ERROR" << std::endl;
                                char buffer[128];
                                std::string result = "";
                                while(!feof(pipe)) {
                                    if(fgets(buffer, 128, pipe) != NULL)
                                        result += buffer;
                                }
                                pclose(pipe);
                                
                                stream_comment << "The Original Run number was already present. The File has been generated in " << result;

                                overload_mysql_comment = stream_comment.str();
                                
                                std::stringstream message;
                                message << "The Run Number is already present in the database. Contact the expert. In the database the Run Number will be: " << overload_mysql_name;
                                
                                TGMsgBox* message_box = new TGMsgBox (gClient->GetRoot(), 0, "Run number already present", message.str().c_str(),0, kMBOk);
                                
                            }
                        }
                    }
                }
                //
            }
        }
        
        
        
    }
    // set write on or off
    save_gui_info_run();
    if(!m_gui_daq_max_event_enabled->IsOn()) m_gui_run_start->SetEnabled(false);
}

void CGuiMainFrameOnline::gui_handle_run_stop()
{
   gui_reset_colors();
    std::stringstream file_path;
    file_path << m_config->get_working_path().string() << "/gui_run_info.txt";
    remove(file_path.str().c_str());


   //send stop
   bool error = false;

   if(!m_eb->get_daq_client()->send_stop()) {
      error = true;
   }
   
   if(error) {
      //error failed start
      ULong_t color;
      gClient->GetColorByName("red", color);
      m_gui_run_stop->TGWindow::SetBackgroundColor(color);
   }
  // else {
      run_is_ongoing=false;
      m_gui_run_start->SetEnabled(true);
      m_gui_daq_load_config_file->SetEnabled(true);
      m_gui_daq_load_pedestal_file->SetEnabled(true);
      //m_gui_daq_max_event_enabled->SetEnabled(true);
    
    
    boost::posix_time::ptime m_run_time = boost::posix_time::second_clock::local_time();
    date_stop = boost::gregorian::to_iso_string(m_run_time.date());
    time_stop = boost::posix_time::to_iso_string(m_run_time.time_of_day());
    
    if (m_gui_daq_write_enabled->IsOn()){

    if(m_gui_daq_mysql->IsOn() && stop_clicked==true)
    {
        stop_clicked=false;
        write_to_database();
    }
    else if ( m_gui_daq_mysql->IsOn() == false && stop_clicked==true ){
        
        write_to_file();
    }
    
 //  }
    }
   
}

void CGuiMainFrameOnline::gui_handle_run_configure()
{
   
   m_eb->get_daq_client()->send_configure(m_gui_daq_label_config_file->GetText()->Data());

   //if stopped
   
   //send configure
   
   //wait for msg id change / or condition?
   // apply new
   
}

void CGuiMainFrameOnline::save_gui_info_run()
{
sleep(1);



    std::stringstream file_path;
    file_path << m_config->get_working_path().string() << "/gui_run_info.txt";
    std::ofstream gui_info;
    gui_info.open(file_path.str().c_str());





    gui_info << m_gui_run_type->GetSelected() << std::endl;


    gui_info << m_gui_daq_write_enabled->IsOn()  << std::endl;


    gui_info << m_gui_daq_mysql->IsOn()  << std::endl;
    gui_info << m_gui_daq_max_event_enabled->IsOn()  << std::endl;
    gui_info << m_gui_evt_counter->GetNumber()  << std::endl;

//Co pilot?
   // gui_info << m_gui_co_pilot->IsOn()  << std::endl;
    
//    if (*(m_gui_daq_label_pedestal_file->GetText()) =="")
//        gui_info << "No_entry" << std::endl;
//    else
//        gui_info << TString(*(m_gui_daq_label_pedestal_file->GetText()))  << std::endl;

    std::stringstream  stream_config;
    stream_config << *(m_gui_daq_label_config_file->GetText());

    if(stream_config.str()!="")
        gui_info << stream_config.str() << std::endl;
    else
        gui_info << "NO_entry" << std::endl;


    std::stringstream  ss;
    ss << *(m_gui_daq_label_pedestal_file->GetText());

    if(ss.str()!="")
        gui_info << ss.str() << std::endl;
    else
        gui_info << "NO_entry" << std::endl;
    
    gui_info << ConfigRun::GetInstance()->GetSetup() << std::endl;
    
    gui_info << m_gui_options_zerofactor->GetNumber()  << std::endl;

    std::stringstream  ss1;
    ss1 << (m_gui_comment_edit->GetText());

    if(ss1.str()!="")
        gui_info << ss1.str() << std::endl;
    else
        gui_info << "NO_entry" << std::endl;



    gui_info << m_gui_type_physics->GetSelected()  << std::endl;
    
    gui_info << date_start  << std::endl; 
    
    gui_info << time_start  << std::endl;

    gui_info << overload_mysql_name  << std::endl;

    gui_info << overload_mysql_comment  << std::endl;

    
    gui_info.close();

}



void CGuiMainFrameOnline::gui_dummy_det_config()
{
    fMain->CloseWindow();
  //  sleep(10);
    ConfigRun::GetInstance()->setSetup(-1);  
    ConfigRun::GetInstance()->check_complete();
    //gui_handle_run_start();

}

void CGuiMainFrameOnline::write_to_file()
{
    
    try {


       std::cout << "STARTING WRITING RUN REPORT" <<std::endl;
       std::stringstream ss;
        ss << CShmemReader::run_name;
        std::string ss_st = ss.str();
        if (ss_st == "no writing" ) ss_st = "-1";
       
        size_t curr_entry = m_eb->get_current_event_number();
        double zero_suppr = m_gui_options_zerofactor->GetNumber();
        

        std::ofstream myfile;
        myfile.open("Output.txt", std::ios::ate | std::ios::app);
        std::stringstream sql_stream;

        sql_stream << "INSERT INTO "<<ConfigDatabase::GetInstance()->GetRunTableName().c_str() << " (RunNumber, NumberOfEvents, DateStart, TimeStart, DateStop, TimeStop, ZeroSupp, Type_Run, Type_Physics, Pedestal, Configuration_ID, Comments) VALUES( "<< ss_st << "," << curr_entry << "," << date_start<<"," << time_start <<"," <<date_stop<<"," << time_stop << "," << zero_suppr << ",'" <<  m_gui_run_type->GetSelectedEntry()->GetTitle() << "','"  <<  m_gui_type_physics->GetSelectedEntry()->GetTitle() << "','" <<  pedestal_number   << "'," << ConfigRun::GetInstance()->GetSetup() << ",'" <<  m_gui_comment_edit->GetText() << "')";

        std::string to_write = sql_stream.str();
        myfile<< to_write << "\n";
        myfile.close();
        
    }
    
    catch (int e)
    {
        std::cout << "An exception occurred. Exception Nr. " << e << '\n';
    }
    


    
}


void CGuiMainFrameOnline::gui_database_config()
{
    
    ConfigDatabase* create_config = ConfigDatabase::GetInstance();
    sleep(.1);
    
    
    fMain_database = new TGTransientFrame(gClient->GetRoot(), m_mainframe, 10, 10, kMainFrame | kVerticalFrame);
    fMain_database->SetCleanup(kDeepCleanup);

    fMain_database->Connect("CloseWindow()", "browser::CGuiMainFrameOnline", this, "CloseWindow_transient_database()");

  
    TGGroupFrame* group3frame = new TGGroupFrame(fMain_database, "IP", kLHintsCenterX);
    {
        
        database_ip = new TGTextEntry(group3frame);
        group3frame->AddFrame(database_ip, new TGLayoutHints(kLHintsCenterX ,10,10,10,10));
        database_ip->Resize(150, 20);
        database_ip->SetText(ConfigDatabase::GetInstance()->GetIp().c_str());
        
    }
    fMain_database->AddFrame(group3frame, new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2));

    
    TGGroupFrame* group2frame = new TGGroupFrame(fMain_database, "Username", kLHintsCenterX);
    {
        
        database_username = new TGTextEntry(group2frame);
        group2frame->AddFrame(database_username, new TGLayoutHints(kLHintsCenterX,10,10,10,10));
        database_username->Resize(150, 20);
        database_username->SetText(ConfigDatabase::GetInstance()->GetUsername().c_str());

    }
    fMain_database->AddFrame(group2frame, new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2));

    
    TGGroupFrame* group1frame = new TGGroupFrame(fMain_database, "Password", kLHintsCenterX);
    {
        
        database_password = new TGTextEntry(group1frame);
        group1frame->AddFrame(database_password, new TGLayoutHints(kLHintsCenterX,10,10,10,10));
        database_password->Resize(150, 20);
        database_password->SetText(ConfigDatabase::GetInstance()->GetPassword().c_str());

    }
    fMain_database->AddFrame(group1frame, new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2));

    
    TGGroupFrame* group4frame = new TGGroupFrame(fMain_database, "Name of database", kLHintsCenterX);
    {
        
        database_name = new TGTextEntry(group4frame);
        group4frame->AddFrame(database_name, new TGLayoutHints(kLHintsCenterX,10,10,10,10));
        database_name->Resize(150, 20);
        database_name->SetText(ConfigDatabase::GetInstance()->GetDatabaseName().c_str());

    }
    fMain_database->AddFrame(group4frame, new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2));
    
    TGGroupFrame* group5frame = new TGGroupFrame(fMain_database, "Name of Configuration Table", kLHintsCenterX);
    {
        
        configuration_table_name = new TGTextEntry(group5frame);
        group5frame->AddFrame(configuration_table_name, new TGLayoutHints(kLHintsCenterX,10,10,10,10));
        configuration_table_name->Resize(150, 20);
        configuration_table_name->SetText(ConfigDatabase::GetInstance()->GetConfigurationTableName().c_str());
        
    }
    fMain_database->AddFrame(group5frame, new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2));
    
    TGGroupFrame* group6frame = new TGGroupFrame(fMain_database, "Name of Run Table", kLHintsCenterX);
    {
        
        run_table_name = new TGTextEntry(group6frame);
        group6frame->AddFrame(run_table_name, new TGLayoutHints(kLHintsCenterX,10,10,10,10));
        run_table_name->Resize(150, 20);
        run_table_name->SetText(ConfigDatabase::GetInstance()->GetRunTableName().c_str());
        
    }
    fMain_database->AddFrame(group6frame, new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2));

    TGTextButton* m_save_config_database = new TGTextButton(fMain_database, "&SAVE for Single session");
    m_save_config_database->Resize(110, 22);
    m_save_config_database->Connect("Clicked()", "browser::CGuiMainFrameOnline", this, "save_database_settings()");
    
    fMain_database->AddFrame(m_save_config_database, new TGLayoutHints( kLHintsTop | kLHintsExpandX, 10, 10, 1, 1));

    
    TGTextButton* m_save_config_database_to_file = new TGTextButton(fMain_database, "&SAVE TO FILE");
    m_save_config_database_to_file->Resize(110, 22);
    m_save_config_database_to_file->Connect("Clicked()", "browser::CGuiMainFrameOnline", this, "save_database_settings_to_file()");
    
    fMain_database->AddFrame(m_save_config_database_to_file, new TGLayoutHints( kLHintsTop | kLHintsExpandX, 10, 10, 1, 1));
    
    TGTextButton* m_new_database = new TGTextButton(fMain_database, "&NEW Tables");
    m_new_database->Resize(110, 22);
    m_new_database->Connect("Clicked()", "browser::CGuiMainFrameOnline", this, "create_new_database()");
    
    fMain_database->AddFrame(m_new_database, new TGLayoutHints( kLHintsTop | kLHintsExpandX, 10, 10, 1, 1));
    
    
    fMain_database->SetWindowName("Database Configuration");
    fMain_database->MapSubwindows();
    
    fMain_database->MapWindow();
    fMain_database->Resize(300, 600);
    
}

void CGuiMainFrameOnline::CloseWindow_transient_database()
{
    
    
    fMain_database->CloseWindow();
    
}

void CGuiMainFrameOnline::CloseWindow_summary()
{
    
    
    fMain_summary->CloseWindow();
    
}

void CGuiMainFrameOnline::create_new_database()
{
    
    
    fMain_new_database = new TGTransientFrame(gClient->GetRoot(), m_mainframe, 10, 10, kMainFrame | kVerticalFrame);
    fMain_new_database->SetCleanup(kDeepCleanup);
    
    
    TGGroupFrame* group1frame = new TGGroupFrame(fMain_new_database, "Name of Configurations table", kLHintsCenterX);
    {
        
        m_database_new_config = new TGTextEntry(group1frame);
        group1frame->AddFrame(m_database_new_config, new TGLayoutHints(kLHintsCenterX ,10,10,10,10));
        m_database_new_config->Resize(150, 20);
        
    }
    
    fMain_new_database->AddFrame(group1frame, new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2));

    
    TGGroupFrame* group2frame = new TGGroupFrame(fMain_new_database, "Name of Runs table", kLHintsCenterX);
    {
        
        m_database_new_runs = new TGTextEntry(group2frame);
        group2frame->AddFrame(m_database_new_runs, new TGLayoutHints(kLHintsCenterX ,10,10,10,10));
        m_database_new_runs->Resize(150, 20);
        
    }
    
    fMain_new_database->AddFrame(group2frame, new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2));

    
    TGTextButton* m_save_config_new_database = new TGTextButton(fMain_new_database, "&SAVE and CREATE");
    m_save_config_new_database->Resize(110, 22);
    m_save_config_new_database->Connect("Clicked()", "browser::CGuiMainFrameOnline", this, "save_new_database()");
    
    fMain_new_database->AddFrame(m_save_config_new_database, new TGLayoutHints( kLHintsTop | kLHintsExpandX, 10, 10, 1, 1));
    
    
    fMain_new_database->SetWindowName("NEW tables");
    fMain_new_database->MapSubwindows();
    
    fMain_new_database->MapWindow();
    
    fMain_new_database->Resize(300, 200);

    
}

void CGuiMainFrameOnline::save_new_database()
{

    std::stringstream cmd;
    
    cmd <<"mysql://"<<ConfigDatabase::GetInstance()->GetIp()<<"/"<<ConfigDatabase::GetInstance()->GetDatabaseName();
    
    TSQLServer* serv = TSQLServer::Connect(cmd.str().c_str(), ConfigDatabase::GetInstance()->GetUsername().c_str(), ConfigDatabase::GetInstance()->GetPassword().c_str());
    if (! serv) std::cout<<"------- Error connecting to the database. Contact the Server administrator ------- "<<std::endl;
    else{
    
    
        std::stringstream ss;
        ss << "CREATE TABLE "<< m_database_new_config->GetText() << " SELECT * FROM " << ConfigDatabase::GetInstance()->GetConfigurationTableName() << " WHERE 1=0 ";
        
        std::cout << ss.str() << std::endl;
        
        std::stringstream s1;

        s1 << "CREATE TABLE "<< m_database_new_runs->GetText() << " SELECT * FROM " << ConfigDatabase::GetInstance()->GetRunTableName() << " WHERE 1=0 ";
        std::cout << s1.str() << std::endl;

        
        usleep(500);
        
        serv->Exec(ss.str().c_str());
        

        usleep(500);

        serv->Exec(s1.str().c_str());

        
        std::stringstream s2; //add unique key
        
        s2 << "ALTER TABLE " << m_database_new_config->GetText() << " ADD UNIQUE (ID)";
        serv->Exec(s2.str().c_str());
        s2.str(std::string());

        s2 << "ALTER TABLE " << m_database_new_config->GetText() << " CHANGE ID ID INT( 11 ) NOT NULL AUTO_INCREMENT" ;
        serv->Exec(s2.str().c_str());
        s2.str(std::string());

        s2 << "ALTER TABLE " << m_database_new_runs->GetText() << " ADD UNIQUE (ID)";
        serv->Exec(s2.str().c_str());
        s2.str(std::string());
        
        s2 << "ALTER TABLE " << m_database_new_runs->GetText() << " CHANGE ID ID INT( 11 ) NOT NULL AUTO_INCREMENT" ;
        serv->Exec(s2.str().c_str());
        s2.str(std::string());
        
    }
    
    configuration_table_name->SetText(m_database_new_config->GetText());
    run_table_name->SetText(m_database_new_runs->GetText() );
    
    fMain_new_database->CloseWindow();
}



void CGuiMainFrameOnline::save_database_settings()
{
    
    ConfigDatabase::GetInstance()->SetDatabaseName(database_name->GetText());
    ConfigDatabase::GetInstance()->SetUsername(database_username->GetText());
    ConfigDatabase::GetInstance()->SetIp(database_ip->GetText());
    ConfigDatabase::GetInstance()->SetPassword(database_password->GetText());
    ConfigDatabase::GetInstance()->SetRunTableName(run_table_name->GetText());
    ConfigDatabase::GetInstance()->SetConfigurationTableName(configuration_table_name->GetText());
    
    CloseWindow_transient_database();
}

void CGuiMainFrameOnline::save_database_settings_to_file()
{
    
    ConfigDatabase::GetInstance()->SetDatabaseName(database_name->GetText());
    ConfigDatabase::GetInstance()->SetUsername(database_username->GetText());
    ConfigDatabase::GetInstance()->SetIp(database_ip->GetText());
    ConfigDatabase::GetInstance()->SetPassword(database_password->GetText());
    ConfigDatabase::GetInstance()->SetRunTableName(run_table_name->GetText());
    ConfigDatabase::GetInstance()->SetConfigurationTableName(configuration_table_name->GetText());
    
    ConfigDatabase::GetInstance()->SaveToFile();
    
    CloseWindow_transient_database();
}

void CGuiMainFrameOnline::write_to_database()
{
    
    try {
   std::cout << m_gui_type_physics->GetSelectedEntry()->GetTitle() << std::endl;
      std::cout << "STARTING WRITING RUN REPORT" <<std::endl;
    std::stringstream ss;

        if(overload_mysql_name == "")
        ss << CShmemReader::run_name;
        else
        ss << overload_mysql_name;
 
        std::stringstream ss_comment;
        if(overload_mysql_comment == "")
            ss_comment << m_gui_comment_edit->GetText();
        else
            ss_comment << m_gui_comment_edit->GetText() << " " << overload_mysql_comment;

        std::string ss_st_comment = ss_comment.str();


    
    std::string ss_st = ss.str();
    if (ss_st == "no writing" ) ss_st = "-1";
        
    size_t curr_entry = m_eb->get_current_event_number();
    double zero_suppr = m_gui_options_zerofactor->GetNumber();
        
        std::stringstream cmd;
        cmd <<"mysql://"<<ConfigDatabase::GetInstance()->GetIp()<<"/"<<ConfigDatabase::GetInstance()->GetDatabaseName();
        
        TSQLServer* serv = TSQLServer::Connect(cmd.str().c_str(), ConfigDatabase::GetInstance()->GetUsername().c_str(), ConfigDatabase::GetInstance()->GetPassword().c_str());
        if (! serv) std::cout<< "------- Error connecting to the database. Contact the Server administrator ------- "<<std::endl;
    
        else {
        
    std::stringstream sql_stream;
    
        sql_stream << "INSERT INTO "<<ConfigDatabase::GetInstance()->GetRunTableName().c_str() << " (RunNumber, NumberOfEvents, DateStart, TimeStart, DateStop, TimeStop, ZeroSupp, Type_Run, Type_Physics, Pedestal, Configuration_ID, Comments) VALUES( "<< ss_st << "," << curr_entry << "," << date_start<<"," << time_start <<"," <<date_stop<<"," << time_stop << "," << zero_suppr << ",'" <<  m_gui_run_type->GetSelectedEntry()->GetTitle() << "','"  <<  m_gui_type_physics->GetSelectedEntry()->GetTitle() << "','" <<  pedestal_number   << "'," << ConfigRun::GetInstance()->GetSetup() << ",'" << ss_st_comment  << "')";
    
        std::cout << sql_stream.str() << std::endl;
        
    TSQLResult* uot = serv->Query(sql_stream.str().c_str());
        }
    }
    
    catch (int e)
    {
        std::cout << "An exception occurred. Exception Nr. " << e << '\n';
    }
    
    
}

void CGuiMainFrameOnline::gui_handle_daq_load_config_file()
{
   
   const char *filetypes[] = {
      "XML",   "*.xml",
      "All files",     "*",
      0,               0 };

   int ftype = 0; // default selection

   size_t sz = m_config->get_working_path().string().size();
   char* cwd = new char[sz+1];
   strncpy(cwd, m_config->get_working_path().string().c_str(), sz);
   cwd[sz] = '\0';

   TGFileInfo file_info;
   file_info.fFileTypes = filetypes;
   file_info.fFileTypeIdx = ftype;
   file_info.fIniDir    = cwd;
   new TGFileDialog(gClient->GetRoot(), m_mainframe, kFDOpen, &file_info); // do not delete
   
   
   if (!file_info.fFilename) {
      m_gui_daq_label_config_file->SetText("");
      m_gui_top_frame->Layout();
      return;
   }
   
   if (!bfs::exists(file_info.fIniDir) || !bfs::is_directory(file_info.fIniDir)) {
      std::cout << "WARNING: bad input path: " + std::string(file_info.fIniDir) << std::endl;
      m_gui_daq_label_config_file->SetText("");
      m_gui_top_frame->Layout();
      return;
   }
   if (!bfs::exists(file_info.fFilename) || !bfs::is_regular(file_info.fFilename)) {
      std::cout << "WARNING: bad input file: " + std::string(file_info.fFilename) << std::endl;
      m_gui_daq_label_config_file->SetText("");
      m_gui_top_frame->Layout();
      return;
   }
   
   //ok here
   m_gui_daq_label_config_file->SetText(file_info.fFilename);
   m_gui_top_frame->Layout();

    
   gui_reset_colors();
   //send stop

   bool error = false;
   
   if(!m_eb->get_daq_client()->send_configure(file_info.fFilename)) { 
      error = true;
   }
   ULong_t color;
   if(error) {
      //error failed start
      gClient->GetColorByName("red", color);
      m_gui_daq_label_config_file->TGWindow::SetBackgroundColor(color);
   }
   else {
     // ULong_t color;
      gClient->GetColorByName("green", color);
      m_gui_daq_label_config_file->TGWindow::SetBackgroundColor(color);
   }
   
   //ok here

   
   // ULong_t color;
   //read pedestal file from DAQ
   // std::string config_read;
   // if(m_eb->get_daq_client()->read_daq_configuration_file_name(config_read)) {
   //   OK read
   //  
    //  TODO: check same filename file_info.fFilename == config_read!!!!
      // gClient->GetColorByName("green", color);
   // }
   // else {
   //   ERROR read
      // gClient->GetColorByName("red", color);
   // }
   m_gui_daq_label_config_file->TGWindow::SetBackgroundColor(color);
  // m_gui_daq_label_config_file->SetText(config_read.c_str());
   m_gui_top_frame->Layout();

// working dir 



}

void CGuiMainFrameOnline::gui_handle_daq_load_pedestals_file()
{
   const char *filetypes[] = {
      "ROOT",   "*.root",
      "All files",     "*",
      0,               0 };
   
   int ftype = 0; // default selection
   size_t sz = m_config->get_working_path().string().size();
   char* cwd = new char[sz+1];
   strncpy(cwd, m_config->get_working_path().string().c_str(), sz);
   cwd[sz] = '\0';

   TGFileInfo file_info;
   file_info.fFileTypes = filetypes;
   file_info.fFileTypeIdx = ftype;
   file_info.fIniDir    = cwd;
   new TGFileDialog(gClient->GetRoot(), m_mainframe, kFDOpen, &file_info); // do not delete
   if (!file_info.fFilename) {
      m_gui_daq_label_pedestal_file->SetText("");
      m_gui_top_frame->Layout();
      return;
   }
   
   if (!bfs::exists(file_info.fIniDir) || !bfs::is_directory(file_info.fIniDir)) {
      std::cout << "WARNING: bad input path: " + std::string(file_info.fIniDir) << std::endl;
      m_gui_daq_label_pedestal_file->SetText("");
      m_gui_top_frame->Layout();
      return;
   }
   if (!bfs::exists(file_info.fFilename) || !bfs::is_regular(file_info.fFilename)) {
      std::cout << "WARNING: bad input file: " + std::string(file_info.fFilename) << std::endl;
      m_gui_daq_label_pedestal_file->SetText("");
      m_gui_top_frame->Layout();
      return;
   }
   
   //ok here
    


    boost::filesystem::path my_path( file_info.fFilename );
    pedestal_number = boost::filesystem::basename (my_path);
   m_gui_daq_label_pedestal_file->SetText(file_info.fFilename);
   m_gui_top_frame->Layout();
   //send this to the Pedestal tab
   m_eb->set_pedestal_filename(file_info.fFilename);
   m_eb->set_new_pedestal_flag(true);
   gui_reset_colors();
   //send stop
   bool error = false;
   


   if(!m_eb->get_daq_client()->send_pedestals(file_info.fFilename)) {
      error = true;
   }
   
   if(error) {
      //error failed start
      ULong_t color;
      gClient->GetColorByName("red", color);
      m_gui_daq_label_pedestal_file->TGWindow::SetBackgroundColor(color);
   }
  else {
     ULong_t color;
     gClient->GetColorByName("green", color); 
     m_gui_daq_label_pedestal_file->TGWindow::SetBackgroundColor(color);
  }
   
   //ok here
   
  //TODO: read pedestal file from DAQ
   // std::string pedestal_read;
   // if(m_eb->get_daq_client()->read_daq_pedestals_file_name(pedestal_read)) {
      //OK read
      // ULong_t color;
      // gClient->GetColorByName("green", color);
      // m_gui_daq_label_pedestal_file->TGWindow::SetBackgroundColor(color);
      // m_gui_daq_label_pedestal_file->SetText(pedestal_read.c_str());
      // m_gui_top_frame->Layout();
   // }
   
   
}

void CGuiMainFrameOnline::gui_handle_daq_set_write_enable()
{


   m_eb->get_daq_client()->send_write (m_gui_daq_write_enabled->IsOn());
 //  bool error = false;
 //  ULong_t color;
   //if(!m_eb->get_daq_client()->send_write (m_gui_daq_write_enabled->IsOn())) {
      //error failed set write
   //   error = true;
   //   gClient->GetColorByName("red", color);
  // }
  // else {
   //   gClient->GetColorByName("green", color);
  // }
   
  // m_gui_daq_write_enabled->TGWindow::SetBackgroundColor(color);
   
}


void CGuiMainFrameOnline::do_gui_handle_load_file()
{
//   std::cout << "----- CGuiMainFrameOnline::do_gui_handle_load_file() -----\n";

   {
      
      m_gui_tree_item = 0;
      clean_tree();

      {
         //      m_eb->open_file();
      }
      
      //    m_mainframe->reset_tree_selection();
      
      //GUIO update after file open
     // size_t curr_entry = 0; //m_eb->get_current_entries();
    //  size_t max_entry = m_eb->get_max_entry();
//      m_gui_current_entry->SetLimits( TGNumberFormat::kNELLimitMax,0,max_entry-1); //Values(0, max_entry);
//      m_gui_current_entry->SetNumber(curr_entry);
      m_gui_progressbar->Reset();
      
      std::stringstream ss;
      ss << m_config->get_current_file_name() << " (" << m_config->get_raw_tree_name() << ")";
      m_gui_statusbar->SetText(ss.str().c_str(), 0);
      m_gui_statusbar->SetText(m_config->get_current_file_dirname().c_str(), 3);
      
      //end tabs
   }
   clean_tree();
   update();
   
}


void CGuiMainFrameOnline::gui_set_zerofactor()//-//
{
    if ( m_eb )
    {
        std::cout << " Zero Suppression Factor was        "<< m_eb->get_daq_client()->get_zero_suppression_apv() << std::endl;
        m_eb->get_daq_client()->set_zero_suppression_apv(m_gui_options_zerofactor->GetNumber());
    }
    else
        std::cout << "Zero Suppression Factor: Not available in offline mode " << std::endl;
}

///timed execution of this function drives display
void CGuiMainFrameOnline::do_gui_handle_current_entry_set(Long_t )
{
//   std::cout << "----- CGuiMainFrameOnline::do_gui_handle_current_entry_set() -----\n";

   void* selected_detbase = 0;
   std::string selected_fullname;
   {
      if (m_gui_tree_item) {
         selected_detbase = m_gui_tree_item->GetUserData();
         selected_fullname = static_cast<CDetBase*>(selected_detbase)->get_full_name();
      }
   }
   m_eb->set_current_entry(0);
   
   m_eb->get_current_event_number();
   
   if (m_gui_view_pause_display->IsOn()) {
      gui_update_status_bar();
      return;
   }

   update();


   {
      if (!m_gui_tree_list) {
//         std::cout << "----- CGuiMainFrameOnline::do_gui_handle_current_entry_set()  m_gui_tree_list == NULL (update_tabs skipped) -----\n";
         return;
      }
      m_gui_tree_item = m_gui_tree_list->FindItemByObj(m_gui_tree_list->GetFirstItem(), selected_detbase);
      
      if(!m_gui_tree_item) {
         m_gui_tree_item = find_item_by_fullname(selected_fullname);
      }
      
      
      if (!m_gui_tree_item) {
//         std::cout << "----- CGuiMainFrameOnline::do_gui_handle_current_entry_set() no tree item selected (update_tabs skipped)-----\n";
         return;
      }
      
      m_gui_tree_list->SetSelected(m_gui_tree_item);
      m_gui_tree_list->HighlightItem(m_gui_tree_item);
      update_tabs(m_gui_tree_item);
   }
//   update();
//   update_tabs(m_gui_tree_item);
}

TGListTreeItem* CGuiMainFrameOnline::find_item_by_fullname(std::string const& fullname)
{
   if(!m_gui_tree_list) {
      return 0;
   }
   
   TGListTreeItem* item = m_gui_tree_list->GetFirstItem();
   
   while (item) {
      void* selected_detbase = item->GetUserData();
      CDetBase* base = static_cast<CDetBase*>(selected_detbase);
      
      if(base->get_full_name() == fullname) {
         return item;
      }
      TGListTreeItem* goodchild = 0;
      if((goodchild = find_item_by_fullname_descendants(fullname, item->GetFirstChild()))) {
         return goodchild;
      }
      
      item = item->GetNextSibling();
   }
   return 0;
}


TGListTreeItem* CGuiMainFrameOnline::find_item_by_fullname_descendants(std::string const& fullname, TGListTreeItem* child)
{
   
   TGListTreeItem* item = child;
   
   while (item) {
      void* selected_detbase = item->GetUserData();
      CDetBase* base = static_cast<CDetBase*>(selected_detbase);
      if(base->get_full_name() == fullname) {
         return item;
      }
      TGListTreeItem* goodchild = 0;
      if( (goodchild = find_item_by_fullname_descendants(fullname, item->GetFirstChild())) ) {
         return goodchild;
      }
      item = item->GetNextSibling();
   }
   return 0;
}


void CGuiMainFrameOnline::do_gui_handle_tree_click(TGListTreeItem* entry, int number)
{
   m_gui_tree_item = entry;
   do_update();

//   update_tabs(entry);
}

void CGuiMainFrameOnline::do_gui_handle_tree_check(TObject* , Bool_t )
{
   do_update();
//   update_tabs(m_gui_tree_item);
}


CDetBase* CGuiMainFrameOnline::do_get_tree_selected_user_data()
{

   if (m_gui_tree_item) {
      return static_cast<CDetBase*>(m_gui_tree_item->GetUserData());
   }
   return 0;
}


int CGuiMainFrameOnline::do_get_tree_checked_items(std::vector<TGListTreeItem*>& vec)
{

   int count = 0;
   TGListTreeItem* item = m_gui_tree_list->GetFirstItem();
   
   while (item) {
      if(item->IsChecked()) {
         ++count;
         //         std::cout << count << " checked is " << item->GetText() << std::endl;
         vec.push_back( item);
      }
      get_item_checked_descendants(item->GetFirstChild(), vec);
      item = item->GetNextSibling();
   }
   
   return count;
}

int CGuiMainFrameOnline::do_get_item_checked_descendants(TGListTreeItem* first_item, std::vector<TGListTreeItem*>& vec)
{

   int count = 0;
   TGListTreeItem* item = first_item; //->GetFirstChild();
   while (item) {
      if(item->IsChecked()) {
         ++count;
         //         std::cout << count << " checked is " << item->GetText() << std::endl;
         vec.push_back( item);
      }
      
      get_item_checked_descendants(item->GetFirstChild(), vec);
      
      item = item->GetNextSibling();
   }
   return count;
}


