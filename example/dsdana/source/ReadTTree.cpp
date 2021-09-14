/**
   @file   ReadTTree.cpp
   @date   2020/04/04
   @author goroyabu
   @detail Automatically generated by make_anlpy_project.sh 1.0.0
**/

#include "ReadTTree.hpp"

#include <TFile.h>
#include <TLeaf.h>
#include <TH1D.h>

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <iomanip>
using std::setw;

#include <sstream>

#include <cmath>

#include <bnk.hpp>
#include <evs.hpp>

ReadTTree::ReadTTree()
    : anl::VANL_Module("ReadTTree", "0.2"),
      input_file(nullptr),
      input_tree(nullptr)
{
    /** Parameters can be modified via a method 'SetParameter' in Python **/
    define_parameter<std::string>("file_name", "example_data_cc.root");
    define_parameter<std::string>("tree_name", "eventtree");
    define_parameter<std::string>("branch_select", "ALL");
    define_parameter<std::string>("branch_ignore", "NONE");

    const static int reserved_size_of_string_branch = 1000;
    string_to_reset = std::string(reserved_size_of_string_branch, '\0');
    // dp = 0;
    // pos_x = nullptr;
    // pos_y = nullptr;
    // pos_z = nullptr;
}
ReadTTree::~ReadTTree()
{
}

int ReadTTree::mod_bgnrun()
{
    /** This function is called at the begging of the run **/
    // cout << mod_name2() << "::mod_bgnrun()";
    // cout << " -> Hello! :D" << endl;
    cout << "=========================" << endl;
    cout << "  ReadTTree::mod_bgnrun  " << endl;
    cout << "=========================" << endl;

    
    auto file_name = get_parameter<std::string>( "file_name" );
    input_file = new TFile( file_name.c_str() );    
    if ( !input_file || input_file->IsZombie() ) {
	cerr << "***Error*** : " << file_name << " is not found" << endl;
	return anl::ANL_NG;
    }
    cout << "Opening " << file_name << " is succeeded." << endl;

    
    auto tree_name = get_parameter<std::string>( "tree_name" );
    input_tree = (TTree*)input_file->Get( tree_name.c_str() );
    if ( !input_tree ) {
	cerr << "***Error*** : " << tree_name << " is not found" << endl;
	return anl::ANL_NG;
    }
    cout << tree_name << " is found." << endl;
    
    nentries = input_tree->GetEntries();
    cout << "Total Entries = " << nentries << endl;
    evs::put("ANL_totalevents", nentries);

    auto branch_select = get_parameter<std::string>("branch_select");
    this->switch_branch(branch_select, ":", true);

    auto branch_ignore = get_parameter<std::string>("branch_ignore");
    this->switch_branch(branch_ignore, ":", false);
    
    static const int nbins = 20;
    this->h_of_order_of_velems
        = new TH1D( "h_of_order_of_velems", "", nbins, -0.5, nbins-0.5 );

    auto status = this->read_all_branch();
    if ( status != anl::ANL_OK ) return anl::ANL_NG;
    
    bnk::define<long>( "ReadTTree_entry" );
    bnk::put<long>( "ReadTTree_entry", -1 );

    return anl::ANL_OK;
}

int ReadTTree::mod_ana()
{
    auto tree_entry = bnk::get<long>( "ReadTTree_entry" ) + 1;
    // if ( tree_entry >= nentries ) return anl::ANL_QUIT;
    if ( tree_entry >= nentries ) return anl::ANL_LOOP;

    for ( auto key : list_of_string_brances )
	bnk::put<std::string>( key, string_to_reset );
	
    input_tree->GetEntry( tree_entry );
    
    // cout << bnk::get<std::string>("proc_name") << endl;
    bnk::put<long>( "ReadTTree_entry", tree_entry ); return anl::ANL_OK;
    
    // return 
    // cout << "eventid=" << tree_entry << endl;        
    // cout << "get entry is OK" << endl;

    //auto livetime = bnk::get<unsigned int>("livetime");
    //cout << livetime  << endl;
    // auto ev =  bnk::getv<double>("edep");
    // cout << "e=";
    // if ( ev.size() != 0 ) 
    // 	for ( auto e : ev ) cout << e << ", ";
    // cout << endl;
    // cout << "end" << endl;
    
    // auto px = bnk::getv<double>("pos_x");
    // cout << "x=";
    // // //for ( auto x : *pos_x ) cout << x << ", ";
    // for ( auto x : px ) cout << x << ", ";
    // cout << endl;

    // auto py = bnk::getv<double>("pos_y");
    // // cout << "member y=";
    // // for ( auto y : *pos_y ) cout << y << ", ";
    // // cout << endl;
    // cout << "bnk getv y=";
    // for ( auto y : py ) cout << y << ", ";
    // cout << endl;
            
    // auto pz = bnk::getv<double>("pos_z", 0, 1);
    // cout << "z=";
    // for ( auto z : *pos_z ) cout << z << ", ";
    // cout << endl;

    
    // cout << "e=";
    // for ( auto e : *edep ) cout << e << ", ";
    // cout << endl;

    // cout << "size=" << ptrlist_double[0]->size() << endl;
    // for ( auto y : *bnk::getvecptr<double>("pos_y") ) cout << y << ", ";
    
    // cout << endl;
    

    
    return anl::ANL_OK;
}

