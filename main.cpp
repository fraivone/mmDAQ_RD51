//
//  main.cpp
//  browser
//
//  Created by Marcin Byszewski on 11/15/11.
//  Copyright (c) 2011 CERN - PH/ADE. All rights reserved.
//

#include "EventBrowserOnline.h"
#include "EventBrowserOffline.h"

#include "ProgException.h"

#include <TApplication.h>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include <string>
#include <iostream>

namespace bpo = boost::program_options;

//using std::vector;
using std::string;


int main (int argc, char * argv[])
{
    
    // insert code here...
    std::cout << "Hello, World! - said the event browser - Hello!\n";
    
    
    std::vector<std::string> argvector(argv+1, argv+argc);
    boost::filesystem::path path = boost::filesystem::initial_path();
    argvector.push_back(path.string());

   boost::program_options::options_description options_description("event-browser options", 256);
   boost::program_options::variables_map options_varmap;

   
   options_description.add_options()
   ("help", "display help message")
   ("config-file,c", bpo::value<string>(), "load configuration from file")
   ("data-dir,d", bpo::value< string>(),"data directory")
   ("data-file,f", bpo::value< string>(),"data file")
   ("online" ,"online configuration")
   ;
   
   bpo::positional_options_description popt;
   popt.add("data-file", -1);
   
   bpo::store(bpo::command_line_parser(argc, argv).
              options(options_description).positional(popt).run(), options_varmap);
   bpo::notify(options_varmap);

   
   if (options_varmap.count("help")) {
      std::cout << options_description << "\n";
      return 0;
   }
   
   
   // check for online option
   bool online = false;
   if (options_varmap.count("online")) {
      online = true;
   }

   
    TApplication *myapp = new TApplication("EventBrowser", &argc, argv);
    
    browser::CEventBrowser* evbrowser = 0;
   
   try {
      
      if (online) {
         evbrowser = new browser::CEventBrowserOnline(options_varmap);
      }
      else {
         evbrowser = new browser::CEventBrowserOffline(options_varmap);
      }
        
    }
  
   catch (std::exception& e) {
       std::cerr << "Terminating after: " << e.what() << std::endl;
       delete evbrowser;
       return EXIT_FAILURE;
    }
    
    myapp->Run(); // blocks
    
   delete evbrowser;

   return 0;
}

