/**
   @file com_cli_1.cpp
   @author Goro Yabu
   @date 2019/03/18
   @version 2.0
**/
#include "com_cli.hpp"
#include "com_cli_private.hpp"

#include <sstream>
#include <iomanip>
#include <cstring>
#include <iostream>

#include <readline/readline.h>
#include <readline/history.h>

namespace ccp = com_cli::com_cli_private;
/**
   @brief Initialization
**/
int com_cli::init(std::string progname, std::string his_name)
{
    ccp::set_progname( progname );
    using_history();
    ccp::set_historyfile( his_name );
    read_history( his_name.c_str() );
    return CLI_OK;
}
/**
   @brief
**/
int com_cli::end()
{
    write_history( ccp::get_historyfile().c_str() );
    return CLI_OK;
}
/**
   @brief Read a value from standard input.
   @param[in] quest Question or description of value for app users.
   @param[in,out] value A variable modifed to new input value.
   
   @details In the source code,
   @code
   int n = 100;
   com_cli::read_value<int>("number of event", &n);
   @endcode
   
   Then,
**/
template<typename T> T com_cli::read_value(std::string quest, const T& defvalue)
{
    using namespace std;
    T buf = defvalue;
    stringstream ss; ss << defvalue;
    string line = ss.str();
    
    ccp::get_line(quest, &line);
    try{
	istringstream(line) >> buf;
	return buf;
    }
    catch(exception &e){
	//*value = buf;
	return buf;
    }
}
template int         com_cli::read_value<int>        (std::string quest, const int&         value);
template float       com_cli::read_value<float>      (std::string quest, const float&       value);
template double      com_cli::read_value<double>     (std::string quest, const double&      value);
template std::string com_cli::read_value<std::string>(std::string quest, const std::string& value);

std::string com_cli::read_text(const std::string quest, const std::string& text)
{
    auto buf = text;
    ccp::get_line(quest, &buf);
    return buf;

}

/**
   @brief Ask the question and 'yes' is modified into true when typed "yes" or false for "no".
**/
bool com_cli::ask_yesno(std::string quest, const bool& defanswer)
{
    std::string buf;
    if ( defanswer ) buf = "yes";
    else buf = "no";
    
    quest = quest+"(y/n)";
    auto answer = read_value<std::string>(quest, buf);

    if(answer=="yes" || answer=="y" || answer=="YES" || answer=="Y"){
	return true;
    }else if(answer=="no" || answer=="n" || answer=="NO" || answer=="N"){
	return false;
    }
    
    return defanswer;
}

template<typename T> std::vector<T> com_cli::read_value
(std::string quest, std::vector<std::string> table, const std::vector<T>& defvalue)
{
    using namespace std;
    using namespace com_cli::com_cli_private;

    std::vector<T> answers;
    int ntab = (int)table.size();
    int nval = (int)defvalue.size();
    if(nval<=0 || nval!=ntab) return answers;
    
    cout << " " << quest << endl;
    cout << endl;    
    for(int i=0; i<nval; ++i) {
        answers.emplace_back( read_value<T>(table[i], defvalue[i]) );
    }
    //return com_cli::CLI_OK;
    return answers;
}
template std::vector<int>com_cli::read_value<int>
(std::string quest, std::vector<std::string> table, const std::vector<int>& defvalue);
template std::vector<short int>com_cli::read_value<short int>
(std::string quest, std::vector<std::string> table, const std::vector<short int>& defvalue);
template std::vector<unsigned int>com_cli::read_value<unsigned int>
(std::string quest, std::vector<std::string> table, const std::vector<unsigned int>& defvalue);
template std::vector<unsigned short int>com_cli::read_value<unsigned short int>
(std::string quest, std::vector<std::string> table, const std::vector<unsigned short int>& defvalue);
template std::vector<long int> com_cli::read_value<long int>
(std::string quest, std::vector<std::string> table, const std::vector<long int>& defvalue);
template std::vector<float> com_cli::read_value<float>
(std::string quest, std::vector<std::string> table, const std::vector<float>& defvalue);
template std::vector<double> com_cli::read_value<double>
(std::string quest, std::vector<std::string> table, const std::vector<double>& defvalue);
template std::vector<std::string> com_cli::read_value<std::string>
(std::string quest, std::vector<std::string> table, const std::vector<std::string>& defvalue);

