
#ifndef ConfigXml_h
#define ConfigXml_h
#include <vector>
#include <TGTextView.h>

using namespace std;

extern	vector<vector<string> > fec;
extern	vector<vector<string> > det;
extern	vector<string>  apv_temp;
extern	vector<string>  connector_temp;
extern	vector<vector<string> >  apv_final;
extern	vector<vector<string> >  connector_final;

extern	vector<vector<bool> > apv;
extern TGTextView*	fEdit;

extern TGTextView*	fEdit_det;
extern TGTextView*	apv_view;
#endif