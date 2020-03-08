/**
   @file ANLCross_2.cpp
   @author Goro Yabu
   @date 2018/12/04
   @date 2019/03/29 v2.0 ANLCross is chaged to a series of functions from a class
   @version 2.0
**/
#include "ANLCross.hpp"
std::string anlcross::anl_routine_name(int imodule)
{
    int status;
    return (*(get_module_chain()+imodule))->mod_name2(status);
    //return (*(mModuleChain +imodule))->mod_name2(mStatus);
}
std::string anlcross::anl_routine_version(int imodule)
{
    int status;
    return (*(get_module_chain() +imodule))->mod_version(status);
}
int anlcross::anl_routine_startup(int imodule)
{
    int status;
    (*(get_module_chain() +imodule))->mod_startup(status);
    return status;
}
int anlcross::anl_routine_init(int imodule)
{
    int status;
    (*(get_module_chain() +imodule))->mod_init(status);
    return status;
}
int anlcross::anl_routine_com(int imodule)
{
    int status;
    (*(get_module_chain() +imodule))->mod_com(status);
    return status;
}
int anlcross::anl_routine_his(int imodule)
{
    int status;
    (*(get_module_chain() +imodule))->mod_his(status);
    return status;
}
int anlcross::anl_routine_bgnrun(int imodule)
{
    int status;
    (*(get_module_chain() +imodule))->mod_bgnrun(status);
    return status;
}
int anlcross::anl_routine_ana(int imodule, int& nevent, int& eventid)
{
    int status;
    //std::cout << "routine imodule= " << imodule << std::endl;
    (*(get_module_chain() +imodule))->mod_ana(nevent, eventid, status);
    return status;
}
int anlcross::anl_routine_endrun(int imodule)
{
    int status;
    (*(get_module_chain() +imodule))->mod_endrun(status);
    return status;
}
int anlcross::anl_routine_exit(int imodule)
{
    int status;
    (*(get_module_chain() +imodule))->mod_exit(status);
    return status;
}
