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

int main(int argc, char* argv[])
{
    cout << "hello!" << endl;

    auto manager = new anl::ANLmanager();

    auto read = new ReadTTree();
    read->set_parameter<std::string>("file_name", "example_data_cc.root");
    read->set_parameter<std::string>("tree_name", "eventtree");
    read->set_parameter<std::string>("branch_ignore", "chflag*:cmn*_ex:ref*");
    manager->add_module(read);

    auto db = new DSDdatabase();
    db->set_parameter<std::string>("file_name", "database.txt");
    manager->add_module(db);

    auto apply = new ApplyDatabase();
    manager->add_module(apply);
    
    manager->show_analysis();
    manager->read_data(1000, 100);
    
    return 0;
}
