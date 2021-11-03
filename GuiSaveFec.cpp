


#include "GuiSaveFec.h"
#include "ConfigXml.h"

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
	
#include <iostream>
#include <algorithm>
#include <fstream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <functional>
#include <TApplication.h>

using namespace std;
CGuiSaveFec::CGuiSaveFec(TGWindow *main, int h , int z)
	
{
	
fMain = new TGTransientFrame(gClient->GetRoot(), main, 10, 10, kMainFrame | kVerticalFrame);


fMain->Connect("CloseWindow()", "CGuiSaveFec", this, "CloseWindow()");
fMain->DontCallClose();
fMain->SetCleanup(kDeepCleanup);	


TGGroupFrame* group3frame = new TGGroupFrame(fMain, "Name", kLHintsCenterX);
{

name_fec = new TGTextEntry(group3frame);
group3frame->AddFrame(name_fec, new TGLayoutHints(kLHintsCenterX ,10,10,10,10));
name_fec->Resize(150, 20);

}

fMain->AddFrame(group3frame, new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2));




TGGroupFrame* group2frame = new TGGroupFrame(fMain, "Adress", kLHintsCenterX);
{
	
adress = new TGTextEntry(group2frame);
group2frame->AddFrame(adress, new TGLayoutHints(kLHintsCenterX,10,10,10,10));
adress->Resize(150, 20);
}

fMain->AddFrame(group2frame, new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2));


TGGroupFrame* group7frame = new TGGroupFrame(fMain, "", kLHintsCenterX);
{
	
	selectall_button = new TGCheckButton(group7frame, "select all",1);
	selectall_button->Connect("Clicked()", "CGuiSaveFec", this, "select_all()");
	group7frame->AddFrame(selectall_button, new TGLayoutHints(kLHintsCenterX,3,3,3,3));
}

fMain->AddFrame(group7frame, new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2));




TGGroupFrame* group4frame = new TGGroupFrame(fMain, "Channel", kLHintsCenterX);
{
	channel0 = new TGCheckButton(group4frame, "channel 0",1);
	group4frame->AddFrame(channel0, new TGLayoutHints(kLHintsCenterX,3,3,3,3));
	
	channel1 = new TGCheckButton(group4frame, "channel 1",1);
	group4frame->AddFrame(channel1, new TGLayoutHints(kLHintsCenterX,3,3,3,3));
	
	channel2 = new TGCheckButton(group4frame, "channel 2",1);
	group4frame->AddFrame(channel2, new TGLayoutHints(kLHintsCenterX,3,3,3,3));
	
	channel3 = new TGCheckButton(group4frame, "channel 3",1);
	group4frame->AddFrame(channel3, new TGLayoutHints(kLHintsCenterX,3,3,3,3));
	
	channel4 = new TGCheckButton(group4frame, "channel 4",1);
	group4frame->AddFrame(channel4, new TGLayoutHints(kLHintsCenterX,3,3,3,3));
	
	channel5 = new TGCheckButton(group4frame, "channel 5",1);
	group4frame->AddFrame(channel5, new TGLayoutHints(kLHintsCenterX,3,3,3,3));
	
	channel6 = new TGCheckButton(group4frame, "channel 6",1);
	group4frame->AddFrame(channel6, new TGLayoutHints(kLHintsCenterX,3,3,3,3));
	
	channel7 = new TGCheckButton(group4frame, "channel 7",1);
	group4frame->AddFrame(channel7, new TGLayoutHints(kLHintsCenterX,3,3,3,3));
	
	channel8 = new TGCheckButton(group4frame, "channel 8",1);
	group4frame->AddFrame(channel8, new TGLayoutHints(kLHintsCenterX,3,3,3,3));
	
	channel9 = new TGCheckButton(group4frame, "channel 9",1);
	group4frame->AddFrame(channel9, new TGLayoutHints(kLHintsCenterX,3,3,3,3));
	
	channel10 = new TGCheckButton(group4frame, "channel 10",1);
	group4frame->AddFrame(channel10, new TGLayoutHints(kLHintsCenterX,3,3,3,3));
	
	channel11 = new TGCheckButton(group4frame, "channel 11",1);
	group4frame->AddFrame(channel11, new TGLayoutHints(kLHintsCenterX,3,3,3,3));
	
	channel12 = new TGCheckButton(group4frame, "channel 12",1);
	group4frame->AddFrame(channel12, new TGLayoutHints(kLHintsCenterX,3,3,3,3));
	
	channel13 = new TGCheckButton(group4frame, "channel 13",1);
	group4frame->AddFrame(channel13, new TGLayoutHints(kLHintsCenterX,3,3,3,3));
	
	channel14 = new TGCheckButton(group4frame, "channel 14",1);
	group4frame->AddFrame(channel14, new TGLayoutHints(kLHintsCenterX,3,3,3,3));
	
	channel15 = new TGCheckButton(group4frame, "channel 15",1);
	group4frame->AddFrame(channel15, new TGLayoutHints(kLHintsCenterX,3,3,3,3));
}

fMain->AddFrame(group4frame, new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2));



TGHorizontalFrame* group5frame = new TGHorizontalFrame(fMain, kLHintsCenterX);
{	//
	Add_Fec = new TGTextButton(group5frame,"Add Fec");
	group5frame->AddFrame(Add_Fec, new TGLayoutHints(kLHintsCenterX,3,3,3,3));
	// Add_Fec->Connect("Clicked()", "CGuiSaveFec", this, "AddFec()");
	Add_Fec->Connect("Clicked()", "CGuiSaveFec", this, "AddFec()");

	// close_button = new TGTextButton(group5frame,"Close");
// 	group5frame->AddFrame(close_button, new TGLayoutHints(kLHintsCenterX,3,3,3,3));
// 	close_button->Connect("Clicked()", "CGuiSaveFec", this, "close()");

}

