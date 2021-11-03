

#include "GuiTabXmlCreator.h"

#include "GuiTabFec.h"
#include "GuiTabSaveXml.h"

#include "GuiTabDetXml.h"
#include "EventBrowser.h"
#include "GuiMainFrame.h"

#include "GuiMainFrameOffline.h"
#include "GuiMainFrameOnline.h"
#include <TGTab.h>

#include "DetReadout.h"

#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>

#include <TGButton.h>
#include <TGNumberEntry.h>
#include <TString.h>
#include <TGFileDialog.h>
#include <TGClient.h>
#include <TGMsgBox.h>
#include <TH1.h>
#include <TH2.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TCanvas.h>
#include <TGFrame.h>
#include <TRootEmbeddedCanvas.h>
#include "TGLabel.h"
#include <TStyle.h>

#include <iostream>
#include <algorithm>
#include <fstream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <functional>

using namespace browser;
namespace bfs = boost::filesystem;
CGuiTabXml::CGuiTabXml(CGuiBase* gui_parent,
                                 CConfiguration* config, 
                                 CEventBrowser*   eb,
                                 TGTab* main_tabs, 
                                 const std::string& name) :
CGuiTab(gui_parent, config, eb, main_tabs,name),
m_last_detbase(0)

{
   construct_tab();
}


CGuiTabXml::~CGuiTabXml()
{

}

void CGuiTabXml::do_construct_tab()
{
   
    m_tab_frame->SetLayoutManager(new TGVerticalLayout(m_tab_frame));
   
    {
       //tabs
       // tab widget
       m_gui_right_tab = new TGTab(m_tab_frame, 484, 316);
       m_gui_right_tab->Connect("Selected(Int_t)", "browser::CGuiTabXml", this, "update_right_tab()");
   m_right_tabs.push_back(new CGuiTabFec(this,   m_config, m_eb, m_gui_right_tab, "FEC"));
   m_right_tabs.push_back(new CGuiTabDetXml(this,   m_config, m_eb, m_gui_right_tab, "DET")); 
   m_right_tabs.push_back(new CGuiTabSaveXml(this,   m_config, m_eb, m_gui_right_tab, "Save Xml")); 
   
   
   
   
   
   m_gui_right_tab->SetTab(0);
   m_gui_right_tab->Resize(m_tab_frame->GetDefaultSize());
   m_tab_frame->AddFrame(m_gui_right_tab, new TGLayoutHints( kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY));
   
   }
   
    // m_tab_frame->SetLayoutManager(new TGVerticalLayout(m_tab_frame));
  //
  //
  //         //open file button
  //         save_button = new TGTextButton(m_tab_frame, "&save xml file");
  //         save_button->Resize(280, 22);
  //
  //         save_button->Connect("Clicked()", "browser::CGuiTabXml", this, "gui_save_xml()");
  //         m_tab_frame->AddFrame(save_button, new TGLayoutHints( kLHintsTop, 1, 2, 0, 0));
  //
  //    	 fec_button = new TGTextButton(m_tab_frame, "&Add a new Fec");
  //         fec_button->Resize(280, 22);
  //
  //         fec_button->Connect("Clicked()", "browser::CGuiTabXml", this, "add_system()");
  //         m_tab_frame->AddFrame(fec_button, new TGLayoutHints( kLHintsTop, 1, 2, 0, 0));
  //
       

/**
 on user click - rename to handle_...?
 */}
		  
		  
// void CGuiTabXml::gui_save_xml()
// 	{
//
// 		// std::cout << "ciao" << std::endl;
// 		  // //
// 		  // 	       const char *filetypes[] = {
// 		  // 	          "XML file",   "*.xml"
// 		  // };
// //
// // 	       size_t sz = m_config->get_working_path().string().size();
// // 	       char* cwd = new char[sz+1];
// // 	       strncpy(cwd, m_config->get_working_path().string().c_str(), sz);
// // 	       cwd[sz] = '\0';
// // 	       int ftype = 0; //"raw_apv"
// 	      const char *filetypes[] = {
// 	         "XML",   "*.xml",
// 	         0,               0 };
//
// 	      int ftype = 0; // default selection
//
// 	      size_t sz = m_config->get_working_path().string().size();
// 	      char* cwd = new char[sz+1];
// 	      strncpy(cwd, m_config->get_working_path().string().c_str(), sz);
// 	      cwd[sz] = '\0';
//
// 	       TGFileInfo file_info;
// 	       file_info.fFileTypes = filetypes;
// 	       file_info.fFileTypeIdx = ftype;
// 	       file_info.fIniDir    = cwd;
// 	       new TGFileDialog(gClient->GetRoot(), m_tab_frame, kFDSave, &file_info); // do not delete
// 	       if (!file_info.fFilename) {
// 	          return;
// 	       }
//
// 		   else {
// 			   save_xml_file(file_info.fFilename);
//
// 		   }
//
//
//
// 	}
		  



// 	void CGuiTabXml::add_system()
// 	{
//
//
//
//
//
//
// 	}
//
//
//
// void CGuiTabXml::save_xml_file(const std::string& filename)
// {
//
// 	// xml file writing
// 	ofstream output_file;
// 	std::string output_filename = filename+".xml";
// 	  output_file.open (output_filename.c_str());
//
// 	  output_file << "ciccio pluto";
//
// 	  output_file.close();
//
// 	  TGMsgBox* message_box = new TGMsgBox (gClient->GetRoot(), 0, "Writing xml file", "Xml configuration file has been written successfully!",0, kMBClose);
//
// }

void CGuiTabXml::update_right_tab()
{
   CGuiMainFrame* mf = dynamic_cast<CGuiMainFrame*>(m_gui_parent);
   if (!mf) {
      return;
   }
   
   
   
   m_last_detbase = mf->get_tree_selected_user_data();
   if (m_last_detbase) {
      update(m_last_detbase);   
	  
   }
   
}

/**
 update currevt active tab with data from det element
 */
void CGuiTabXml::do_update(const CDetBase* detbase)   
{

   
}

void CGuiTabXml::do_clear_tab()
{
   std::for_each(m_right_tabs.begin(), m_right_tabs.end(), boost::bind(&CGuiTab::clear_tab, _1));
}


