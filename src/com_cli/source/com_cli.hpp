/**
   @file com_cli.hpp
   @author Goro Yabu
   @date 2019/03/18
   @version 2.0
**/
#ifndef com_cli_hpp
#define com_cli_hpp

#include <vector>
#include <string>

namespace com_cli
{
    const int CLI_OK = 0;
    const int CLI_NG = -1;
    
    /**
       Basic Functions
    **/
    
    /* For Setting */
    int init(std::string progname, std::string history_file="~/.cli_history");
    int end();

    /* For Reading */

    /** @brief Return the input value **/
    template<typename T> T read_value(std::string quest, const T& defvalue);

    /** @brief Return the list of input values **/
    template<typename T> std::vector<T> read_value(std::string quest, std::vector<std::string> table, const std::vector<T>& defvalue);

    /** @brief Return the index of the selected keyword **/
    int read_keyword(std::string quest, std::vector<std::string> table);//, const int& defvalue=-1);

    /** @brief Return the index list of the selected keywords **/
    std::vector<int> read_keyword(std::string quest, std::vector<std::string> table, int nreply);//, const std::vector<int>& defvalue);

    /** @brief Return the input text not splited by the delimiter **/
    std::string read_text(const std::string quest, const std::string& text);

    /** @brief Return true or false **/
    bool ask_yesno(std::string quest, const bool& defanswer);
    
    /*--------------------*/
    
    /**
       Extended Functions
    **/
    
    /* For Manipulating Strings */
    int to_upper(std::string* str);
    int to_upper(char* str);
    int erase_endspace(std::string* str);
    int remove_comment(std::string* str, std::string sign="#");
    int is_only_integer(std::string& str);
    int find_keyword(const std::vector<std::string>& table, std::string word, int* index);
    
    /* For Show Messages */
    int cli_error(int level, std::string place, std::string message);
    int show_keyword(std::string message, const std::vector<std::string>& table);
    
    /* For Setting of the Custom Tab-Completion */
    int enable_custom_complete();
    int disable_custom_complete();
    template<typename... Arg> int set_candidates(Arg... arg);
    int set_candidates(const std::vector<std::string>& list);
}   
#endif
