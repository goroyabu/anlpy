/**
   @file ANLCross.hpp
   @author Goro Yabu
   @date 2018/12/04
   @date 2019/03/29 v2.0 ANLCross is chaged to a series of functions from a class
   @version 2.0
   @brief 
**/
#ifndef ANLCross_hpp
#define ANLCross_hpp

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <thread>

#include <bnk.hpp>
#include <evs.hpp>
#include <com_cli.hpp>

#include "ANLStatus.hpp"
#include "ANLModuleBase.hpp"

namespace anlcross {
    
    struct analysis_status
    {
        int entry;
        int loop_entry;
        int next;
        int skip;
        int quit;
    };
    const int MAX_MODULE = 100;
    const int DEFAULT_BNK_BUF_SIZE = 1000000;
    const int MAXNEST = 8;

    /* ANLCROSS 0 */
    int is_module_def(std::string key);
    int is_module_on(std::string key);
    int is_module_on(int index);
    int enable_module(int index, bool on=true);
    int find_module(std::string key, int* index);
    int add_module(ANLModuleBase* mod);
    int get_nmodules();
    std::vector<ANLModuleBase*>::iterator get_module_chain();
    analysis_status* get_count(int index);
    //int get_module(std::string key, ANLModuleBase* mod);//int AnlMod(std::string key, ANLModuleBase* mod);
    ANLModuleBase* get_module(std::string key);//ANLModuleBase* AnlMod(std::string key);
    //ANLModuleBase* get_module(int index);
    
    int show_module_switch();
    int call_bgnrun();
    int call_endrun();
    
    int anl_init();
    int anl_end();
    int anl_body();
    int anl_ql_body(std::vector<ANLModuleBase*> &chain);
    int anl_ql2_body(std::vector<ANLModuleBase*> &chain);
    /* end ANLCROSS 0 */

    /* ANLCROSS 1 */
    int anl_define_analysis();
    int anl_show_analysis();
    int anl_modify_param();
    int anl_book_histgram();
    int anl_talk_menu();
    int anl_read_data();
    int anl_save_hist();
    int anl_clear_hist();
    int anl_show_status();
    int anl_reset_status();
    int anl_ana_menu();
    int anl_exit_proc();
    int anl_flush();
    int anl_error(int level, std::string place, std::string message);
    /* end ANLCROSS 1 */

    /* ANLCROSS 2 */
    std::string anl_routine_name(int imodule);
    std::string anl_routine_version(int imodule);
    int anl_routine_startup(int imodule);
    int anl_routine_init(int imodule);
    int anl_routine_com(int imodule);
    int anl_routine_his(int imodule);
    int anl_routine_bgnrun(int imodule);
    int anl_routine_ana(int imodule, int& nevent, int& eventid);
    int anl_routine_endrun(int imodule);
    int anl_routine_exit(int imodule);
    /* end ANLCROSS 2 */
}
#endif
