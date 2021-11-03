
#include "GuiTabDetXml.h"
#include "GuiSaveDet.h"
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
CGuiTabDetXml::CGuiTabDetXml(CGuiBase* gui_parent,
                                 CConfiguration* config, 
                                 CEventBrowser*   eb,
                                 TGTab* main_tabs, 
                                 const std::string& name) :
CGuiTab(gui_parent, config, eb, main_tabs,name)

{
   construct_tab();
}


CGuiTabDetXml::~CGuiTabDetXml()
{

}

void CGuiTabDetXml::do_construct_tab()
{
   
   
    m_tab_frame->SetLayoutManager(new TGVerticalLayout(m_tab_frame));
	
	
	TGHorizontalFrame* horizontalframe = new TGHorizontalFrame(m_tab_frame);
	 {
	TGTextView* fEdit2 = new TGTextView(horizontalframe, 300, 200);
	fEdit_det = fEdit2;
	//
	  TGLayoutHints* fL2 = new TGLayoutHints( kLHintsExpandX|kLHintsExpandY, 2, 2, 2, 2);
	   horizontalframe->AddFrame(fEdit_det, fL2);
	   
	   
	   TGVerticalFrame* verticalframe = new TGVerticalFrame(horizontalframe);
	   
	   {
	
	   add_det_button = new TGTextButton(verticalframe,"Add Chamber");
	   add_det_button->SetTextJustify(36);
	   add_det_button->SetMargins(0,0,0,0);
	   add_det_button->SetWrapLength(-1);
	   // add_det_button->Resize(100,25);
	   
	   add_det_button->Connect("Clicked()", "browser::CGuiTabDetXml", this, "open_save_det()");
	   
	   verticalframe->AddFrame(add_det_button, new TGLayoutHints( kLHintsCenterX|kLHintsCenterY, 3, 3, 3, 3));
	   
	   TGTextButton* remove_all_button = new TGTextButton(verticalframe,"Remove all");
	   remove_all_button->SetTextJustify(36);
	   remove_all_button->SetMargins(0,0,0,0);
	   remove_all_button->SetWrapLength(-1);
	   remove_all_button->Connect("Clicked()", "browser::CGuiTabDetXml", this, "delete_all_det()");
	   
	   
	   verticalframe->AddFrame(remove_all_button,  new TGLayoutHints( kLHintsCenterX|kLHintsCenterY, 3, 3, 3, 3));
	   
   		}
	  
	   horizontalframe->AddFrame(verticalframe,new TGLayoutHints(kLHintsCenterX|kLHintsCenterY,2,2,2,2)); 
	}
		m_tab_frame->AddFrame(horizontalframe, new TGLayoutHints( kLHintsExpandX|kLHintsExpandY, 2, 2, 2, 2));
		
		
		do_reading_det();
}
		  
		  


/**
 update currevt active tab with data from det element
 */
void CGuiTabDetXml::do_update(const CDetBase* detbase)
{
//   std::cout << "CGuiTabReadout1D::update(" <<detbase->get_name() << ")\n";
   const CDetReadoutBase* rd = dynamic_cast<const CDetReadoutBase*>(detbase);
   if (!rd) {
      return;
   }
   
}
void CGuiTabDetXml::do_clear_tab()
{
   std::for_each(m_right_tabs.begin(), m_right_tabs.end(), boost::bind(&CGuiTab::clear_tab, _1));
}


void CGuiTabDetXml::do_reading_det()
	
{


 if (det.size()==0) {

	 fEdit_det->Clear();
	 fEdit_det->AddText(new TGText("\tNo chambers inserted yet"));}

	
	
}

void CGuiTabDetXml::open_save_det()
{
   CGuiSaveDet *ed = new CGuiSaveDet(m_tab_frame, 500, 400);
   
   
   
}

void CGuiTabDetXml::delete_all_det()
{
	
	
	
	det.erase (det.begin(),det.end());
	apv_final.erase (apv_final.begin(),apv_final.end());
	connector_final.erase (connector_final.begin(),connector_final.end());
	det.clear();
	apv_final.clear();
	connector_final.clear();

	do_reading_det();
   
   
   
}

