#ifndef browser_GuiSaveFec_h
#define browser_GuiSaveFec_h


#include <string>
class TGTransientFrame;
class TGWindow;
class TGTextButton;
class TGTextEntry;
class TGCheckButton;
class CGuiSaveFec

	 
{
	
public:
	
	
	CGuiSaveFec(TGWindow *main, int h , int z);
	~CGuiSaveFec();
	
	
	
	TGTransientFrame *fMain;
	
	std::string plug(int a, int b);
	void CloseWindow();
	void close();
	void AddFec();
	void select_all();
	TGCheckButton *channel0;
	TGCheckButton *channel1;
	TGCheckButton *channel2;
	TGCheckButton *channel3;
	TGCheckButton *channel4;
	TGCheckButton *channel5;
	TGCheckButton *channel6;
	TGCheckButton *channel7;
	TGCheckButton *channel8;
	TGCheckButton *channel9;
	TGCheckButton *channel10;
	TGCheckButton *channel11;
	TGCheckButton *channel12;
	TGCheckButton *channel13;
	TGCheckButton *channel14;
	TGCheckButton *channel15;
	TGCheckButton *selectall_button;
	
	
	TGTextButton *Add_Fec;
	TGTextButton *close_button;
	
	TGTextEntry *name_fec;
	TGTextEntry *adress;
};




#endif