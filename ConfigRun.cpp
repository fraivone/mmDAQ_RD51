#include <stddef.h>  // defines NULL
#include "ConfigRun.h"
#include <iostream>
#include <string>


// Global static pointer used to ensure a single instance of the class.
ConfigRun* ConfigRun::mpoSssInstance = NULL;

/** This function is called to create an instance of the class.
 Calling the constructor publicly is not allowed. The constructor
 is private and is only called by this Instance function.
 */

ConfigRun* ConfigRun::GetInstance()
{
    if (!mpoSssInstance)   // Only allow one instance of class to be generated.
        mpoSssInstance = new ConfigRun;
    return mpoSssInstance;
}


void ConfigRun::check_complete(){

    if (ConfigRun::GetInstance()->GetSetup() !=0)
    ConfigRun::GetInstance()->completed(true);

}
