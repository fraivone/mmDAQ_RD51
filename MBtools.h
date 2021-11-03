

/*
 *  MBtools.h
 *  batreco
 *
 *  Created by Marcin Byszewski on 10/4/10.
 *  Copyright 2010 CERN. All rights reserved.
 *
 */


#ifndef EPS
#define EPS 0.00000001
#endif


#ifndef MBTools_h
#define MBTools_h

#include "TString.h"
#include "TObjArray.h"
#include "TObjString.h"


#include <numeric>
#include <vector>
#include <cmath>
#include <sstream>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <iomanip>
#include <functional>
#include <utility>

class TString;
class TObjArray;


//template <class T>
//const char* SplitFileName(const char* file, T parts[])
//{
//   TString filename(file);
//	
//   TString filenameext = "";
//   TString filenamepath = "./";
//   TString filenamebase;
//   int pos1 = filename.Last('/') + 1;
//   int pos2 = filename.Last('.');
//	
//   if	(pos1 == kNPOS && pos2 == kNPOS) {
//      filenamepath = TString("./");
//      filenamebase = filename.Data();
//      filenameext = TString("");
//   }
//   else if (pos1 == kNPOS) {
//      filenamepath = TString("./");
//      filenamebase = TString(filename(0, pos2));
//      filenameext  = TString(filename(pos2 + 1, filename.Length() - pos2));
//   }
//   else if (pos2 == kNPOS) {
//      filenamepath = TString(filename(0, pos1));
//      filenamebase = TString(filename(pos1, filename.Length() - pos1));
//      filenameext  = TString("");
//   }
//   else {
//      filenamepath = TString(filename(0, pos1));
//      filenamebase = TString(filename(pos1, pos2 - pos1));
//      filenameext  = TString(filename(pos2 + 1, filename.Length() - pos2));
//   }
//   parts[0] = filenamepath.Data();
//   parts[1] = filenamebase.Data();
//   parts[2] = filenameext.Data();
//   return filenamebase.Data();
//}

class IDList 
{
	private:
	int nID; // creates unique widget's IDs
	public:
	IDList():nID(0) {}
	~IDList() {}
	int GetUniqueID(void) {return ++nID;}
};

//Text not by const reference so that the function can be used with a character array as argument
template <typename T>
T StringToNumber(const std::string &Text)
{
   std::stringstream ss(Text);
   T result;
   return ss >> result ? result : 0;
}

template <typename T>
std::string NumberToString(const T val)
{
   std::stringstream ss;
   ss << val;
	return ss.str();
}

//______________________________________________________________________________
// functiones used by GetBranchRange ()
//TODO make template
//______________________________________________________________________________


template <class T>
T GetVectorRange(std::vector<T>* v, T range[])
{
   //we are analysing a vector for the event i
   //loop vector
   T vmax = 0, vmin = 0;
   T  ve = 0;                           // j-th value from vector at event i
   if (!v) {
      return -1;
   }
   for (unsigned j = 0; j < v->size(); j++) {
      ve = v->at(j);
      if (fabs(ve) < EPS) continue;            //ignore zero, get any value if max,in=0
      // if (ve == 0) continue;               //ignore zero, get any value if max,in=0
      if (fabs(vmax) < EPS) {
         vmax = ve;
      }
      if (fabs(vmin) < EPS) {
         vmin = ve;
      }
      vmax = vmax > ve ? vmax : ve;        //get the bigger/smaller value
      vmin = vmin < ve ? vmin : ve;        //comparing double with int...
   }
   range[0] = vmin;
   range[1] = vmax;
   return 0;
}


//______________________________________________________________________________
template <class T>
T GetVectorElement(std::vector<T>* invector, unsigned ind)
{
   if (!invector) return (T)0;
   unsigned sz = invector->size();
   if (ind < sz)
      return invector->at(ind);
   return (T)0;
}

//TODO: change to GetMean(<T> container), and return T::value_type sum/n
template <class T>
T GetVectorMean(std::vector<T>* vec)
{
   if (!vec) return 0.0;
   unsigned n = vec->size();
   if (!n) return 0;
	T sum = std::accumulate(vec->begin(), vec->end(),(T)0);
   //T sumd = (T) sum;
   return sum / n;
}

//template <class T>
//int  GetMean( <T> container)
//{
//   if (!container) return 0.0;
//   unsigned n = container->size();
//   if (!n) return 0;
//	T::value_type sum = std::accumulate(vec->begin(), vec->end(),(T::value_type)0);
//   //T sumd = (T) sum;
//   return static_cast<T::value_type> (sum / n );
//}


template <class T>
T diffsquare( T a, T b) 
{ 
	return (a-b)*(a-b); 
}

template <class T>
T GetVectorStdDev(std::vector<T>* vec)
{
   if (!vec) return 0.0;
   unsigned n = vec->size();
   if (n<2) return 0;
	T vecmean = GetVectorMean(vec);
	typename std::vector<T>::iterator iter;
	T sumdiffs = 0;
	for (iter = vec->begin(); iter!= vec->end(); iter++) {
		sumdiffs += (*iter - vecmean) * (*iter - vecmean);
	}
   return sqrt( sumdiffs / (n-1) );
}

