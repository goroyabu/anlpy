/**
   @file   Project2Photon3D.cpp
   @date   2020/05/18
   @author 
   @detail Automatically generated by make_anlpy_project.sh 1.0.0
**/

#include "Project2Photon3D.hpp"

#include <iostream>
using std::cout;
using std::endl;

#include <bnk.hpp>
#include <evs.hpp>

#include <TROOT.h>
#include <TSeqCollection.h>
#include <TFile.h>
#include <TH2D.h>

Project2Photon3D::Project2Photon3D()
    : anl::VANL_Module("Project2Photon3D", "1.0"),
      outfile(nullptr), h2_energy_coin(nullptr)
{
    /** Parameters can be modified via a method 'SetParameter' in Python **/
    // define_parameter<std::string>("input_file", "input.txt");
    define_parameter<std::string>("input_file1", "input1.root");
    define_parameter<std::string>("input_tree1", "hittree1");
    define_parameter<std::string>("input_file2", "input2.root");
    define_parameter<std::string>("input_tree2", "hittree2");

    define_parameter<std::string>("output_file", "output.root");
    define_parameter<std::string>("output_tree1", "resptree1");
    define_parameter<std::string>("output_tree2", "resptree2");
    
    define_parameter<double>("e_window_begin", 100.0);
    define_parameter<double>("e_window_end", 300.0);
    define_parameter<double>("cone_thick_deg", 0.5);
    define_parameter<double>("distance_index", 2.0);
    define_parameter<double>("e_threshold_si", 5.0);
    define_parameter<double>("e_threshold_cdte", 5.0);
    define_parameter<int>("nbins_1axis", 100);
    define_parameter<double>("axis_minimum", -25.0);
    define_parameter<double>("axis_maximum", 25.0);
    define_parameter<double>("theta_max_degree", 150.0);
    define_parameter<double>("detector_z_position", -41.0);

    define_parameter<double>("e1_window_begin", 160);
    define_parameter<double>("e1_window_end",   180);
    define_parameter<double>("e2_window_begin", 235);
    define_parameter<double>("e2_window_end",   255);
    define_parameter<int>("time_window", 10);
    
    projectors[1] = new ProjectCone3D();
    projectors[1]->set_parameter<std::string>("copyid", "cc1");
    projectors[2] = new ProjectCone3D();
    projectors[2]->set_parameter<std::string>("copyid", "cc2");

    
}
Project2Photon3D::~Project2Photon3D()
{
}

int Project2Photon3D::mod_bgnrun()
{
    auto input_file1 = get_parameter<std::string>("input_file1");
    auto input_tree1 = get_parameter<std::string>("input_tree1");    
    auto input_file2 = get_parameter<std::string>("input_file2");
    auto input_tree2 = get_parameter<std::string>("input_tree2");
    auto output_file = get_parameter<std::string>("output_file");
    auto output_tree1 = get_parameter<std::string>("output_tree1");
    auto output_tree2 = get_parameter<std::string>("output_tree2");

    projectors[1]->set_parameter<std::string>("input_file", input_file1);
    projectors[1]->set_parameter<std::string>("input_tree", input_tree1);
    projectors[1]->set_parameter<std::string>("output_file", output_file);
    projectors[1]->set_parameter<std::string>("output_tree", output_tree1);
    projectors[2]->set_parameter<std::string>("input_file", input_file2);
    projectors[2]->set_parameter<std::string>("input_tree", input_tree2);
    projectors[2]->set_parameter<std::string>("output_file", output_file);
    projectors[2]->set_parameter<std::string>("output_tree", output_tree2);
    
    auto pname_int = { "nbins_1axis" };
    auto pname_double = { "axis_minimum", "axis_maximum",
			  "e_window_begin", "e_window_end", "cone_thick_deg",
			  "distance_index", "e_threshold_si", "e_threshold_cdte",
			  "theta_max_degree", "detector_z_position" };

    for ( auto cc : projectors ) {
	for ( auto p : pname_int ) {
	    cc.second->set_parameter<int>( p, get_parameter<int>(p) );
	}
	for ( auto p : pname_double ) {
	    cc.second->set_parameter<double>( p, get_parameter<double>(p) );
	}
    }

    for ( auto cc : projectors )
	if ( cc.second->mod_bgnrun() != anl::ANL_OK ) return anl::ANL_NG; 


    outfile = open_file( output_file );
    if ( !outfile ) return anl::ANL_NG;
    h2_energy_coin = new TH2D( "h2_energy_coin", "Energy Si+CdTe;CC1(keV);CC2(keV)",
			       1000, -0.5, 999.5, 1000, -0.5, 999.5 );

    h1_energy_cc1 = new TH1D( "h1_energy_cc1", "Energy Si+CdTe on CC1;keV",
			      1000, -0.5, 999.5);
    
    h1_energy_cc2 = new TH1D( "h1_energy_cc2", "Energy Si+CdTe on CC2;keV",
			      1000, -0.5, 999.5);
    
    e1_window_begin = get_parameter<double>("e1_window_begin");
    e1_window_end   = get_parameter<double>("e1_window_end");
    e2_window_begin = get_parameter<double>("e2_window_begin");
    e2_window_end   = get_parameter<double>("e2_window_end");
    time_window     = get_parameter<int>("time_window");

    evs::define("Coincidece events in time window");
    evs::define("SC-2hits and SC-2hits coincidence");
    evs::define("CC1 in E-range1 and CC2 in E-range2");
    evs::define("CC1 in E-range2 and CC2 in E-range1");
    
    return anl::ANL_OK;
}

