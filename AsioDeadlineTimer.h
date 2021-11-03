//
//  AsioDeadlineTimer.h
//  eventbrowser
//
//  Created by Marcin Byszewski on 1/25/13.
//  Copyright (c) 2013 CERN - PH/ADE. All rights reserved.
//

#ifndef eventbrowser_AsioDeadlineTimer_h
#define eventbrowser_AsioDeadlineTimer_h

#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>


namespace MBTools {
   
/// AsioDeadlineTimer::run() repeatedly calls void fun(void) every X miliseconds, until stop()
//   use example:
//   static int iii = 0;
//   void printer() {
//      std::cout << iii++ << std::endl;
//   }
//   int main() {
//      boost::function<void(void)> fun = boost::bind(printer);
//      MBtools::AsioDeadlineTimer timer(500, fun);
//      timer.run_loop();
//      std::cout << "ends" << std::endl;
//   }
   
   class AsioDeadlineTimer
   {
   public:
      AsioDeadlineTimer(int duration_ms, boost::function<void(void)> fun) :
      io_(),
      th_(new boost::thread(boost::bind(&boost::asio::io_service::run, &io_))),
      strand_(io_),
      timer1_(io_, boost::posix_time::microseconds(duration_ms)),
      fun_(fun),
      duration_(duration_ms),
      stop_(false)
      {
//         std::cout << "----- AsioDeadlineTimer() -thid="
//         << th_->get_id() << " this-thid=" << boost::this_thread::get_id();

      }
      
      void run_once() {
         stop_ = true;
         io_.reset();
         timer1_.async_wait(strand_.wrap(boost::bind(&AsioDeadlineTimer::work, this)));
         th_.reset(new boost::thread(boost::bind(&boost::asio::io_service::run, &io_)));
      }
      
      void run() {
//         std::cout << "----- AsioDeadlineTimer::run() -----\n";
//         std::cout << "----- AsioDeadlineTimer::run()1 -thid="
//         << th_->get_id() << " this-thid=" << boost::this_thread::get_id();
         stop_ = false;
         io_.reset();
         timer1_.async_wait(strand_.wrap(boost::bind(&AsioDeadlineTimer::work, this)));
         th_.reset(new boost::thread(boost::bind(&boost::asio::io_service::run, &io_)));
//         std::cout << "----- AsioDeadlineTimer::run()2 -thid="
//         << th_->get_id() << " this-thid=" << boost::this_thread::get_id();


      }
      
      
      void work() {
//         std::cout << "----- AsioDeadlineTimer::work() -thid="
//         << th_->get_id() << " this-thid=" << boost::this_thread::get_id();
         fun_();
         if(!stop_) {
            timer1_.expires_at(timer1_.expires_at() + boost::posix_time::milliseconds(duration_));
            timer1_.async_wait(strand_.wrap(boost::bind(&AsioDeadlineTimer::work, this)));
         }
      }
      
      
      void stop() {
//         std::cout << "----- AsioDeadlineTimer::stop() -----\n";
//         std::cout << "----- AsioDeadlineTimer::stop() -thid="
//         << th_->get_id() << " this-thid=" << boost::this_thread::get_id();

         stop_ = true;
         timer1_.cancel();
         th_->join();
      }
      
      void set_period_milliseconds(int period)
      {
         duration_ = period;
      }
      
      ~AsioDeadlineTimer() {}
      
   private:
      boost::asio::io_service io_;
      boost::scoped_ptr<boost::thread> th_; //(boost::bind(&boost::asio::io_service::run, &io));
      boost::asio::strand strand_;
      boost::asio::deadline_timer timer1_;
      boost::function<void(void)> fun_;
      int duration_;
      bool stop_;
      
   };
  
}

//use::



#endif