template <class T, class V>
V GetVectorSumOfDiff(const std::vector<T>* vec, V setmean)
{
   if (!vec)
		return (V)(0.0);
   //unsigned n = vec->size();
	typename std::vector<T>::const_iterator iter;
	V sumdiffs = 0;
	for (iter = vec->begin(); iter!= vec->end(); ++iter) {
		sumdiffs += (*iter - setmean);
	}
   return sumdiffs;
}


template <class T>
size_t MakeElementsUnique(T& container )
{
   std::sort(container.begin(), container.end());
	typename T::iterator iter = unique(container.begin(), container.end());
	container.resize(iter - container.begin());
	return container.size();
}

template <class T>
T MakeElementsUniqueCopy(T container )
{
   std::sort(container.begin(), container.end());
	typename T::iterator iter = unique(container.begin(), container.end());
	container.resize(iter - container.begin());
	return container;
}

template <class C> void FreeClear(C & cntr)
{
	while (!cntr.empty()) {
		typedef typename C::value_type ElementType;
		ElementType el = cntr.front();
		cntr.erase(cntr.begin());
		delete el; el = 0;
	}
}

//template <class T>
//static bool deleteAll( T theElement ) { delete theElement; theElement = 0; return true; }
//
//
//template <class C>
//void FreeClear(C & cntr)
//{
//   typedef typename C::value_type ElementT;
//   std::remove_if(cntr.begin(), cntr.end(), deleteAll<ElementT>);
//}

//template <class C> 
//void FreeClear(C & cntr)
//{
//   for (typename C::iterator it = cntr.begin(); it != cntr.end(); ++it) {
//      delete (*it);
//		*it = 0;
//   }
//   cntr.clear();
//}

//from S.Meyers Effective STL
template <typename InputIterator, typename OutputIterator, typename Predicate>
OutputIterator copy_if(InputIterator begin, InputIterator end, OutputIterator destBegin, Predicate p) {
   while (begin != end) {
      if (p(*begin)) *destBegin++ = *begin;
      ++begin;
   }
   return destBegin;
}

//template <class T>
//void FillCSVToVector(const TString &line, std::vector<T>& list, const char* delim)
//{
//   //tokenize the line
//   if (!delim) {
//      delim = ",";
//   }
//   TObjArray* strings = line.Tokenize(delim);
//   std::vector<unsigned> values;
//   std::vector<unsigned>::iterator ival;
//   if (strings->GetEntriesFast()) {
//      TIter istring(strings);
//      TObjString* os = 0;
//      Int_t j = 0;
//      while ((os = (TObjString*)istring())) {
//         j++;
//         TString s = os->GetString();
//         s.Remove(TString::kBoth, ' ');
//         list.push_back( static_cast<T>(std::string(s.Data())) );		//into vector of strings
//      }
//   }//if(strings->GetEntriesFast())
//   delete strings;
//}
//
//template <class T>
//std::vector<T> FillCSVToVector(const TString &line, const char* delim)
//{
//   std::vector<T> list;
//   //tokenize the line
//   if (!delim) {
//      delim = ",";
//   }
//   TObjArray* strings = line.Tokenize(delim);
//   std::vector<unsigned> values;
//   std::vector<unsigned>::iterator ival;
//   if (strings->GetEntriesFast()) {
//      TIter istring(strings);
//      TObjString* os = 0;
//      Int_t j = 0;
//      while ((os = (TObjString*)istring())) {
//         j++;
//         TString s = os->GetString();
//         s.Remove(TString::kBoth, ' ');
//         list.push_back( static_cast<T>(std::string(s.Data())) );		//into vector of strings
//      }
//   }//if(strings->GetEntriesFast())
//   delete strings;
//   return list;
//}
//
//
//template <class T>
//void FillCSVToNumVector(const TString& line, std::vector<T>& vec, const char* delim)
//{
//   if (!delim) {
//      delim = ",";
//   }
//   //tokenize the rest of string
//   TObjArray* strings = line.Tokenize(delim);
//   std::vector<unsigned> values;
//   //   std::vector<unsigned>::iterator ival;
//   if (strings->GetEntriesFast()) {
//      TIter istring(strings);
//      TObjString* os = 0;
//      Int_t j = 0;
//      while ((os = (TObjString*)istring())) {
//         j++;
//         TString s = os->GetString();
//         s.Remove(TString::kBoth, ' ');
//         vec.push_back( static_cast<T>(s.Atof() ));
//      }
//   }//if(strings->GetEntriesFast())
//   delete strings;
//}


