/**
   @file   WriteTTree.hpp
   @date   2020/04/08
   @author goroyabu
   @detail Automatically generated by make_anlpy_project.sh 1.0.0
**/

#ifndef WriteTTree_hpp
#define WriteTTree_hpp

#include <vector>
#include <string>
#include <map>
#include <iostream>

#include <TTree.h>

#include <bnk.hpp>
#include <VANL_Module.hpp>

class TFile;

class WriteTTree : public anl::VANL_Module
{
public:
    WriteTTree();
    ~WriteTTree();

    int mod_bgnrun() override;
    int mod_ana() override;
    int mod_endrun() override;

protected:
    TFile * output_file;
    TTree * output_tree;
    std::vector<std::string> disabled_branch;

    std::map< std::string, std::vector<int>* > vptr_list_int;
    std::map< std::string, std::vector<float>* > vptr_list_float;
    std::map< std::string, std::vector<double>* > vptr_list_double;

    template<typename T>
    int write_branch(const std::string& key, const std::string& typesymbol);

    // template<typename T>
    // int write_vector_branch(const std::string& key, const std::string& leaflist);

    int switch_branch
    (const std::string& selection, const std::string& delim, bool enable);
    
    std::string replace_all
    (std::string before, const std::string& from, const std::string& to);

    std::string convert_regex(std::string wildcard);
    
    int write_all_branch();

    bool is_disabled(const std::string& key);
};

template<typename T>
int WriteTTree::write_branch(const std::string& key, const std::string& typesymbol)
{
    auto sizekey = bnk::getkeytosize<T>(key);
    
    // if ( sizekey=="std::vector" ) {
    // 	std::cout << "vector " << key << std::endl;
    // 	write_vector_branch<T>( key, "vector" );
    // 	return anl::ANL_OK;
    // }
    
    auto leaflist = key+"/"+typesymbol;	
    if ( sizekey!="1" ) leaflist = key+"["+sizekey+"]/"+typesymbol;
    
    // std::cout << leaflist << std::endl;
    
    auto branch = output_tree->Branch
	( key.c_str(), bnk::getptr<T>(key), leaflist.c_str() );
    if ( !branch ) return anl::ANL_NG;
    
    return anl::ANL_OK;
}

// template<typename T> int WriteTTree::write_vector_branch
// (const std::string& key, const std::string& leaflist)
// {
//     vptr_list_double[ key ] = bnk::getvecptr<double>(key);
//     return anl::ANL_OK;
// }

#endif
