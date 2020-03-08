/**
   @file ANLCross_1.cpp
   @author Goro Yabu
   @date 2018/12/04
   @date 2019/03/29 v2.0 ANLCross is chaged to a series of functions from a class
   @version 2.0
**/
#include "ANLCross.hpp"
int anlcross::anl_define_analysis()
{
    using namespace std;
    cout << "You can switch on/off modules." << endl;
    cout << endl;

    int nmodules = get_nmodules();
    if ( nmodules < 1 ) cout << "No analysis chain is defined. " << endl;
    else show_module_switch();

    while(true){
	int mod_num = -1;
	com_cli::read_value<int>("What module is switched on/off:( to exit: -1)", &mod_num);
	
	if(mod_num == -1) break;
	else if( (mod_num < -1) || (mod_num >= nmodules) ) cout << "No such module !" << endl;
	else{
	    enable_module(mod_num, !is_module_on(mod_num));
	    show_module_switch();
	}	
    }

    show_module_switch();    
    return ANL_OK;
}
int anlcross::anl_show_analysis()
{
    using namespace std;
    cout << endl;
    cout << "      ***************************" << endl;
    cout << "      ***** Analysis chain ******" << endl;
    cout << "      ***************************" << endl;
    cout << endl;

    int nmodules = get_nmodules();
    if ( nmodules < 1) cout << "No analysis chain is defined. " << endl;
    else{
	for(int i=0; i<nmodules; i++){
	    cout << right << setw(5) << i << "   ";
	    cout << left << setw(20) << anl_routine_name(i) << "   ";
	    cout << setw(10) << anl_routine_version(i) << endl;
	}
    }
    cout << endl;
    return ANL_OK;
}
int anlcross::anl_modify_param()
{
    std::vector<std::string> table;
    int nmodules = get_nmodules();
    for(int i=0; i<nmodules; i++) table.push_back( anl_routine_name(i) );
    
    table.push_back("DISPLAY");
    table.push_back("NONE");
    table.push_back("FULL");
    int tablesize = table.size();
    
    //Ask the question
    std::vector<int> ichoice;
    com_cli::read_keyword("ANLCROSS *** Modify Parameter ***", table, -1, &ichoice);

    int nchoice = ichoice.size();
    if(nchoice<1) return ANL_OK;
    
    for(int i=0; i<nchoice; i++){
	int ichain = ichoice[i];// -1;
	if(ichain < 0 || ichain >= tablesize) continue;
	std::string answer = std::string(table[ichain]);
	
	if( answer == "NONE" || ichain >= tablesize || ichain < 0 ) break;
	else if(answer == "DISPLAY"){
	    
	    int msec = 1;//mUSleepTime/1000;
	    com_cli::read_value<int>("Display interval [msec]", &msec);
	    //mUSleepTime = msec * 1000;

	}else if( answer == "FULL"){
	    
	    for(int i=0; i<nmodules; i++){	
		anl_flush();
		std::cout << "Entering " << anl_routine_name(i) << " com()" << std::endl;
		if( is_module_on(i) ) anl_routine_com(i);
		std::cout << std::endl;
		anl_flush();
	    }
	    break;
	    
	}else{
	    anl_flush();
	    std::cout << "Entering " << anl_routine_name(ichain) << " com()" << std::endl;
	    if( is_module_on(i) ) anl_routine_com(i);
	    anl_flush();
	}	
    }
    return ANL_OK;
}
int anlcross::anl_book_histgram()
{
    return ANL_OK;
}
int anlcross::anl_talk_menu()
{
    std::vector<std::string> table = {
	"SHOW_ANALYSIS",
	"DEFINE_ANALYSIS",
	"MODIFY_PARAM",
	"ANALYZE_DATA",
	"QUIT"
    };
    std::vector<int> ichoice;
    
    while(true){
	com_cli::init("ANL", ".anl_history");
	if( com_cli::read_keyword("ANLCROSS *** Talk Menu ****", table, 1, &ichoice)==com_cli::CLI_NG ) continue;

	std::string answer( table[ ichoice[0] ] );
	if( answer == "QUIT"){ return ANL_QUIT; }
	else if(answer == "SHOW_ANALYSIS") anl_show_analysis();
	else if(answer == "DEFINE_ANALYSIS") anl_define_analysis();
	else if(answer == "MODIFY_PARAM") anl_modify_param();
	else if(answer == "ANALYZE_DATA") break;
    }
    return ANL_OK;
}
int anlcross::anl_read_data()
{
    evs::EvsClrAll();    
    if( call_bgnrun()!=ANL_OK ) return ANL_NG;
    
    int loop_return[MAXNEST];
    int nmodules = get_nmodules();
    int quit_flag = 0; int imodule = 0; int status = ANL_OK;
    int ievent, irecv, frq_count, nest_level, evs_count, nevent, eventid;

    // ask event parameter
    int n_rest_events = 1;
    com_cli::read_value<int>("Number of events (-1=all, 0=exit)", &n_rest_events);
    if(n_rest_events == 0){
	get_count(imodule)->quit++;    
	if( call_endrun()!=ANL_OK ) return ANL_NG;
	return ANL_OK;
    }

    static int event_freq = 1000;
    com_cli::read_value<int>("Event number printout frequency", &event_freq);    

    // main loop     
    ievent = 0; irecv = 0; frq_count = 0; nest_level = 0;
    while( n_rest_events == -1 || (ievent != n_rest_events && irecv  != -n_rest_events) ){
	ievent++; nevent = ievent; frq_count++;

	//std::cout << "Event.... " << ievent << "(" << irecv << ")" << std::endl; anl_flush();
	if( frq_count == 1 ){ std::cout << "Event.... " << ievent << "(" << irecv << ")" << std::endl; anl_flush(); }
	if( frq_count == event_freq ) frq_count = 0;
	
	imodule = 0;	  
	do{
	    evs_count = ANL_TRUE;
	    while( imodule < nmodules ){
		status = ANL_OK;
		
		if( is_module_on(imodule) ){
		    //std::cout << "imodule " << imodule << std::endl;
		    get_count(imodule)->entry++;
		    status = anl_routine_ana(imodule, nevent, eventid);
		    //std::cout << "ok" << std::endl;
		}
		
		if( status != ANL_OK ){
		    if( status == ANL_QUIT ){ status = ANL_ENDLOOP + ANL_DISCARD + ANL_NOCOUNT; } 
		    else if( status == ANL_SKIP ) status = ANL_DISCARD;		      
		    else if( status == ANL_LOOP ) status = ANL_NEWROOT;
		    
		    if( status & ANL_NOCOUNT ) evs_count = ANL_FALSE;
		    
		    if( status & ANL_ENDLOOP ){ nest_level = 0; n_rest_events = ievent; }
		    
		    if( status & ANL_DISCARD ){			  
			if( status & ANL_ENDLOOP ){ quit_flag++; get_count(imodule)->quit++; }
			else{ get_count(imodule)->skip++; }
			imodule = nmodules; break;
		    }
		    
		    if( status & ANL_NEWROOT ){ loop_return[nest_level] = imodule; nest_level++; }
		}
		get_count(imodule)->next++; imodule++;
	    }//end while
	    
	    if( evs_count ) evs::EvsAcm();		
	    if( nest_level > 0 ){ nest_level--; imodule = loop_return[nest_level]; get_count(imodule)->loop_entry++; }
	    
	} while ( imodule < nmodules );
	
	if ( imodule == nmodules ) irecv++;
    }   
    
    if( quit_flag == 0 ) get_count(imodule)->quit++;    
    if( call_endrun()!=ANL_OK ) return ANL_NG;

    return ANL_OK;
}
int anlcross::anl_save_hist()
{
    return ANL_OK;
}
int anlcross::anl_clear_hist()
{
    return ANL_OK;
}
int anlcross::anl_show_status()
{
    using namespace std;
    
    cout << endl;
    cout << "      ***************************" << endl;
    cout << "      ***** Analysis chain ******" << endl;
    cout << "      ***************************" << endl << endl;
    cout << "               PUT: " << get_count(0)->entry << endl;
    
    if( get_count(0)->quit > 0 ) cout << "QUIT:           |" << get_count(0)->quit << endl;
    else cout << "                |" << endl;

    int nmodules = get_nmodules();
    for(int i=0; i<nmodules; i++){
	if( !is_module_on(i) ) continue;
	if( get_count(i)->quit > 0 ) cout << "<--- ";
	else cout << "     ";
	
	cout << "[" << setw(2) << right << i << "]" << "  ";
	cout << setw(26) << left << anl_routine_name(i) << "  ";
	
	if( get_count(i)->loop_entry > 0 ){
	    cout << "version " << anl_routine_version(i) << endl;
	    cout << "   <------- LOOP: " << get_count(i)->loop_entry << endl;
	}else{
	    cout << "version " << anl_routine_version(i) << endl;
	}	
	
	if( i == nmodules || get_count(i+1)->quit == 0 ){    
	    cout << "                | OK: " << get_count(i)->next;
	    cout << "/" << get_count(i)->entry;
	}else{
	    cout << "QUIT: " << get_count(i+1)->quit;
	    cout << "         | OK: " << get_count(i)->next;
	    cout << "/" << get_count(i)->entry;
	}
	
	if( get_count(i)->skip > 0 ) cout << "                      -------> SKIP: " << get_count(i)->skip << endl;
	else cout << endl;
    }
    
    cout << "               GET: " << get_count(nmodules-1)->next << endl;

    cout << endl;
    evs::EvsOut();
    
    cout << endl;
    bnk::bnk_list();
    
    return ANL_OK;
}
int anlcross::anl_reset_status()
{
    int nmodules = get_nmodules();
    for(int i = 0; i< nmodules; i++){
	get_count(i)->entry = 0;
	get_count(i)->loop_entry = 0;
	get_count(i)->next = 0;
	get_count(i)->skip = 0;
	get_count(i)->quit = 0;
    }
    evs::EvsRstAll();
    return ANL_OK;
}

