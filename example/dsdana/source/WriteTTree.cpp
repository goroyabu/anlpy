/**
   @file   WriteTTree.cpp
   @date   2020/04/08
   @author
   @detail Automatically generated by make_anlpy_project.sh 1.0.0
**/

#include "WriteTTree.hpp"

#include <TFile.h>
#include <TLeaf.h>

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <iomanip>
using std::setw;

#include <sstream>

#include <regex>

#include <bnk.hpp>
#include <evs.hpp>

WriteTTree::WriteTTree()
    : anl::VANL_Module("WriteTTree", "20211015")
{
    /** Parameters can be modified via a method 'SetParameter' in Python **/
    // define_parameter<std::string>("input_file", "input.txt");
    define_parameter<std::string>("file_name", "output.root");
    define_parameter<std::string>("tree_name", "outtree");
    define_parameter<std::string>("branch_ignore", "NONE");
}
WriteTTree::~WriteTTree()
{
    if ( this->output_file->IsOpen() ) output_file->Close();
    cout << " - Closed : " << this->get_parameter<std::string>("file_name") << endl;
}

int WriteTTree::mod_bgnrun()
{
    /** This function is called at the begging of the run **/
    // cout << mod_name2() << "::mod_bgnrun()";
    // cout << " -> Hello! :D" << endl;
    cout << "==========================" << endl;
    cout << "  WriteTTree::mod_bgnrun  " << endl;
    cout << "==========================" << endl;

    auto file_name = get_parameter<std::string>( "file_name" );
    output_file = new TFile( file_name.c_str(), "recreate" );
    if ( !output_file || output_file->IsZombie() ) {
        cerr << "***Error*** : Opening " << file_name << " is failed." << endl;
        return anl::ANL_NG;
    }
    cout << " - Opened : " << file_name << endl;

    auto tree_name = get_parameter<std::string>( "tree_name" );
    output_tree = new TTree( tree_name.c_str(), tree_name.c_str() );
    if ( !output_tree ) {
        cerr << "***Error*** : Generating " << tree_name << " is failed." << endl;
        return anl::ANL_NG;
    }
    cout << " - Generated : " << tree_name << endl;

    auto branch_ignore = get_parameter<std::string>("branch_ignore");
    this->switch_branch( branch_ignore, ":", false);

    auto status = this->write_all_branch();
    if ( status != anl::ANL_OK ) return anl::ANL_NG;

    return anl::ANL_OK;
}

int WriteTTree::mod_ana()
{
    /** This function is called at every events **/
    // cout << mod_name2() << "::mod_ana()" << endl;

    // auto tree_entry = bnk::get<long>( "ReadTTree_entry" );
    // cout << "ent=" <<  tree_entry << endl;

    output_tree->Fill();

    return anl::ANL_OK;
}

int WriteTTree::mod_endrun()
{
    /** This function is called at the end of the run **/
    // cout << mod_name2() << "::mod_endrun()";
    // cout << " -> Good Bye. :)" << endl;

    /** Example of operations described here **/

    /** Closing files **/
    // ifs.close(); // std::ifstream

    // cout << "WriteTTree::mod_endrun" << endl;
    // auto outname =  output_file->GetName();
    output_file->cd();
    output_tree->Write();
    // output_file->Close();
    // // cout << "WT end" << endl;
    // cout << " - Closed : " << outname << endl;
    // cout << " - End : WriteTTree" << endl;

    return anl::ANL_OK;
}

int WriteTTree::switch_branch(const std::string& selection, const std::string& delim, bool enable)
{
    if ( (selection=="ALL" || selection=="all") && enable ) return anl::ANL_OK;
    if ( (selection=="NONE" || selection=="none") && !enable ) return anl::ANL_OK;

    std::stringstream ss(selection); std::string word;
    while( std::getline(ss, word, delim.c_str()[0]) ){
	auto reg = convert_regex(word);
	disabled_branch.emplace_back(reg);
    }

    return anl::ANL_OK;
}

