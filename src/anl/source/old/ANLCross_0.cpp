/**
   @file ANLCross_0.cpp
   @author Goro Yabu
   @date 2018/12/04
   @date 2019/03/29 v2.0 ANLCross is chaged to a series of functions from a class
   @version 2.0
   @brief Original = ANLPlus_o.cc
**/
#include "ANLCross.hpp"

namespace anlcross
{
    static std::map<std::string, int> gModuleIndexTable;
    static std::vector<ANLModuleBase*> gModuleChain;
    static std::vector<bool> gModuleSwitch;
    //static useconds_t mUSleepTime;
    std::chrono::microseconds mUSleepTime;
    static std::vector<anlcross::analysis_status> mCount;
    static int mStatus = ANL_FALSE;
}

int anlcross::is_module_def(std::string key)
{
    if( gModuleIndexTable.find(key) == gModuleIndexTable.end() ) return ANL_NG;
    return ANL_OK;
}
int anlcross::is_module_on(std::string key)
{
    int index;
    if( find_module(key, &index) == ANL_NG ) return ANL_NG;
    return gModuleSwitch[index];
}
int anlcross::is_module_on(int index)
{
    if( index>get_nmodules() ) return ANL_NG;
    return gModuleSwitch[index];
}
int anlcross::enable_module(int index, bool on)
{
    if( index>get_nmodules() ) return ANL_NG;
    gModuleSwitch[index] = on;
    return ANL_OK;
}
int anlcross::find_module(std::string key, int* index)
{
    if( is_module_def(key) == ANL_NG ) return ANL_NG;
    *index = gModuleIndexTable[key];
    return ANL_OK;
}
/*
int anlcross::get_module(std::string key, ANLModuleBase* mod)
{
    int index;
    if( find_module(key, &index) == ANL_NG ) return ANL_NG;
    mod = gModuleChain[index];
    return ANL_OK;
}
*/
anlcross::ANLModuleBase* anlcross::get_module(std::string key)
{
    int index;
    if( find_module(key, &index) == ANL_NG ) return nullptr;
    return gModuleChain[index];
}
int anlcross::add_module(ANLModuleBase* mod)
{
    int status;
    std::string key = mod->mod_name(status);
    if( is_module_def(key) == ANL_OK ) return ANL_NG;
    gModuleChain.push_back(mod);
    gModuleSwitch.push_back(ANL_ON);
    gModuleIndexTable[key] = gModuleChain.size() - 1;
    return ANL_OK;
}
int anlcross::get_nmodules()
{
    return gModuleChain.size();
}
std::vector<anlcross::ANLModuleBase*>::iterator anlcross::get_module_chain()
{
    return gModuleChain.begin();
}
anlcross::analysis_status* anlcross::get_count(int index)
{
    if((int)mCount.size()<=index) return nullptr;
    return &mCount[index];
}

int anlcross::show_module_switch()
{
    using namespace std;
    int nmodules = get_nmodules();
    for(int i=0; i<nmodules; i++){
        cout << i << "   " << anl_routine_name(i) << "   " << anl_routine_version(i) << "   ";
        if( is_module_on(i) ) cout << " ON" << endl;
        else cout << " OFF"  << endl;
    }
    return ANL_OK;
}
int anlcross::call_bgnrun()
{
    int nmodules = get_nmodules();
    int status = ANL_OK;
    for(int imodule=0; imodule<nmodules; imodule++){
        anl_flush();
	status = ANL_OK;
        if( is_module_on(imodule) ) {
	    anl_flush();
            std::cout << "Entering " << anl_routine_name(imodule) << " bgnrun()" << std::endl;
	    status = anl_routine_bgnrun(imodule);
	}
	if( status!=ANL_OK ) return ANL_NG;
        anl_flush();
    }
    if(nmodules>0) std::cout << std::endl;
    return ANL_OK;
}
int anlcross::call_endrun()
{
    int nmodules = get_nmodules();
    int status = ANL_OK;
    if(nmodules>0) std::cout << std::endl;
    for(int imodule=0; imodule<nmodules; imodule++){
        anl_flush();
	status = ANL_OK;
        if( is_module_on(imodule) ){
	    anl_flush();
	    std::cout << "Entering " << anl_routine_name(imodule) << " endrun()" << std::endl;
	    status = anl_routine_endrun(imodule);
	}
	if( status!=ANL_OK ) return ANL_NG;
        anl_flush();
    }
    if(nmodules>0) std::cout << std::endl;
    return ANL_OK;
}
int anlcross::anl_init()
{
    //int status = bnk::BnkIni();//DEFAULT_BNK_BUF_SIZE);
    if( bnk::bnk_init()!=ANL_OK ){
	std::cout << "ANL:: BNK initialization failed. " << std::endl; return ANL_NG;
    }    
    if( evs::EvsIz()!=ANL_OK ){
	std::cout << "ANL:: EVS initialization failed. " << std::endl; return ANL_NG;
    }
    mUSleepTime = std::chrono::microseconds(1000000);
    mStatus = ANL_TRUE;
    com_cli::init("ANL","~/.anl_hist");
    
    return ANL_OK;
}
int anlcross::anl_end()
{
    com_cli::end();
    mStatus = ANL_FALSE;
    return ANL_OK;
}
int anlcross::anl_body()
{
    using namespace std;
    using namespace anlcross;
    if( mStatus!=ANL_TRUE ){
	std::cout << "anlcross::init()" << std::endl;
	return ANL_NG;
    }
    
    int nmodules = get_nmodules();
    mCount.resize(nmodules+1);
    
    for(int i=0; i<nmodules; i++){ if( anl_routine_startup(i)==ANL_NG ) return ANL_NG; }
    
    if( anl_talk_menu()==ANL_QUIT ){ std::cout << "Quit from ANL...." << std::endl; return ANL_NG; }
    
    anl_show_analysis();
    anl_reset_status();
    
    for(int i=0; i<nmodules; i++){
	int status = ANL_OK;
	anl_flush();
	if( is_module_on(i) ){
            std::cout << "Entering " << anl_routine_name(i) << " init()" << std::endl;
	    status = anl_routine_init(i);
	}
	anl_flush();
	if( status!=ANL_OK ) return ANL_NG;
	anl_flush();
	if( is_module_on(i) ){
	    std::cout << "Entering " << anl_routine_name(i) << " his()" << std::endl;
	    status = anl_routine_his(i);
	}
	anl_flush();
	if( status!=ANL_OK ) return ANL_NG;   
    }
    
    anl_ana_menu();
    cout << endl;
    
    anl_exit_proc();
    
    return ANL_OK;
}
