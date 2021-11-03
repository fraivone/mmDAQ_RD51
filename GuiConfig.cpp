//
//  GuiConfig.cpp
//  eventbrowser
//
//  Created by Edoardo Farina on 03/04/2015.
//  Copyright (c) 2015 CERN - PH/ADE. All rights reserved.
//


#include "GuiConfig.h"
#include "ConfigXml.h"
#include "GuiMainFrameOnline.h"
#include "ConfigDatabase.h"

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
#include <TGListTree.h>
#include "GuiTabFec.h"
#include "TGListBox.h"
#include "TGComboBox.h"
#include "TGButtonGroup.h"
#include "TGNumberEntry.h"
#include <fstream>
#include "ConfigRun.h"
#include <TSQLServer.h>
#include <TSQLStatement.h>
#include <unistd.h>

#include <iostream>

#include <iostream>
#include <algorithm>
#include <fstream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <functional>
#include <TApplication.h>

using namespace browser;
CGuiConfig::CGuiConfig(TGWindow *main, int h , int z)
	
{
    
    number_of_chamber = 22;
    
    id1=0;
    TGLayoutHints* fL5 = new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0);
    TGLayoutHints* fL4 = new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0);
    TGLayoutHints* fL6 = new TGLayoutHints( 0, 10, 0, 0);
    TGLayoutHints* fL7 = new TGLayoutHints( 0, 100, 0, 0);
    TGLayoutHints* fL8 = new TGLayoutHints( 0, 100, 0, 0);
	
fMain = new TGTransientFrame(gClient->GetRoot(), main, 10, 10, kMainFrame | kVerticalFrame);


fMain->Connect("CloseWindow()", "CGuiConfig", this, "CloseWindow()");
fMain->DontCallClose();
fMain->SetCleanup(kDeepCleanup);	

    
    
   TGHorizontalFrame* m_gui_bottom_frame = new TGHorizontalFrame(fMain, 650, 200, kHorizontalFrame);
    

    
            //det tree
            //tree widget
            TGCanvas *m_gui_tree_canvas = new TGCanvas(m_gui_bottom_frame, 120, 500);
//            m_gui_tree_viewport = m_gui_tree_canvas->GetViewPort();
            // list tree
            m_gui_tree_list = new TGListTree(m_gui_tree_canvas, kHorizontalFrame);
            m_gui_tree_list->SetName("m_treelistbox");
            m_gui_tree_list->SetCheckMode(TGListTree::kRecursive);
            m_gui_tree_list->Connect("Clicked(TGListTreeItem*,Int_t)",
                                     "CGuiConfig",
                                     this,
                                     "gui_handle_tree_click(TGListTreeItem*,Int_t)");
//            m_gui_tree_list->Connect("Checked(TObject*, Bool_t)",
//                                     "browser::CGuiConfig",
//                                     this,
//                                     "gui_handle_tree_check(TObject*, Bool_t)");
    
            m_gui_tree_canvas->AddFrame(m_gui_tree_list);
