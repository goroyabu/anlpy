/**
   @date 2020/05/14
**/

#include "Extract2Photon.hpp"
#include <ANLmanager.hpp>

#include <string>
#include <iostream>
using std::cout;
using std::endl;

#include <cstdlib>

int ananlysis(std::string in1, std::string in2, std::string out1, std::string out2, int twindow)
{
    auto manager = new anl::ANLmanager();

    auto extract = new Extract2Photon();
    extract->set_parameter<std::string>("input_file1", in1);
    extract->set_parameter<std::string>("input_file2", in2);
    extract->set_parameter<std::string>("output_file1", out1);
    extract->set_parameter<std::string>("output_file2", out2);
    extract->set_parameter<int>("time_window", twindow);

    manager->add_module(extract);
    
    manager->show_analysis();
    manager->read_data(1,1);

    return 0;
}
int main(int argc, char* argv[])
{
    if ( argc<6 ) {
	cout << "usage: extract_2photon INPUT1 INPUT2 OUTPUT1 OUTPUT2 TWINDOW" << endl;
	return -1;
    }

    auto in1 = std::string(argv[1]);
    auto in2 = std::string(argv[2]);
    auto out1 = std::string(argv[3]);
    auto out2 = std::string(argv[4]);
    auto twindow = std::atoi(argv[5]);
    
    return ananlysis(in1, in2, out1, out2, twindow);
}



