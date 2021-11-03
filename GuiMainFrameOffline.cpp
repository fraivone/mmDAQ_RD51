//
//  GuiMainFrameOffline.cpp
//  eventbrowser
//
//  Created by Marcin Byszewski on 1/17/13.
//  Copyright (c) 2013 CERN - PH/ADE. All rights reserved.
//

#include "GuiMainFrameOffline.h"
#include "GuiTabStrip.h"
#include "GuiTabReadout.h"
#include "GuiTabPlane.h"
#include "GuiTabChamber.h"
#include "GuiTabDetector.h"
#include "GuiTabMainOptions.h"
#include "EventBrowser.h"
#include "DetDetector.h"



#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>

//#include <TGNumberEntryField.h>
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
#include <TGTextView.h>
//#include <TRootEmbeddedCanvas.h>
//#include <TCanvas.h>

#include <sstream>

using namespace browser;
namespace bfs = boost::filesystem;


CGuiMainFrameOffline::CGuiMainFrameOffline() :
CGuiMainFrame(this, 0),
m_eb(0),
m_mainframe(0),
m_gui_statusbar(0),
m_gui_progressbar(0),
m_gui_top_frame(0),
m_gui_bottom_frame(0),
m_gui_load_file(0),
m_gui_current_entry(0),
m_gui_tree_canvas(0),
m_gui_tree_viewport(0),
m_gui_tree_list(0),
m_gui_tree_item(0),
m_gui_pic_open(0),
m_gui_pic_close(0),
m_gui_dnd_rootcanvas(0),
m_gui_dnd_canvas(0),
m_gui_main_tabs(0),
m_tabs()
{ }


CGuiMainFrameOffline::CGuiMainFrameOffline(CEventBrowser* eb, CConfiguration* config) :
CGuiMainFrame(this, config),
m_eb(eb),
m_mainframe(0),
m_gui_statusbar(0),
m_gui_progressbar(0),
m_gui_top_frame(0),
m_gui_bottom_frame(0),
m_gui_load_file(0),
m_gui_current_entry(0),
m_gui_tree_canvas(0),
m_gui_tree_viewport(0),
m_gui_tree_list(0),
m_gui_tree_item(0),
m_gui_pic_open(0),
m_gui_pic_close(0),
m_gui_dnd_rootcanvas(0),
m_gui_dnd_canvas(0),
m_gui_main_tabs(0),
m_tabs()
{
   SetCleanup(kDeepCleanup);
   construct_window();
}


CGuiMainFrameOffline::~CGuiMainFrameOffline()
{
   Cleanup();
}

CGuiMainFrame* CGuiMainFrameOffline::do_get_gui_main_frame()
{
   return this;
}

void CloseWindow();

TGMainFrame*  CGuiMainFrameOffline::do_get_tg_main_frame()
{
   return m_mainframe;
}


/**
 construct all gui elements
 */
