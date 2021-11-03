
#include "GuiTabFec.h"


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

#include "GuiConfig.h"

#include "ConfigXml.h"

using namespace browser;
namespace bfs = boost::filesystem;
CGuiTabFec::CGuiTabFec(CGuiBase* gui_parent,
                                 CConfiguration* config, 
                                 CEventBrowser*   eb,
                                 TGTab* main_tabs, 
                                 const std::string& name) :
CGuiTab(gui_parent, config, eb, main_tabs,name)

{
   construct_tab();
}


CGuiTabFec::~CGuiTabFec()
{

}

void CGuiTabFec::do_construct_tab()
{
   
   
    // m_tab_frame->SetLayoutManager(new TGVerticalLayout(m_tab_frame));
	
	
	TGHorizontalFrame* horizontalframe = new TGHorizontalFrame(m_tab_frame);
	 {
	TGTextView* fEdit2 = new TGTextView(horizontalframe, kLHintsExpandX|kLHintsExpandY);
	fEdit = fEdit2;
//
	  TGLayoutHints* fL1 = new TGLayoutHints( kLHintsExpandX|kLHintsExpandY, 2, 2, 2, 2);
	   horizontalframe->AddFrame(fEdit, fL1);
	   
	   
	   
	   TGVerticalFrame* verticalframe = new TGVerticalFrame(horizontalframe);
	   
	   {
	
	   add_fec_button = new TGTextButton(verticalframe,"Add Fec");
	   add_fec_button->SetTextJustify(36);
	   add_fec_button->SetMargins(0,0,0,0);
	   add_fec_button->SetWrapLength(-1);
	   // add_fec_button->Resize(100,25);
	   add_fec_button->Connect("Clicked()", "browser::CGuiTabFec", this, "open_save_fec()");
	   
	   verticalframe->AddFrame(add_fec_button,new TGLayoutHints(kLHintsCenterX|kLHintsCenterY,3,3,3,3)); 
	   
	   
	   
	   TGTextButton* remove_button = new TGTextButton(verticalframe,"Remove all");
	   remove_button->SetTextJustify(36);
	   remove_button->SetMargins(0,0,0,0);
	   remove_button->SetWrapLength(-1);
	   // add_fec_button->Resize(100,25);
	   remove_button->Connect("Clicked()", "browser::CGuiTabFec", this, "remove_all_Fec()");
	   
	   verticalframe->AddFrame(remove_button,new TGLayoutHints(kLHintsCenterX|kLHintsCenterY,3,3,3,3)); 
	   
   		}
	   
	  
	   horizontalframe->AddFrame(verticalframe,new TGLayoutHints(kLHintsCenterY|kLHintsCenterX,2, 2, 2, 2));
	}
	
		m_tab_frame->AddFrame(horizontalframe, new TGLayoutHints( kLHintsExpandX|kLHintsExpandY, 2, 2, 2, 2));
		
		do_reading_fec();
		
}
		  
		  


/**
 update currevt active tab with data from det element
 */
void CGuiTabFec::do_update(const CDetBase* detbase)
{
//   std::cout << "CGuiTabReadout1D::update(" <<detbase->get_name() << ")\n";
   const CDetReadoutBase* rd = dynamic_cast<const CDetReadoutBase*>(detbase);
   if (!rd) {
      return;
   }
   
}
void CGuiTabFec::do_clear_tab()
{
   std::for_each(m_right_tabs.begin(), m_right_tabs.end(), boost::bind(&CGuiTab::clear_tab, _1));
}




void CGuiTabFec::do_reading_fec()
	
{
	
	for (int i =0; i<fec.size(); i++)
		
	{
		
		
		std::ostringstream ss;
		ss << i+1;
		string cd = " \t Fec Number:  ";
		cd +=ss.str(); 
		
		std::cout << cd << std::endl;
		
		 fEdit->AddText(new TGText(cd.c_str()));
		 
		 for (int j=0; j<((fec[i]).size()); j++)
		 {
			 
			 
			 
			
			  string content_fec = "\t"+fec[i][j];
 			// std::ostringstream pluto;
			// pluto << j;
//  			// pluto << j << fec[i][j];
//  			d +=pluto.str();
 			fEdit->AddText(new TGText(content_fec.c_str()));
		 }
		
		
		
	}
	
 if (fec.size()==0) {
	 
	 fEdit->Clear();
	 fEdit->AddText(new TGText("\tNo Fec inserted yet"));}
	
}

void CGuiTabFec::open_save_fec()
	
{
	CGuiSaveFec *ed = new CGuiSaveFec(m_tab_frame, 200, 300);
	
	
}

void CGuiTabFec::remove_all_Fec()
	
{
	
	fec.erase (fec.begin(),fec.end());
	fec.clear();
	apv.erase (apv.begin(),apv.end());
	apv.clear();
	
	do_reading_fec();
	
	//qui devo trovare il modo di cancellare l'array!! Maybe overloading???		
	//riscrivo
	
			
	
}