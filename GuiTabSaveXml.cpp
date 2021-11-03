
#include "GuiTabSaveXml.h"


#include "EventBrowser.h"
#include "GuiMainFrame.h"

#include "GuiMainFrameOffline.h"
#include "GuiMainFrameOnline.h"
#include <TGTab.h>
#include "GuiSaveFec.h"
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
#include <TGTextView.h>

#include <iostream>
#include <algorithm>
#include <fstream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <functional>


#include "ConfigXml.h"

using namespace browser;
namespace bfs = boost::filesystem;
CGuiTabSaveXml::CGuiTabSaveXml(CGuiBase* gui_parent,
                                 CConfiguration* config, 
                                 CEventBrowser*   eb,
                                 TGTab* main_tabs, 
                                 const std::string& name) :
CGuiTab(gui_parent, config, eb, main_tabs,name)

{
   construct_tab();
}


CGuiTabSaveXml::~CGuiTabSaveXml()
{

}

void CGuiTabSaveXml::do_construct_tab()
{
   
   
	TGGroupFrame* group3frame = new TGGroupFrame(m_tab_frame, "Log File", kLHintsCenterX);
		{


		TGHorizontalFrame* horizontalframe = new TGHorizontalFrame(group3frame);
		{
	log_file_text = new TGTextEntry(horizontalframe);
	log_file_text->Resize(150, 20);
	horizontalframe->AddFrame(log_file_text, new TGLayoutHints(kLHintsCenterX ,10,10,10,10));
	log_file_text->Resize(150, 20);
	
	
    TGPictureButton *ch0_button = new TGPictureButton(horizontalframe,gClient->GetPicture("open.xpm"));
    // ch0_button->MoveResize(250, 100, 20, 20);
    ch0_button->Connect("Clicked()", "browser::CGuiTabSaveXml", this, "open_log_file()");
	horizontalframe->AddFrame(ch0_button, new TGLayoutHints(kLHintsCenterX, 10,10,10,10));
		}

	group3frame->AddFrame(horizontalframe);

	}
	
	m_tab_frame->AddFrame(group3frame);
   
 
 
 
	TGGroupFrame* group4frame = new TGGroupFrame(m_tab_frame, "Data Folder", kLHintsCenterX);
		{


		TGHorizontalFrame* horizontalframe = new TGHorizontalFrame(group4frame);
		{
	data_folder = new TGTextEntry(horizontalframe);
	data_folder->Resize(150, 20);	
	horizontalframe->AddFrame(data_folder, new TGLayoutHints(kLHintsCenterX ,10,10,10,10));
	data_folder->Resize(350, 20);
	
	
    TGPictureButton *ch0_button = new TGPictureButton(horizontalframe,gClient->GetPicture("open.xpm"));
    // ch0_button->MoveResize(250, 100, 20, 20);
    ch0_button->Connect("Clicked()", "browser::CGuiTabSaveXml", this, "open_data_directory()");
	horizontalframe->AddFrame(ch0_button, new TGLayoutHints(kLHintsCenterX, 10,10,10,10));
		}

	group4frame->AddFrame(horizontalframe);

	}
	m_tab_frame->AddFrame(group4frame);
 
   TGTextButton* save = new TGTextButton(m_tab_frame,"Save XML configuration");
   save->SetTextJustify(36);
   save->SetMargins(0,0,0,0);
   save->SetWrapLength(-1);
   save->Connect("Clicked()", "browser::CGuiTabSaveXml", this, "save_config()");
 
 
 
 
	m_tab_frame->AddFrame(save,new TGLayoutHints(kLHintsLeft, 10,10,10,10));
	TGGroupFrame* group6frame = new TGGroupFrame(m_tab_frame, "Attention before saving", kLHintsCenterX);
		{
 	TGLabel* allert  = new TGLabel(group6frame, "Please consider that detector type files and configuration files have to be in the same directory");
    group6frame->AddFrame(allert, new TGLayoutHints(kLHintsCenterX, 10,10,10,10));
 
}
m_tab_frame->AddFrame(group6frame,new TGLayoutHints(kLHintsLeft, 10,10,10,10));

}
		  
		  


/**
 update currevt active tab with data from det element
 */