//            m_gui_tree_list->SetLayoutManager(new TGHorizontalLayout(m_gui_tree_list));
//            m_gui_tree_list->MapSubwindows();
//            m_gui_tree_canvas->SetContainer(m_gui_tree_list);
//            m_gui_tree_canvas->MapSubwindows();
            m_gui_bottom_frame->AddFrame(m_gui_tree_canvas,
                                         new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandY, 0,0,0,0));
        

    

    TGVerticalFrame* frame_chamb = new TGVerticalFrame(m_gui_bottom_frame);
    // chamber grande
    
    TGHorizontalFrame* hor_fr = new TGHorizontalFrame(frame_chamb);
    // chamber grande

    
    TGGroupFrame* br = new TGGroupFrame(hor_fr, "Chambers",kVerticalFrame);

    {
    

        type = new TGComboBox(br);
        if (ConfigRun::GetInstance()->get_check_online()){
            
            
            int i = 1;
            //
            try {
                // check configuration create database connection
                
                std::stringstream cmd;
                cmd <<"mysql://"<<ConfigDatabase::GetInstance()->GetIp()<<"/"<<ConfigDatabase::GetInstance()->GetDatabaseName();
                
                TSQLServer* serv = TSQLServer::Connect(cmd.str().c_str(), ConfigDatabase::GetInstance()->GetUsername().c_str(), ConfigDatabase::GetInstance()->GetPassword().c_str());
                
                if (! serv) std::cout<<"------- Error connecting to the database. Contact the Server administrator ------- "<<std::endl;
                
                else {
                    
                    stringstream   sql_stream;
                    
                    sql_stream << "SELECT * FROM "<< "MMChamber";
                    usleep(5000);
                    
                    TSQLStatement* stmt = serv->Statement(sql_stream.str().c_str(), 4000);
                    
                    if (stmt->Process()) {
                        stmt->StoreResult();
                        
                        if (stmt->GetNumFields()>0){
                            while (stmt->NextResultRow()) {
                                
                                type->AddEntry(TString(stmt->GetString(1)), i);
                                name_chambers.push_back(stmt->GetString(1));
                                number_resistive.push_back(stmt->GetDouble(3));
                                number_drift.push_back(stmt->GetDouble(2));

                                i++;

                            }
                            
                            
                        }
                    }
                }
            }
            catch (int e) {
                std::cout << "An exception occurred. Exception Nr. " << e << '\n';
            }
            
        }
        

        type->Resize(150,20);
        type->SetEnabled(false);
        br->AddFrame(type, fL4);
        
        type->Connect("Selected(Int_t)", "CGuiConfig", this, "SetType(Int_t)");

    }
    hor_fr->AddFrame(br);
    
    
    TGGroupFrame* id_frame = new TGGroupFrame(hor_fr, "ID",kHorizontalFrame);

    id_entry = new TGNumberEntry(id_frame, 0, 9,999, TGNumberFormat::kNESInteger,
                                                              TGNumberFormat::kNEAAnyNumber,
                                                              TGNumberFormat::kNELLimitMinMax,
                                 0, 99999);
    id_entry->Resize(60, 20);
    id_entry->Connect("ValueSet(Long_t)", "CGuiConfig", this, "read_online_config()");
    id_frame->AddFrame(id_entry , new TGLayoutHints(kLHintsTop |kLHintsRight, 0, 0, 0, 0));

    id_entry->SetNumber(ConfigRun::GetInstance()->GetSetup() );

        
    current_id_button = new TGTextButton(id_frame, "&Current ID");
    current_id_button->Resize(60, 20);
    current_id_button->Connect("Clicked()", "CGuiConfig", this, "current_id_entry()");
    id_frame->AddFrame(current_id_button, new TGLayoutHints( kLHintsTop|kLHintsRight , 10, 10, 0, 0));
        
    hor_fr->AddFrame(id_frame, new TGLayoutHints(kLHintsTop | kLHintsRight , 0, 0, 0, 0));

    
    
    frame_chamb->AddFrame(hor_fr,new TGLayoutHints(kLHintsTop | kLHintsExpandX , 0, 0, 0, 0));
    
    

    
    // tensioni
    
    TGGroupFrame* br2 = new TGGroupFrame(frame_chamb, "HV",kHorizontalFrame); {

        
        TGGroupFrame* drift = new TGGroupFrame(br2, "Drift",kHorizontalFrame); {

        
            for (int j=0; j<4; j++){
                
                HV_entry.push_back(new TGTextEntry(drift));
                HV_entry[j]->Resize(30,15);
                HV_entry[j]->SetEnabled(false);
                
            }
        

            for (int j=0; j<4; j++){
        
                drift->AddFrame(HV_entry[j], fL4);
        
            }
            
        }
          //end drift
        
        br2->AddFrame(drift,fL4);
        
        TGGroupFrame* readout_frame = new TGGroupFrame(br2, "Readout",kVerticalFrame); {
            
            
            TGHorizontalFrame* pcb1 = new TGHorizontalFrame(readout_frame); {

            
            
            for (int j=4; j<12; j++){
                
                HV_entry.push_back(new TGTextEntry(pcb1));
                HV_entry[j]->Resize(30,15);
                HV_entry[j]->SetEnabled(false);
                
            }
            
            
            for (int j=4; j<12; j++){
                
                pcb1->AddFrame(HV_entry[j], fL4);
                
            }
            }
            
            readout_frame->AddFrame(pcb1,fL4);
            

            
            ////////pcb2
            TGHorizontalFrame* pcb2 = new TGHorizontalFrame(readout_frame); {
                
                
                
                for (int j=12; j<20; j++){
                    
                    HV_entry.push_back(new TGTextEntry(pcb2));
                    HV_entry[j]->Resize(30,15);
                    HV_entry[j]->SetEnabled(false);
                    
                }
                
                
                for (int j=12; j<20; j++){
                    
                    pcb2->AddFrame(HV_entry[j], fL4);
                    
                }
            }
            
            readout_frame->AddFrame(pcb2,fL4);
            
            //////pcb3
            TGHorizontalFrame* pcb3 = new TGHorizontalFrame(readout_frame); {
                
                
                
                for (int j=20; j<28; j++){
                    
                    HV_entry.push_back(new TGTextEntry(pcb3));
                    HV_entry[j]->Resize(30,15);
                    HV_entry[j]->SetEnabled(false);
                    
                }
                
                
                for (int j=20; j<28; j++){
                    
                    pcb3->AddFrame(HV_entry[j], fL4);
                    
                }
            }
            
            readout_frame->AddFrame(pcb3,fL4);
            
            
            
        }
        //
        
        br2->AddFrame(readout_frame,fL4);
        
    //
} //end tensione
    
    
    for(int i=0; i<27; i++)
        HV_entry[i]->Connect("TabPressed()","TGTextEntry",HV_entry[i+1],"SetFocus()");
    
    
    frame_chamb->AddFrame(br2,fL4);

    
    //start  2
    TGHorizontalFrame* pos_frame = new TGHorizontalFrame(frame_chamb); {
         
    TGGroupFrame* br3 = new TGGroupFrame(pos_frame, "Position",kVerticalFrame); {
        
        std::string position_label[3] = {"Pos X", "Pos Y", "Pos Z"};
        
        for (int j=0; j<3; j++){
            
            
            TGGroupFrame* br4 = new TGGroupFrame(br3, position_label[j].c_str(),kVerticalFrame); {
            
            position_entry.push_back(new TGTextEntry(br4));
            
            position_entry[j]->Resize(30,15);
            position_entry[j]->SetEnabled(false);
            br4->AddFrame(position_entry[j],fL4);
                br3->AddFrame(br4);
            }
            
        }
        } //end position


         pos_frame->AddFrame(br3);
         
         
         TGGroupFrame* angle_frame = new TGGroupFrame(pos_frame, "Angle",kVerticalFrame); {
             std::string angle_label[3] = {"X-Rot", "Y-Rot", "Z-Rot"};
             
             for (int j=3; j<6; j++){
                 
                 TGGroupFrame* br33 = new TGGroupFrame(angle_frame, angle_label[j-3].c_str(),kVerticalFrame); {

                 position_entry.push_back(new TGTextEntry(br33));
                 position_entry[j]->Resize(30,15);
                 position_entry[j]->SetEnabled(false);
                 br33->AddFrame(position_entry[j],fL4);
                     angle_frame->AddFrame(br33);
             }
         } //end position
        }
         pos_frame->AddFrame(angle_frame);
 
         
         for(int i=0; i<5; i++)
             position_entry[i]->Connect("TabPressed()","TGTextEntry", position_entry[i+1],"SetFocus()");
         
         TGGroupFrame* gas_frame = new TGGroupFrame(pos_frame, "Gas Mixture",kVerticalFrame); {
             std::string gas_label[3]= {"Argon", "CO2"};
             
             for (int j=0; j<2; j++){
                 
                 TGGroupFrame* br33 = new TGGroupFrame(gas_frame, gas_label[j].c_str(),kVerticalFrame); {
                     
                     gas_entry.push_back(new TGTextEntry(br33));
                     gas_entry[j]->Resize(30,15);
                     gas_entry[j]->SetEnabled(false);
                     br33->AddFrame(gas_entry[j],fL4);
                     gas_frame->AddFrame(br33);
                 }
             } //end position
         }
         pos_frame->AddFrame(gas_frame);
         
         for(int i=0; i<1; i++)
             gas_entry[i]->Connect("TabPressed()","TGTextEntry", gas_entry[i+1],"SetFocus()");
         
         
         
         TGGroupFrame* mesh_frame = new TGGroupFrame(pos_frame, "Mesh Type",kVerticalFrame); {
             std::string mesh_label[3] = {"Wire-d", "Opening"};
             
             for (int j=0; j<2; j++){
                 
                 TGGroupFrame* br33 = new TGGroupFrame(mesh_frame, mesh_label[j].c_str(),kVerticalFrame); {
                     
                     mesh_entry.push_back(new TGTextEntry(br33));
                     mesh_entry[j]->Resize(30,15);
                     mesh_entry[j]->SetEnabled(false);
                     br33->AddFrame(mesh_entry[j],fL4);
                     mesh_frame->AddFrame(br33);
                 }
             } //end position
         }
         pos_frame->AddFrame(mesh_frame);
         
         for(int i=0; i<1; i++)
             mesh_entry[i]->Connect("TabPressed()","TGTextEntry", mesh_entry[i+1],"SetFocus()");
         
         
     }
    
    
    
    frame_chamb->AddFrame(pos_frame,fL4);

    TGHorizontalFrame* hor_frame2 = new TGHorizontalFrame(frame_chamb); {

    
        TGGroupFrame* br4 = new TGGroupFrame(hor_frame2,"Electronics",kVerticalFrame);{
    //        groupFrame->SetTitlePos(TGGroupFrame::kLeft);
    
    
    TGButtonGroup* electr_butt = new TGButtonGroup(br4);
    
    fR[0] = new TGRadioButton(electr_butt,new TGHotString("&APV"));
    fR[1] = new TGRadioButton(electr_butt,new TGHotString("&VMM_Mini2 "));
    fR[2] = new TGRadioButton(electr_butt,new TGHotString("&VMM_MMF8"));
    fR[3] = new TGRadioButton(electr_butt,new TGHotString("&Others "));
            electr_butt->Show();

//    electr_butt->Connect("Clicked(Int_t)", "browser::CGuiTabConfig", this, "set_electr(Int_t)");

    br4->AddFrame(electr_butt, fL4);
    
        hor_frame2->AddFrame(br4);
        
        }
        
        
        TGGroupFrame* br5 = new TGGroupFrame(hor_frame2,"Zebra connector",kVerticalFrame);{
            //        groupFrame->SetTitlePos(TGGroupFrame::kLeft);
            
            
            TGButtonGroup* electr_butt = new TGButtonGroup(br5);
            
            fR[4] = new TGRadioButton(electr_butt,new TGHotString("&Zeta-Axis"));
            fR[5] = new TGRadioButton(electr_butt,new TGHotString("&Fujipoli "));
            fR[6] = new TGRadioButton(electr_butt,new TGHotString("&Not Present "));
            fR[7] = new TGRadioButton(electr_butt,new TGHotString("&Other "));

            electr_butt->Show();
            
            //    electr_butt->Connect("Clicked(Int_t)", "browser::CGuiTabConfig", this, "set_electr(Int_t)");
            
            br5->AddFrame(electr_butt, fL4);
            
            hor_frame2->AddFrame(br5);
            
        }
        
        
    }
    
    frame_chamb->AddFrame(hor_frame2,fL4);

 
    {
        //start run button
        new_button = new TGTextButton(frame_chamb, "&NEW CHAMBER");
        new_button->Resize(110, 22);
        new_button->Connect("Clicked()", "CGuiConfig", this, "new_chamber()");
        frame_chamb->AddFrame(new_button, new TGLayoutHints( kLHintsTop , 10, 10, 0, 0));
    }
    
    
    {
       
        save_button = new TGTextButton(frame_chamb, "&SAVE CHAMBER");
        save_button->Resize(110, 22);
        save_button->Connect("Clicked()", "CGuiConfig", this, "save()");
        frame_chamb->AddFrame(save_button, new TGLayoutHints( kLHintsTop , 10, 10, 0, 0));
    }
    
    {
        
        modify_button = new TGTextButton(frame_chamb, "&MODIFY CHAMBER");
        modify_button->Resize(150, 22);
        modify_button->Connect("Clicked()", "CGuiConfig", this, "modify()");
        frame_chamb->AddFrame(modify_button, new TGLayoutHints( kLHintsTop , 10, 10, 0, 0));
    }
    
    {
        //start run button
        delete_button = new TGTextButton(frame_chamb, "&DELETE CHAMBER");
        delete_button->Resize(110, 22);
        delete_button->Connect("Clicked()", "CGuiConfig", this, "delete_element()");
        frame_chamb->AddFrame(delete_button, new TGLayoutHints( kLHintsTop , 10, 10, 0, 0));
    }
    
    {
      
        finish_button = new TGTextButton(frame_chamb, "&CHECK/ADD CONFIGURATION");
        finish_button->Resize(150, 22);
        finish_button->Connect("Clicked()", "CGuiConfig", this, "check_configuration()");
        frame_chamb->AddFrame(finish_button, new TGLayoutHints( kLHintsTop , 10, 10, 0, 0));
    }
    {
        check_mysql = new TGCheckButton(frame_chamb, "Enable ONLINE check");
        check_mysql->Resize(122, 22);
        check_mysql->SetEnabled(false);
//        check_mysql->SetDisabledAndSelected(ConfigRun::GetInstance()->get_check_online());
        if (ConfigRun::GetInstance()->get_check_online())
        check_mysql->SetState(kButtonDown);
        
        check_mysql->SetEnabled(false);

        frame_chamb->AddFrame(check_mysql, new TGLayoutHints(kLHintsLeft, 0, 0, 0, 27));

    }
    
    
    
    save_button->SetEnabled(false);
    delete_button->SetEnabled(false);
    finish_button->SetEnabled(false);
    modify_button->SetEnabled(false);

    
    

    m_gui_bottom_frame->AddFrame(frame_chamb, new TGLayoutHints(kLHintsLeft | kLHintsExpandY |kLHintsExpandX, 2, 2, 2, 2));
    fMain->AddFrame(m_gui_bottom_frame, new TGLayoutHints(kLHintsLeft| kLHintsExpandY | kLHintsExpandX, 2, 2, 2, 2));


    fMain->SetWindowName("Chambers");
    fMain->MapSubwindows();

    fMain->MapWindow();
    fMain->MoveResize(200, 0, 700, 600);
    
    disable_all();
