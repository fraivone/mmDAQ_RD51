#include "ConfigXml.h"

#include <vector>
#include <iostream>

#include <TGTextView.h>
#include <iostream>
#include <algorithm>
#include <fstream>



std::vector< std::vector<string> > fec;
std::vector< std::vector<bool> > apv;
std::vector< std::vector<string> > det;
std::vector<string> connector_temp;
std::vector<string>  apv_temp;
std::vector<vector<string> >  connector_final;
std::vector<vector<string> >  apv_final;
TGTextView* fEdit;
TGTextView* fEdit_det;
TGTextView* apv_view;