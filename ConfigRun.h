
#ifndef browser_ConfigurationRun_h
#define browser_ConfigurationRun_h

#include <string>

// Singleton base class hpp file

class ConfigRun
{
    
public:
    static ConfigRun* GetInstance();
    static bool exists();
    inline std::string getElectr(){ return electr; };
    inline void setElectr(std::string inpunt){ electr = inpunt; };
    inline bool IsComplete()  {return complete;};
    void completed(bool input) {complete = input;};
    void setSetup(int setup_) {setup = setup_;};
    int GetSetup() {return setup;};
    void check_complete();
    bool get_check_online() {return check_online;};
    void set_check_online(bool check) {check_online = check;};

    //    virtual void setDataY(int _in) = 0;
private:
    ConfigRun(){ complete = false; check_online=false;};  //   // Private so that it can  not be called
    ConfigRun(ConfigRun const&){};
    virtual ~ConfigRun(){};
    static ConfigRun* mpoSssInstance;
    std::string electr;
    bool complete;
    int setup;
    bool check_online;
};

#endif
