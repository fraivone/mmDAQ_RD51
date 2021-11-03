

#ifndef browser_GuiTabSaveXml_h
#define browser_GuiTabSaveXml_h

#include "GuiTab.h"

class TRootEmbeddedCanvas;
class TCanvas;
class TH1F;
class TH2F;
class TGraph;
class TMultiGraph;
class TGTextView;
class TGTextEntry;
class TGTextButton;
class TGMainFrame;

namespace browser {
	
    class CDetBase;
   
    class CEventBrowser;
    class CEventBrowserOnline;
   
   class CGuiTabSaveXml : public CGuiTab
   {
      
   public:
      CGuiTabSaveXml(CGuiBase* gui_parent, CConfiguration* config, CEventBrowser* eb, TGTab* main_tabs, const std::string& name );
      virtual ~CGuiTabSaveXml();
      virtual void do_construct_tab();
      virtual void do_update(const CDetBase* entry);
      virtual void do_clear_tab();
	  void open_log_file();
	  void open_data_directory();
	  void save_config();
	void write_telescope();
	void write_srs();
	void write_tmm();

	 
   private:
      TRootEmbeddedCanvas*	m_re_canvas; 
      TCanvas*             m_canvas;
	 TGTextEntry* log_file_text;
	 TGTextEntry* data_folder;
	 
	std::string data_complete;
	std::string data_filename;
	std::string output_filename;
	std::string file_name;
	std::string log_filename;
	std::string log_complete;
	  

//      THStack*             m_qstack;
//      THStack*             m_tstack;
   };
   
} // namespace

#endif
