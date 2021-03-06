/**
   @file   ApplyEnergyResponse.cpp
   @date   2020/08/24
   @author 
   @detail Automatically generated by make_anlpy_project.sh 1.0.0
**/

#include "ApplyEnergyResponse.hpp"

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <complex>

#include <TRandom3.h>

#include <bnk.hpp>
#include <evs.hpp>

namespace bnk
{
    template<typename T> int define
    (const std::string& key, const int size, const std::string& sizeref)
    {
        auto result = define<T>( key, size );
        if ( result!=0 ) return result;
        return setkeytosize<T>( key, sizeref );
    }
}

ApplyEnergyResponse::ApplyEnergyResponse()
    : anl::VANL_Module("ApplyEnergyResponse", "0.1")
{
    /** Parameters can be modified via a method 'SetParameter' in Python **/
    // define_parameter<std::string>("input_file", "input.txt");

    parameter.is_enabled_randomize = true;
    define_parameter<int>("use_randomize_energy", parameter.is_enabled_randomize);

    parameter.electronics_noise = 1.6;
    define_parameter<double>("electronics_noise", parameter.electronics_noise);
    
    // gRandom->SetSeed( time(NULL) );
    define_parameter<int>("random_seed",0);
}
ApplyEnergyResponse::~ApplyEnergyResponse()
{
}

int ApplyEnergyResponse::mod_bgnrun()
{
    // if ( bnk::is_def("nhits_dif")==false ) {
    // 	cerr << "***Error*** in ApplyEnergyResponse::mod_bgnrun : ";
    // 	// cerr << "This module needs the following BNK 'nhits_raw', 'detid_raw', 'strip_x_raw', 'strip_y_raw', 'edep_raw', 'pos_x_raw', 'pos_y_raw', 'pos_z_raw', 'dir_x_raw', 'dir_y_raw' and 'dir_z_raw'." << endl;
    //     return anl::ANL_NG;
    // }
    
    constexpr static int npixels = 100;
    bnk::define<int>( "nhits_x_res" );
    bnk::define<int>( "detid_x_res", npixels, "nhits_x_res" );
    bnk::define<int>( "material_x_res", npixels, "nhits_x_res" );
    bnk::define<int>( "flag_x_res", npixels, "nhits_x_res" );
    bnk::define<int>( "strip_x_res", npixels, "nhits_x_res" );
    bnk::define<double>( "edep_x_res", npixels, "nhits_x_res" );
    bnk::define<double>( "pos_x_res", npixels, "nhits_x_res" );
    
    bnk::define<int>( "nhits_y_res" );
    bnk::define<int>( "detid_y_res", npixels, "nhits_y_res" );
    bnk::define<int>( "material_y_res", npixels, "nhits_y_res" );
    bnk::define<int>( "flag_y_res", npixels, "nhits_y_res" );
    bnk::define<int>( "strip_y_res", npixels, "nhits_y_res" );
    bnk::define<double>( "edep_y_res", npixels, "nhits_y_res" );
    bnk::define<double>( "pos_y_res", npixels, "nhits_y_res" );
    // bnk::define<double>( "pos_z_res", npixels, "nhits_res" );

    parameter.electronics_noise = get_parameter<double>("electronics_noise");
    parameter.is_enabled_randomize = get_parameter<int>("use_randomize_energy");

    gRandom->SetSeed( get_parameter<int>("random_seed") );
    return anl::ANL_OK;
}