void CGuiMainFrameOffline::construct_window()
{
   m_mainframe = new TGMainFrame(gClient->GetRoot(), 10, 10, kMainFrame | kVerticalFrame);
   ////////////////////////////////////////////////////////////////////////////
   
   {
      //top control vertical frame
      m_gui_top_frame = new TGHorizontalFrame(m_mainframe, 650, 100, kHorizontalFrame);
      
      {
         //open file button
         m_gui_load_file = new TGTextButton(m_gui_top_frame, "&Open File");
         m_gui_load_file->Resize(110, 22);
         m_gui_load_file->Connect("Clicked()", "browser::CGuiMainFrameOffline", this, "gui_handle_load_file()");
         m_gui_top_frame->AddFrame(m_gui_load_file, new TGLayoutHints( kLHintsTop, 1, 1, 1, 1));
      }
      
      
      {
         // next, prev, quit, curr
         TGLabel* label_centry  = new TGLabel(m_gui_top_frame, "Current entry:");
         m_gui_top_frame->AddFrame(label_centry, new TGLayoutHints(kLHintsLeft | kLHintsTop , 5, 5, 5, 5));
         
         m_gui_current_entry = new TGNumberEntry(m_gui_top_frame, 0, 7,
                                                 m_id.GetUniqueID(),
                                                 TGNumberEntry::kNESInteger,
                                                 TGNumberFormat::kNEANonNegative);
         m_gui_current_entry->Connect("ValueSet(Long_t)",
                                      "browser::CGuiMainFrame",
                                      this,
                                      "gui_handle_current_entry_set(Long_t)");
         m_gui_top_frame->AddFrame(m_gui_current_entry,
                                   new TGLayoutHints(kLHintsLeft | kLHintsTop , 0, 0, 0, 0));
          
          
           TGLabel* label_event  = new TGLabel(m_gui_top_frame, "Current event:");
          m_gui_top_frame->AddFrame(label_event, new TGLayoutHints(kLHintsLeft | kLHintsTop , 5, 5, 5, 5));
          
          
          m_gui_current_event = new TGNumberEntryField(m_gui_top_frame, m_id.GetUniqueID(), 0, TGNumberEntryField::kNESInteger, TGNumberEntryField::kNEANonNegative, TGNumberEntryField::kNELLimitMin, 0.0,9e9 );//
          m_gui_current_event->Resize(65, 20);
          m_gui_top_frame->AddFrame(m_gui_current_event,
                                    new TGLayoutHints(kLHintsLeft | kLHintsTop , 0, 0, 0, 0));
          
      }
      
      
      
      
      m_mainframe->AddFrame(m_gui_top_frame, new TGLayoutHints(kLHintsExpandX , 1, 1, 1, 1));
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
            
            m_tabs.push_back(new CGuiTabStrip(this,   m_config, m_eb, m_gui_main_tabs, "Strip"));
            m_tabs.push_back(new CGuiTabReadout(this, m_config, m_eb, m_gui_main_tabs, "Readout"));
            m_tabs.push_back(new CGuiTabPlane(this,   m_config, m_eb, m_gui_main_tabs, "Layer"));
            m_tabs.push_back(new CGuiTabChamber(this, m_config, m_eb, m_gui_main_tabs, "Chamber"));
            m_tabs.push_back(new CGuiTabDetector(this, m_config, m_eb, m_gui_main_tabs, "Detector"));
            m_tabs.push_back(new CGuiTabMainOptions(this, m_config, m_eb, m_gui_main_tabs, "Main Options"));
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
      Int_t parts[] = {20, 20, 15, 45};
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
   m_mainframe->SetWindowName("Offline Event Browser");
   m_mainframe->MapSubwindows();
   m_mainframe->Resize(m_mainframe->GetDefaultSize());
   m_mainframe->MapWindow();
   m_mainframe->Resize(750, 650);
   m_mainframe->MoveResize(100, 0, (unsigned)(gClient->GetDisplayWidth()*0.7), (unsigned)(gClient->GetDisplayHeight()*0.7));
   m_mainframe->Connect("CloseWindow()", "browser::CGuiMainFrame", this, "CloseWindow()");
   
   update();
   for (int ii = 0; ii < m_gui_main_tabs->GetNumberOfTabs()-1; ++ii) {
      m_gui_main_tabs->GetTabTab(ii)->SetEnabled(false);
   }
   
}


void CGuiMainFrameOffline::do_update()
{
   std::stringstream ss;
   
   
   ss.str("");
   std::vector<size_t> curr_entry = m_eb->get_current_entry();
   size_t max_entry = m_eb->get_max_entry();
   ss << "Current entry = " ;
   BOOST_FOREACH(size_t v, curr_entry) {
      ss << v << ",";
   }
   
   ss << " / " << max_entry;
   m_gui_statusbar->SetText(ss.str().c_str(), 1);
   update_tree(m_eb->get_detector());
   //   clear_tabs();
   
}



void CGuiMainFrameOffline::update_tree( CDetDetector* det)
{
   //clean_tree();
   if (det) {
      det->gui_populate_tree(m_gui_tree_list, (TGListTreeItem*)0);
   }
   
}

void CGuiMainFrameOffline::clear_tabs()
{
   std::for_each(m_tabs.begin(), m_tabs.end(), boost::bind(&CGuiTab::clear_tab, _1) );
}

/**
 call detector element from selected tree item and update its gui tab
 */
void CGuiMainFrameOffline::update_tabs(TGListTreeItem* )
{
   for (int ii = 0; ii < m_gui_main_tabs->GetNumberOfTabs()-1; ++ii) {
      m_gui_main_tabs->GetTabTab(ii)->SetEnabled(false);
   }
   //update gui tabs
   if (m_gui_tree_item) {
      CDetBase* detbase = static_cast<CDetBase*>(m_gui_tree_item->GetUserData());
      int active = detbase->update_my_gui_tab(m_tabs);
      if (active >= 0) {
         m_gui_main_tabs->SetEnabled(active);
         m_gui_main_tabs->SetTab(active);
      }
      
   }
}


void CGuiMainFrameOffline::do_update_progress(double val)
{
   m_gui_progressbar->SetPosition(val*100.0);
   std::stringstream ss;
   ss << val;
   m_gui_statusbar->SetText(ss.str().c_str(), 0);
}


void CGuiMainFrameOffline::clean_tree()
{
   //clean tree
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


void CGuiMainFrameOffline::do_gui_handle_load_file()
{
   m_gui_tree_item = 0;
   
   {
      const char *filetypes[] = {
         "ROOT APV v.3",   "*.root",
         "ROOT APV raw",    "*.root",
         "ROOT processed",    "*.root",
         "All files",     "*",
         0,               0 };
      const char* tree_names[] = {
         "apv_raw",
         "raw",
         "raw_processed"
      };
      
      size_t sz = m_config->get_working_path().string().size();
      char* cwd = new char[sz+1];
      strncpy(cwd, m_config->get_working_path().string().c_str(), sz);
      cwd[sz] = '\0';
      
      // set default type selection in dialo box
      int ftype = 0; //"raw_apv"
      if (m_config->get_raw_tree_name() == "raw_processed") {
         ftype = 2;
      } else if (m_config->get_raw_tree_name() == "raw") {
         ftype = 3;
      }
      
      
      TGFileInfo file_info;
      file_info.fFileTypes = filetypes;
      file_info.fFileTypeIdx = ftype;
      file_info.fIniDir    = cwd;
      new TGFileDialog(gClient->GetRoot(), m_mainframe, kFDOpen, &file_info); // do not delete
      if (!file_info.fFilename) {
         return;
      }
      
      if (!bfs::exists(file_info.fIniDir) || !bfs::is_directory(file_info.fIniDir)) {
         std::cout << "WARNING: bad input path: " + std::string(file_info.fIniDir) << std::endl;
         return;
      }
      if (!bfs::exists(file_info.fFilename) || !bfs::is_regular(file_info.fFilename)) {
         std::cout << "WARNING: bad input file: " + std::string(file_info.fFilename) << std::endl;
         return;
      }
      
      m_config->set_working_path(file_info.fIniDir);
      m_config->set_current_file_path(file_info.fFilename);
      m_config->set_raw_tree_name(tree_names[file_info.fFileTypeIdx/2]);
      m_eb->open_file();
      
      //      m_tree_current_item = 0;
      //   std::cout << " CGuiTabDetConfig::gui_handle_load(): fname = " << file_info.fFilename << std::endl;
      //        CDetDetector& det = m_daq->get_detector();
      //        det.load_detector_configuration(file_info.fFilename);
   }
   
   //    m_mainframe->reset_tree_selection();
   
   //GUIO update after file open
   size_t curr_entry = 0; //m_eb->get_current_entries();
   size_t max_entry = m_eb->get_max_entry();
   m_gui_current_entry->SetLimits( TGNumberFormat::kNELLimitMax,0,max_entry-1); //Values(0, max_entry);
   m_gui_current_entry->SetNumber(curr_entry);
   m_gui_progressbar->Reset();
   
   std::stringstream ss;
   ss << m_config->get_current_file_name() << " (" << m_config->get_raw_tree_name() << ")";
   m_gui_statusbar->SetText(ss.str().c_str(), 0);
   m_gui_statusbar->SetText(m_config->get_current_file_dirname().c_str(), 3);
   
   //end tabs
   
   clean_tree();
   update();
   
   
   
}


void CGuiMainFrameOffline::do_gui_handle_current_entry_set(Long_t )
{
   void* selected_detbase = 0;
   if (m_gui_tree_item) {
      selected_detbase = m_gui_tree_item->GetUserData();
   }
    std::stringstream ss;
   m_eb->set_current_entry(m_gui_current_entry->GetNumber());

    ss <<m_eb->get_current_event_number();
    m_gui_current_event->SetText(ss.str().c_str());
    
   update();
   m_gui_tree_item = m_gui_tree_list->FindItemByObj(m_gui_tree_list->GetFirstItem(), selected_detbase);
   if (m_gui_tree_item) {
      m_gui_tree_list->SetSelected(m_gui_tree_item);
      m_gui_tree_list->HighlightItem(m_gui_tree_item);
      update_tabs(m_gui_tree_item);
   }
}

void CGuiMainFrameOffline::do_gui_handle_tree_click(TGListTreeItem* entry, int)
{
   m_gui_tree_item = entry;
   update_tabs(entry);
}

void CGuiMainFrameOffline::do_gui_handle_tree_check(TObject* , Bool_t )
{
   update_tabs(m_gui_tree_item);
}


CDetBase* CGuiMainFrameOffline::do_get_tree_selected_user_data()
{
   if (m_gui_tree_item) {
      return static_cast<CDetBase*>(m_gui_tree_item->GetUserData());
   }
   return 0;
}


int CGuiMainFrameOffline::do_get_tree_checked_items(std::vector<TGListTreeItem*>& vec)
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

int CGuiMainFrameOffline::do_get_item_checked_descendants(TGListTreeItem* first_item, std::vector<TGListTreeItem*>& vec)
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
