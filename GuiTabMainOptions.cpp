//
//  GuiTabMainOptions.cpp
//  browser
//
//  Created by Marcin Byszewski on 12.4.12.
//  Copyright (c) 2012 CERN - PH/ADE. All rights reserved.
//

#include "GuiTabMainOptions.h"
#include "Configuration.h"
#include "RootTreeVmm1Tac.h"
#include "SrsChannelId.h"
#include "VMM1Parameters.h"

#include "EventBrowser.h"
#include "EventBrowserOnline.h"
#include "DaqClientIpc.h"


#include <TGTab.h>
#include <TGButtonGroup.h>
#include <TGButton.h>
#include <TGNumberEntry.h>
#include <TString.h>
#include <TGFileDialog.h>
#include <TFile.h>


#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <functional>


namespace bfs = boost::filesystem;

using namespace browser;

CGuiTabMainOptions::CGuiTabMainOptions(CGuiBase* gui_parent, 
                                       CConfiguration* config, 
                                       CEventBrowser* eb, 
                                       TGTab* main_tabs,
                                       const std::string& name) :
CGuiTab(gui_parent, config, eb, main_tabs, name),
m_eb(dynamic_cast<CEventBrowserOnline*>(eb)),
m_gui_radio_raw(0),
m_gui_raw_source(),
m_gui_qfitting_show(0),
m_gui_clusters_show(0),
m_gui_load_threshold_file(0),
IDs() //--//
{
   construct_tab();
//      m_parent_tabs->GetTabTab(m_name.c_str())->SetEnabled(true);
}


CGuiTabMainOptions::~CGuiTabMainOptions()
{
   
}


void CGuiTabMainOptions::do_construct_tab()
{
   m_tab_frame->SetLayoutManager(new TGHorizontalLayout(m_tab_frame));

   
  
   {
      TGGroupFrame* group2frame = new TGGroupFrame(m_tab_frame, "Overlays", kVerticalFrame);
      {
         m_gui_qfitting_show = new TGCheckButton(group2frame, "Overlay fit results on strip data");
         group2frame->AddFrame(m_gui_qfitting_show , new TGLayoutHints(kLHintsLeft, 5, 5, 3, 4));
         m_gui_qfitting_show->Connect("Toggled(Bool_t)", "browser::CGuiTabMainOptions", this, "gui_handle_overlay_fitting(bool)");
         m_gui_clusters_show = new TGCheckButton(group2frame, "Overlay clusters on raw data");
         group2frame->AddFrame(m_gui_clusters_show , new TGLayoutHints(kLHintsLeft, 5, 5, 3, 4));
      }
      m_tab_frame->AddFrame(group2frame, new TGLayoutHints(kLHintsLeft, 2, 2, 2, 2));
   }
  
	
   {
      //top section
      TGGroupFrame* group1frame = new TGGroupFrame(m_tab_frame, "Thresholds Adjustment", kVerticalFrame);
      {
         {
            //open file button
            m_gui_load_threshold_file = new TGTextButton(group1frame, "&Load File");
            m_gui_load_threshold_file->Resize(110, 22);
            m_gui_load_threshold_file->Connect("Clicked()", "browser::CGuiTabMainOptions", this, "gui_handle_load_thresholds_file()");
            group1frame->AddFrame(m_gui_load_threshold_file, new TGLayoutHints( kLHintsTop, 1, 1, 1, 1));
         }
         {
            //clear thresholds button
            m_gui_load_threshold_file = new TGTextButton(group1frame, "&Clear");
            m_gui_load_threshold_file->Resize(110, 22);
            m_gui_load_threshold_file->Connect("Clicked()", "browser::CGuiTabMainOptions", this, "gui_handle_clear_thresholds_file()");
            group1frame->AddFrame(m_gui_load_threshold_file, new TGLayoutHints( kLHintsTop, 1, 1, 1, 1));
         }
      }
      m_tab_frame->AddFrame(group1frame, new TGLayoutHints(kLHintsLeft, 2, 2, 2, 2));
      
   }

   
   
}

void CGuiTabMainOptions::do_update(const CDetBase* /*entry*/)
{
   
}



void CGuiTabMainOptions::gui_handle_raw_selection(int bid) const
{
   TGButton* b = m_gui_radio_raw->GetButton(bid);
   TGRadioButton* rb = dynamic_cast<TGRadioButton*>(b);
   if (rb) {
      m_config->set_raw_tree_name(rb->GetString().Data());
   }
}

void CGuiTabMainOptions::gui_handle_overlay_fitting(bool val) const
{
   m_config->set_overlay_fitting(val);
}


