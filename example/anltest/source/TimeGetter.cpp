/**
   @date 2020/04/02
**/

#include "TimeGetter.hpp"

#include "bnk.hpp"
#include "evs.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <thread>

using std::cout;
using std::endl;

TimeGetter::TimeGetter()
    : anl::VANL_Module("TimeGetter", "0.1")
{
    define_parameter<std::string>("input_file", "input.txt");
    define_parameter<double>("scale_factor", 2.0);
    define_parameter<int>("interval_sec", 2);
}

int TimeGetter::mod_init()
{
    interval_sec = get_parameter<int>("interval_sec");

    bnk::define<std::string>("time_stamp");
    bnk::define<int>("seconds");

    evs::define("multiple_of_1");
    evs::define("multiple_of_3");
    
    return anl::ANL_OK;
}

int TimeGetter::mod_ana()
{
    auto ima = std::chrono::system_clock::now();
    auto ima_time = std::chrono::system_clock::to_time_t(ima);
    
    std::stringstream ss;
    ss << std::put_time( std::localtime(&ima_time), "%Y-%m-%d %H:%M:%S" );    
    auto ima_str = ss.str();
    bnk::put<std::string>("time_stamp", ima_str);

    // cout << ima_str << endl;
    ss.str("");
    ss << std::put_time( std::localtime(&ima_time), "%S" );
    auto sec = std::stoi( ss.str() );
    bnk::put<int>("seconds", sec);

    evs::set("multiple_of_1");
    if ( sec%3 == 0 ) evs::set("multiple_of_3");
    
    std::this_thread::sleep_for( std::chrono::seconds(interval_sec) );
 
    return anl::ANL_OK;
}
