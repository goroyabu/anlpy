/**
   @file ANLmoduleStacker.hpp
   @author Goro Yabu
   @date 2018/12/04
   @date 2019/03/29 v2.0 ANLCross is chaged to a series of functions from a class
   @version 2.0
   @brief 
**/
#ifndef ANLmoduleStacker_hpp
#define ANLmoduleStacker_hpp

#include <string>
#include <vector>

#include "ANLStatus.hpp"
#include "VANL_Module.hpp"

namespace anl {

    class ANLmoduleStacker
    {
	
    public:

	struct analysis_status
	{
	    int entry; int loop_entry;
	    int next;  int skip; int quit;

	    analysis_status()
		: entry(0), loop_entry(0),
		  next(0), skip(0), quit(0) {}
	    
	    void reset()
	    {
		entry = 0; loop_entry = 0;
		next = 0; skip = 0; quit = 0;
	    }
	};
	
    public:
	
	ANLmoduleStacker();
	ANLmoduleStacker(const ANLmoduleStacker& other);
	~ANLmoduleStacker();	
	ANLmoduleStacker& operator = (const ANLmoduleStacker& other);
	
	int get_nmodules() const { return module_chain.size(); }
	
	int add(VANL_Module* mod);
	VANL_Module* get(const std::string& key);
	VANL_Module* get(int index);
	analysis_status* status_count(int index);
	
	int show_analysis();
	void reset_status();	
	
    private:
	
	std::map<std::string, int>     module_index_table;
	std::vector<bool>              module_switch;
	std::vector<VANL_Module*>      module_chain;
	std::vector<analysis_status*>  module_status;

	bool is_module_def(const std::string& key);
	int find_module(const std::string& key);
    };
        
}
#endif