void CGuiTabMainOptions::gui_handle_load_thresholds_file()
{

//   m_gui_tree_item = 0;
   
   {
      const char *filetypes[] = {
         "VMM1 TAC file",   "*.root",
         "Offset text file",   "*.txt",
         "All files",     "*",
         0,               0 };
      
      size_t sz = m_config->get_working_path().string().size();
      char* cwd = new char[sz+1];
      strncpy(cwd, m_config->get_working_path().string().c_str(), sz);
      cwd[sz] = '\0';
      int ftype = 0; //"raw_apv"

      
      TGFileInfo file_info;
      file_info.fFileTypes = filetypes;
      file_info.fFileTypeIdx = ftype;
      file_info.fIniDir    = cwd;
      new TGFileDialog(gClient->GetRoot(), m_tab_frame, kFDOpen, &file_info); // do not delete
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

      if (bfs::extension(file_info.fFilename) == ".root") {
          load_thresholds_root_file(file_info.fFilename);
      }
      else if (bfs::extension(file_info.fFilename) == ".txt") {
         load_thresholds_text_file(file_info.fFilename);
      }
      else {
         std::cout << "gui_handle_load_thresholds_file() unknown file extension" << std::endl;
      }
      
     
   }
   
}

void CGuiTabMainOptions::gui_handle_clear_thresholds_file()
{
   std::cout << "CGuiTabMainOptions::gui_handle_clear_thresholds_file() "  << std::endl;
 m_config->set_vmm1_offsets(std::map<CSrsChannelId, VMM1Parameters >() );

}



/*
 // Declaration of leaf types
 Int_t                board;         /// vmm1 board id = chip id 16-32
 std::vector<int>     *channel;      /// channel number 0-63
 Int_t                set_t_tac;     /// TAC set on chip
 std::vector<double>  *q_pedMean;    /// mean by measured neighbours to pulsed channel (info from K.N. 26/11/2012)
 std::vector<double>  *q_pedSigma;   /// sigma of ped by measured neighbours to pulsed channel (info from K.N. 26/11/2012)
 std::vector<double>  *q_gainSlope;  /// readout value vs pulser amplitude
 std::vector<double>  *q_gainIntercept; /// readout value vs pulser amplitude
 std::vector<double>  *set_q_gainFactor; /// (?) readout value vs pulser amplitude
 std::vector<double>  *t_tacSlope;   /// time form pdo to ns , which is x,y?
 std::vector<double>  *t_tacIntercept; /// time from tdo to ns , intercept at ?
 */


void CGuiTabMainOptions::load_thresholds_root_file(const std::string& filename)
{
   std::cout << "CGuiTabMainOptions::load_thresholds_root_file() " << filename << std::endl;
   
   std::map<CSrsChannelId, VMM1Parameters > vmm_tac_map;
   
   try {
      RootTreeVmm1Tac vmm_tac(filename.c_str());
      
      if (vmm_tac.GetEntries() == 0) {
         throw std::runtime_error("ERROR: load_thresholds_file(): TAC file tree is empty");
      }
   
      //read file
      int evt = 0;
      while (vmm_tac.GetEntry(evt++)) {
         double tac = vmm_tac.set_t_tac;
         int board_id = vmm_tac.board;
         size_t vecsize = vmm_tac.channel->size(); //64
         
         //TODO: \todo replace fecno by all of FEC numbers read from bnl_raw tree ?
         //identify VMM chip in our data (get FEC for VMMs)
         uint32_t fecno = 3;
         
         for (size_t ii = 0; ii < vecsize; ++ii) {
            int chno = (*vmm_tac.channel)[ii];
            double slope = (*vmm_tac.t_tacSlope)[ii];
            double offset = (*vmm_tac.t_tacIntercept)[ii];
            
            CSrsChannelId chid(fecno, board_id, chno);
            double qoffset = (*vmm_tac.q_pedMean)[ii];
            double qslope = 1.0;
            vmm_tac_map[chid] = VMM1Parameters(qoffset, qslope, tac, offset, slope);
         }
      
      }//while getentry()

      m_config->set_vmm1_offsets(vmm_tac_map);
      //  std::map<CSrsChannelId, VMM1Parameters > vmm_tac_map;

      //or set that in chips: TACstruct {qth, t_slope, t_interc, tac}
      
   } catch (std::runtime_error& re) {
      std::cout << "ERROR:CGuiTabMainOptions::load_thresholds_file: " << re.what() << std::endl;
      //throw;
   }
   

}