int com_cli::read_keyword(std::string quest, std::vector<std::string> table)//, const int& defvalue)
{
    //std::vector<int> vanswer(1, defvalue);
    int nreply = 1;
    //if( read_keyword(quest, table, nreply, vanswer)==com_cli::CLI_NG ) return com_cli::CLI_NG;
    auto vanswer = read_keyword(quest, table, nreply);// vanswer);
    if( vanswer.size()<=0 ) return -1;
    //*answer = vanswer[0];
    return vanswer[0];
}
/**
   @param quest
   @param table Table of string
   @param nreply This function tries to pick-up exactly nreply answers when nreply>0, and equal to or more than nreply when nreply<=0.
   @param answer Array of index that corresponding to elements of options
**/
std::vector<int> com_cli::read_keyword(std::string quest, std::vector<std::string> table, int nreply)//, const std::vector<int>& defvalue)
{
    using namespace std;
    
    com_cli::enable_custom_complete();
    com_cli::set_candidates(table);
    
    string prompt = ccp::get_progname();
    if(prompt.size()!=0) prompt = prompt+": ";
    stringstream ss;
    if(nreply<=0) ss << prompt << "Select " << setw(3) << -nreply << " or More, Then OK";
    else ss << prompt << "Select " << setw(3) << nreply << " Option";
    
    
    string line; int nwords; vector<string> words;
    while(true){
        line = ""; nwords = 0; words.clear();
	
        if( com_cli::show_keyword(quest, table)<0 ) {
	    com_cli::cli_error(2, "com_cli::read_keyword", "Size of table <= 0");
	    return std::vector<int>();
	}
	cout << endl;
	cout << ss.str() << endl;
        ccp::get_line("", &line);
	ccp::word_split(line, &words);
	
        nwords = (int)words.size();
        if(nwords<=0) continue;

	if(nreply>0 && nwords==nreply) break;
	
	com_cli::to_upper(&words[nwords-1]);
        if(nreply<=0 && strcmp(words[nwords-1].c_str(),"OK")==0) break;
	
        cout << endl;
    }
    
    int itemp; std::vector<int> answer;
    for(auto word : words){
        if(com_cli::find_keyword(table, word, &itemp)==0) answer.emplace_back(itemp);
    }
    com_cli::disable_custom_complete();
    cout << endl;

    //if((int)answer->size()!=nreply) return answer;//com_cli::CLI_NG;
    return answer;//com_cli::CLI_OK;
}
int com_cli::find_keyword(const std::vector<std::string>& table, std::string word, int* index)
{
    std::string elem_up = word; com_cli::to_upper(&elem_up);
    int ntab = table.size(); *index = ntab;
    for(int i=0; i<ntab; ++i){
        std::string buf = table[i]; com_cli::to_upper(&buf);
        if( strcmp(elem_up.c_str(),buf.c_str())==0 ){ *index = i; return CLI_OK; }
    }
    return com_cli::CLI_NG;
}
int com_cli::show_keyword(std::string message, const std::vector<std::string>& table)
{
    using namespace std;
    cout << " " << message << endl; cout << endl;
    
    int nopt = (int)table.size();
    stringstream ss; ss << " ";
    int ss_line = 0;
    
    for(int i=0; i<nopt; ++i){
        if(ss.str().length()+table[i].length()-ss_line*44>44){ ss << "\n " << table[i]+" "; ss_line++;}
	else ss << table[i]+" ";
    }
    cout << ss.str() << endl;
    return com_cli::CLI_OK;
}
