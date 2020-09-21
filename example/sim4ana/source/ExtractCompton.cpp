/**
   @file   ExtractCompton.cpp
   @date   2020/09/09
   @author 
   @detail Automatically generated by make_anlpy_project.sh 1.0.0
**/

#include "ExtractCompton.hpp"

#include <iostream>
using std::cout;
using std::endl;

#include <bnk.hpp>
#include <evs.hpp>

ExtractCompton::ExtractCompton()
    : anl::VANL_Module("ExtractCompton", "0.1")
{
    /** Parameters can be modified via a method 'SetParameter' in Python **/
    // define_parameter<std::string>("input_file", "input.txt");

    define_parameter<double>("si_threshold", 10.0);
    define_parameter<double>("cdte_threshold", 10.0);
    define_parameter<double>("si_energy_max", 120.0);
    define_parameter<double>("cdte_energy_max", 800.0);

    define_parameter<std::string>("incident_energy_list", "511.0,31.0");

    define_parameter<double>("source_origin_x", 0);
    define_parameter<double>("source_origin_y", 0);
    define_parameter<double>("source_origin_z", 41.35);
}
ExtractCompton::~ExtractCompton()
{
}

int ExtractCompton::mod_bgnrun()
{
    bnk::define<int>( "detid_lv3_mod", 20 );
    bnk::define<int>( "strip_x_lv3", 20 );
    bnk::define<int>( "strip_y_lv3", 20 );
    bnk::define<int>( "hitid_over_ethre", 20 );   
    bnk::define<float>( "epi_lv3_mod", 20 );

    bnk::define<double>( "epi_si" );
    bnk::define<double>( "epi_x_si" );
    bnk::define<double>( "epi_y_si" );
    bnk::define<double>( "pos_x_si" );
    bnk::define<double>( "pos_y_si" );
    bnk::define<double>( "pos_z_si" );
    bnk::define<double>( "epi_cdte" );
    bnk::define<double>( "epi_x_cdte" );
    bnk::define<double>( "epi_y_cdte" );
    bnk::define<double>( "pos_x_cdte" );
    bnk::define<double>( "pos_y_cdte" );
    bnk::define<double>( "pos_z_cdte" );
    bnk::define<int>( "flag_fluor" );
    
    bnk::define<double>( "epi_total_compton" );
    bnk::define<double>( "theta_kinetic"     );
    bnk::define<double>( "theta_geometric"   );
    bnk::define<double>( "angular_resolution_measure" );
    
    evs::define( "Below_2Hits_Before_Cut"   );
    evs::define( "Not_Include_Fluor_Hits"   );
    evs::define( "Exist_Si_Hit_After_Cut"   );
    evs::define( "Exist_CdTe_Hit_After_Cut" );
    evs::define( "Si_CdTe_2Hits_After_Cut"  );
    evs::define( "Si_CdTe_Coin_After_Cut"   );
    evs::define( "Ene_Consistent_Abso_Hits" );
    evs::define( "Si_CdTe_Compton_Event"    );    
    
    evs::define( "Saturated_Hits_on_Si"     );
    evs::define( "Large_Deposit_on_Si1"     );
    evs::define( "Fluor_Hits_on_Si"         );

    evs::define( "Saturated_Hits_on_CdTe"    );    
    
    evs::define( "Hit_on_Si1_Before_Cut" );
    evs::define( "Hit_on_Si2_Before_Cut" );
    evs::define( "Hit_on_CdTe1_Before_Cut" );
    evs::define( "Hit_on_CdTe2_Before_Cut" );
    evs::define( "Hit_on_CdTe3_Before_Cut" );

    evs::define( "Hit_on_Si1_After_Cut" );
    evs::define( "Hit_on_Si2_After_Cut" );
    evs::define( "Hit_on_CdTe1_After_Cut" );
    evs::define( "Hit_on_CdTe2_After_Cut" );
    evs::define( "Hit_on_CdTe3_After_Cut" );    
    
    /** Example of operations described here **/

    /** Opening files **/
    // std::string input_file = get_parameter<std::string>( "input.txt" );
    // ifs = std::ifstream( input_file );

    /** Definition of data buffers (BNK) **/
    // bnk::define<std::string>( "time_stamp" );
    // bnk::define<double>( "energy_deposit" );

    /** Definition of event flags (EVS) **/
    // evs::define( "multiple_signals_event" );

    parameter.si_threshold = get_parameter<double>("si_threshold");
    parameter.cdte_threshold = get_parameter<double>("cdte_threshold");
    parameter.si_energy_max = get_parameter<double>("si_energy_max");
    parameter.cdte_energy_max = get_parameter<double>("cdte_energy_max");

    auto elist
	= split_to_double( get_parameter<std::string>("incident_energy_list") );
    std::sort( elist.begin(), elist.end() );
    parameter.incident_energy_list = elist;

    auto orig_x = get_parameter<double>("source_origin_x");
    auto orig_y = get_parameter<double>("source_origin_y");
    auto orig_z = get_parameter<double>("source_origin_z");
    parameter.source_origin = TVector3( orig_x, orig_y, orig_z ); 
    
    return anl::ANL_OK;
}