//

    read_online_config();

    

    
}



void CGuiConfig::CloseWindow()
{
delete fMain;
    
}


void CGuiConfig::check_configuration()
{
    
    if (ConfigRun::GetInstance()->get_check_online()){
    
        id1=0;
//    
    try {
        // check configuration create database connection
        
        std::stringstream cmd;
        cmd <<"mysql://"<<ConfigDatabase::GetInstance()->GetIp()<<"/"<<ConfigDatabase::GetInstance()->GetDatabaseName();
        
        TSQLServer* serv = TSQLServer::Connect(cmd.str().c_str(), ConfigDatabase::GetInstance()->GetUsername().c_str(), ConfigDatabase::GetInstance()->GetPassword().c_str());
        
        if (! serv) std::cout<<"------- Error connecting to the database. Contact the Server administrator ------- "<<std::endl;
        
        else {
        


        string ss = to_read();

        
            
//        std::cout << "String to read on the database:" << ss.c_str() << std::endl;
        
        
        usleep(5000);

        TSQLStatement* stmt = serv->Statement(ss.c_str(), 4000);

        if (stmt->Process()) {
            // store result of statement in buffer
            stmt->StoreResult();
            
            // display info about selected field
//            cout << "NumFields = " << stmt->GetNumFields() << endl;
//            for (int n=0;n<stmt->GetNumFields();n++)
//                cout << "Field " << n << "  = " << stmt->GetFieldName(n) << endl;
            
            if (stmt->GetNumFields()>0){
            while (stmt->NextResultRow()) {
                
            id1 = stmt->GetDouble(0);
                std::cout << "id1:  " << id1<< std::endl;
            }
            
                if (id1==0) {
               
                    std::cout << "THE CONFIGURATION HAS NOT BEEN FOUND ---- PROCEED TO CREATE A NEW ONE" << std::endl;
    ///////////////////////////START CREATION
                    
                    
                    create_new_config();
                    
        
     ////////////END CREATION
              }
            
                else {
                
                
                    ConfigRun::GetInstance()->setSetup(id1);
                    ConfigRun::GetInstance()->check_complete();
                
                }
            }
        }
    }
    }//finish try
        catch (int e) {
          std::cout << "An exception occurred. Exception Nr. " << e << '\n';
    } //finish catch

//
//
        } //end if clause
    
    
    
    //inizia scritturq su file
    
        else {
            
            
            write_to_file();
        
            
            }
    
    id_entry->SetNumber( ConfigRun::GetInstance()->GetSetup());
    ULong_t green;
    gClient->GetColorByName("green", green);
    current_id_button->TGWindow::SetBackgroundColor(green);
    gClient->NeedRedraw(current_id_button);
    
}