int anlcross::anl_ana_menu()
{
    std::vector<std::string> table = {
	"READ_DATA",
	"SHOW_STATUS",
	"RESET_STATUS",
	"EXIT"
    };
    std::vector<int> ichoice;
    
    while(true){
	if( com_cli::read_keyword("ANLPLUS *** Analysis Menu ****", table, 1, &ichoice) == com_cli::CLI_NG ) continue;
	std::string answer( table[ ichoice[0] ] );
	if(answer == "EXIT") break;
	else if(answer == "READ_DATA") anl_read_data();
	else if(answer == "SHOW_STATUS") anl_show_status();
	else if(answer == "RESET_STATUS") anl_reset_status();
    }
    return anl_show_status();
}
int anlcross::anl_exit_proc()
{
    int nmodules = get_nmodules();
    for(int i = 0; i < nmodules; i++ ){
	anl_flush();
	if( is_module_on(i) ){
	    std::cout << "Entering " << anl_routine_name(i) << " exit()" << std::endl;
	    anl_routine_exit(i);
	}
	anl_flush();	
    }
    bnk::bnk_end();
    evs::EvsEnd();
    return ANL_OK;
}
int anlcross::anl_flush()
{
    std::cout.flush();
    std::cerr.flush();
    return ANL_OK;
}
int anlcross::anl_error(int level, std::string place, std::string message)
{
    if(level==0){
        std::cout << "ANL-Information in ";
    }else if(level==1){
        std::cout << "ANL-WARNING in ";
    }else if(level==2){
        std::cout << "ANL-ERROR in ";
    }
    std::cout << place << " : " << message << std::endl;
    return ANL_NG;
}
