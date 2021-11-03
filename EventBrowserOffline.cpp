//
//  EventBrowserOffline.cpp
//  eventbrowser
//
//  Created by Marcin Byszewski on 1/17/13.
//  Copyright (c) 2013 CERN - PH/ADE. All rights reserved.
//

#include "EventBrowserOffline.h"
#include "GuiMainFrameOffline.h"

#include "DetDetector.h"
#include "DetectorBuilder.h"

#include <iostream>
#include <stdexcept>

using namespace browser;

CEventBrowserOffline::CEventBrowserOffline(boost::program_options::variables_map& options_varmap) :
CEventBrowser(),
m_config(options_varmap),
m_reader(m_config),
m_detector(),
m_gui(0),
m_event_strips(),
m_overlay_strips(),
m_pedestal_strips()
{
   init();
   m_gui = new CGuiMainFrameOffline(this, &m_config);
}


CEventBrowserOffline::~CEventBrowserOffline()
{
   delete m_gui;
}

CDetDetector* CEventBrowserOffline::do_get_detector()
{
   return m_detector.get();
}

void CEventBrowserOffline::init()
{
   CDetectorBuilder builder(&m_reader, m_gui);
   m_detector = builder.get_detector();
   set_current_entry(0);
}

void CEventBrowserOffline::do_open_file()
{
   try {
      m_reader.open_file(m_config.get_current_file_path().string());
      init();
   } catch (std::runtime_error& re) {
      std::cerr << "ERROR: " << re.what() << std::endl;
      //reset cuurent file in config,
   }
}



void CEventBrowserOffline::do_set_current_entry(size_t entry)
{
   m_reader.set_tree_current_entry(entry);
   load_event_data();
   //   m_detector->print();
}


std::vector<size_t> CEventBrowserOffline::do_get_current_entry() const
{
   return m_reader.get_tree_current_entries();
}

size_t CEventBrowserOffline::do_get_max_entry() const
{
   return m_reader.get_tree_max_entry();
}


size_t CEventBrowserOffline::do_get_current_event_number() const
{
   return m_reader.get_current_event_number();
}

/**
 parse root file trees and create all data to be passed to the detector
 */
void CEventBrowserOffline::do_load_event_data()
{
   m_event_strips.clear();
   m_overlay_strips.clear();
   m_pedestal_strips.clear();
   
   //   std::cout << "CEventBrowser::load_event_data() a size=" << m_event_strips.size() << std::endl;
   m_detector->clear_event_data();
   //from root file
   m_event_strips = m_reader.load_event_data_raw(m_detector.get());
   m_overlay_strips = m_reader.load_event_data_overlay(m_detector.get());
    m_pedestal_strips = m_reader.load_event_data_pedestal(m_detector.get());
    std::cout << "CEventBrowser::load_event_data() b size=" << m_event_strips.size() << std::endl;
   m_detector->load_event_data(m_event_strips, m_overlay_strips);
   m_detector->load_pedestal_data(m_pedestal_strips);
}