void CGuiTabSaveXml::do_update(const CDetBase* detbase)
{
//   std::cout << "CGuiTabReadout1D::update(" <<detbase->get_name() << ")\n";
   const CDetReadoutBase* rd = dynamic_cast<const CDetReadoutBase*>(detbase);
   if (!rd) {
      return;
   }
   
}
void CGuiTabSaveXml::do_clear_tab()
{
   std::for_each(m_right_tabs.begin(), m_right_tabs.end(), boost::bind(&CGuiTab::clear_tab, _1));
}


void CGuiTabSaveXml::open_log_file()
	
{
	
	
    const char *filetypes[] = {
       "Log Files",   "*.log",
       0,               0 };

    int ftype = 0; // default selection


 static TString dir(".");
 TGFileInfo file_info;
             file_info.fFileTypes = filetypes;
             file_info.fIniDir    = StrDup(dir);
 //

 //
  TGFileDialog* dialog =  new TGFileDialog(gClient->GetRoot(), m_tab_frame, kFDOpen, &file_info); // do not delete
 //
 
 
 
  if (!file_info.fFilename) {
     log_file_text->SetText("");
     return;
  }
 
 
  if (!bfs::exists(file_info.fIniDir) || !bfs::is_directory(file_info.fIniDir)) {
     std::cout << "WARNING: bad input path: " + std::string(file_info.fIniDir) << std::endl;
     log_file_text->SetText("");
     return;
  	}// do not delete
//
//  //
    if (!bfs::exists(file_info.fFilename) || !bfs::is_regular(file_info.fFilename)) {
       std::cout << "WARNING: bad input file: " + std::string(file_info.fFilename) << std::endl;
       log_file_text->SetText("");
       return;
    }
 //

//
//
 //
    boost::filesystem::path my_path( file_info.fFilename );
  log_filename = boost::filesystem::basename (my_path);
  log_complete = boost::filesystem::canonical(my_path).string();

 log_file_text->SetText(log_filename.c_str());

	
	
	
}


void CGuiTabSaveXml::open_data_directory()
	
{
	
	
    const char *filetypes[] = {
       "Type directory's name and OPEN",   "*",
       0,               0 };

    int ftype = 0; // default selection


 static TString dir(".");
 TGFileInfo file_info;
             file_info.fFileTypes = filetypes;
             file_info.fIniDir    = StrDup(dir);
 //

 //
  TGFileDialog* dialog =  new TGFileDialog(gClient->GetRoot(), m_tab_frame, kFDOpen, &file_info); // do not delete
 //
  
  
  if (!file_info.fFilename) {
     data_folder->SetText("");
     return;
  }
  
  if (!bfs::exists(file_info.fIniDir) || !bfs::is_directory(file_info.fIniDir)) {
     std::cout << "WARNING: bad input path: " + std::string(file_info.fIniDir) << std::endl;
     data_folder->SetText("");	
     return;
  	}// do not delete
 
 //
    if (!bfs::exists(file_info.fFilename) ) {
       std::cout << "WARNING: bad input file: " + std::string(file_info.fFilename) << std::endl;
       data_folder->SetText("");
       return;
    }
	
	



   
    boost::filesystem::path my_path( file_info.fFilename );
 data_filename = boost::filesystem::basename (my_path);
 data_complete = boost::filesystem::canonical(my_path).string();

 data_folder->SetText(data_complete.c_str());
	
	
	
	
}

void CGuiTabSaveXml::save_config()
{
	
	write_tmm();
	write_telescope();
	write_srs();
	

	
}

