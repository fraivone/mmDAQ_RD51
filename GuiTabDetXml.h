

#ifndef browser_CGuiTabDetXml_h
#define browser_CGuiTabDetXml_h

#include "GuiTab.h"

class TRootEmbeddedCanvas;
class TCanvas;
class TH1F;
class TH2F;
class TGraph;
class TMultiGraph;
class TGTextView;
class TGTextButton;
class TGMainFrame;

namespace browser {
	
    class CDetBase;
   
    class CEventBrowser;
    class CEventBrowserOnline;
   
   class CGuiTabDetXml : public CGuiTab
   {
      
   public:
      CGuiTabDetXml(CGuiBase* gui_parent, CConfiguration* config, CEventBrowser* eb, TGTab* main_tabs, const std::string& name );
      virtual ~CGuiTabDetXml();
      virtual void do_construct_tab();
      virtual void do_update(const CDetBase* entry);
      virtual void do_clear_tab();
	  void do_reading_det();
	  void open_save_det();
	  void delete_all_det();
	  	  // void do_reading_fec();
// 	  void open_save_fec();
   private:
      TRootEmbeddedCanvas*	m_re_canvas; 
      TCanvas*             m_canvas;
	 
	  TGTextButton* add_det_button;

//      THStack*             m_qstack;
//      THStack*             m_tstack;
   };
   
} // namespace

#endif