std::string WriteTTree::replace_all
(std::string before, const std::string& from, const std::string& to)
{
    std::string::size_type pos = 0;
    while ( (pos = before.find(from, pos)) != std::string::npos ) {
	// pos = before.find(from, pos);
	// if ( pos == std::string::npos ) break;
	before.replace(pos, from.length(), to);
	pos += to.length();
    }
    return before;
}
std::string WriteTTree::convert_regex(std::string wildcard)
{
    auto reg = replace_all(wildcard, "*", ".*");
    reg = replace_all(reg, "?", ".");
    reg = replace_all(reg, "!", "^");
    reg = replace_all(reg, ",", "|");
    reg = replace_all(reg, "{", "(");
    reg = replace_all(reg, "}", ")");
    return "^"+reg+"$";
}

int WriteTTree::write_all_branch()
{

    auto list_of_bnk = bnk::get_list_of_buffers();

    cout << endl;
    cout << "## Branch List of " << output_tree->GetName() << " ##" << endl;

    auto line = std::string(62, '-');
    cout << line << endl;
    cout << setw(25) << "Type" << " " << setw(25) << "Key" << " " << setw(10) << endl;
    cout << line << endl;

    for ( auto buffer : list_of_bnk ) {

	auto key = buffer.first;
	auto type = buffer.second;
	// cout << "-" << key << "----" << type << "-" << endl;

	if ( is_disabled( key ) ) continue;

	// cout << "-" << key << "----" << type << "-" << endl;
	// cout << setw(25) << key << " " << setw(25) << type << endl;

	if ( type == "int" ) {
	    if( bnk::getkeytosize<int>(key) == "std::vector" ) {
		vptr_list_int[ key ] = bnk::getvecptr<int>(key);
		cout << setw(25) << "std::vector<int>" << " ";
		cout << setw(25) << key << endl; continue;
	    }
	    write_branch<int>( key, "I" );
	    cout << setw(25) << type << " " << setw(25) << key << endl;
	}
	else if ( type == "unsigned int" ) {
	    write_branch<unsigned int>( key, "i" );
	    cout << setw(25) << type << " " << setw(25) << key << endl;
	}
	else if ( type == "unsigned short" ) {
	    write_branch<unsigned short>( key, "s" );
	    cout << setw(25) << type << " " << setw(25) << key << endl;
	}
	else if ( type == "short" ) {
	    write_branch<short>( key, "S" );
	    cout << setw(25) << type << " " << setw(25) << key << endl;
	}
	else if ( type == "float" ) {
	    if( bnk::getkeytosize<float>(key) == "std::vector" ) {
		vptr_list_float[ key ] = bnk::getvecptr<float>(key);
		cout << setw(25) << "std::vector<float>" << " ";
		cout << setw(25) << key << endl; continue;
	    }
	    write_branch<float>( key, "F" );
	    cout << setw(25) << type << " " << setw(25) << key << endl;
	}
	else if ( type == "double" ) {
	    if( bnk::getkeytosize<double>(key) == "std::vector" ) {
		vptr_list_double[ key ] = bnk::getvecptr<double>(key);
		cout << setw(25) << "std::vector<double>" << " ";
		cout << setw(25) << key << endl; continue;
	    }
	    write_branch<double>( key, "D" );
	    cout << setw(25) << type << " " << setw(25) << key << endl;
	}
	else if ( type == "long" ) {
	    write_branch<long>( key, "L" );
	    cout << setw(25) << type << " " << setw(25) << key << endl;
	}
	else if ( type == "long long" ) {
	    write_branch<long long>( key, "L");
	    cout << setw(25) << type << " " << setw(25) << key << endl;
	}else cout << setw(25) << type << " " << setw(25) << key << endl;
    }

    for ( auto&& [ key, ptr ] : vptr_list_int )
	output_tree->Branch( key.c_str(), ptr );

    for ( auto&& [ key, ptr ] : vptr_list_float )
	output_tree->Branch( key.c_str(), ptr );

    for ( auto&& [ key, ptr ] : vptr_list_double )
	output_tree->Branch( key.c_str(), ptr );

    cout << line << endl;

    return anl::ANL_OK;
}

bool WriteTTree::is_disabled(const std::string& key)
{
    for ( auto branch : disabled_branch ) {
	std::smatch result; std::regex pattern(branch);
	//cout << branch << " " << key << endl;

	if ( std::regex_search( key, result, pattern ) ) {
	    return true;
	    // cout << "match " <<  branch << " and " << key << endl; return true;
	}
    }
    return false;
}