void CGuiConfig::delete_element()
{
    std::string to_delete = m_gui_tree_list->GetSelected()->GetText();
    
    std::vector<std::string>::iterator itr;
    size_t n;
    for(n = 0; n < chamber_vec.size(); ++n)
    {
        itr = find(chamber_vec[n].begin(), chamber_vec[n].end(), to_delete);
        if(chamber_vec[n].end() != itr)
            break;
    }
    
    
    chamber_vec.erase(n+chamber_vec.begin());
    update_tree();
    delete_button->SetEnabled(false);
    if (chamber_vec.size()==0){
        modify_button->SetEnabled(false);
        delete_button->SetEnabled(false);
        finish_button->SetEnabled(false);}
    

}


void CGuiConfig::modify()
{

    std::string to_delete = m_gui_tree_list->GetSelected()->GetText();

    std::vector<std::string>::iterator itr;
    size_t n;
    for(n = 0; n < chamber_vec.size(); ++n)
    {
        itr = find(chamber_vec[n].begin(), chamber_vec[n].end(), to_delete);
        if(chamber_vec[n].end() != itr)
            break;
    }

    
    chamber_vec.erase(n+chamber_vec.begin());
    delete_button->SetEnabled(false);
    new_button->SetEnabled(false);
    save_button->SetEnabled(true);
    modify_button->SetEnabled(false);
    finish_button-> SetEnabled(false);
   
    type->SetEnabled(false);
    
    for (int i=0; i<6; i++){
        
        position_entry[i]->SetEnabled(true);
    }

    for (int i=0; i<8; i++){
        fR[i]->SetEnabled(true);
    }
    for (int i=0; i<2; i++){
        mesh_entry[i]->SetEnabled(true);
    }
    for (int i=0; i<2; i++){
        gas_entry[i]->SetEnabled(true);
    }
    
    
   
        
        
        
    for (int i=0; i<28; i++){
       
        HV_entry[i]->SetEnabled(false);
        
    }
    
    std::vector<std::string>::iterator it = find (name_chambers.begin(), name_chambers.end(), type->GetSelectedEntry()->GetTitle());
    double id = it- name_chambers.begin();
    
    for (int i=0; i<number_drift[id]; i++){
        HV_entry[i]->SetText("");
        HV_entry[i]->SetEnabled(true);
        
    }
    
    
    for (int i=0; i<number_resistive[id]; i++){
        HV_entry[i+4]->SetText("");
        HV_entry[i+4]->SetEnabled(true);
        
    }
    
    
}

void CGuiConfig::write_to_file()

{
    
    /////////////////
       ////////////////////
    
    ofstream myfile;
    myfile.open ("Output.txt", ios::ate |ios::app);
   
    string read = to_read();
    string write = to_write();
    
    myfile <<(read) << "\n";
    myfile <<(write) << "\n";
    
    myfile.close();
    
    
    id1=-1;
    
    ConfigRun::GetInstance()->setSetup(id1);
    ConfigRun::GetInstance()->check_complete();
    
}



void CGuiConfig::read_online_config()
{

    if (ConfigRun::GetInstance()->get_check_online() &&  id_entry->GetNumberEntry()->GetIntNumber()!=0 && id_entry->GetNumberEntry()->GetIntNumber()!=-1){

    
    
    try {
    
    

        // check configuration create database connection
        std::stringstream cmd;
        cmd <<"mysql://"<<ConfigDatabase::GetInstance()->GetIp()<<"/"<<ConfigDatabase::GetInstance()->GetDatabaseName();
        
        TSQLServer* serv = TSQLServer::Connect(cmd.str().c_str(), ConfigDatabase::GetInstance()->GetUsername().c_str(), ConfigDatabase::GetInstance()->GetPassword().c_str());
        
        if (! serv) std::cout<<"------- Error connecting to the database. Contact the Server administrator ------- "<<std::endl;
        
        else {
        stringstream ss;
        
        ss << "SELECT * FROM "<<ConfigDatabase::GetInstance()->GetConfigurationTableName().c_str()<< " WHERE ID = " << id_entry->GetNumberEntry()->GetIntNumber();
        
//        std::cout << "String to read on the database:" << ss.str().c_str() << std::endl;
        
        
        usleep(5000);
        
        TSQLStatement* stmt = serv->Statement(ss.str().c_str(), 4000);
            std::vector <std::string > read_db;
        if (stmt->Process()) {
                        // store result of statement in buffer
            stmt->StoreResult();
            // display info about selected field
//            cout << "NumFields = " << stmt->GetNumFields() << endl;
            //            for (int n=0;n<stmt->GetNumFields();n++)
            //                cout << "Field " << n << "  = " << stmt->GetFieldName(n) << endl;
            
            if (stmt->GetNumFields()>0){
                while (stmt->NextResultRow()) {

                    id1 = stmt->GetDouble(0);
                    std::cout << "id1:  " << id1<< std::endl;

                    chamber_vec.clear();
                    finish_button->SetEnabled(true);
                    //MMSW1
                    for (int k = 0; k<stmt->GetNumFields(); k++){
                        if(stmt->GetString(k)!="" && stmt->GetString(k)!="0")
                        read_db.push_back(stmt->GetString(k));}
                    
                    for(int s=0; s<name_chambers.size(); s++){
                        std::vector <std::string> temp;
                        std::vector<std::string>::iterator  it = find (read_db.begin(), read_db.end(), name_chambers[s]);
                        
                        if (it != read_db.end()){
                            double pos = it-read_db.begin();
                            
                            int l;
                            
                            temp.push_back(read_db[pos]);

                            for(l=0; l<number_drift[s]; l++)
                            {
                                temp.push_back(read_db[pos+l+1]);
                            }
                            while (l<4){
                                temp.push_back("");
                                l++;
                            }
                            for(l=0; l<number_resistive[s]; l++)
                            {
                                temp.push_back(read_db[pos+l+number_drift[s]+1]);
                            }
                            while (l<24){
                                temp.push_back("");
                                l++;
                            }
                            
                            for(l=0; l<12; l++)
                            {
                                temp.push_back(read_db[pos+l+number_drift[s]+number_resistive[s]+1]);
                            }
                            
   
                            chamber_vec.push_back(temp);

                        }
                        

                    }


                    
//                    for (int k=0; k<2; k++) {
//                    if (TString(stmt->GetString(1+25*k))!=""){
//                        vector<string> temp;
//                        for (int i=1+25*k; i<14+25*k; i++){
//                            temp.push_back(stmt->GetString(i));}
//                        for (int i=14; i<30; i++){
//                            temp.push_back("");}
//                        for (int i=14+25*k; i<26+25*k; i++){
//                            temp.push_back(stmt->GetString(i));}
//                            
//                        chamber_vec.push_back(temp);
//                    
//                    }
//                    }
//                    //MMSW2
//           
//                    
//
//                    //T2 and all the other
//                    for(int j=0; j<20; j++){
//                    if (TString(stmt->GetString(51+15*j))!=""){
//                        vector<string> temp;
//                        
//                        for (int i=51+15*j; i<53+15*j; i++){
//                            temp.push_back(stmt->GetString(i));}
//                        for (int i=0; i<3; i++){
//                            temp.push_back("");}
//                        temp.push_back(stmt->GetString(53+15*j));
//                        for (int i=0; i<23; i++){
//                            temp.push_back("");}
//                        for (int i=54+15*j; i<66+15*j; i++){
//                            temp.push_back(stmt->GetString(i));}
//                        
//                        chamber_vec.push_back(temp);
//                    }
//                    }
//                    if (TString(stmt->GetString(351))!=""){
//                        vector<string> temp;
//                        for (int i=351; i<392; i++){
//
//                            temp.push_back(stmt->GetString(i));}
//                        
//
////                        for (int i=0; i<temp.size(); i++)
////                            std::cout <<temp[i] << std::endl;
//                    }

            
                    }
                }
            }
            }
        } //end try
        
        
    
    catch (int e) {
        std::cout << "An exception occurred. Exception Nr. " << e << '\n';
    }
    
    update_tree();
    /////////////////
        
    } // end if
    
}




