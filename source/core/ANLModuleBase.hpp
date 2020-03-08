/**
   @file ANLModuleBase.hpp
   @author Goro Yabu
   @date 2018/11/23
   @version 1.0
   @brief This file is modified from VANL_Module.hh
**/
#ifndef ANLModuleBase_hpp
#define ANLModuleBase_hpp

#include <string>

#include "ANLStatus.hpp"

namespace anlcross
{

    class ANLModuleBase
    {
    protected:
	std::string module_name;
	std::string module_version;
	
    private:
	std::string copyid;

    public:
	ANLModuleBase() : module_name("ANLModuleBase"), module_version("1.0"), copyid("") {}
	ANLModuleBase(std::string name, std::string version) : module_name(name), module_version(version), copyid("") {}
	virtual ~ANLModuleBase(){}
	
	virtual void mod_startup(int& status){ status = ANL_OK; }
	virtual void mod_init(int& status){ status = ANL_OK; }
	virtual void mod_his(int& status){ status = ANL_OK; }
	virtual void mod_com(int& status){ status = ANL_OK; }
	virtual void mod_bgnrun(int& status){ status = ANL_OK; }  
	virtual void mod_ana([[maybe_unused]] int& nevent, [[maybe_unused]] int& eventid, int& status) { mod_ana(status); }
	virtual void mod_ana(int& status){ status = ANL_OK; }
	virtual void mod_endrun(int& status){ status = ANL_OK; }
	virtual void mod_exit(int& status){ status = ANL_OK; }
	
	virtual void mod_display(){}
	virtual void mod_ql_com(){}
	
    public:
	std::string mod_name(int& status){ status = ANL_OK; return module_name;}
	std::string mod_name(){ return module_name; }
	std::string mod_version(int& status){ status = ANL_OK; return module_version;}
	
	std::string mod_name2(int& status)
	{
	    std::string module_name2 = module_name;
	    if(!copyid.empty())
		{
		    module_name2 += "_";
		    module_name2 += copyid;
		}
	    status = ANL_OK;
	    return module_name2;
	}
	void set_copyid(std::string str){copyid = str;}    
    };
}
#endif
    
