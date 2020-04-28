/**
   @file   ReadTTree.hpp
   @date   2020/04/04
   @author goroyabu
   @detail Automatically generated by make_anlpy_project.sh 1.0.0
**/

#ifndef ReadTTree_hpp
#define ReadTTree_hpp

#include <vector>
#include <string>
#include <map>

#include <TTree.h>

#include <bnk.hpp>
#include <VANL_Module.hpp>

class TFile;
class TLeaf;

class ReadTTree : public anl::VANL_Module
{
public:
    ReadTTree();
    ~ReadTTree();

    int mod_bgnrun() override;
    int mod_ana() override;
    int mod_endrun() override;

protected:
    TFile * input_file;
    TTree * input_tree;
    long nentries;

    template<typename T>
    int read_branch
    (const std::string& key, int maxsize=1, const std::string& leafcount="");

    int switch_branch
    (const std::string& selection, const std::string& delim, bool enable);
    int read_all_branch();
    int get_maxsize(TLeaf* leaf);
    std::string get_leafcount(TLeaf* leaf);
    
    std::map< std::string, std::vector<int>* > vptr_list_int;
    std::map< std::string, std::vector<float>* > vptr_list_float;
    std::map< std::string, std::vector<double>* > vptr_list_double;

    int read_vint_branch(const std::string& key);
    int read_vfloat_branch(const std::string& key);
    int read_vdouble_branch(const std::string& key);

    void print_branch_info
    (const std::string& key, const std::string& type, int size);
    
    // std::vector< std::vector<int>* > ptrlist_int;
    // std::vector< std::vector<float>* > ptrlist_float;
    // std::vector< std::vector<double>* > ptrlist_double;
    // std::vector< std::string > strlist_double;
    // std::vector<double>* edep;
    // int dp;
    // std::vector<double>* pos_x;
    // std::vector<double>* pos_y;
    // std::vector<double>* pos_z;    
    //std::vector<double> * e;
};

template<typename T>
int ReadTTree::read_branch
(const std::string& key, int maxsize, const std::string& leafcount)
{       
    if ( maxsize==1 ) bnk::define<T>(key);
    else {
	bnk::define<T>(key, maxsize);    
	bnk::resize<T>(key, maxsize);
    }
    
    if( !input_tree->FindBranch(key.c_str()) ) return anl::ANL_NG;
    
    input_tree->SetBranchAddress(key.c_str(), bnk::getptr<T>(key));

    if ( leafcount == "" )  return anl::ANL_OK;    
    bnk::setkeytosize<T>( key, leafcount ); return anl::ANL_OK;

    // if ( leafcount == "std::vector" ) {

    // 	if ( key != "edep" ) return anl::ANL_OK;
	
    // 	//bnk::resize<T>(key, 32);
    // 	//auto ptr = bnk::getvecptr<T>(key);

    // 	//std::vector<double> * ptr = nullptr;
    // 	//dlistptr.emplace_back(nullptr);
    // 	//dlistptr.emplace_back( new std::vector<double> );
    // 	ptrlist_double.emplace_back( bnk::getvecptr<double>(key) );
    // 	input_tree->SetBranchAddress(key.c_str(), &(dlistptr[dlistptr.size()-1]) );
	

    // 	//input_tree->SetBranchAddress(key.c_str(), &(bnk::getvecptr<T>(key)) );
	
    // 	//input_tree->SetBranchAddress(key.c_str(), &e);
	
    // 	return anl::ANL_OK;
    // } else 
}

#endif
