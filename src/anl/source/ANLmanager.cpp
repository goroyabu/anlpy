/**
   @date 2020/04/02
**/

#include "ANLmanager.hpp"

#include <bnk.hpp>
#include <evs.hpp>

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
using std::flush;

#include <iomanip>
using std::right;
using std::left;
using std::setw;

anl::ANLmoduleStacker* anl::ANLmanager::module_stacker;

anl::ANLmanager::ANLmanager()
    : VANL_Module( "ANLmanager", "1.0" )
{
    if ( !module_stacker )
	module_stacker = new ANLmoduleStacker();
    define_parameter<std::string>( "program_name", "ANL" );
    define_parameter<int>( "display_interval_msec", 1000000.0 );
}
anl::ANLmanager::ANLmanager(const ANLmanager& other)
    : VANL_Module(other)
{
    module_stacker = other.module_stacker;
}
anl::ANLmanager::~ANLmanager()
{
    this->exit_process();
}

int anl::ANLmanager::add_module(VANL_Module* mod)
{
    return module_stacker->add(mod);
}
int anl::ANLmanager::show_analysis()
{
    if ( module_stacker->get_nmodules()==0 ) {
	return message(1, "ANLmanager::show_analysis", "No module is defined");
    }
    
    cout << endl;
    cout << "      ***************************" << endl;
    cout << "      ***** Analysis chain ******" << endl;
    cout << "      ***************************" << endl;
    cout << endl;

    module_stacker->show_analysis();
    this->show_parameters();

    return ANL_OK;
}
int anl::ANLmanager::read_data(long int nevent, long int print_freq)
{
    int nmodules = module_stacker->get_nmodules();
    if ( nmodules==0 ) 
	return message(1, "ANLmanager::show_analysis", "No module is defined");
    
    // auto msec = get_parameter<int>("display_interval_msec");
    // auto usleep_time = std::chrono::microseconds( msec );    

    bnk::init();
    bnk::define<long>("ANL_eventid");
    bnk::put<long>("ANL_eventid", 0);
    // bnk::define<std::string>("ANLmanager::program_name");
    // auto program_name = get_parameter<std::string>("program_name");
    // bnk::put<std::string>("ANLmanager::program_name", program_name);

    evs::clear_all();

    if ( this->call_init_and_his() != ANL_OK ) return ANL_NG;
    if ( this->call_bgnrun() != ANL_OK ) return ANL_NG;    

    auto run_status = new ANLmoduleStacker::analysis_status();   

    auto status       = ANL_OK;
    bool run_has_quit = false;
    bool run_broke = false;

    long ievent = -1;
    while ( ++ievent<nevent && run_broke==false ) {
	//++ievent;
	bnk::put<long>("ANL_eventid", ievent);
	
	if ( ievent%print_freq == 0 ) {
	    cout << ievent << "/" << nevent;
	    cout << "(" << (double)ievent/nevent*100.0 << "%)" << endl;
	}

	bool event_has_evs_count = true;
	bool event_is_discarded  = false;
	
	for ( int imodule=0; imodule<nmodules; ++imodule ) {	    
	    
	    auto module = module_stacker->get(imodule);	    
	    status = module->mod_ana();

	    auto status_count = module_stacker->status_count(imodule);
	    status_count->entry++;
	    
	    // if ( status == ANL_OK ) continue;
	    if( status == ANL_QUIT ) 
		status = ANL_ENDLOOP + ANL_DISCARD + ANL_NOCOUNT;	    
	    else if ( status == ANL_SKIP )
		status = ANL_DISCARD;
	    else if ( status == ANL_LOOP )
		status = ANL_ENDLOOP + ANL_DISCARD + ANL_NOCOUNT + ANL_NEWROOT;
		// status = ANL_NEWROOT;

	    if ( status & ANL_NOCOUNT )
		event_has_evs_count = false;

	    // if ( status & ANL_ENDLOOP ) {
	    // 	nest_level = 0; n_rest_events = ievent;
	    // }

	    if ( status & ANL_NEWROOT )
		run_broke = true;
	    
	    if ( status & ANL_DISCARD ) {
		if( status & ANL_ENDLOOP ) {
		    run_has_quit = true; status_count->quit++;
		}
		else status_count->skip++;
		event_is_discarded = true; break;
	    }
	    
	    // if ( status & ANL_NEWROOT ) {
	    // 	loop_return[nest_level] = imodule; nest_level++;
	    // }

	    status_count->next++;
	    
	} /* for imodule */

	if ( event_has_evs_count ) evs::accumulate();	

	// if ( nest_level > 0 ) {
	//     nest_level--; imodule = loop_return[nest_level];
	//     get_count(imodule)->loop_entry++;
	// }	    	
	// if ( is_discarded ) irecv++;
		
    } /* while ievent */

    if ( run_has_quit == false ) run_status->quit++;
    if ( this->call_endrun() != ANL_OK ) return ANL_NG;
    
    this->show_status();
    
    return ANL_OK;    
}
int anl::ANLmanager::show_status()
{   
    cout << endl;
    cout << "      ***************************" << endl;
    cout << "      ***** Analysis chain ******" << endl;
    cout << "      ***************************" << endl;
    cout << endl;
    
    int nmodules = module_stacker->get_nmodules();
    if ( nmodules == 0 ) return ANL_OK;
    
    auto first_count = module_stacker->status_count(0);
    
    cout << "               PUT: " << first_count->entry << endl;
    
    if( first_count->quit > 0 ) 
	cout << "QUIT:           |" << first_count->quit << endl;    
    else
	cout << "                |" << endl;
    
    
    for(int imodule=0; imodule<nmodules; imodule++){
	
	auto status_count = module_stacker->status_count( imodule );
	if ( !status_count ) continue;
	
	auto module  = module_stacker->get( imodule );
	
        if( status_count->quit > 0 )
	    cout << "<--- ";
        else
	    cout << "     ";

        cout << "[" << setw(2) << right << imodule << "]" << "  ";
        cout << setw(26) << left << module->mod_name() << "  ";

        if( status_count->loop_entry > 0 ){
            cout << "version " << module->mod_version() << endl;
            cout << "   <------- LOOP: " << status_count->loop_entry << endl;
        }else{
            cout << "version " << module->mod_version() << endl;
        }

	auto next_module_status = module_stacker->status_count( imodule+1 );
	if ( !next_module_status || next_module_status->quit == 0 ) {
	    cout << "                | OK: " << status_count->next;
            cout << "/" << status_count->entry;
	} else {
	    cout << "QUIT: " << next_module_status->quit;
	    cout << "         | OK: " << status_count->next;
	    cout << "/" << status_count->entry;	    
	}
	
        if( status_count->skip > 0 ) {
	    cout << "                      -------> SKIP: ";
	    cout << status_count->skip << endl;
	} else
	    cout << endl;
    }

    auto final_count = module_stacker->status_count( nmodules-1 );
    cout << "               GET: " << final_count->next << endl;
    
    cout << endl;
    evs::output();
    
    cout << endl;
    bnk::list();

    return ANL_OK;
}
int anl::ANLmanager::reset_status()
{
    module_stacker->reset_status();
    evs::reset_all();
    return ANL_OK;
}
anl::VANL_Module* anl::ANLmanager::get_module(const std::string& key)
{
    return module_stacker->get(key);
}