//CRTP
template <typename CSVToVectorFiller, class T>
class CSVToVectorFillerBase {
   std::vector<T> m_locvec;
   std::vector<T>& m_vec;
   std::string m_delim;
   
public:
   CSVToVectorFillerBase(std::vector<T>& vec, const std::string& delim) : m_locvec(), m_vec(vec), m_delim(delim) { }
   CSVToVectorFillerBase(const std::string& delim) : m_locvec(), m_vec(m_locvec), m_delim(delim) { }
   ~CSVToVectorFillerBase() {};
   std::vector<T> operator() (const TString& line) {
      //tokenize the rest of string
      TObjArray* strings = line.Tokenize(m_delim);
      if (strings->GetEntriesFast()) {
         TIter istring(strings);
         TObjString* os = 0;
         Int_t j = 0;
         while ((os = (TObjString*)istring())) {
            j++;
            TString s = os->GetString();
            s.Remove(TString::kBoth, ' ');
            //            static_cast<CSVToVectorFiller*>(this)->convert(); 
            //or?            m_vec.push_back( convert(s) );
            m_vec.push_back(static_cast<CSVToVectorFiller*>(this)->convert(s));
         }
      } //if(strings->GetEntriesFast())
      delete strings;
      return m_vec;
   }
   //   virtual T convert(const TString& s) const = 0;
};

template <class T>
class CSVToVectorFiller : public CSVToVectorFillerBase<CSVToVectorFiller<T>, T> {
public:
   CSVToVectorFiller(std::vector<T>& vec, const std::string& delim ) : CSVToVectorFillerBase<CSVToVectorFiller<T>, T>(vec, delim) { }
   CSVToVectorFiller(const std::string& delim = ",") : CSVToVectorFillerBase<CSVToVectorFiller<T>, T>(delim) { }
   T convert(const TString& s) const {
      return static_cast<T> (s.Atof());
   }
};

template <>
class CSVToVectorFiller<std::string> : public CSVToVectorFillerBase<CSVToVectorFiller<std::string>, std::string> {
public:
   CSVToVectorFiller(std::vector<std::string>& vec, const char* delim ) : CSVToVectorFillerBase<CSVToVectorFiller<std::string>, std::string>(vec, delim) { }
   CSVToVectorFiller(const std::string& delim = ",") : CSVToVectorFillerBase<CSVToVectorFiller<std::string>, std::string>(delim) { }
   std::string convert(const TString& s) const {
      return std::string(s.Data());
   }
};

template <>
class CSVToVectorFiller<int> : public CSVToVectorFillerBase<CSVToVectorFiller<int>, int> {
public:
   CSVToVectorFiller(std::vector<int>& vec, const char* delim ) : CSVToVectorFillerBase<CSVToVectorFiller<int>, int>(vec, delim) { }
   CSVToVectorFiller(const std::string& delim = ",") : CSVToVectorFillerBase<CSVToVectorFiller<int>, int>(delim) { }
   int convert(const TString& s) const {
      return s.Atoi();
   }
};


template <class T>
class CProgressPrinter {
   int progressTimeOfLastPrint;
   int progressOutCtrPrint;
   T last_evt;
   
public:
   explicit CProgressPrinter(): progressTimeOfLastPrint(0), progressOutCtrPrint(0) {};
   
   void print(T evt) {
      last_evt = evt;
      int now = (int)time(0);
      if (progressTimeOfLastPrint == -1) progressTimeOfLastPrint = now;
      if (progressTimeOfLastPrint != -1) {
         if (now - progressTimeOfLastPrint >= 1) {
            progressTimeOfLastPrint = now;
            char outFlg[] = { '|', '/', '-', '\\', 0 };
            std::cout << "\r" << outFlg[ progressOutCtrPrint++ ] << std::setw(6) << evt;
            if (outFlg[ progressOutCtrPrint ] == 0) progressOutCtrPrint = 0;
            fflush(stdout);
         }
      }
   }
   void print_last() {
      std::cout << "\r" << "+" << std::setw(6) << last_evt;
   }
};


template <class P1>
class FirstComparator : public std::unary_function< P1, bool> {
   typename P1::first_type m_first;
   
public:
   explicit FirstComparator(const typename P1::first_type& p) : m_first(p) {}
   bool operator() (const P1& p) const {
      return p.first == m_first;
   }
};

template <class P1>
class FirstLess : public std::binary_function< P1, P1, bool> {   
public:
   explicit FirstLess()  {}
   bool operator() (const P1& p, const P1& q) const {
      return p.first < q.first;
   }
};
      
      
      template <class P1>
      class SecondLess : public std::binary_function< P1, P1, bool> {
      public:
         explicit SecondLess()  {};
         bool operator() (const P1& p, const P1& q) const
         {
            return (p.second < q.second);
         }
      };
      

      

//
//template <class C1, class C2>
//class FirstComparator : public std::unary_function< std::pair< C1,C2> >, bool> {
//   std::pair<C1,C2> m_pair;
//   
//public:
//   FirstComparator(const std::pair<C1,C2>& p) : m_pair(p) {}
//   bool operator() (const std::pair<C1,C2>& p) const {
//      return p.first == m_pair.first;
//   }
//};

#endif