int Project2Photon3D::mod_ana()
{
    for ( auto [ id, cc ] : projectors )
	if ( !cc->next() ) return anl::ANL_LOOP;

    auto [ si1, cdte1 ] = projectors[1]->get_sc2hit_event();
    auto [ eventid1, delta_t1 ] = projectors[1]->coin_info();
    auto [ si2, cdte2 ] = projectors[2]->get_sc2hit_event();
    auto [ eventid2, delta_t2 ] = projectors[2]->coin_info();    
    
    if ( eventid1!=eventid2 ) {
	cout << "***Error*** : coin_eventid in 2 trees are diffrent." << endl;
	return anl::ANL_LOOP;
    }
    if ( delta_t1!=delta_t2 ) {
	cout << "***Error*** : coin_delta_t in 2 trees are diffrent." << endl;
	return anl::ANL_LOOP;
    }
    
    if ( time_window<delta_t1 ) return anl::ANL_SKIP;
    evs::set("Coincidece events in time window");
    
    if ( si1.Energy()<=0.0 || cdte1.Energy()<=0.0 ) return anl::ANL_SKIP;
    if ( si2.Energy()<=0.0 || cdte2.Energy()<=0.0 ) return anl::ANL_SKIP;
    
    evs::set("SC-2hits and SC-2hits coincidence");
    
    auto energy1 = si1.Energy() + cdte1.Energy();
    auto energy2 = si2.Energy() + cdte2.Energy();
    h2_energy_coin->Fill( energy1, energy2 );
    h1_energy_cc1->Fill( energy1 );
    h1_energy_cc2->Fill( energy2 );
   
    if ( is_in_energy_range1(energy1) && is_in_energy_range2(energy2) ) {
	evs::set("CC1 in E-range1 and CC2 in E-range2");	
    }
    else if ( is_in_energy_range2(energy1) && is_in_energy_range1(energy2) ) {
	evs::set("CC1 in E-range2 and CC2 in E-range1");
    }
    else return anl::ANL_SKIP;
    
    projectors[1]->projection( si1, cdte1 );
    projectors[2]->projection( si2, cdte2 );    
    
    // // evs::set("Si-CdTe 2hits event");

    // if ( has_flour(si) || has_flour(cdte) )
    //     return anl::ANL_SKIP;

    // // evs::set("SC2hits w/o flour");

    // if ( !is_in_energy_range( si.Energy()+cdte.Energy() ) )
    //     return anl::ANL_SKIP;

    // // evs::set("SC2hits in E-window");

    // if ( !is_in_theta_range( si, cdte ) )
    //     return anl::ANL_SKIP;

    // this->projection( image, si, cdte );
    
    return anl::ANL_OK;
}

int Project2Photon3D::mod_endrun()
{
    /** This function is called at the end of the run **/
    cout << mod_name2() << "::mod_endrun()";
    cout << " -> Good Bye. :)" << endl;

    /** Example of operations described here **/

    /** Closing files **/
    // ifs.close(); // std::ifstream

    for ( auto cc : projectors )
	if ( cc.second->mod_endrun() != anl::ANL_OK ) return anl::ANL_NG; 

    h2_energy_coin->Write();
    h1_energy_cc1->Write();
    h1_energy_cc2->Write();
    
    return anl::ANL_OK;
}

TFile* Project2Photon3D::open_file(std::string file_name)
{
    // auto ofname = get_parameter<std::string>("output_file");
    if ( !gROOT ) return nullptr;
    TSeqCollection * list = gROOT->GetListOfFiles();
    if ( !list ) return nullptr;
    auto opened = list->FindObject( file_name.c_str() );
    // if ( !!opened ) {
    //     cout << opened->GetName() << " " << opened->ClassName() << endl;
    // }
    // else cout << "null" << endl;
    
    if ( !!opened ) {
	if ( !opened->ClassName()==(TString)"TFile" ) {
	    cout << file_name << " is generated as " << opened->ClassName();
	    return nullptr;
	}
	else if ( ((TFile*)opened)->IsWritable() ) {
	    cout << file_name << " is already opened." << endl;
	    ((TFile*)opened)->cd();
	    return (TFile*)opened;		
	}
	else {
	    cout << "***Error*** : " << file_name << " is not writable." << endl;
	    return nullptr;
	}
    }	
    else {
	auto f = new TFile( file_name.c_str(), "recreate" );
	if ( !f || f->IsZombie() ) {
	    cout << "Creating " << file_name << " is failed." << endl;
	    return nullptr;
	}
	cout << file_name << " is created." << endl;
	return f;
    }
    return nullptr;
}
