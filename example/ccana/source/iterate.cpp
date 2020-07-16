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

int analysis(std::string in, std::string out, int niter, int last, int nthread)
{
    auto manager = new anl::ANLmanager();

    auto iterate = new IterateCone3D();
    iterate->set_parameter<std::string>("input_file", in);
    iterate->set_parameter<std::string>("input_tree", "resptree");
    iterate->set_parameter<std::string>("output_file", out);
    iterate->set_parameter<int>("n_of_iterations", niter);
    iterate->set_parameter<int>("first_entry", 0);
    iterate->set_parameter<int>("last_entry", last);
    iterate->set_parameter<int>("n_threads", nthread);
    
    manager->add_module(iterate);
    
    manager->show_analysis();
    manager->read_data(1,1);

    return 0;
}
int main(int argc, char* argv[])
{
    if ( argc<6 ) {
	cout << "usage: iterate_mlem INPUT OUTPUT NITER LAST NTHREAD" << endl;
	return -1;
    }

    auto in = std::string(argv[1]);
    auto out = std::string(argv[2]);
    auto niter = std::atoi(argv[3]);
    auto last = std::atoi(argv[4]);
    auto nthread = std::atoi(argv[5]);
    
    return analysis(in, out, niter, last, nthread);
}