int anl::ANLmanager::call_init_and_his()
{
    auto status = ANL_OK;
    auto nmodules = module_stacker->get_nmodules();
    
    for ( int imodule=0; imodule<nmodules; ++imodule ) {
	
	auto module = module_stacker->get( imodule );
	if ( !module ) continue;
	
	cout << flush; status = module->mod_init(); cout << flush;
	if ( status != ANL_OK ) return status; 
	
	cout << flush; status = module->mod_his(); cout << flush;
	if ( status != ANL_OK ) return status;
	
    }
    return status;
}
int anl::ANLmanager::call_bgnrun()
{
    auto status = ANL_OK;
    auto nmodules = module_stacker->get_nmodules();
    
    for ( int imodule=0; imodule<nmodules; ++imodule ) {

	auto module = module_stacker->get( imodule );
	if ( !module ) continue;
	
	cout << flush; status = module->mod_bgnrun(); cout << flush;
    }
    return status;
}
int anl::ANLmanager::call_endrun()
{
    auto status = ANL_OK;
    auto nmodules = module_stacker->get_nmodules();
    
    for ( int imodule=0; imodule<nmodules; ++imodule ) {

	auto module = module_stacker->get( imodule );
	if ( !module ) continue;
	
	cout << flush; status = module->mod_endrun(); cout << flush;
    }
    return status;
}
int anl::ANLmanager::exit_process()
{
    auto status = ANL_OK;
    auto nmodules = module_stacker->get_nmodules();
    
    for ( int imodule=0; imodule<nmodules; ++imodule ) {

	auto module = module_stacker->get( imodule );
	if ( !module ) continue;
	
	cout << flush; status = module->mod_exit(); cout << flush;
    }
    
    bnk::end();
    // evs::end();
    
    return status;
}

int anl::ANLmanager::message(int level, std::string place, std::string message)
{
    if(level<=0){
        cerr << "ANL-Information in ";
	cerr << place << " : " << message << endl;
	return ANL_OK;
    }else if(level==1){
        cerr << "ANL-WARNING in ";
	cerr << place << " : " << message << endl;
	return ANL_NG;
    }else if(level>=2){
        cerr << "ANL-ERROR in ";
	cerr << place << " : " << message << endl;
	return ANL_NG;
    }    
    return ANL_OK;
}
