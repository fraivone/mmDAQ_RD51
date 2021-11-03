
#include "GuiAddApv.h"
#include "ConfigXml.h"

#include <TGButton.h>
#include <TGNumberEntry.h>
#include <TGComboBox.h>
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

using namespace std;

CGuiAddApv::CGuiAddApv(TGWindow *main, int h , int z)
{
	
fMain = new TGTransientFrame(gClient->GetRoot(), main, 10, 10, kMainFrame | kVerticalFrame);


fMain->Connect("CloseWindow()", "CGuiAddApv", this, "CloseWindow()");
fMain->DontCallClose();
fMain->SetCleanup(kDeepCleanup);




TGGroupFrame* group3frame = new TGGroupFrame(fMain, "Name connector", kLHintsCenterX);
{

name_connector = new TGTextEntry(group3frame);
group3frame->AddFrame(name_connector, new TGLayoutHints(kLHintsCenterX ,10,10,10,10));
name_connector->Resize(150, 20);

}

fMain->AddFrame(group3frame, new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2));




TGGroupFrame* group2frame = new TGGroupFrame(fMain, "APV", kLHintsCenterX);
{
	
apv_conn = new 	TGComboBox(group2frame);
group2frame->AddFrame(apv_conn, new TGLayoutHints(kLHintsCenterX,10,10,10,10));
apv_conn->Resize(150,20);
add_combo_apv();
}

fMain->AddFrame(group2frame, new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2));

TGTextButton* save_button = new TGTextButton(fMain,"Save APV");
fMain->AddFrame(save_button, new TGLayoutHints(kLHintsCenterX,3,3,3,3));
save_button->Connect("Clicked()", "CGuiAddApv", this, "save_apv_linked()");

fMain->SetWindowName("Insert a new APV");
fMain->MapSubwindows();

fMain->MapWindow();
 fMain->Resize(350, 250);




}	



void CGuiAddApv::CloseWindow()
{
	
delete fMain;
}



CGuiAddApv::~CGuiAddApv()
{
	
	
}


void CGuiAddApv::add_combo_apv()
{
	
	
	for (int i = 0; i < fec.size(); i++)
	{
		
		
		for (int j=0; j<apv[i].size(); j++)
		{
			
			if (apv[i][j] == kTRUE)
			{
				std::ostringstream ss;
				
				if (j<10)
				ss << "APV" << "." << i+1 <<".0"<<j;
				else
				ss << "APV" << "." << i+1 <<"."<<j;	
				
				 
				apv_conn->NewEntry(ss.str().c_str());
							
				
			}
						
		}
		
	}
	
	
	
}

void CGuiAddApv::save_apv_linked()
{
	
	std::ostringstream ss;
	
	ss << "\t" << name_connector->GetText() << "-->\t" << ((apv_conn->GetSelectedEntry())->GetTitle());
	
	connector_temp.push_back(name_connector->GetText());
	apv_temp.push_back((apv_conn->GetSelectedEntry())->GetTitle());
	apv_view->AddText(new TGText((ss.str()).c_str())); 
	
	
	name_connector->Clear();
	apv_conn->Clear();
	
	
}