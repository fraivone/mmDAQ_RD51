//
//  SrsChipId.cpp
//  browser
//
//  Created by Marcin Byszewski on 10.4.12.
//  Copyright (c) 2012 CERN - PH/ADE. All rights reserved.
//

#include "SrsChipId.h"

#include <iostream>
#include <sstream>
#include <limits>

using namespace browser;

uint32_t browser::CSrsChipId::InvalidChipId(std::numeric_limits<uint32_t>::max());


CSrsChipId::CSrsChipId() : m_id(0)
{   
}

CSrsChipId::CSrsChipId(uint32_t chip_id) : 
m_id(chip_id )
{ }

//CSrsChipId::CSrsChipId(uint32_t fecNo, uint32_t chipNo) : 
//m_id(fecNo<<4 | chipNo ) 
//{ }

CSrsChipId::CSrsChipId(uint32_t fecNo, uint32_t chipNo) :
m_id(fecNo<<8 | (chipNo & 0xFF) )
{ }

CSrsChipId::~CSrsChipId()
{
   
}



void CSrsChipId::set_fec_no(uint32_t fec_no) 
//{ m_id = fec_no << 4 | chip_no() ; }
{ m_id = fec_no << 8 | chip_no() ; }

uint32_t CSrsChipId::chip_id() const 
{ return m_id;}

uint32_t CSrsChipId::chip_no() const 
//{ return m_id        & 0xF;}
{ return m_id        & 0xFF;}


uint32_t CSrsChipId::fec_no()  const 
//{ return (m_id >> 4) & 0xFF;}
{ return (m_id >> 8) & 0xFF;}


std::string CSrsChipId::get_string() const 
{ 
   std::stringstream ss;
   ss << fec_no() << ":" << chip_no();
   return ss.str();
}



