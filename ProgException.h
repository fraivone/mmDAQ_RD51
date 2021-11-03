//
//  ProgException.h
//  atlas_tracks
//
//  Created by Marcin Byszewski on 7/18/11.
//  Copyright 2011 CERN - PH/ADE. All rights reserved.
//

#include <string>
#include <ostream>
#include <stdexcept>


class CProgException : public std::exception {
public:
   char const* what() const throw();
};


