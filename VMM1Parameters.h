//
//  VMM1Parameters.h
//  eventbrowser
//
//  Created by Marcin Byszewski on 16/11/2012.
//  Copyright (c) 2012 CERN - PH/ADE. All rights reserved.
//

#ifndef eventbrowser_VMM1Parameters_h
#define eventbrowser_VMM1Parameters_h

//or set that in chips: TACstruct {qth, t_slope, t_interc, tac}

namespace browser {
   struct VMM1Parameters {
      VMM1Parameters() : charge_offset(0.0),
      charge_slope(0.0),
      tac(0.0),
      time_offset(0.0),
      time_slope(0.0)
      {}
      VMM1Parameters(double q_offset, double q_slope, double t_tac, double t_offset, double t_slope) :
      charge_offset(q_offset), charge_slope(q_slope), tac(t_tac), time_offset(t_offset), time_slope(t_slope)
      {}
      
      
      double charge_offset;
      double charge_slope;
      double tac;             // TAC length
      double time_offset;
      double time_slope;
      
      static const double VMM1TdoVoltsCoef ; //3.3/16384.0; // Qsignal(V): = pdo*3.3/16384
      static const double VMM1PdoVoltsCoef ; //3.3/16384.0; // Tsignal(V): = tdo*3.3/16384
      
      //
   };

}


#endif