fMain->AddFrame(group5frame, new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2));






fMain->SetWindowName("Insert a new FEC");
fMain->MapSubwindows();

fMain->MapWindow();
 fMain->Resize(300, 650);


	
}



void CGuiSaveFec::CloseWindow()
{
	
delete fMain;
}



CGuiSaveFec::~CGuiSaveFec()
{
	
	
}


void CGuiSaveFec::AddFec()
{	//
	vector<std::string> tmp;
	tmp.push_back(name_fec->GetText());
	tmp.push_back(adress->GetText());
	
	tmp.push_back(plug(channel0->IsDown(), 0));
	tmp.push_back(plug(channel1->IsDown(), 1));
	tmp.push_back(plug(channel2->IsDown(), 2));
	tmp.push_back(plug(channel3->IsDown(), 3));
	tmp.push_back(plug(channel4->IsDown(), 4));
	tmp.push_back(plug(channel5->IsDown(), 5));
	tmp.push_back(plug(channel6->IsDown(), 6));
	tmp.push_back(plug(channel7->IsDown(), 7));
	tmp.push_back(plug(channel8->IsDown(), 8));
	tmp.push_back(plug(channel9->IsDown(), 9));
	tmp.push_back(plug(channel10->IsDown(), 10));
	tmp.push_back(plug(channel11->IsDown(), 11));
	tmp.push_back(plug(channel12->IsDown(), 12));
	tmp.push_back(plug(channel13->IsDown(), 13));
	tmp.push_back(plug(channel14->IsDown(), 14));
	tmp.push_back(plug(channel15->IsDown(), 15));
	tmp.push_back("   ");
	
	fec.push_back(tmp);
	
	vector<bool> tmp_avp;
	tmp_avp.push_back(channel0->IsDown());
	
	tmp_avp.push_back(channel1->IsDown());
	tmp_avp.push_back(channel2->IsDown());
	tmp_avp.push_back(channel3->IsDown());
	tmp_avp.push_back(channel4->IsDown());
	tmp_avp.push_back(channel5->IsDown());
	tmp_avp.push_back(channel6->IsDown());
	tmp_avp.push_back(channel7->IsDown());
	tmp_avp.push_back(channel8->IsDown());
	tmp_avp.push_back(channel9->IsDown());
	tmp_avp.push_back(channel10->IsDown());
	tmp_avp.push_back(channel11->IsDown());
	tmp_avp.push_back(channel12->IsDown());
	tmp_avp.push_back(channel13->IsDown());
	tmp_avp.push_back(channel14->IsDown());
	tmp_avp.push_back(channel15->IsDown());
	
	
	apv.push_back(tmp_avp);
	
		
	fEdit->Clear();
	name_fec->Clear();
	adress->Clear();
	channel0->SetState(kButtonUp);
	channel1->SetState(kButtonUp);
	channel2->SetState(kButtonUp);
	channel3->SetState(kButtonUp);
	channel4->SetState(kButtonUp);
	channel5->SetState(kButtonUp);
	channel6->SetState(kButtonUp);
	channel7->SetState(kButtonUp);
	channel8->SetState(kButtonUp);
	channel9->SetState(kButtonUp);
	channel10->SetState(kButtonUp);
	channel11->SetState(kButtonUp);
	channel12->SetState(kButtonUp);
	channel13->SetState(kButtonUp);
	channel14->SetState(kButtonUp);
	channel15->SetState(kButtonUp);
	selectall_button->SetState(kButtonUp);
	
	for (int i =0; i<fec.size(); i++)
		
	{
		
		std::ostringstream ss;
		ss << i+1;
		string cd = " Fec Number:  ";
		cd +=ss.str(); 
		
		
		 fEdit->AddText(new TGText(cd.c_str()));
		 
		 for (int j=0; j<((fec[i]).size()); j++)
		 {
			 
			 
			 
			
			  string content_fec = "\t"+fec[i][j];
			  
 			
 			fEdit->AddText(new TGText(content_fec.c_str()));
		 }
	
	
}

}



std::string CGuiSaveFec::plug(int a, int b)

{
	std::string output;
	std::ostringstream ss;
	ss << "channel " << b;
	output = ss.str();
	
	if (a==1)  {output += "\tPlugged";}
	else {output += "\tUnplugged";}
	return output;
	
}


void CGuiSaveFec::close()

{
	CloseWindow();
	
}


void CGuiSaveFec::select_all()

{
	channel0->SetState(kButtonDown);
	channel1->SetState(kButtonDown);
	channel2->SetState(kButtonDown);
	channel3->SetState(kButtonDown);
	channel4->SetState(kButtonDown);
	channel5->SetState(kButtonDown);
	channel6->SetState(kButtonDown);
	channel7->SetState(kButtonDown);
	channel8->SetState(kButtonDown);
	channel9->SetState(kButtonDown);
	channel10->SetState(kButtonDown);
	channel11->SetState(kButtonDown);
	channel12->SetState(kButtonDown);
	channel13->SetState(kButtonDown);
	channel14->SetState(kButtonDown);
	channel15->SetState(kButtonDown);
}