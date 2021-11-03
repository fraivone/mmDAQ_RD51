//
//  TreeBase.h
//  eventbrowser
//
//  Created by Marcin Byszewski on 01/08/2012.
//  Copyright 2012 CERN - PH/ADE. All rights reserved.
//

#ifndef eventbrowser_TreeBase_h
#define eventbrowser_TreeBase_h

#include <TROOT.h>
#include "DetTypes.h"

#include <boost/shared_ptr.hpp>
class TTree;

namespace browser {
   
   class CDetDetector;
   class CGuiMainFrame;
   
   class CTreeBase
   {
      
   public:
      CTreeBase(const std::string& name);
      virtual ~CTreeBase();
      
      virtual Int_t    GetEntry(Long64_t entry);
      virtual Long64_t GetCurrentEntryNumber() const;
      virtual long     GetEntries() const;
       virtual long     GetEventNumb() const;

       virtual Long64_t LoadTree(Long64_t entry);

      virtual size_t size() const = 0;
      virtual void LoadEventData(DetBasePtrCont& strips, CDetDetector* det) = 0;
      virtual void parse_tree(CDetDetector*, CGuiMainFrame*) = 0;
      virtual void Init(TTree *tree) = 0;
      const std::string& GetName() const { return m_treename; }
      
   protected:
      
      TTree*   fChain;   //!pointer to the analyzed TTree or TChain
      Int_t    fCurrent; //!current Tree number in a TChain
      int      ferror;
      Long64_t m_current_entry;
       Long64_t m_current_event; //current event

       
       std::string m_treename;
      
   };
   
   
   typedef  boost::shared_ptr<CTreeBase> TreeBasePtr;
   
} // namespace

#endif