void CGuiTabMainOptions::load_thresholds_text_file(const std::string& filename)
{
   std::cout << "CGuiTabMainOptions::load_thresholds_text_file() " << filename << std::endl;

   struct stat stFileInfo;
   if (stat(filename.c_str(), &stFileInfo)) {
      std::stringstream ss;
      ss << "CGuiTabMainOptions::load_thresholds_text_file(): '"<< filename << "' does not exist.";
      throw std::runtime_error(ss.str().c_str());
   }
   
   std::ifstream file(filename.c_str(), std::ifstream::in);
   if (!file.is_open()) {
      std::stringstream ss;
      ss << "CGuiTabMainOptions::load_thresholds_text_file(): can't open '"<< filename << "'.";
      throw std::runtime_error(ss.str().c_str());
   }
   
   std::map<CSrsChannelId, VMM1Parameters > vmm_tac_map;
   
   int lineCounter = 0;
   char buff[256];
   while (file.getline(buff, 256)) {
      lineCounter += 1;
      std::string line(buff);
      //skip empty and comment lines
      boost::trim(line);
      if (boost::starts_with(line, "#") || line.empty()) {
         continue;
      }
      
      
      typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
      boost::char_separator<char> sep(", \t");
      tokenizer tokens(line, sep);
      tokenizer::iterator tok_iter = tokens.begin();
      
      std::string fec;
      std::string board;
      std::string channel;
      std::string qoffset;
      std::string qslope;
      std::string timetac;
      std::string toffset;
      std::string tslope;

      
      enum {state_fec, state_board, state_channel, state_qoffset, state_qslope, state_tac, state_toffset, state_tslope} parse_state;
      parse_state = state_fec;
      
      for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter) {
         if (parse_state == state_fec) {
            fec = boost::trim_copy(*tok_iter);
            parse_state = state_board;
         }
         else if (parse_state == state_board) {
            board = boost::trim_copy(*tok_iter);
            parse_state = state_channel;
         }
         else if (parse_state == state_channel) {
            channel = boost::trim_copy(*tok_iter);
            parse_state = state_qoffset;
         }
         else if (parse_state == state_qoffset) {
            qoffset = boost::trim_copy(*tok_iter);
            parse_state = state_qslope;
         }
         else if (parse_state == state_qslope) {
            qslope = boost::trim_copy(*tok_iter);
            parse_state = state_toffset;
         }
         else if (parse_state == state_tac) {
            timetac = boost::trim_copy(*tok_iter);
            parse_state = state_toffset;
         }
         else if (parse_state == state_toffset) {
            toffset = boost::trim_copy(*tok_iter);
            parse_state = state_tslope;
         }
         else if (parse_state == state_tslope) {
            tslope = boost::trim_copy(*tok_iter);
         }
      
      }
      if (parse_state != state_tslope) {
         //error
         std::stringstream ss;
         ss << "ERROR: CGuiTabMainOptions::load_thresholds_text_file()::read_strip_map_file '" << filename
         << "' error parsing line " << lineCounter;
         throw std::runtime_error(ss.str().c_str());
      }
    
      try {
         
         unsigned fecno = boost::numeric_cast<unsigned>(boost::lexical_cast<unsigned>( fec ));
         unsigned board_id = boost::numeric_cast<unsigned>(boost::lexical_cast<unsigned>( board ));
         unsigned chno = boost::numeric_cast<unsigned>(boost::lexical_cast<unsigned>( channel ));
         
         double qoff = boost::numeric_cast<double>(boost::lexical_cast<double>( qoffset ));
         double qslo = boost::numeric_cast<double>(boost::lexical_cast<double>( qslope ));
         double ttac = boost::numeric_cast<double>(boost::lexical_cast<double>( timetac ));
         double toff = boost::numeric_cast<double>(boost::lexical_cast<double>( toffset ));
         double tslo = boost::numeric_cast<double>(boost::lexical_cast<double>( tslope ));
         
         
         CSrsChannelId chid(fecno, board_id, chno);
         vmm_tac_map[chid] = VMM1Parameters(qoff, qslo, ttac, toff, tslo);
         
          m_config->set_vmm1_offsets(vmm_tac_map);
         
      } catch( boost::bad_lexical_cast const& ) {
         std::cout << "ERROR: CGuiTabMainOptions::load_thresholds_text_file() '" << filename
         << "' not valid entry in line " << lineCounter << std::endl;
         throw std::runtime_error("bad parameters");
      } catch( boost::bad_numeric_cast const& ) {
         std::cout << "CGuiTabMainOptions::load_thresholds_text_file() '" << filename
         << "' value out of range in line " << lineCounter << std::endl;
         throw std::runtime_error("bad parameters");
      }
      
   }//while getline

   
   
}