void CGuiConfig::create_new_config()
{
    
    try {
    
        std::stringstream cmd;
        cmd <<"mysql://"<<ConfigDatabase::GetInstance()->GetIp()<<"/"<<ConfigDatabase::GetInstance()->GetDatabaseName();
        
        TSQLServer* serv = TSQLServer::Connect(cmd.str().c_str(), ConfigDatabase::GetInstance()->GetUsername().c_str(), ConfigDatabase::GetInstance()->GetPassword().c_str());
        
        if (! serv) std::cout<<"------- Error connecting to the database. Contact the Server administrator ------- "<<std::endl;
        
        else {
    string ss;
    ss = to_write();
    
//    std::cout << ss << std::endl;

    serv->Query(ss.c_str());
    
    
    string ss1 = to_read();
    
    usleep(5000);
    
    TSQLStatement* stmt = serv->Statement(ss1.c_str(), 4000);
    
    if (stmt->Process()) {
        // store result of statement in buffer
        stmt->StoreResult();
        
        // display info about selected field
//        cout << "NumFields = " << stmt->GetNumFields() << endl;
        //            for (int n=0;n<stmt->GetNumFields();n++)
        //                cout << "Field " << n << "  = " << stmt->GetFieldName(n) << endl;
        
        if (stmt->GetNumFields()>0){
            while (stmt->NextResultRow()) {
                
                int  id1 = stmt->GetDouble(0);
                
                
                ConfigRun::GetInstance()->setSetup(id1);

                ConfigRun::GetInstance()->check_complete();
                
            }
        }
    }
        }
    }//end try
    
    catch (int e) {
        std::cout << "An exception occurred. Exception Nr. " << e << '\n';
    }
    
    }



