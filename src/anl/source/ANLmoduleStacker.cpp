/**
   @date 2020/04/02
**/

#include "ANLmoduleStacker.hpp"

#include <iostream>
#include <iomanip>
using std::cout; using std::endl;
using std::left; using std::right;
using std::setw;

#include <algorithm>
#include <iterator>

anl::ANLmoduleStacker::ANLmoduleStacker()
{
    const static int nmax = 20;
    module_chain.reserve(nmax);
    module_switch.reserve(nmax);
    module_status.reserve(nmax);
}
anl::ANLmoduleStacker::ANLmoduleStacker(const ANLmoduleStacker& other)
{
    module_index_table = other.module_index_table;
    module_chain.clear();
    module_switch.clear();
    module_status.clear();
    std::copy( other.module_chain.begin(), other.module_chain.end(),
	       std::back_inserter( module_chain ) );
    std::copy( other.module_switch.begin(), other.module_switch.end(),
	       std::back_inserter( module_switch ) );
    std::copy( other.module_status.begin(), other.module_status.end(),
	       std::back_inserter( module_status ) );
}

anl::ANLmoduleStacker& anl::ANLmoduleStacker::operator = (const ANLmoduleStacker& other)
{
    module_index_table = other.module_index_table;
    module_chain.clear();
    module_switch.clear();
    module_status.clear();
    std::copy( other.module_chain.begin(), other.module_chain.end(),
	       std::back_inserter( module_chain ) );
    std::copy( other.module_switch.begin(), other.module_switch.end(),
	       std::back_inserter( module_switch ) );
    std::copy( other.module_status.begin(), other.module_status.end(),
	       std::back_inserter( module_status ) );
    return *this;
}

int anl::ANLmoduleStacker::add(VANL_Module* mod)
{
    std::string key = mod->mod_name();     
    if( is_module_def(key) ) return ANL_NG;

    module_index_table[ key ] = module_chain.size();
    module_chain.emplace_back( mod );
    module_switch.emplace_back( true );
    module_status.emplace_back( new analysis_status() );

    return ANL_OK;
}
anl::VANL_Module* anl::ANLmoduleStacker::get(const std::string& key)
{
    auto index = find_module( key ); if ( index < 0 ) return nullptr;
    return module_chain[ index ];
}
anl::VANL_Module* anl::ANLmoduleStacker::get(int index)
{
    if ( index >= module_chain.size() ) return nullptr;
    return module_chain[index];	    
}
anl::ANLmoduleStacker::analysis_status*
anl::ANLmoduleStacker::status_count(int index)
{
    if ( index >= module_chain.size() ) return nullptr;
    return module_status[index];
}
int anl::ANLmoduleStacker::show_analysis()
{
    int imodule = 0;
    for ( auto module : module_chain ) {
	cout << right << setw(5) << imodule << "   ";
	cout << left << setw(20) << module->mod_name() << "    ";
	cout << setw(10) << module->mod_version() << endl;
	++imodule;
    }
    cout << endl;
    
    for ( auto module : module_chain ) {
	module->show_parameters();	
    }
        
    cout << endl;
    return ANL_OK;
}
void anl::ANLmoduleStacker::reset_status()
{
    for ( auto&& status : module_status ) status->reset();
}

bool anl::ANLmoduleStacker::is_module_def(const std::string& key)
{
    if ( module_index_table.find(key) == module_index_table.end() ) 
	return false;    
    return true;
}
int anl::ANLmoduleStacker::find_module(const std::string& key)
{
    if ( !is_module_def(key) ) return -1;
    return module_index_table[ key ];
}
