//
//  TreeBase.cpp
//  eventbrowser
//
//  Created by Marcin Byszewski on 01/08/2012.
//  Copyright (c) 2012 CERN - PH/ADE. All rights reserved.
//

#include "TreeBase.h"

#include <TTree.h>
#include <TChain.h>

#include <iostream>

using namespace browser;

CTreeBase::CTreeBase(const std::string& name) :
fChain(0), fCurrent(0), ferror(0), m_current_entry(0), m_treename(name)
{
   
}


CTreeBase::~CTreeBase()
{
   
}

Int_t CTreeBase::GetEntry(Long64_t entry)
{
	// Read contents of entry.
   if (!fChain)
      return 0;
   int rc = fChain->GetEntry(entry);
   if (rc > 0) {
      m_current_entry = entry;
   }
   else {
      // no such entry
   }
   return rc;
}



Long64_t CTreeBase::GetCurrentEntryNumber() const
{
   return m_current_entry;
}


long CTreeBase::GetEntries() const
{
   if (!fChain) return -1;
//   std::cout << "CTreeBase::GetEntries() entries =" << fChain->GetEntries() << std::endl;
   return fChain->GetEntries();
}

long CTreeBase::GetEventNumb() const {
    
    return m_current_event;
    
}

Long64_t CTreeBase::LoadTree(Long64_t entry)
{
	// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (!fChain->InheritsFrom(TChain::Class()))  return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      //      Notify();
   }
   return centry;
}

