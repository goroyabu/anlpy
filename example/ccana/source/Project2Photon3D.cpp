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

    define_parameter<std::string>("output_file1", "output1.root");
    define_parameter<std::string>("output_file2", "output2.root");
    define_parameter<std::string>("output_tree1", "resptree1");
    define_parameter<std::string>("output_tree2", "resptree2");
    
    define_parameter<double>("e_rough_window_begin", 100.0);
    define_parameter<double>("e_rough_window_end", 300.0);
    define_parameter<double>("cone_thick_deg", 0.5);
    define_parameter<double>("distance_index", 2.0);
    define_parameter<double>("e_threshold_si", 5.0);
    define_parameter<double>("e_threshold_cdte", 5.0);
    define_parameter<int>("nbins_1axis", 100);
    define_parameter<double>("axis_minimum", -25.0);
    define_parameter<double>("axis_maximum", 25.0);
    define_parameter<double>("theta_max_degree", 150.0);
    define_parameter<double>("detector_z_position", -41.0);

    define_parameter<double>("e1_peak_energy", 171.3);
    define_parameter<double>("e2_peak_energy", 245.4);
    define_parameter<double>("e1_window_begin", 160);
    define_parameter<double>("e1_window_end",   180);
    define_parameter<double>("e2_window_begin", 235);
    define_parameter<double>("e2_window_end",   255);
    define_parameter<int>("time_window", 10);

    define_parameter<int>("use_si_energy_only", 0);
    define_parameter<int>("event_list_only", 0);
    
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
    auto output_file1 = get_parameter<std::string>("output_file1");
    auto output_file2 = get_parameter<std::string>("output_file2");
    auto output_tree1 = get_parameter<std::string>("output_tree1");
    auto output_tree2 = get_parameter<std::string>("output_tree2");

    projectors[1]->set_parameter<std::string>("input_file", input_file1);
    projectors[1]->set_parameter<std::string>("input_tree", input_tree1);
    projectors[1]->set_parameter<std::string>("output_file", output_file1);
    projectors[1]->set_parameter<std::string>("output_tree", output_tree1);
    projectors[2]->set_parameter<std::string>("input_file", input_file2);
    projectors[2]->set_parameter<std::string>("input_tree", input_tree2);
    projectors[2]->set_parameter<std::string>("output_file", output_file2);
    projectors[2]->set_parameter<std::string>("output_tree", output_tree2);
    
    auto pname_int = { "nbins_1axis", "event_list_only" };
    auto pname_double = { "axis_minimum", "axis_maximum",
			  "cone_thick_deg",
			  "distance_index", "e_threshold_si", "e_threshold_cdte",
			  "theta_max_degree", "detector_z_position" };

    cout << "  ----- Parameters of ProjectCone3D modules in Project2Photon3D ----- ";
    cout << endl;

    projectors[1]->set_parameter("rotation_around_vertical_deg", 0.0);
    projectors[2]->set_parameter("rotation_around_vertical_deg", 180.0);
    
    for ( auto cc : projectors ) {

	for ( auto p : pname_int ) {
	    cc.second->set_parameter<int>( p, get_parameter<int>(p) );
	}
	for ( auto p : pname_double ) {
	    cc.second->set_parameter<double>( p, get_parameter<double>(p) );
	}

	cc.second->set_parameter<double>( "e_window_begin", get_parameter<double>("e_rough_window_begin") );
	cc.second->set_parameter<double>( "e_window_end", get_parameter<double>("e_rough_window_end") );

	cc.second->show_parameters();
    }        
    
    for ( auto cc : projectors )
	if ( cc.second->mod_bgnrun() != anl::ANL_OK ) return anl::ANL_NG; 


    outfile = open_file( output_file1 );
    if ( !outfile ) return anl::ANL_NG;
    h2_energy_coin = new TH2D( "h2_energy_coin", "Energy Si+CdTe;CC1(keV);CC2(keV)",
			       1000, -0.5, 999.5, 1000, -0.5, 999.5 );

    h1_energy_cc1 = new TH1D( "h1_energy_cc1", "Energy Si+CdTe on CC1;keV",
			      1000, -0.5, 999.5);
    
    h1_energy_cc2 = new TH1D( "h1_energy_cc2", "Energy Si+CdTe on CC2;keV",
			      1000, -0.5, 999.5);

    h2_energy_cc1 = new TH2D( "h2_energy_cc1", "Energy on CC1;CdTe(keV);Si(keV)",
			      1000, -0.5, 999.5, 1000, -0.5, 999.5 ); 

    h2_energy_cc2 = new TH2D( "h2_energy_cc2", "Energy on CC2;CdTe(keV);Si(keV)",
			      1000, -0.5, 999.5, 1000, -0.5, 999.5 ); 

    e1_peak_energy = get_parameter<double>("e1_peak_energy");
    e2_peak_energy = get_parameter<double>("e2_peak_energy");
    e1_window_begin = get_parameter<double>("e1_window_begin");
    e1_window_end   = get_parameter<double>("e1_window_end");
    e2_window_begin = get_parameter<double>("e2_window_begin");
    e2_window_end   = get_parameter<double>("e2_window_end");
    time_window     = get_parameter<int>("time_window");

    use_si_energy_only = get_parameter<int>("use_si_energy_only");
    is_event_list_only = get_parameter<int>("event_list_only");
    
    evs::define("Coincidece events in time window");
    evs::define("SC-2hits and SC-2hits coincidence");
    evs::define("Coincidece events in theta range");
    evs::define("CC1 in E-range1 and CC2 in E-range2");
    evs::define("CC1 in E-range2 and CC2 in E-range1");

    evs::define("E-range but Cone1 not projected");
    evs::define("E-range but Cone2 not projected");
    evs::define("E-range but Both Cone not projected");
    
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
    
    if ( time_window<fabs(delta_t1) ) return anl::ANL_SKIP;
    evs::set("Coincidece events in time window");
    
    if ( si1.Energy()<=0.0 || cdte1.Energy()<=0.0 ) return anl::ANL_SKIP;
    if ( si2.Energy()<=0.0 || cdte2.Energy()<=0.0 ) return anl::ANL_SKIP;
    
    evs::set("SC-2hits and SC-2hits coincidence");
    
    auto energy1 = si1.Energy() + cdte1.Energy();
    auto energy2 = si2.Energy() + cdte2.Energy();

    h2_energy_coin->Fill( energy1, energy2 );
    h1_energy_cc1->Fill( energy1 );
    h1_energy_cc2->Fill( energy2 );
    
    h2_energy_cc1->Fill( si1.Energy(), cdte1.Energy() );
    h2_energy_cc2->Fill( si2.Energy(), cdte2.Energy() );

    if ( !projectors[1]->is_in_theta_range( si1, cdte1 ) ) 
	return anl::ANL_SKIP;
    if ( !projectors[2]->is_in_theta_range( si2, cdte2 ) ) 
	return anl::ANL_SKIP;        
    evs::set("Coincidece events in theta range");
    
    if ( is_in_energy_range1(energy1) && is_in_energy_range2(energy2) ) {
	evs::set("CC1 in E-range1 and CC2 in E-range2");
	if ( use_si_energy_only && !is_event_list_only ) {
	    cdte1.e = e1_peak_energy - si1.e;
	    cdte2.e = e2_peak_energy - si2.e;
	}
    }
    else if ( is_in_energy_range2(energy1) && is_in_energy_range1(energy2) ) {
	evs::set("CC1 in E-range2 and CC2 in E-range1");
	if ( use_si_energy_only && !is_event_list_only ) {
	    cdte1.e = e2_peak_energy - si1.e;
	    cdte2.e = e1_peak_energy - si2.e;
	}
    }
    else return anl::ANL_SKIP;
    
    // auto [ si2_new, cdte2_new ] = convert_coordinate( 2, si2, cdte2 );
    
    if ( !is_event_list_only ) {	
	auto is_filled_cc1 = projectors[1]->projection( si1, cdte1 );
	auto is_filled_cc2 = projectors[2]->projection( si2, cdte2 );
	
	// if ( !is_filled_cc1 || !is_filled_cc2 ) return anl::ANL_OK;
	if ( is_filled_cc1==false && is_filled_cc2 ) {
	    evs::set("E-range but Cone1 not projected");
	    return anl::ANL_SKIP;
	}
	else if ( is_filled_cc1 && is_filled_cc2==false ) {
	    evs::set("E-range but Cone2 not projected");
	    return anl::ANL_SKIP;
	}
	else if ( is_filled_cc1==false && is_filled_cc2==false ) {
	    evs::set("E-range but Both Cone not projected");
	    return anl::ANL_SKIP;
	}	

    }

    projectors[1]->fill();
    projectors[2]->fill();	    
    
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
    h2_energy_cc1->Write();
    h2_energy_cc2->Write();
    
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

std::tuple<ProjectCone3D::hit, ProjectCone3D::hit>
Project2Photon3D::convert_coordinate
(int camera_id, const ProjectCone3D::hit& si, const ProjectCone3D::hit& cdte)
{    
    if ( camera_id != 1 && camera_id != 2 )
	return std::make_tuple( si, cdte ); 
    
    auto si_new = si;
    auto cdte_new = cdte;

    // if ( camera_id == 2 ) {
    // 	si_new.x = -1.0 * si.x;
    // 	si_new.y = si.y;
    // 	si_new.z = -1.0 * si.z;
    // 	cdte_new.x = -1.0 * cdte.x;
    // 	cdte_new.y = cdte.y;
    // 	cdte_new.z = -1.0 * cdte.z;
    // }

    return std::make_tuple( si_new, cdte_new );    
}
