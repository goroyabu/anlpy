/**
   @date 2020/04/02
**/

#include "LoggerText.hpp"

#include <bnk.hpp>

#include <iostream>
#include <fstream>

using std::cout;
using std::endl;

LoggerText::LoggerText()
    : anl::VANL_Module("LoggerText", "1.0")
{
    define_parameter<std::string>("output_file", "output.txt");
    define_parameter<double>("scale_factor", 2.0);
}

int LoggerText::mod_init()
{
    output_file = get_parameter<std::string>("output_file");
    output_stream = std::ofstream( output_file, std::ios_base::out );
    scale_factor = get_parameter<double>("scale_factor");
    
    if ( !output_stream.is_open() ) return anl::ANL_NG;

    cout << "Opening " << output_file << " is succeeded!" << endl;        
    return anl::ANL_OK;
}

int LoggerText::mod_ana()
{
    auto time_stamp = bnk::get<std::string>("time_stamp");
    auto seconds = bnk::get<int>("seconds");

    if ( seconds%7 == 0 ) return anl::ANL_SKIP;

    output_stream << "Time " << time_stamp << ", sec=" << seconds << endl;
    return anl::ANL_OK;
}
