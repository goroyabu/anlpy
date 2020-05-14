/**
   @file ANLmanager.hpp
   @author Goro Yabu
   @date 2018/12/04
   @date 2019/03/29 v2.0 ANLCross is chaged to a series of functions from a class
   @date 2020/04/03 
   @version 1.0
   @brief 
**/
#ifndef ANLmanager_hpp
#define ANLmanager_hpp

#include <string>
#include <vector>
#include <memory>

#include "ANLStatus.hpp"
#include "VANL_Module.hpp"

#include "ANLmoduleStacker.hpp"

namespace anl {
    
    class ANLmanager : public VANL_Module
    {
   
    public:
	
	ANLmanager();
	// ANLmanager(const ANLmanager& other);
	~ANLmanager();
	
	/** @brief Add a module to the analysis chain **/
	int add_module(VANL_Module* mod);
	
	/** @brief Start to read and analysis data **/
	int read_data(long int nevent, long int print_freq);
	
	/** @brief Show the defined analysis chain **/
	int show_analysis();
	
	/** @brief Show the analysis status of results **/
	int show_status();
	
	/** @brief Optional. Call before a next analysis **/
	int reset_status();
	
	VANL_Module* get_module(const std::string& key);

	//std::shared_ptr<>
	static ANLmanager& Instance();
	//static int quit();
	int quit();
	
    private:
	
	//static ANLmoduleStacker* module_stacker;
	//static std::unique_ptr<ANLmoduleStacker> module_stacker;
	static ANLmoduleStacker* module_stacker;
	static std::unique_ptr<ANLmanager> manager_instance;
	//static ANLmanager* manager_instance;

	/** @brief Called at the beginning of read_data **/
	int call_init_and_his();

	/** @brief Called at the beginning of read_data **/
	int call_bgnrun();
	
	/** @brief Called at the end of read_data **/
	int call_endrun();
	
	/** @brief Called in the destructor **/
	int exit_process();
	
	int message(int level, std::string place, std::string message);
    };
    
}
#endif
