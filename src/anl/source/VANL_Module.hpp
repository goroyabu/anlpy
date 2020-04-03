/**
   @file VANL_Module.hpp
   @author Goro Yabu
   @date 2018/11/23
   @version 1.0
   @brief This file is modified from VANL_Module.hh
**/
#ifndef VANL_Module_hpp
#define VANL_Module_hpp

#include <map>
#include <string>
#include <iostream>

#include "ANLStatus.hpp"
#include "VANL_Parameters.hpp"

namespace anl
{
    class VANL_Module;
    using ANLModuleBase = VANL_Module;
    
    class VANL_Module
    {
    protected:
	std::string module_name;
	std::string module_version;
	std::string copyid;
	
	ANL_Iparameters int_params;
	ANL_Dparameters double_params;
	ANL_Sparameters string_params;
	ANL_Bparameters bool_params;

    public:
	VANL_Module();
	VANL_Module(const std::string& name, const std::string& version);
	virtual ~VANL_Module(){}

	VANL_Module(const VANL_Module& other);
	VANL_Module& operator =(const VANL_Module& other);
	
	virtual int mod_startup () { return anl::ANL_OK; }
	virtual int mod_init    () { return anl::ANL_OK; }
	virtual int mod_his     () { return anl::ANL_OK; }
	virtual int mod_com     () { return anl::ANL_OK; }
	virtual int mod_bgnrun  () { return anl::ANL_OK; }
	virtual int mod_ana     () { return anl::ANL_OK; }
	virtual int mod_endrun  () { return anl::ANL_OK; }
	virtual int mod_exit    () { return anl::ANL_OK; }
	
	virtual int mod_ana (long& nevent, long& eventid) { return anl::ANL_OK; }
	
	virtual void mod_display() {};
	virtual void mod_ql_com() {};
	
    public:
	
	std::string mod_name() const;
	std::string mod_version() const;
	std::string mod_copyid() const;
	std::string mod_name2() const;	
	// void set_copyid(const std::string& str)
	// {
	//     if ( str.size()!=0 ) copyid = str;
	// }
	
	template<typename T> VANL_Module* set_parameter(const std::string& key, const T& value);
	template<typename T> VANL_Module* define_parameter(const std::string& key, const T& defvalue);
	template<typename T> T get_parameter(const std::string& key) const;
	
	void show_parameters();
	
    };
    
    // template<> VANL_Module* VANL_Module::define_parameter<int>
    // (const std::string& key, const int& defvalue)
    // {
    // 	int_params.define(key, defvalue); return this;
    // }
    // template<> VANL_Module* VANL_Module::set_parameter<int>
    // (const std::string& key, const int& value)
    // {
    // 	int_params.set(key, value); return this;
    // }
    // template<> VANL_Module* VANL_Module::define_parameter<double>
    // (const std::string& key, const double& defvalue)
    // {
    // 	double_params.define(key, defvalue); return this;
    // }
    // template<> VANL_Module* VANL_Module::set_parameter<double>
    // (const std::string& key, const double& value)
    // {
    // 	double_params.set(key, value); return this;
    // }
    // template<> VANL_Module* VANL_Module::define_parameter<std::string>
    // (const std::string& key, const std::string& defvalue)
    // {
    // 	string_params.define(key, defvalue); return this;
    // }
    // template<> VANL_Module* VANL_Module::set_parameter<std::string>
    // (const std::string& key, const std::string& value)
    // {
    // 	string_params.set(key, value); return this;
    // }
    // template<> VANL_Module* VANL_Module::define_parameter<bool>
    // (const std::string& key, const bool& defvalue)
    // {
    // 	bool_params.define(key, defvalue); return this;
    // }
    // template<> VANL_Module* VANL_Module::set_parameter<bool>
    // (const std::string& key, const bool& value)
    // {
    // 	bool_params.set(key, value); return this;
    // }
    
    // VANL_Module::VANL_Module()
    // 	: module_name("VANL_Module"), module_version("1.0"), copyid("")
    // {
    // 	define_parameter<std::string>( "module_name", "VANL_Module" );
    // 	define_parameter<std::string>( "module_version", "1.0" );
    // 	define_parameter<std::string>( "copyid", "" );
    // }
    // VANL_Module::VANL_Module(const std::string& name, const std::string& version)
    // 	: module_name(name), module_version(version), copyid("")
    // {
    // 	define_parameter<std::string>( "module_name", name );
    // 	define_parameter<std::string>( "module_version", version );
    // 	define_parameter<std::string>( "copyid", "" );
    // }

}

#endif
    
