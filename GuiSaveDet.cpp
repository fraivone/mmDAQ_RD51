


#include "GuiSaveDet.h"
#include "ConfigXml.h"
#include "GuiAddApv.h"

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
#include <TGTextEntry.h>
#include "GuiTabFec.h"


#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
	
#include <iostream>
#include <algorithm>
#include <fstream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <functional>
#include <TApplication.h>
#include <vector>

using namespace std;
namespace bfs = boost::filesystem;

CGuiSaveDet::CGuiSaveDet(TGWindow *main, int h , int z)
{
	
fMain = new TGTransientFrame(gClient->GetRoot(), main, 10, 10, kMainFrame | kVerticalFrame);


fMain->Connect("CloseWindow()", "CGuiSaveFec", this, "CloseWindow()");
fMain->DontCallClose();
fMain->SetCleanup(kDeepCleanup);



TGGroupFrame* group3frame = new TGGroupFrame(fMain, "Name", kLHintsCenterX);
{

 name_chamber = new TGTextEntry(group3frame);
group3frame->AddFrame(name_chamber, new TGLayoutHints(kLHintsCenterX ,10,10,10,10));
name_chamber->Resize(150, 20);

}

fMain->AddFrame(group3frame, new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2));

TGGroupFrame* group4frame = new TGGroupFrame(fMain, "Type chamber", kLHintsCenterX);
{

   TGHorizontalFrame* group6frame = new TGHorizontalFrame(group4frame, kLHintsCenterX);
   {	
	
	type_det = new TGTextEntry(group6frame);
	type_det->Resize(150, 20);
	group6frame->AddFrame(type_det, new TGLayoutHints(kLHintsCenterX, 10,10,10,10));
	
    TGPictureButton *ch0_button = new TGPictureButton(group6frame,gClient->GetPicture("open.xpm"));
    // ch0_button->MoveResize(250, 100, 20, 20);
    ch0_button->Connect("Clicked()", "CGuiSaveDet", this, "open_type_chamber()");
	group6frame->AddFrame(ch0_button, new TGLayoutHints(kLHintsCenterX, 10,10,10,10));
	
	}

	group4frame->AddFrame(group6frame, new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2));
}

fMain->AddFrame(group4frame, new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2));


	TGTextView* fEdit2 = new TGTextView(fMain, 250, 200);
	apv_view = fEdit2;
//
	  TGLayoutHints* fL1 = new TGLayoutHints( kLHintsExpandX|kLHintsExpandY, 2, 2, 2, 2);
	   fMain->AddFrame(apv_view, new TGLayoutHints(kLHintsCenterX ,10,10,10,10));




	   TGHorizontalFrame* group5frame = new TGHorizontalFrame(fMain, kLHintsCenterX);
	   {	//
	   	TGTextButton* Save_det = new TGTextButton(group5frame,"Save");
	   	group5frame->AddFrame(Save_det, new TGLayoutHints(kLHintsCenterX,3,3,3,3));
	   	Save_det->Connect("Clicked()", "CGuiSaveDet", this, "add_current_chamber()");

	   	TGTextButton* add_apv = new TGTextButton(group5frame,"Add connector-APV");
	   	group5frame->AddFrame(add_apv, new TGLayoutHints(kLHintsCenterX,3,3,3,3));
	   	add_apv->Connect("Clicked()", "CGuiSaveDet", this, "open_add_apv()");

	   }

	   fMain->AddFrame(group5frame, new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2));



fMain->SetWindowName("Insert a new Chamber");
fMain->MapSubwindows();

fMain->MapWindow();
 fMain->Resize(400, 420);




}	



void CGuiSaveDet::CloseWindow()
{
	
	apv_temp.clear();
	connector_temp.clear();
	
delete fMain;
}



CGuiSaveDet::~CGuiSaveDet()
{
	
	
}


void CGuiSaveDet::open_type_chamber()

{
   const char *filetypes[] = {
      "XML",   "*.xml",
      "All files",     "*",
      0,               0 };

   int ftype = 0; // default selection


   // size_t sz = m_config->get_working_path().string().size();
  //  char* cwd = new char[sz+1];
  //  strncpy(cwd, m_config->get_working_path().string().c_str(), sz);
  //  cwd[sz] = '\0';
static TString dir(".");
TGFileInfo file_info;
            file_info.fFileTypes = filetypes;
            file_info.fIniDir    = StrDup(dir);
//
//
//
//    TGFileInfo file_info;
//    file_info.fFileTypes = filetypes;
//    file_info.fFileTypeIdx = ftype;
//    file_info.fIniDir    = cwd;
//
 TGFileDialog* prova =  new TGFileDialog(gClient->GetRoot(), fMain, kFDOpen, &file_info); // do not delete
//
 if (!file_info.fFilename) {
    type_det->SetText("");
    return;
 }
 
 if (!bfs::exists(file_info.fIniDir) || !bfs::is_directory(file_info.fIniDir)) {
    std::cout << "WARNING: bad input path: " + std::string(file_info.fIniDir) << std::endl;
    type_det->SetText("");
    return;
 	}// do not delete

//
   if (!bfs::exists(file_info.fFilename) || !bfs::is_regular(file_info.fFilename)) {
      std::cout << "WARNING: bad input file: " + std::string(file_info.fFilename) << std::endl;
      type_det->SetText("");
      return;
   }
 
//


 
   
   boost::filesystem::path my_path( file_info.fFilename );
string result1 = boost::filesystem::basename (my_path);
path_complete = boost::filesystem::canonical(my_path).string();

type_det->SetText(result1.c_str());
   
}

void CGuiSaveDet::open_add_apv()
{
	
	CGuiAddApv* apv_frame = new CGuiAddApv(fMain, 200, 150);
}


void CGuiSaveDet::add_current_chamber()
{
	
	std::vector<string> tmp;
	tmp.push_back(name_chamber->GetText());
	tmp.push_back(type_det->GetText());
	tmp.push_back(path_complete);
	
	det.push_back(tmp);
	
	apv_final.push_back(apv_temp);
	
	connector_final.push_back(connector_temp);
	
	
	fEdit_det->Clear();
	
	for (int j=0; j<det.size(); j++)
	{
		std::ostringstream name;
		std::ostringstream type;
		
		name << "\t" << "Name chamber:\t" << det[j][0];
		type << "\t" << "type chamber:\t" << det[j][1];
		
		fEdit_det->AddText(new TGText(name.str().c_str()));
		fEdit_det->AddText(new TGText(type.str().c_str()));
	
		for (int i = 0; i < apv_final[j].size(); i++)
		{
			
			std::ostringstream connectors;
			connectors << "\t" << connector_final[j][i] << "\t-->\t" << apv_final[j][i]; 
			
			fEdit_det->AddText(new TGText(connectors.str().c_str()));
			
		
		
		}
		fEdit_det->AddText(new TGText(""));
	}
	
	apv_temp.clear();
	connector_temp.clear();
	
	name_chamber->Clear();
	type_det->Clear();
	apv_view->Clear();
	
	
	
	

}