int ReadTTree::mod_endrun()
{
    /** This function is called at the end of the run **/
    // cout << mod_name2() << "::mod_endrun()";
    // cout << " -> Good Bye. :)" << endl;

    auto inname = input_file->GetName();
    input_file->Close();

    cout << inname << " is closed." << endl;
    
    return anl::ANL_OK;
}

int ReadTTree::switch_branch(const std::string& selection, const std::string& delim, bool enable)
{
    if ( (selection=="ALL" || selection=="all") && enable ) return anl::ANL_OK;
    if ( (selection=="NONE" || selection=="none") && !enable ) return anl::ANL_OK;

    std::vector<std::string> selected_branch;
    std::stringstream ss(selection); std::string word;
    while( std::getline(ss, word, delim.c_str()[0]) ){
        selected_branch.emplace_back(word);
    }

    input_tree->SetBranchStatus( "*", !enable );
    
    for ( auto branch : selected_branch ) 
	input_tree->SetBranchStatus( branch.c_str(), enable );
    
    return anl::ANL_OK;
}
int ReadTTree::read_all_branch()
{
    auto list_of_leaves = input_tree->GetListOfLeaves();
    auto nleaves = list_of_leaves->GetEntries();   

    cout << "## Branch List of " << input_tree->GetName() << " ##" << endl;
    
    auto line = std::string(62, '-');
    cout << line << endl;
    cout << setw(25) << "Type" << " " << setw(25) << "Key" << " " << setw(10) << endl;
    cout << line << endl;
    
    for ( int i=0; i<nleaves; ++i ) {
        auto leaf = (TLeaf*)list_of_leaves->At(i);
        if ( !leaf->InheritsFrom("TLeaf") ) return anl::ANL_NG;
	if ( !input_tree->GetBranchStatus( leaf->GetName() ) ) continue;
	
	auto key        = (std::string)leaf->GetName();	
	auto typestring = (std::string)leaf->GetTypeName();
	auto maxsize    = get_maxsize( leaf );
	auto leafcount  = get_leafcount( leaf );

	auto status     = anl::ANL_OK;
	
	if ( typestring == "Int_t" ) {
	    status = read_branch<int>( key, maxsize, leafcount );
	    print_branch_info(key, "int", maxsize);
	}
	else if ( typestring == "UInt_t" ) {
	    status = read_branch<unsigned int>( key, maxsize, leafcount );
	    print_branch_info(key, "unsigned int", maxsize);
	}
	else if ( typestring == "UShort_t" ) {
	    status = read_branch<unsigned short>( key, maxsize, leafcount );
	    print_branch_info(key, "unsigned short", maxsize);
	}
	else if ( typestring == "Short_t" ) {
	    status = read_branch<short>( key, maxsize, leafcount );
	    print_branch_info(key, "short", maxsize);
	}
	else if ( typestring == "Float_t" ) {
	    status = read_branch<float>( key, maxsize, leafcount );
	    print_branch_info(key, "float", maxsize);
	}
	else if ( typestring == "Double_t" ) {
	    status = read_branch<double>( key, maxsize, leafcount );
	    print_branch_info(key, "double", maxsize);
	}
	else if ( typestring == "Long_t" ) {
	    status = read_branch<long>( key, maxsize, leafcount );
	    print_branch_info(key, "long", maxsize);
	}	
	else if ( typestring == "Long64_t" ) {
	    status = read_branch<long long>( key, maxsize, leafcount );
	    print_branch_info(key, "long long", maxsize);
	}
	else if ( typestring == "Char_t" ) {
	    bnk::define<std::string>(key);
	    bnk::put<std::string>( key, string_to_reset );
	    
	    if( !input_tree->FindBranch(key.c_str()) ) {
		status = anl::ANL_NG; continue;
	    }
	    input_tree->SetBranchAddress
		(key.c_str(), (*bnk::getvecptr<std::string>(key))[0].data() );
	    list_of_string_brances.emplace_back( key );

	    print_branch_info(key, "string", maxsize );
	}
	else if ( typestring == "vector<int>" ) {
	    status = read_vint_branch( key );
	    // print_branch_info(key, "std::vector<int>", maxsize);
	}
	else if ( typestring == "vector<float>" ) {
	    status = read_vfloat_branch( key );
	    // print_branch_info(key, "std::vector<float>", maxsize);
	}
	else if ( typestring == "vector<double>" ) {
	    status = read_vdouble_branch( key );
	    // print_branch_info(key, "std::vector<double>", maxsize);
	}	
	
	if ( status != anl::ANL_OK ) return anl::ANL_NG;
    }
    cout << line << endl;

    for ( auto&& [ key, ptr ] : vptr_list_int )
	input_tree->SetBranchAddress( key.c_str(), &(ptr) );

    for ( auto&& [ key, ptr ] : vptr_list_float )
	input_tree->SetBranchAddress( key.c_str(), &(ptr) );
    
    for ( auto&& [ key, ptr ] : vptr_list_double )
	input_tree->SetBranchAddress( key.c_str(), &(ptr) );
    
    // int n = ptrlist_double.size();
    // for ( int i=0; i<n; ++i ) {
    // 	input_tree->SetBranchAddress( strlist_double[i].c_str(), &(ptrlist_double[i]) );
    // }
    
    // auto status = bnk::define<double>("pos_x", 32);
    // status = bnk::define<double>("pos_y", 32);
    // ptrlist_double.emplace_back( bnk::getvecptr<double>("pos_x") );
    // ptrlist_double.emplace_back( bnk::getvecptr<double>("pos_y") );
    // input_tree->SetBranchAddress	
    // 	( "pos_x", &( ptrlist_double.at(0) ) );
    // //	( "pos_x", &( ptrlist_double.at(ptrlist_double.size()-1) ) );
    // //input_tree->SetBranchAddress( "pos_x", &pos_x );

    // input_tree->SetBranchAddress
    // 	( "pos_y", &( ptrlist_double.at(1) ) );
    // //( "pos_y", &( ptrlist_double.at(ptrlist_double.size()-1) ) );
    // //input_tree->SetBranchAddress( "pos_y", &pos_y );    

    cout << endl;
    return anl::ANL_OK;
}

