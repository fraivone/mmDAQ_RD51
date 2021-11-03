#ifndef GuiAddApv_h
#define GuiAddApv_h


#include <string>
class TGTransientFrame;
class TGWindow;
class TGTextButton;
class TGTextEntry;
class TGCheckButton;
class TGTextView;
class TGTextLabel;
class TGListBox;
class TGComboBox;

class CGuiAddApv

	 
{
	
public:
	
	
	CGuiAddApv(TGWindow *main, int h , int z);
	~CGuiAddApv();
	void CloseWindow();
	void save_apv_linked();
	void add_combo_apv();
	TGTransientFrame *fMain;
	TGTextEntry* name_connector;
	TGComboBox* apv_conn;

};
#endif