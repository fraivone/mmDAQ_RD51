#ifndef browser_GuiSaveDet_h
#define browser_GuiSaveDet_h


#include <string>
class TGTransientFrame;
class TGWindow;
class TGTextButton;
class TGTextEntry;
class TGCheckButton;
class TGTextView;
class TGTextLabel;
class CGuiSaveDet

	 
{
	
public:
	
	
	CGuiSaveDet(TGWindow *main, int h , int z);
	~CGuiSaveDet();
	void CloseWindow();
	void open_type_chamber();
	void open_add_apv();
	void add_current_chamber();
	
	TGTransientFrame *fMain;
	TGTextEntry* 	type_det;
	TGTextEntry* 	name_chamber;
	std::string path_complete;
	

};




#endif