std::string CGuiConfig::to_read()
{
    
    std::stringstream sql_stream;

    fill_remaining();
     sql_stream << "SELECT * FROM "<<ConfigDatabase::GetInstance()->GetConfigurationTableName().c_str()<< " WHERE ";
    
    for (int i=0; i<chamber_vec.size(); i++){
        
        
        sql_stream << chamber_vec[i][0] << " = " << "'" << chamber_vec[i][0] << "'" << " AND ";
        
        
        //HV for MMSW
        
        if (chamber_vec[i][0]=="MMSW1" || chamber_vec[i][0]=="MMSW2"){
         for (int j = 1; j < 5 ; j++){
        
        
            sql_stream  << chamber_vec[i][0] << "_HV_DR_" << j << " = " << "'" << chamber_vec[i][j] << "'" << " AND ";
        
            }
        

        
        for (int j = 5; j < 9 ; j++){
            
                
                sql_stream << chamber_vec[i][0]<< "_HV_RO_R" << j-4 << " = " << "'" << chamber_vec[i][j] << "'" << " AND ";
            
       }
    
        for (int j = 9; j < 13 ; j++){
           
                sql_stream << chamber_vec[i][0]<< "_HV_RO_L" << j-8 << " = " << "'" << chamber_vec[i][j] << "'" << " AND ";
            
            }
        }
         //end HV
        
        //START Other chamber
        
        
        //////LM2
        
        
       else if (chamber_vec[i][0]=="LM2"){
        
           for (int j = 1; j < 5 ; j++){
               
               sql_stream  << chamber_vec[i][0]<< "_HV_DR_" << j << " = " << "'" << chamber_vec[i][j] << "'" << " AND ";
               
           }
           
           for (int j = 5; j < 9 ; j++){
               
               sql_stream  << chamber_vec[i][0]<< "_HV_RO_PCB1_R" << j-4 <<" = " << "'" << chamber_vec[i][j] << "'" << " AND ";
               
           }
           
           for (int j = 9; j < 13 ; j++){
               
               sql_stream << chamber_vec[i][0]<< "_HV_RO_PCB1_L" << j-8 <<" = " << "'" << chamber_vec[i][j] << "'" << " AND ";
               
           }
           //end pcb1
           
           for (int j = 13; j < 17 ; j++){
               
               sql_stream  << chamber_vec[i][0]<< "_HV_RO_PCB2_R" << j-12 <<" = " << "'" << chamber_vec[i][j] << "'" << " AND ";
               
           }
           
           for (int j = 17; j < 21 ; j++){
               
               sql_stream << chamber_vec[i][0]<< "_HV_RO_PCB2_L" << j-16 << " = " << "'" << chamber_vec[i][j] << "'" << " AND ";
               
           }
           
           for (int j = 21; j < 25 ; j++){
               
               sql_stream << chamber_vec[i][0]<< "_HV_RO_PCB3_R" << j-20 << " = " << "'" << chamber_vec[i][j] << "'" << " AND ";
               
           }
           
           for (int j = 25; j < 29 ; j++){
               
               sql_stream << chamber_vec[i][0]<< "_HV_RO_PCB3_L" << j-24 << " = " << "'" << chamber_vec[i][j] << "'" << " AND ";
               
           }
           

        //endLM2
        
        }
        
        
        
//        else {
//        
//            sql_stream << chamber_vec[i][0]<< "_HV_DR_1 = " << "'" << chamber_vec[i][1] << "'" << " AND ";
//            sql_stream << chamber_vec[i][0]<< "_HV_RO_1 = " << "'" << chamber_vec[i][5] << "'" << " AND ";
//        }
        

         else {
           std::vector<std::string>::iterator  it = find (name_chambers.begin(), name_chambers.end(), chamber_vec[i][0].c_str());
           double pos = it- name_chambers.begin();
        
        
          for (int j=0; j< number_drift[pos]; j++)
            sql_stream << chamber_vec[i][0]<< "_HV_DR_" <<j+1 << " = " << "'" << chamber_vec[i][j+1] << "'" << " AND ";
          for (int j=0; j< number_resistive[pos]; j++)
            sql_stream << chamber_vec[i][0]<< "_HV_RO_" <<j+1 << " = " << "'" << chamber_vec[i][j+5] << "'" << " AND ";
       }  
        
        std::string position[3] = {"X","Y","Z"};
        
        for (int j = 29; j < 32 ; j++){
            
            sql_stream << chamber_vec[i][0] << "_POS_"<< position[j-29] << " = "  << chamber_vec[i][j] << " AND ";
            
        }
        
        std::string angle_position[3] =  {"XROT","YROT","ZROT"};
        //ANGLE
        for (int j = 33; j < 36 ; j++){
            
            sql_stream << chamber_vec[i][0] << "_ANGLE_"<< angle_position[j-33] << " = "  << chamber_vec[i][j] << " AND ";
            
        }
        
        sql_stream << chamber_vec[i][0] << "_ZEBRA_CONNECTOR_TYPE" << " = '"  << chamber_vec[i][36] << "' AND ";
        

        
        sql_stream << chamber_vec[i][0] << "_ARGON"<<  " = "  << chamber_vec[i][37] << " AND ";
        sql_stream << chamber_vec[i][0] << "_CO2"<<  " = "  << chamber_vec[i][38] << " AND ";

        sql_stream << chamber_vec[i][0] << "_MESHWIRE"<<  " = "  << chamber_vec[i][39] << " AND ";
        sql_stream << chamber_vec[i][0] << "_OPENING"<<  " = "  << chamber_vec[i][40] << " AND ";

   
        
    } //end for i

    
    for (int i=0; i<chamber_vec.size()-1; i++){
        
        
        sql_stream  << chamber_vec[i][0] << "_Electronics = " << "'" << chamber_vec[i][32] << "' AND ";
        
    }
    
    
    
    for (int g=0; g<chamber_null.size(); g++) {
        
        sql_stream << chamber_null[g] << " = " << "'" << "'" << " AND ";
        
    }
    
    
    sql_stream  << chamber_vec[chamber_vec.size()-1][0] << "_Electronics" << " = " << "'" << chamber_vec[chamber_vec.size()-1][32] << "'";
    
    
    return sql_stream.str();
    
}


CGuiConfig::~CGuiConfig()
{

    delete fMain;
}

void CGuiConfig::fill_remaining()
{
    chamber_null.clear();
    vector<string> chamber_;
    
    std::vector<string>::iterator it;
    vector<string> chamber_inserted;
    
    for (int j=0; j<chamber_vec.size(); j++) {
        
        chamber_inserted.push_back(chamber_vec[j][0]);
    }
    
    
    
    for (int i=0; i<name_chambers.size(); i++){
        
    it = std::find (chamber_inserted.begin(), chamber_inserted.end(), name_chambers[i]);
    
        if (it == chamber_inserted.end()){
            vector<string> temp;
            temp.push_back(name_chambers[i]);
            chamber_null.push_back(name_chambers[i]);
        }

    }
    
    
    for (int j=0; j<chamber_null.size(); j++) {
//        std::cout << chamber_null[j] << std::endl;
    }
    
    
}