void CGuiTabSaveXml::write_telescope()
{
	

	string editor =
		
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
"<!--\n"
" This is the main configuration file\n"
" a part of mmDAQ configuration\n"
" generated automatically by mmdaq3\n"  
" -->\n"
			"\n"
"<server_config>\n"
"   <!-- UDP data receiver TODO MULTI option: make mutliple receivers and define accepted remote addresses for each -->\n"
"   <udp_receiver>\n"
"      <ip>10.0.0.3:6006</ip>\n"
"      <!-- <ip>127.0.0.2:9999</ip> -->\n"
"   </udp_receiver>\n"
						"\n"
"   <!-- TCP data receiver\n"  
"   <tcp_client>\n"
"      <config_file>Dummy-tcp-config.xml</config_file>\n"
"   </tcp_client>\n"
"   -->\n"
"\n"
"   <event_builder>\n"							
"      <!-- use only AssembleBySrsTriggerCounter with correct SRS/FEC settings-->\n"
"       <mode>AssembleBySrsTriggerCounter</mode>\n"
"       <!--    <mode>AssembleByDaqEventNumber</mode> -->\n"
"      <!--other AssembleByDaqTimeStamp, AssembleByDaqEventNumber, AssembleBySrsTriggerCounter -->\n"
"   </event_builder>\n"
	"\n"
"    <!-- SRS electronics file -->\n"
"    <srs_file>srs.xml</srs_file>\n"
"   \n"
"    <!-- detector configuration file -->\n"
"    <!--<detector_file>detector_RF-T7-T8.xml</detector_file>-->\n"
"    <detector_file>"+file_name+".xml"+"</detector_file>\n"
"      \n"
"    <!-- logging and DB connectivity if any-->\n"
"    <mmdaq_log>\n"
"    	<local_file>mmdaq_internal.log</local_file>   <!-- ignore, n/a -->\n"
"    </mmdaq_log>\n"
"   \n"
"    <run_log>\n"
"    	<local_file>"+log_complete+"</local_file>\n"
"    </run_log>\n"
"   \n"
"    <!--path to save data files -->\n"
"    <data_directory>"+data_complete+"</data_directory>\n"
"\n"
"    <config_db>\n" 
"    	<!-- n/a could we ask a DB for detector_file and srs_file ?-->\n"
"    </config_db>\n"
"   \n"
"</server_config>\n";

	
	ofstream output_file;
	
	
	
   


	
	 	std::string output_filename = "telescope.xml";
	output_file.open (output_filename.c_str());

	output_file << editor;

		output_file.close();
		
		
	}
	
	
	void CGuiTabSaveXml::write_srs()
	{
		
	

		string editor =
"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
"<!--\n"
" This is SRS readout electronics configuration file\n"
" a part of mmDAQ configuration\n"
" generated automatically by mmdaq3\n"
" -->\n"
"\n"
"<srs>\n"
"   <APV25>\n"
"      <!-- this should go from here -->\n"
"	   <!-- this should go from here it is not easy to change this file for each run\n"
"      <zero_suppression_threshold>0.8</zero_suppression_threshold>\n"
"      <pedestal_file></pedestal_file> -->\n"
"      \n"
"      <decoder>\n"
"         <common_mode_correction>\n"
"            <enable>true</enable>\n"
"            <signal_detection_factor>10.0</signal_detection_factor>\n"
"         </common_mode_correction>\n"
"      </decoder>\n"
"      <!--  but crosstalk could stay, as property of the system -->\n"
"      <crosstalk_file>apv-crosstalk.map</crosstalk_file>\n"
"   </APV25>\n"
"\n"
"   <!--\n"
"   <BNL>\n"
"      <pedestal_file></pedestal_file>\n"
"   </BNL>\n"
"    -->\n"
"   \n"
"   <!-- a system without SRU -->\n"
"   <!-- First FEC -->\n"
"   <!-- TODO make this an error define srs elements from 1 -->\n";



		for (int i=1; i<(fec.size()+1); i++)
		{


			std::ostringstream ss;
			std::ostringstream ss1;

			ss << i;
			ss1<<i-1;
			editor +=
"	<fec id = \""+ss.str()+"\">\n"
"           <dcs_file>fec"+ss1.str()+".xml</dcs_file> <!-- dcs n/a-->\n"
"           <name>"+fec[i-1][0]+"</name>\n"
"            <ip>"+fec[i-1][1]+"</ip>\n"
"           <!-- chip id determined by hdmi cable connection -->\n";

		for (int j=0; j<apv[i-1].size(); j++)

		{
			if (apv[i-1][j]==kTRUE){
			std::ostringstream ss3;
			ss3<<j;
			std::ostringstream ss2;
			if (j<10)			ss2<<"0" <<j;
			else ss2<<j;
			editor +=
"	         <chip id = \""+ss3.str()+"\"  type = \"APV25\"> <name> APV."+ss.str()+"."+ss2.str()+" </name>   </chip>\n";


		}

		}
		editor+=
"   </fec>\n";			

		}
		editor +=
"  \n"
"   \n"
"   \n"
"  <!--TODO chip hybrid type defines mapping file (but not chip type ..) -->\n"
"  <!--TODO these should be the same type as in <chip> -->\n"
"  <chip_hybrid_maps>\n"
"     <map_file type=\"APV25\">apv_map.txt</apv_map>\n"
"     <map_file type=\"VMM01-BNLMini-L\">bnl-left-map.txt</apv_map>\n"
"     <map_file type=\"VMM01-BNLMini-R\">bnl-right-map.txt</apv_map>\n"
"  </chip_hybrid_maps>\n"
"   \n"
"</srs>\n";

			ofstream output_file;

		 	std::string output_filename = "srs.xml";
			output_file.open (output_filename.c_str());

			output_file << editor;

				output_file.close();

	
	}
	
	void CGuiTabSaveXml::write_tmm()
	{
	
	
		string editor =
"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
"<!--\n" 
"	This is the configuration file of the detector,\n" 
"	list of chambers and connections to electronics.\n"
"	A part of mmDAQ configuration\n"
"	generated automatically by mmdaq3\n"  
"	-->\n"
"\n"
"<detector name = \"LabTelescope\">\n"
"   <comment> Dummy detector comment</comment>\n"
"   <position> <x>0.0</x> <y>0.0</y> <z>10.0</z> </position>\n"
"   <rotation> <x>0.0</x> <y>0.0</y> <z> 0.0</z> </rotation>\n"
"\n"
" <!-- Chambers in detector, repeat with unique names for each chamber -->\n";

	
			//ruiepi
			
			
			for (int i=1; i<(det.size()+1); i++)
			{

				editor+=
"     <chamber name = \""+det[i-1][0]+"\">\n"
"       <!-- config file describes chamber type-->\n"
"       <config_file>"+det[i-1][1]+".xml</config_file>\n"
"       <drift_gap> 5.0 </drift_gap>\n"
"       <voltage>  <drift> 300</drift> <mesh> 0</mesh> <strips> 500</strips> </voltage>\n"
"       <position> <x>0.0</x> <y>0.0</y> <z> 0.0</z> </position> <!-- in mm -->\n"
"       <rotation> <x>0.0</x> <y>0.0</y> <z> 0.0</z> </rotation> <!-- in deg-->\n"
"       <!-- Connectors in chamber, repeat for each chip plugged -->\n";
				

			for (int j=0; j<apv_final[i-1].size(); j++)

			{
				editor+=
					
"		<connector name = \""+connector_final[i-1][j]+"\"> <chip> <name>"+apv_final[i-1][j]+"</name> </chip> <map_file>apv-map.txt</map_file> </connector>\n";				

			}
			editor +=
"	</chamber>\n"
	"\n";
			
				

			}

			editor+=
				"\n"
"</detector>\n	";				


		    const char *filetypes[] = {
		       "Xml",   "*.xml",
		       0,               0 };
			    
		    int ftype = 0; // default selection


			static TString dir(".");
			 TGFileInfo file_info;
			             file_info.fFileTypes = filetypes;
			             file_info.fIniDir    = StrDup(dir);
						 // file_info.fFilename = filename;
			//
			TGFileDialog* dialog =  new TGFileDialog(gClient->GetRoot(), m_tab_frame, kFDSave, &file_info);
				
				
				
				
		    if (!file_info.fFilename) {
		       
		       return;
		    }
  
		    if (!bfs::exists(file_info.fIniDir) || !bfs::is_directory(file_info.fIniDir)) {
		       
		       return;
		    	}// do not delete
 
		   //
		     

				ofstream output_file;
				if (!file_info.fFilename){
					
					std::cout << "Insert a valid file name" << std::endl;
					
				}


			    boost::filesystem::path my_path( file_info.fFilename );
			std::string output_filename = boost::filesystem::basename (my_path);
			 	
				file_name = output_filename;
				output_filename+=".xml";
				
				output_file.open (output_filename.c_str());

				output_file << editor;

					output_file.close();
					
					
					
		  TGMsgBox* message_box = new TGMsgBox (gClient->GetRoot(), 0, "Writing xml file", "Xml configuration file has been written successfully!",0, kMBClose);
				

	
	
	}
	
	
	