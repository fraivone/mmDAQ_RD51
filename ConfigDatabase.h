
#ifndef browser_ConfigurationDatabase_h
#define browser_ConfigurationDatabase_h

#include <string>
#include <fstream>

// Singleton base class hpp file

class ConfigDatabase
{
    
public:
    static ConfigDatabase* GetInstance();
    static bool exists();
    
    void SaveToFile();

    inline std::string GetUsername(){ return username; };
    inline std::string GetPassword(){ return password; };
    inline std::string GetDatabaseName(){ return database_name; };
    inline std::string GetRunTableName(){ return run_table_name; };
    inline std::string GetConfigurationTableName(){ return config_table_name; };
    inline std::string GetIp(){ return ip_string; };
    
    inline void SetUsername(std::string inpunt){ username = inpunt; };
    inline void SetConfigurationTableName(std::string inpunt){ config_table_name = inpunt; };
    inline void SetRunTableName(std::string inpunt){ run_table_name = inpunt; };
    inline void SetPassword(std::string inpunt){ password = inpunt; };
    inline void SetDatabaseName(std::string inpunt){ database_name = inpunt; };
    inline void SetIp(std::string inpunt){ ip_string = inpunt; };


    //    virtual void setDataY(int _in) = 0;
private:
    ConfigDatabase();  // Private so that it can  not be called
    ConfigDatabase(ConfigDatabase const&){};
    virtual ~ConfigDatabase(){};
    static ConfigDatabase* mpoSssInstance;
 
    
    
    std::string username;
    std::string password;
    std::string database_name;
    std::string ip_string;
    std::string config_table_name;
    std::string run_table_name;
    
};

#endif