int ApplyEnergyResponse::mod_ana()
{
    auto nhits_x_in = bnk::get<int>("nhits_x_trg");
    auto nhits_y_in = bnk::get<int>("nhits_y_trg");
    auto detid_x_in = bnk::getv<int>("detid_x_trg");
    auto detid_y_in = bnk::getv<int>("detid_y_trg");
    auto material_x_in = bnk::getv<int>("material_x_trg");
    auto material_y_in = bnk::getv<int>("material_y_trg");
    auto strip_x_in = bnk::getv<int>("strip_x_trg");
    auto strip_y_in = bnk::getv<int>("strip_y_trg");
    auto edep_x_in = bnk::getv<double>("edep_x_trg");
    auto edep_y_in = bnk::getv<double>("edep_y_trg");
    auto pos_x_in = bnk::getv<double>("pos_x_trg");
    auto pos_y_in = bnk::getv<double>("pos_y_trg");
    // auto pos_z_in = bnk::getv<double>("pos_z_dif");
    
    int nhits_x_res = 0;
    int nhits_y_res = 0;
    std::vector<int> detid_x_res;
    std::vector<int> detid_y_res;
    std::vector<int> material_x_res;
    std::vector<int> material_y_res;
    std::vector<int> strip_x_res;
    std::vector<int> strip_y_res;
    std::vector<double> edep_x_res;
    std::vector<double> edep_y_res;
    std::vector<double> pos_x_res;
    std::vector<double> pos_y_res;
    // std::vector<double> pos_z_res;

    // static constexpr double electronics_noise = 1.6;
    
    for ( int index=0; index<nhits_x_in; ++index ) {
	auto strip = strip_x_in[index];
	auto pos = pos_x_in[index];	
	auto detid = detid_x_in[index];
	auto material = material_x_in[index];
	auto mate = "si";
	if ( material==1 ) mate = "cdte";
	
	auto edep = randomize_energy_cathode( edep_x_in[index],
					      parameter.electronics_noise,
					      mate );

	detid_x_res.emplace_back( detid );
	material_x_res.emplace_back( material );
	strip_x_res.emplace_back( strip );
	edep_x_res.emplace_back( edep );
	pos_x_res.emplace_back( pos );
	++nhits_x_res;
	
	// auto pos_z = pos_z_in[index];
	// pos_z_res.emplace_back( pos_z ); 
	
    }
    for ( int index=0; index<nhits_y_in; ++index ) {
    	auto strip = strip_y_in[index];
    	auto pos = pos_y_in[index];
	auto detid = detid_y_in[index];
	auto material = material_y_in[index];
	auto mate = "si";
	if ( material==1 ) mate = "cdte";	

	auto edep = randomize_energy_anode( edep_y_in[index],
					    parameter.electronics_noise,
					    mate );

	detid_y_res.emplace_back( detid );
	material_y_res.emplace_back( material );
	strip_y_res.emplace_back( strip );
	edep_y_res.emplace_back( edep );
	pos_y_res.emplace_back( pos );
	++nhits_y_res;
    }
    
    bnk::put<int>("nhits_x_res", nhits_x_res);
    bnk::put<int>("detid_x_res", detid_x_res, 0, nhits_x_res);
    bnk::put<int>("material_x_res", material_x_res, 0, nhits_x_res);
    bnk::put<int>("strip_x_res", strip_x_res, 0, nhits_x_res);
    bnk::put<double>("edep_x_res", edep_x_res, 0, nhits_x_res);
    bnk::put<double>("pos_x_res", pos_x_res, 0, nhits_x_res);
    
    bnk::put<int>("nhits_y_res", nhits_y_res);
    bnk::put<int>("detid_y_res", detid_y_res, 0, nhits_y_res);
    bnk::put<int>("material_y_res", material_y_res, 0, nhits_y_res);    
    bnk::put<int>("strip_y_res", strip_y_res, 0, nhits_y_res);
    bnk::put<double>("edep_y_res", edep_y_res, 0, nhits_y_res);
    bnk::put<double>("pos_y_res", pos_y_res, 0, nhits_y_res);
    // bnk::put<double>("pos_z_res", pos_z_res, 0, nhits_res);
    
    return anl::ANL_OK;
}

int ApplyEnergyResponse::mod_endrun()
{
    // /** This function is called at the end of the run **/
    // cout << mod_name2() << "::mod_endrun()";
    // cout << " -> Good Bye. :)" << endl;

    // /** Example of operations described here **/

    // /** Closing files **/
    // // ifs.close(); // std::ifstream
    return anl::ANL_OK;
}

double ApplyEnergyResponse::randomize_energy_cathode
(double energy, double electronics_noise, const std::string& mate)
{
    if ( parameter.is_enabled_randomize==false ) return energy;
    auto de = energy_resolution_cathode(energy, electronics_noise, mate);
    return gRandom->Gaus(energy, de/2.35);
}
double ApplyEnergyResponse::randomize_energy_anode
(double energy, double electronics_noise, const std::string& mate)
{
    if ( parameter.is_enabled_randomize==false ) return energy;
    auto de = energy_resolution_anode(energy, electronics_noise, mate);
    return gRandom->Gaus(energy, de/2.35);
}
double ApplyEnergyResponse::energy_resolution_cathode
(double energy, double electronics_noise, const std::string& mate)
{
    if ( mate=="cdte" )
	return std::sqrt(electronics_noise*electronics_noise+0.003231*energy) + 0.0149*energy;    
    return std::sqrt(electronics_noise*electronics_noise+0.002187*energy);
}
double ApplyEnergyResponse::energy_resolution_anode
(double energy, double electronics_noise, const std::string& mate)
{
    if ( mate=="cdte" )
	return std::sqrt(electronics_noise*electronics_noise+0.003231*energy) + 0.0149*energy;    
    return std::sqrt(electronics_noise*electronics_noise+0.002187*energy);	    
}
