/**
   @date 2020/04/02
**/

#include "VANL_Module.hpp"

#include <iostream>
#include <iomanip>
using std::cout;
using std::endl;

void anl::VANL_Module::show_parameters()
{
    cout << " ##### " << mod_name() << " ##### " << endl;
    cout << "  Type   Name = Value" << endl;
    
    auto ip = int_params.show();
    for ( auto p : ip )  {
	cout << "  int    " << std::get<0>(p) << " = " << std::get<1>(p) << endl; 
    }
    
    auto dp = double_params.show();
    for ( auto p : dp ) {
	cout << "  double " << std::get<0>(p) << " = " << std::get<1>(p) << endl;
    }
    
    auto bp = bool_params.show();
    for ( auto p : bp ) {
	cout << "  bool   " << std::get<0>(p) << " = " << std::get<1>(p) << endl;
    }
    	    
    auto sp = string_params.show();
    for ( auto p : sp ) {
	cout << "  string " << std::get<0>(p) << " = " << std::get<1>(p) << endl;
    }
    cout << endl;
}

template<> anl::VANL_Module* anl::VANL_Module::define_parameter<int>
(const std::string& key, const int& defvalue)
{
    int_params.define(key, defvalue); return this;
}
template<> anl::VANL_Module* anl::VANL_Module::set_parameter<int>
(const std::string& key, const int& value)
{
    int_params.set(key, value); return this;
}
template<> int anl::VANL_Module::get_parameter<int>(const std::string& key) const
{
    return int_params.get(key);
}

template<> anl::VANL_Module* anl::VANL_Module::define_parameter<double>
(const std::string& key, const double& defvalue)
{
    double_params.define(key, defvalue); return this;
}
template<> anl::VANL_Module* anl::VANL_Module::set_parameter<double>
(const std::string& key, const double& value)
{
    double_params.set(key, value); return this;
}
template<> double anl::VANL_Module::get_parameter<double>(const std::string& key) const
{
    return double_params.get(key);
}

template<> anl::VANL_Module* anl::VANL_Module::define_parameter<std::string>
(const std::string& key, const std::string& defvalue)
{
    string_params.define(key, defvalue); return this;
}
template<> anl::VANL_Module* anl::VANL_Module::set_parameter<std::string>
(const std::string& key, const std::string& value)
{
    string_params.set(key, value); return this;
}
template<> std::string anl::VANL_Module::get_parameter<std::string>(const std::string& key) const
{
    return string_params.get(key);
}

template<> anl::VANL_Module* anl::VANL_Module::define_parameter<bool>
(const std::string& key, const bool& defvalue)
{
    bool_params.define(key, defvalue); return this;
}
template<> anl::VANL_Module* anl::VANL_Module::set_parameter<bool>
(const std::string& key, const bool& value)
{
    bool_params.set(key, value); return this;
}
template<> bool anl::VANL_Module::get_parameter<bool>(const std::string& key) const
{
    return bool_params.get(key);
}

anl::VANL_Module::VANL_Module()
    : module_name("VANL_Module"), module_version("1.0"), copyid("")
{
    define_parameter<std::string>( "module_name", "VANL_Module" );
    define_parameter<std::string>( "module_version", "1.0" );
    define_parameter<std::string>( "copyid", "" );
}
anl::VANL_Module::VANL_Module(const std::string& name, const std::string& version)
    : module_name(name), module_version(version), copyid("")
{
    define_parameter<std::string>( "module_name", name );
    define_parameter<std::string>( "module_version", version );
    define_parameter<std::string>( "copyid", "" );
}
anl::VANL_Module::VANL_Module(const anl::VANL_Module& other)
{
    define_parameter<std::string>( "module_name", other.mod_name() );
    define_parameter<std::string>( "module_version", other.mod_version() );
    define_parameter<std::string>( "copyid", "" );

    int_params = other.int_params;
    double_params = other.double_params;
    string_params = other.string_params;
    bool_params = other.bool_params;
}

anl::VANL_Module& anl::VANL_Module:: operator=(const anl::VANL_Module& other)
{
    int_params = other.int_params;
    double_params = other.double_params;
    string_params = other.string_params;
    bool_params = other.bool_params;
    return *this;
}

std::string anl::VANL_Module::mod_name() const
{
    return get_parameter<std::string>( "module_name" );
}
std::string anl::VANL_Module::mod_version() const
{
    return get_parameter<std::string>( "module_version" );
}
std::string anl::VANL_Module::mod_copyid() const
{
    return get_parameter<std::string>( "copyid" );
}
std::string anl::VANL_Module::mod_name2() const
{
    auto copyid = mod_copyid();
    if ( copyid.length()==0 ) return mod_name();
    return mod_name()+"_"+copyid;
}
