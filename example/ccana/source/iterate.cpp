/**
   @date 2020/05/22
**/

#include "IterateCone3D.hpp"
#include <ANLmanager.hpp>

#include <string>
#include <iostream>
using std::cout;
using std::endl;

#include <cstdlib>

int analysis(std::string in, std::string out, int niter)
{
    auto manager = new anl::ANLmanager();

    auto iterate = new IterateCone3D();
    iterate->set_parameter<std::string>("input_file", in);
    iterate->set_parameter<std::string>("input_tree", "resptree");
    iterate->set_parameter<std::string>("output_file", out);
    iterate->set_parameter<int>("n_of_iterations", niter);
    
    manager->add_module(iterate);
    
    manager->show_analysis();
    manager->read_data(1,1);

    return 0;
}
int main(int argc, char* argv[])
{
    if ( argc<4 ) {
	cout << "usage: iterate_mlem INPUT OUTPUT NITER" << endl;
	return -1;
    }

    auto in = std::string(argv[1]);
    auto out = std::string(argv[2]);
    auto niter = std::atoi(argv[3]);
    
    return analysis(in, out, niter);
}