int ReadTTree::get_maxsize(TLeaf* leaf)
{
    if ( !leaf ) return -1;
    auto maxsize = leaf->GetLen();
    if ( maxsize>0 ) return maxsize;
    auto leafcount = leaf->GetLeafCount();
    if ( !leaf ) {
	cerr << "**Warning** : Size of TLeaf " << leaf->GetName();
	cerr << " is 0 " << endl;
	return -1;
    }
    return leafcount->GetMaximum();
}
std::string ReadTTree::get_leafcount(TLeaf* leaf)
{
    if ( !leaf ) return "";
    auto leafcount = leaf->GetLeafCount();
    if ( !leafcount ) return "";
    return leafcount->GetName();
}

int ReadTTree::read_vint_branch(const std::string& key)
{
    if( !input_tree->FindBranch(key.c_str()) ) return anl::ANL_NG;
    
    auto status = bnk::define<int>(key);
    if ( status != anl::ANL_OK ) return anl::ANL_NG;
    vptr_list_int[ key ] = bnk::getvecptr<int>(key);
    bnk::setkeytosize<int>( key, "std::vector" );
    
    this->input_tree->Draw( 
        (TString)"log10("+key+"@.size())>>h_of_order_of_velems"
        );
    auto maxx = this->get_maximum_bin_center(
        this->h_of_order_of_velems
        );
    auto nsize = std::pow(10,int(maxx))*1.5;
    if ( 1<maxx ) bnk::resize<int>( key, nsize );
    print_branch_info(key, "std::vector<int>", nsize );

    return anl::ANL_OK;
}
int ReadTTree::read_vfloat_branch(const std::string& key)
{
    if( !input_tree->FindBranch(key.c_str()) ) return anl::ANL_NG;
    
    auto status = bnk::define<float>(key);
    if ( status != anl::ANL_OK ) return anl::ANL_NG;
    vptr_list_float[ key ] = bnk::getvecptr<float>(key);
    bnk::setkeytosize<float>( key, "std::vector" );

    this->input_tree->Draw( 
        (TString)"log10("+key+"@.size())>>h_of_order_of_velems"
        );
    auto maxx = this->get_maximum_bin_center(
        this->h_of_order_of_velems
        );
    auto nsize = std::pow(10,int(maxx))*1.5;
    if ( 1<maxx ) bnk::resize<float>( key, nsize );
    print_branch_info(key, "std::vector<float>", nsize );

    return anl::ANL_OK;
}

int ReadTTree::read_vdouble_branch(const std::string& key)
{
    if( !input_tree->FindBranch(key.c_str()) ) return anl::ANL_NG;
    
    auto status = bnk::define<double>(key);
    if ( status != anl::ANL_OK ) return anl::ANL_NG;
    vptr_list_double[ key ] = bnk::getvecptr<double>(key);
    bnk::setkeytosize<double>( key, "std::vector" );

    this->input_tree->Draw( 
        (TString)"log10("+key+"@.size())>>h_of_order_of_velems"
        );
    auto maxx = this->get_maximum_bin_center(
        this->h_of_order_of_velems
        );
    auto nsize = std::pow(10,int(maxx))*1.5;
    if ( 1<nsize ) bnk::resize<double>( key, nsize );
    print_branch_info(key, "std::vector<double>", nsize );

    return anl::ANL_OK;
}

void ReadTTree::print_branch_info
(const std::string& key, const std::string& type, int size)
{
    cout << setw(25) << type << " " << setw(25) << key << " ";
    cout << setw(10) << size << endl;
}
