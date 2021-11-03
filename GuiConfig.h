#ifndef browser_GuiConfig_h
#define browser_GuiConfig_h




#include <string>
#include <vector>

class TGTransientFrame;
class TGWindow;
class TGTextButton;
class TGTextEntry;
class TGCheckButton;
class TGListTreeItem;
class TGRadioButton;
class TGComboBox;
class TGListTree;
class TGNumberEntry;


class CGuiConfig

	 
{
	
public:
	
	
	CGuiConfig(TGWindow *main, int h , int z);
	~CGuiConfig();
	
	
	
	TGTransientFrame *fMain;
	
	void CloseWindow();
	void close();
    void gui_handle_tree_click(TGListTreeItem* entry, int num);
    void new_chamber();
    void save();
    void delete_element();
    void create_new_config();
    void write_to_file();
    void fill_remaining();
    std::string to_write();
    std::string to_read();
    void read_online_config();
    void update_tree();
    void current_id_entry();
    bool check_voltages();
    void modify();
    void move_focus();

    void disable_HV();
    void disable_all();
    void check_configuration();
    std::string conversion(int input);
    int deconversion(std::string input);
    void SetType(int num);

    TGComboBox* type;
    std::vector<TGTextEntry*> position_entry;
    std::vector<TGTextEntry*> gas_entry;
    std::vector<TGTextEntry*> HV_entry;
    std::vector<TGTextEntry*> mesh_entry;

    std::vector < std::vector<std::string > > chamber_vec;
    std::vector<std::string >  chamber_null;
    
    TGRadioButton        *fR[8];
    TGTextButton* save_button;
    TGTextButton* finish_button;
    TGTextButton* modify_button;
    TGTextButton* new_button;
    TGTextButton* delete_button;
    TGListTree* m_gui_tree_list;
    TGCheckButton* check_mysql;
    TGNumberEntry* id_entry;
    TGTextButton* current_id_button;

    int id1;
    int number_of_chamber;
    
    std::vector<std::string> name_chambers;
    std::vector<double> number_resistive;
    std::vector<double> number_drift;
    
    
};




#endif