int ExtractCompton::mod_ana()
{
    // static const float si_threshold    = 10.0;
    // static const float cdte_threshold  = 10.0;
    // static const float si_energy_max   = 120.0;//200.0;

    static const float si_threshold    = parameter.si_threshold;
    static const float cdte_threshold  = parameter.cdte_threshold;
    static const float si_energy_max   = parameter.si_energy_max;
    static const float cdte_energy_max = parameter.cdte_energy_max;
    
    auto nhits  = bnk::get<int>    ( "nhit_lv3"  );
    auto detid  = bnk::getv<int>   ( "detid_lv3" );
    // auto epi    = bnk::getv<double>( "epi_lv3"   );
    auto epi_x  = bnk::getv<float>( "epi_x_lv3" );
    auto epi_y  = bnk::getv<float>( "epi_y_lv3" );
    auto pos_x  = bnk::getv<float>( "pos_x_lv3" );
    auto pos_y  = bnk::getv<float>( "pos_y_lv3" );
    auto pos_z  = bnk::getv<float>( "pos_z_lv3" );
    // auto init_x = bnk::get<double> ( "init_x"    );
    // auto init_y = bnk::get<double> ( "init_y"    );
    // auto init_z = bnk::get<double> ( "init_z"    );
    
    std::vector<int> detid_lv3_mod;
    std::vector<int> strip_x_lv3;
    std::vector<int> strip_y_lv3;
    std::vector<hit*> hit_list;
    std::vector<float> epi_lv3_mod;
    
    for ( int i=0; i<nhits; ++i ) {
	
    	auto h = new hit();
	
    	if ( detid[i]<10 ) h->detid = detid[i];
    	else               h->detid = detid[i] - 8;
    	// Convert old ID. 10->2, 11->3, 12->4

	if      ( h->detid==0 ) evs::set("Hit_on_Si1_Before_Cut");
	else if ( h->detid==1 ) evs::set("Hit_on_Si2_Before_Cut");
	else if ( h->detid==2 ) evs::set("Hit_on_CdTe1_Before_Cut");
	else if ( h->detid==3 ) evs::set("Hit_on_CdTe2_Before_Cut");
	else if ( h->detid==4 ) evs::set("Hit_on_CdTe3_Before_Cut");	

    	h->pos_x = pos_x[i];
    	h->pos_y = pos_y[i];
    	h->pos_z = pos_z[i];

    	if ( IsSi(h->detid) ) h->epi = epi_x[i];
    	else                  h->epi = epi_y[i];
    	// Representative side is p-side for Si & Al-side for CdTe.
    	h->epi_x = epi_x[i];
    	h->epi_y = epi_y[i];

    	h->strip_x = ExtractCompton::GetStripIDX( h->pos_x );
    	h->strip_y = ExtractCompton::GetStripIDY( h->pos_y );

    	// h->init_x = init_x;
    	// h->init_y = init_y;
    	// h->init_z = init_z;
	h->init_x = 0.0;
    	h->init_y = 0.0;
    	h->init_z = 0.0;

    	detid_lv3_mod.emplace_back( h->detid );	
	strip_x_lv3.emplace_back  ( h->strip_x );
    	strip_y_lv3.emplace_back( h->strip_y );
    	hit_list.emplace_back(h);
	epi_lv3_mod.emplace_back( h->epi );
    }
    
    bnk::put<int>( "detid_lv3_mod", detid_lv3_mod, 0, nhits );
    bnk::put<int>( "strip_x_lv3", strip_x_lv3, 0, nhits );
    bnk::put<int>( "strip_y_lv3", strip_y_lv3, 0, nhits );
    bnk::put<float>( "epi_lv3_mod", epi_lv3_mod, 0, nhits );
    
    if ( nhits<2 ) evs::set( "Below_2Hits_Before_Cut" );
    
    std::vector<int> hitid_over_ethre;
    auto nhits_si = 0;
    auto nhits_cdte = 0;
    
    for ( int ihit=0; ihit<nhits; ++ihit ) {
    	auto h = hit_list[ihit];

    	if ( IsSi(h->detid) ){

    	    if ( h->detid==0 && si_energy_max<=h->epi )
    		evs::set( "Large_Deposit_on_Si1" );
	    if ( is_fluor_of_cdte( h->epi ) )
		evs::set( "Fluor_Hits_on_Si" );
	    
    	    if ( h->epi<si_threshold ) continue;
	    if ( si_energy_max<=h->epi ) {
		evs::set( "Saturated_Hits_on_Si" ); 
		continue;
	    }
	    
	    hitid_over_ethre.emplace_back( ihit );
	    ++nhits_si;
	    
	    if      ( h->detid==0 ) evs::set("Hit_on_Si1_After_Cut");
	    else if ( h->detid==1 ) evs::set("Hit_on_Si2_After_Cut");
    	}
    	else if ( IsCdTe(h->detid) ) {
    	    if ( h->epi<cdte_threshold ) continue;
	    if ( cdte_energy_max<=h->epi ) {
		evs::set( "Saturated_Hits_on_CdTe" ); 
		continue;
	    }
	    
	    hitid_over_ethre.emplace_back( ihit );
    	    ++nhits_cdte;
	    
	    if      ( h->detid==2 ) evs::set("Hit_on_CdTe1_After_Cut");
	    else if ( h->detid==3 ) evs::set("Hit_on_CdTe2_After_Cut");
	    else if ( h->detid==4 ) evs::set("Hit_on_CdTe3_After_Cut");	    
    	}
    }

    bnk::put<int>( "hitid_over_ethre", hitid_over_ethre,
		   0, hitid_over_ethre.size() );

    if ( evs::get( "Fluor_Hits_on_Si" ) )
	bnk::put<int>( "flag_fluor", 1 );
    else {
	bnk::put<int>( "flag_fluor", 0 );
	evs::set( "Not_Include_Fluor_Hits" );
    }

    if ( nhits_si>0 )   evs::set( "Exist_Si_Hit_After_Cut"   );
    if ( nhits_cdte>0 ) evs::set( "Exist_CdTe_Hit_After_Cut" );

    if ( nhits_si>0 && nhits_cdte>0 )
	evs::set( "Si_CdTe_Coin_After_Cut" );
    
    if ( nhits_si!=1 || nhits_cdte!=1 ) {
	bnk::put<double>( "epi_si", 0.0);
	bnk::put<double>( "epi_x_si", 0.0);
	bnk::put<double>( "epi_y_si", 0.0);
	bnk::put<double>( "pos_x_si", 0.0 );
	bnk::put<double>( "pos_y_si", 0.0 );
	bnk::put<double>( "pos_z_si", 0.0 );
	bnk::put<double>( "epi_cdte", 0.0 );
	bnk::put<double>( "epi_x_cdte", 0.0 );
	bnk::put<double>( "epi_y_cdte", 0.0 );
	bnk::put<double>( "pos_x_cdte", 0.0 );
	bnk::put<double>( "pos_y_cdte", 0.0 );
	bnk::put<double>( "pos_z_cdte", 0.0 );
	bnk::put<double>( "epi_total_compton", 0.0 );
	bnk::put<double>( "theta_kinetic", -1 );
	bnk::put<double>( "theta_geometric", -1 );
	bnk::put<double>( "angular_resolution_measure", 0.0 );
	return anl::ANL_OK;
    }

    evs::set( "Si_CdTe_2Hits_After_Cut" );   	

    auto hit0 = *(hit_list[ hitid_over_ethre[0] ]);
    auto hit1 = *(hit_list[ hitid_over_ethre[1] ]);
    
    hit si_hit; hit cdte_hit;
    
    if ( hit0.detid<hit1.detid ) {
	si_hit = hit0;
	cdte_hit = hit1;
    }
    else {
	si_hit = hit1;
	cdte_hit = hit0;
    }

    bnk::put<double>( "epi_si", si_hit.epi );
    bnk::put<double>( "epi_x_si", si_hit.epi_x );
    bnk::put<double>( "epi_y_si", si_hit.epi_y );
    bnk::put<double>( "pos_x_si", si_hit.pos_x );
    bnk::put<double>( "pos_y_si", si_hit.pos_y );
    bnk::put<double>( "pos_z_si", si_hit.pos_z );
    bnk::put<double>( "epi_cdte", cdte_hit.epi );
    bnk::put<double>( "epi_x_cdte", cdte_hit.epi_x );
    bnk::put<double>( "epi_y_cdte", cdte_hit.epi_y );
    bnk::put<double>( "pos_x_cdte", cdte_hit.pos_x );
    bnk::put<double>( "pos_y_cdte", cdte_hit.pos_y );
    bnk::put<double>( "pos_z_cdte", cdte_hit.pos_z );

    auto delta_e_cdte = cdte_hit.epi_y - cdte_hit.epi_x;
    auto epi_cdte_ave = ( cdte_hit.epi_y + cdte_hit.epi_x )*0.5;

    static const double delta_e_maximum = 5.0;
    if ( std::fabs(delta_e_cdte)>delta_e_maximum ) return anl::ANL_OK;    
    else evs::set( "Ene_Consistent_Abso_Hits" );	
    
    auto epi_total_compton = si_hit.epi + epi_cdte_ave;
    bnk::put<double>( "epi_total_compton", epi_total_compton );
    
    auto theta_kinetic = compton_angle( si_hit.epi, epi_cdte_ave );
    if ( theta_kinetic>0 ) evs::set( "Si_CdTe_Compton_Event" );
    bnk::put<double>( "theta_kinetic", theta_kinetic );
    
    //static TVector3 orig( 0, 0, 41.35 );
    TVector3 orig = parameter.source_origin;
    TVector3 scat( si_hit.pos_x, si_hit.pos_y, si_hit.pos_z );
    TVector3 abso( cdte_hit.pos_x, cdte_hit.pos_y, cdte_hit.pos_z );
    auto theta_geometric = angle_of_3points( orig, scat, abso );
    bnk::put<double>( "theta_geometric", theta_geometric );
    
    auto arm = theta_geometric - theta_kinetic;
    bnk::put<double>( "angular_resolution_measure", arm );
    
    return anl::ANL_OK;
}

int ExtractCompton::mod_endrun()
{
    /** This function is called at the end of the run **/
    cout << mod_name2() << "::mod_endrun()";
    cout << " -> Good Bye. :)" << endl;

    /** Example of operations described here **/

    /** Closing files **/
    // ifs.close(); // std::ifstream

    return anl::ANL_OK;
}

