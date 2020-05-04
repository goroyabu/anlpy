/**
   @date 2020/04/28
**/
#include "WriteTTree.hpp"
#include "DSDdatabase.hpp"
#include "ApplyDatabase.hpp"
#include "ReadTTree.hpp"
#include "MergeAdjacent.hpp"
#include "CoupleHit.hpp"

#include <ANLmanager.hpp>

#include <string>

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

int analysis(std::string input_file_name);

int main(int argc, char* argv[])
{
    cout << "hello!" << endl;

    if ( argc <= 1 ) {
	cout << "usage: dsdana_cxx FILE.." << endl;
	return 0;
    }

    for ( int i=1; i<argc; ++i ) {
	std::string input_file_name = argv[i];
	analysis(input_file_name);
    }
    
    // input_file_name = "example_data_cc.root";
    return 0;
}

int analysis(std::string input_file_name)
{
    auto manager = new anl::ANLmanager();
    
    auto read = new ReadTTree();
    read->set_parameter<std::string>("file_name", input_file_name);
    read->set_parameter<std::string>("tree_name", "eventtree");
    read->set_parameter<std::string>("branch_ignore", "chflag*:cmn*_ex:ref*");
    manager->add_module(read);

    auto db = new DSDdatabase();
    db->set_parameter<std::string>("file_name", "database.txt");
    manager->add_module(db);

    auto apply = new ApplyDatabase();
    manager->add_module(apply);

    auto merge = new MergeAdjacent();
    manager->add_module(merge);

    auto couple = new CoupleHit();
    manager->add_module(couple);

    auto output_file_name = input_file_name;
    auto pos_ext = output_file_name.find(".root");
    auto len_ext = 5;
    output_file_name.replace(pos_ext, pos_ext+len_ext, "_hittree.root");
    
    auto write = new WriteTTree();
    write->set_parameter<std::string>("file_name", output_file_name);
    write->set_parameter<std::string>("tree_name", "hittree");
    write->set_parameter<std::string>("branch_ignore", "adc*:cmn*:index*:hitnum*");
    manager->add_module(write);
    
    manager->show_analysis();
    manager->read_data(100000000, 1000);
    
    return 0;
}