std::string CGuiConfig::to_write (){


    stringstream s1;
    
    s1 << "INSERT INTO "<<ConfigDatabase::GetInstance()->GetConfigurationTableName().c_str()<< " (";
    for (int i =0; i<chamber_vec.size(); i++){
        
        
        s1 << chamber_vec[i][0] <<" , " ;
        
        std::cout << chamber_vec[i][0] <<std::endl;
        if (chamber_vec[i][0]=="MMSW1" || chamber_vec[i][0]=="MMSW2"){
            
            for (int j = 1; j < 5 ; j++){
                
                s1 << chamber_vec[i][0] << "_HV_DR_" << j << ",";
                
            }

            for (int j = 5; j < 9 ; j++){
                
                s1 << chamber_vec[i][0]<< "_HV_RO_R" << j-4 << ",";
                
            }

            for (int j = 9; j < 13 ; j++){
                
                s1 << chamber_vec[i][0] << "_HV_RO_L" << j-8 << ",";
                
            }
        }

        
        else if (chamber_vec[i][0]=="LM2"){
            
        
            for (int j = 1; j < 5 ; j++){
                
                s1 << chamber_vec[i][0]<< "_HV_DR_" << j << ",";
                
            }
            
            for (int j = 5; j < 9 ; j++){
                
                s1 << chamber_vec[i][0]<< "_HV_RO_PCB1_R" << j-4 << ",";
                
            }
            
            for (int j = 9; j < 13 ; j++){
                
                s1 << chamber_vec[i][0] << "_HV_RO_PCB1_L" << j-8 << ",";
                
            }
            //end pcb1
            
            for (int j = 13; j < 17 ; j++){
                
                s1 << chamber_vec[i][0]<< "_HV_RO_PCB2_R" << j-12 << ",";
                
            }
            
            for (int j = 17; j < 21 ; j++){
                
                s1  << chamber_vec[i][0]<< "_HV_RO_PCB2_L" << j-16 << ",";
                
            }
            
            for (int j = 21; j < 25 ; j++){
                
                s1 << chamber_vec[i][0]<< "_HV_RO_PCB3_R" << j-20 << ",";
                
            }
            
            for (int j = 25; j < 29 ; j++){
                
                s1 << chamber_vec[i][0]<< "_HV_RO_PCB3_L" << j-24 << ",";
                
            }
            
            
            
            
        }
        
//        else {
//            s1 << chamber_vec[i][0] << "_HV_DR_1, ";
//            s1 << chamber_vec[i][0] << "_HV_RO_1, ";
//        }
        else {
          std::vector<std::string>::iterator  it = find (name_chambers.begin(), name_chambers.end(), chamber_vec[i][0].c_str());
          double pos = it- name_chambers.begin();
         
          for (int j=0; j< number_drift[pos]; j++)
            s1 << chamber_vec[i][0] << "_HV_DR_" <<j+1 << " , ";
          for (int j=0; j< number_resistive[pos]; j++)
            s1 << chamber_vec[i][0]<< "_HV_RO_" <<j+1 << " , " ;
        }
        
        std::string position[3] = {"X","Y","Z"};

            for (int j = 29; j< 32; j++)
            s1 <<chamber_vec[i][0] << "_POS_" << position[j-29] <<" , ";
    
        std::string angle_position[3] = {"XROT","YROT","ZROT"};
        
        s1 <<chamber_vec[i][0] << "_ZEBRA_CONNECTOR_TYPE"  <<" , ";


        for (int j = 34; j< 37; j++)
            s1 <<chamber_vec[i][0] << "_ANGLE_" << angle_position[j-34] <<" , ";

            s1 <<chamber_vec[i][0] << "_ARGON"  <<" , ";
            s1 <<chamber_vec[i][0] << "_CO2"  <<" , ";
            s1 <<chamber_vec[i][0] << "_MESHWIRE" <<" , ";
        
            s1 <<chamber_vec[i][0] << "_OPENING"  <<" , ";

        
    
    
    }//END IF I
    
    for (int i =0; i<chamber_vec.size()-1; i++){
        
        s1 <<chamber_vec[i][0] << "_Electronics"<<", ";
        
    }
    
    s1 <<chamber_vec[chamber_vec.size()-1][0] << "_Electronics";
    
    
    s1 << ") VALUES ( ";
    
    for (int i =0; i<chamber_vec.size(); i++){
        
        
        s1  << "'" << chamber_vec[i][0] << "',";
        //
        for (int j = 1; j< 29; j++)
            if (chamber_vec[i][j]!="")
                s1 << chamber_vec[i][j] <<",";
        
        for (int j = 29; j< 32; j++)
            s1 << chamber_vec[i][j]<< ",";
        
         s1 << "'" << chamber_vec[i][36]<< "',";
    
        for (int j = 33; j< 36; j++)
            s1 << chamber_vec[i][j]<< ",";
        
        for (int j = 37; j< 41; j++)
            s1 << chamber_vec[i][j]<< ",";

    
    
    }
    for (int i =0; i<chamber_vec.size()-1; i++){
        
        s1 << "'" << chamber_vec[i][32]<< "',";
        
    }
    
    s1 << "'" << chamber_vec[chamber_vec.size()-1][32]<< "')";
    


    return s1.str();
}






void CGuiConfig::gui_handle_tree_click(TGListTreeItem* entry,Int_t number)

{
    delete_button->SetEnabled(true);
    new_button->SetEnabled(true);
    save_button->SetEnabled(false);
    modify_button->SetEnabled(true);

    std::string to_delete = m_gui_tree_list->GetSelected()->GetText();
    std::cout << to_delete <<std::endl;
    std::vector<std::string>::iterator itr;
    size_t n;
    for(n = 0; n < chamber_vec.size(); ++n)
    {
        itr = find(chamber_vec[n].begin(), chamber_vec[n].end(), to_delete);
        if(chamber_vec[n].end() != itr)
            break;
    }

    number = n;
    
    std::vector<std::string>::iterator  it = find (name_chambers.begin(), name_chambers.end(), entry->GetText());
    double pos = it- name_chambers.begin();
    
    type->Select(pos+1);


    
    
        for (int i=0; i<28; i++)
            HV_entry[i]->SetText(chamber_vec[number][i+1].c_str());
        

    
    
    
    
    position_entry[0]->SetText(chamber_vec[number][29].c_str());
    position_entry[1]->SetText(chamber_vec[number][30].c_str());
    position_entry[2]->SetText(chamber_vec[number][31].c_str());

    
    
    
    for (int i=0; i<8;i++){
//        fR[i]->SetDisabledAndSelected(false);
        fR[i]->SetState(kButtonUp);
        fR[i]->SetEnabled(false);}
    if (chamber_vec[number][32] == "APV"){
        fR[0]->SetState(kButtonDown);
        fR[0]->SetEnabled(false);}
    if (chamber_vec[number][32] == "VMM_Mini2"){
        fR[1]->SetState(kButtonDown);
        fR[1]->SetEnabled(false);}
    if (chamber_vec[number][32] == "VMM_MMF8"){
        fR[2]->SetState(kButtonDown);
        fR[2]->SetEnabled(false);}
    if (chamber_vec[number][32] == "Other"){
        fR[3]->SetState(kButtonDown);
        fR[3]->SetEnabled(false);}

    
    position_entry[3]->SetText(chamber_vec[number][33].c_str());
    position_entry[4]->SetText(chamber_vec[number][34].c_str());
    position_entry[5]->SetText(chamber_vec[number][35].c_str());
    
    if (chamber_vec[number][36] == "Zeta-Axis"){
        fR[4]->SetState(kButtonDown);
        fR[4]->SetEnabled(false);}
    if (chamber_vec[number][36] == "Fujipoli"){
        fR[5]->SetState(kButtonDown);
        fR[5]->SetEnabled(false);}
    if (chamber_vec[number][36] == "Not Present"){
        fR[6]->SetState(kButtonDown);
        fR[6]->SetEnabled(false);}
    if (chamber_vec[number][36] == "Other"){
        fR[7]->SetState(kButtonDown);
        fR[7]->SetEnabled(false);}
    for (int i=0; i<2; i++)
        gas_entry[i]->SetText(chamber_vec[number][37+i].c_str());
    for (int i=0; i<2; i++)
        mesh_entry[i]->SetText(chamber_vec[number][39+i].c_str());

 
    
    
    disable_all();

    
    
}

void CGuiConfig::SetType(int id)

{
    
    
    

        
        
        
    for (int i=0; i<28; i++){
        HV_entry[i]->SetText("");
        HV_entry[i]->SetEnabled(false);
        
    }
        
        
    for (int i=0; i<number_drift[id-1]; i++){
        HV_entry[i]->SetText("");
        HV_entry[i]->SetEnabled(true);
        
    }

  
    for (int i=0; i<number_resistive[id-1]; i++){
        HV_entry[i+4]->SetText("");
        HV_entry[i+4]->SetEnabled(true);
            
    }

    
    for (int i=0; i<3; i++){
        position_entry[i]->SetText("");
        position_entry[i]->SetEnabled(true);
        
    }
    for (int i=0; i<8; i++){
        fR[i]->SetState(kButtonUp);
        fR[i]->SetEnabled(true);
    }
    
    
    }




