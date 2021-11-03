#include <stddef.h>  // defines NULL
#include "ConfigDatabase.h"
#include <iostream>
#include <string>

#include <sstream>

#include <TSQLServer.h>
#include <TSQLStatement.h>

// Global static pointer used to ensure a single instance of the class.
ConfigDatabase* ConfigDatabase::mpoSssInstance = NULL;
/** This function is called to create an instance of the class.
 Calling the constructor publicly is not allowed. The constructor
 is private and is only called by this Instance function.
 */
ConfigDatabase::ConfigDatabase(){

    std::ifstream file;
    file.open ("DatabaseConfig.txt");
    std::string line;
    std::string read_vec[6];
    int i=0;
    
    while (std::getline(file, line))
    {

        if(file.eof())
            break;
        char first_char;
        first_char=line[0];
        char comment = '#';
        if (first_char != comment )
        {


            std::istringstream iss(line);
             // The number in the line
            std::string num;
            //while the iss is a string
            while ((iss >> num))
            {

                read_vec[i]=(num);
                i++;
            }
         
        }
    }
    
std::cout << "Database creation has been called " << std::endl;
    ip_string=read_vec[0];
    database_name=read_vec[1];
    username = read_vec[2];
    password= read_vec[3];
    config_table_name=read_vec[4];
    run_table_name=read_vec[5];


//
//    //try
//    std::stringstream cmd;
//    cmd <<"mysql://"<<ConfigDatabase::GetInstance()->GetIp()<<"/"<<ConfigDatabase::GetInstance()->GetDatabaseName();
//    TSQLServer* serv = TSQLServer::Connect(cmd.str().c_str(), ConfigDatabase::GetInstance()->GetPassword().c_str(), ConfigDatabase::GetInstance()->GetPassword().c_str());
//    if (! serv) std::cout<<"------- Error in reading the database information. Check DatabaseConfig.txt ------- "<<std::endl;
//    else {
//        std::cout << "gggg" << std::endl;
//    }
    
    file.close();
    
}


ConfigDatabase* ConfigDatabase::GetInstance()
{
    if (!mpoSssInstance){
        mpoSssInstance = new ConfigDatabase;
        }
    return mpoSssInstance;
}


void ConfigDatabase::SaveToFile()
{
    std::ofstream file;
    file.open ("DatabaseConfig.txt");
    file << "#IP" << std::endl;
    file << ip_string << std::endl;
    file << "#Database name" << std::endl;
    file << database_name << std::endl;
    file << "#Username" << std::endl;
    file << username << std::endl;
    file << "#Password" << std::endl;
    file << password << std::endl;
    file << "#config table" << std::endl;
    file << config_table_name << std::endl;
    file << "#run table " << std::endl;
    file << run_table_name << std::endl;


}