void CGuiConfig::move_focus()

{

    
    
}

void CGuiConfig::new_chamber()

{
    type->SetEnabled(true);

    for (int i=0; i<6; i++){
        position_entry[i]->SetText("");
        position_entry[i]->SetEnabled(true);
        }
    for (int i=0; i<28; i++){
        HV_entry[i]->SetText("");
    }
    for (int i=0; i<8; i++){
        fR[i]->SetState(kButtonUp);
        fR[i]->SetEnabled(true);
    }
    for (int i=0; i<2; i++){
        mesh_entry[i]->SetText("");
        mesh_entry[i]->SetEnabled(true);
    }
    for (int i=0; i<2; i++){
        gas_entry[i]->SetText("");
        gas_entry[i]->SetEnabled(true);
    }
    
    
    save_button->SetEnabled(true);
    finish_button->SetEnabled(false);
    delete_button->SetEnabled(false);
    modify_button->SetEnabled(false);
    
}


void CGuiConfig::save()

{
    
    
    if( check_voltages()==true &&
       TString(position_entry[0]->GetText())!="" &&
      TString( position_entry[1]->GetText())!="" &&
       TString(position_entry[2]->GetText())!="" &&
       TString(position_entry[3]->GetText())!="" &&
       TString(position_entry[4]->GetText())!="" &&
       TString(position_entry[5]->GetText())!="" &&
       TString(gas_entry[0]->GetText())!="" &&
       TString(gas_entry[1]->GetText())!="" &&
       TString(mesh_entry[0]->GetText())!="" &&
       TString(mesh_entry[1]->GetText())!="" &&
       (fR[0]->IsOn() || fR[1]->IsOn() || fR[2]->IsOn() || fR[3]->IsOn()) &&
       (fR[6]->IsOn() || fR[4]->IsOn() || fR[5]->IsOn() || fR[7]->IsOn()))
       {
    
    
    std::vector<string> temp;
    
    stringstream ss;
    ss << type->GetSelectedEntry()->GetTitle();
    
    temp.push_back(ss.str());
 
    for (int i=0; i<28; i++){
        temp.push_back(HV_entry[i]->GetText() );
    }
    
    temp.push_back(position_entry[0]->GetText());
    temp.push_back(position_entry[1]->GetText());
    temp.push_back(position_entry[2]->GetText());
    

    if (fR[0]->IsOn())
        temp.push_back("APV");
    if (fR[1]->IsOn())
        temp.push_back("VMM_Mini2");
    if (fR[2]->IsOn())
        temp.push_back("VMM_MMF8");
    if (fR[3]->IsOn())
        temp.push_back("Other");
    
    temp.push_back(position_entry[3]->GetText());
    temp.push_back(position_entry[4]->GetText());
    temp.push_back(position_entry[5]->GetText());
    
    if (fR[4]->IsOn())
        temp.push_back("Zeta-Axis");
    if (fR[5]->IsOn())
        temp.push_back("Fujipoli");
    if (fR[6]->IsOn())
        temp.push_back("Not Present");
    if (fR[7]->IsOn())
        temp.push_back("Other");
    for (int i=0; i<2; i++){
        temp.push_back(gas_entry[i]->GetText() );
    }
    for (int i=0; i<2; i++){
        temp.push_back(mesh_entry[i]->GetText() );
    }
    

    
    chamber_vec.push_back(temp);

    update_tree();
    
    
    finish_button->SetEnabled(true);
        new_button->SetEnabled(true);
    save_button->SetEnabled(false);
    modify_button->SetEnabled(false);
        
    disable_all();

        
    } //end if all filled
    
    else {
    
        TGMsgBox* message_box = new TGMsgBox (gClient->GetRoot(), 0, "Error in saving chamber", "Check that all the fields have been filled! ",0, kMBClose);
    
    
    }
    
}

void CGuiConfig::close()

{
	CloseWindow();
	
}

                          
bool CGuiConfig::check_voltages()
                          
{
    bool exit;
    exit= true;


    std::vector<std::string>::iterator it = find (name_chambers.begin(), name_chambers.end(), type->GetSelectedEntry()->GetTitle());
    double pos = it- name_chambers.begin();
    
    for (int i =0; i <number_drift[pos] ; i++){
//        HV_entry[i]->SetText("");
        std::string HV;
        HV = HV_entry[i]->GetText();
        if (HV==""){
            exit = false;
        }
    }
  

    for (int i =0; i <number_resistive[pos] ; i++){
        std::string HV;
        HV = HV_entry[i+4]->GetText();
        if (HV==""){
            exit = false;
        }
    }
    std::cout << exit << std::endl;
    return exit;
}


std::string CGuiConfig::conversion(int input)

{
  
}

int CGuiConfig::deconversion(std::string input)

{

}



void CGuiConfig::disable_all()

{
    type->SetEnabled(false);
    
    for (int i=0; i<28; i++){
        HV_entry[i]->SetEnabled(false);
    }
    for (int i=0; i<6; i++){
        position_entry[i]->SetEnabled(false);
    }
    for (int i=0; i<2; i++){
        gas_entry[i]->SetEnabled(false);
    }
    for (int i=0; i<2; i++){
        mesh_entry[i]->SetEnabled(false);
    }
    for (int i=0; i<8; i++){
        fR[i]->SetEnabled(false);
    }


}


void CGuiConfig::disable_HV()

{
    type->SetEnabled(false);
    
    for (int i=0; i<28; i++){
        HV_entry[i]->SetText("");
        HV_entry[i]->SetEnabled(false);
 
    }
    
}

void CGuiConfig::current_id_entry()

{

    id_entry->SetNumber(ConfigRun::GetInstance()->GetSetup());
    
}


void CGuiConfig::update_tree()

{
    
    TGListTreeItem* FirstItem = m_gui_tree_list->GetFirstItem();
    while (FirstItem) {
        m_gui_tree_list->DeleteChildren(FirstItem);
        m_gui_tree_list->DeleteItem(FirstItem);
        FirstItem = m_gui_tree_list->GetFirstItem();
    }
    m_gui_tree_list->ClearViewPort();
    
    //?
//    TList* itemlist = m_gui_tree_list->GetList();
//    //clear tree
//    TObjLink *lnk = itemlist->FirstLink();
//    while (lnk) {
//        TObjLink* nextlnk = lnk->Next();
//        lnk->GetObject()->Delete();
//        lnk = nextlnk;
//    }
    
    for (int i=0; i<chamber_vec.size(); i++)
    
    {
        stringstream ss;
        ss << chamber_vec[i][0];
        m_gui_tree_list->AddItem((TGListTreeItem*)0, ss.str().c_str());
    }
    
}
