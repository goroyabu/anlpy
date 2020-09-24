/**
   @file   HistogramCompton.cpp
   @date   2020/09/09
   @author 
   @detail Automatically generated by make_anlpy_project.sh 1.0.0
**/

#include "HistogramCompton.hpp"

#include <utility>

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <bnk.hpp>
#include <evs.hpp>

HistogramCompton::histos_each_layer::histos_each_layer(int detector_id, TString sub)
    : detid(detector_id), subname(sub)
{
    static const int nbins = 2000;
    static const double xmin = 0.0;
    static const double xmax = 1000.0;
    
    th2_energy_spectra_each_channels
	= new TH2D( Form("spect2d_ch_detid%d"+subname, detector_id),
		    Form("detid%d"+subname+";ch(0-127=Pt, 128-255=Al);keV",detector_id),
		    256, -0.5, 255.5, nbins*0.25, xmin, xmax );

    auto hname = th2_energy_spectra_each_channels->GetName();

    if ( th2_energy_spectra_each_channels!=nullptr ) {
	cout << "histogram : " << hname << " is defined." << endl;
    }
    else
	cerr << "Creating " << hname << " is failed." << endl;
    
    th1_energy_spectra = nullptr;

    th2_energy_correlation
	= new TH2D( Form("ene_corr_detid%d"+subname, detector_id),
		    Form("detid%d"+subname+";ave=(Y+X)/2[keV];diff=(Y-X)/2[keV]",
			 detector_id ),
		    nbins*0.25, xmin, xmax, 100, -50, 50 );
    th2_diff_vs_cathode
	= new TH2D( Form("ene_diff_cathode_detid%d"+subname, detector_id),
		    Form("detid%d"+subname+";Cathode(X-side)[keV];diff=(Y-X)/2[keV]",
			 detector_id ),
		    nbins*0.25, xmin, xmax, 100, -50, 50 );
    th2_diff_vs_anode
	= new TH2D( Form("ene_diff_anode_detid%d"+subname, detector_id),
		    Form("detid%d"+subname+";Anode(Y-side)[keV];diff=(Y-X)/2[keV]",
			 detector_id ),
		    nbins*0.25, xmin, xmax, 100, -50, 50 );
}
void HistogramCompton::histos_each_layer::Write()
{

    if ( detid<2 )
	th1_energy_spectra = th2_energy_spectra_each_channels->ProjectionY("py",1,128);
    else
	th1_energy_spectra = th2_energy_spectra_each_channels->ProjectionY("py",129,256);
    th1_energy_spectra->SetNameTitle( Form("spect1d_ch_detid%d"+subname, detid),
				      Form("detid%d"+subname+";keV",detid));    
    th2_energy_spectra_each_channels->Write();
    th1_energy_spectra->Write();

    th2_energy_correlation->Write();
    th2_diff_vs_cathode->Write();
    th2_diff_vs_anode->Write();
}

HistogramCompton::histo2d_energy_vs_energy::histo2d_energy_vs_energy()
{
    static const int nbins = 2000;
    static const double xmin = 0.0;
    static const double xmax = 1000.0;
    
    std::vector<std::tuple<std::string,std::string>> hlist =
	{
	 { "th2_eplot_si1_cdte1", "Energy Si1 vs CdTe1" },
	 { "th2_eplot_si1_cdte2", "Energy Si1 vs CdTe2" },
	 { "th2_eplot_si1_cdte3", "Energy Si1 vs CdTe3" },
	 { "th2_eplot_si2_cdte1", "Energy Si2 vs CdTe1" },
	 { "th2_eplot_si2_cdte2", "Energy Si2 vs CdTe2" },
	 { "th2_eplot_si2_cdte3", "Energy Si2 vs CdTe3" },
	 { "th2_eplot_si1_cdte1_ecut", "Energy Si1 vs CdTe1 after ethre cut" },
	 { "th2_eplot_si1_cdte2_ecut", "Energy Si1 vs CdTe2 after ethre cut" },
	 { "th2_eplot_si1_cdte3_ecut", "Energy Si1 vs CdTe3 after ethre cut" },
	 { "th2_eplot_si2_cdte1_ecut", "Energy Si2 vs CdTe1 after ethre cut" },
	 { "th2_eplot_si2_cdte2_ecut", "Energy Si2 vs CdTe2 after ethre cut" },
	 { "th2_eplot_si2_cdte3_ecut", "Energy Si2 vs CdTe3 after ethre cut" }	 
	};

    for ( auto [ hname, htitle ] : hlist ) {
	this->histo[ hname ]
	    = new TH2D( hname.c_str(), (TString)htitle.c_str()+";keV;keV",
			nbins, xmin, xmax, nbins, xmin, xmax
			);
	cout << "histogram : " << hname << " is defined." << endl;
    }

    for ( auto [ hname, h ] : histo ) {
	cout << hname << endl;
	cout << h->GetName() << endl;
    }
}
void HistogramCompton::histo2d_energy_vs_energy::Write()
{
    for ( auto [ hname, h ] : histo ) {
	h->Write();
    }
}

HistogramCompton::histo2d_hitpattern::histo2d_hitpattern(TString subname)
{
    histo_2hits =
	new TH2D( (TString)"hit_pattern_nhits2"+subname,
		  "Hit-patterns 2Hits;0=Si1,1=Si2;2=CdTe1,3=CdTe2,4=CdTe3",
		  5, -0.5, 4.5, 5, -0.5, 4.5 );
    
    histo_3hits =
	new TH2D( (TString)"hit_pattern_nhits3"+subname,
		  "Hit-patterns 3Hits;Hit1;Hit2+Hit3",
		  5, -0.5, 4.5, 15, -0.5, 14.5 );
}
void HistogramCompton::histo2d_hitpattern::Fill2Hits(int det1, int det2)
{
    std::vector<int> det = { det1, det2 };
    std::sort( det.begin(), det.end() );
    histo_2hits->Fill( det[0], det[1] );
}
void HistogramCompton::histo2d_hitpattern::Fill3Hits(int det1, int det2, int det3)
{
    std::vector<int> det = { det1, det2, det3 };
    std::sort( det.begin(), det.end() );
    auto hitid = hit_pattern_id( det[1], det[2] );
    histo_3hits->Fill( det[0], hitid );
}
void HistogramCompton::histo2d_hitpattern::Write()
{
    histo_2hits->Write();
    histo_3hits->Write();
}
HistogramCompton::histos_energy_peak::histos_energy_peak
(double peak, double width)
{
    incident_energy = peak;
    window_half_width = width;

    th1_arm = nullptr;
    th2_scat_vs_arm =
	new TH2D( Form("th2_scat_vs_arm_%03.0fkev",incident_energy),
		  Form("ARM of %5.1fkeV peak;ARM[degree];Si Energy[keV]",
		       incident_energy),
		  360, -180, 180, 1000, -0.5, 999.5 );
    th2_geom_vs_kine =
	new TH2D( Form("th2_geom_vs_kine_%03.0fkev",incident_energy),
		  Form("Theta Correlation %5.1fkeV;Geometric[deg];Kinetic[deg]",
		       incident_energy),
		  360, -180, 180, 360, -180, 180 );
}
void HistogramCompton::histos_energy_peak::Write()
{
    th1_arm = th2_scat_vs_arm->ProjectionX("px");
    th1_arm->SetName( Form("th1_arm_%03.0fkev",incident_energy) );
    th1_arm->Write();    
    th2_scat_vs_arm->Write();
    th2_geom_vs_kine->Write();
}


HistogramCompton::HistogramCompton()
    : anl::VANL_Module("HistogramCompton", "0.1")
{
    define_parameter<std::string>( "output_file", "output.root" );
    output_file = nullptr;
    th2_energy_vs_energy = nullptr;

    define_parameter<double>("energy_window_half", 3.0);
    define_parameter<std::string>("incident_energy_list", "511.0,31.0");

    define_parameter<double>("source_origin_x", 0);
    define_parameter<double>("source_origin_y", 0);
    define_parameter<double>("source_origin_z", 41.35);
    
    define_parameter<std::string>("cdte_energy_mode", "Anode");
    define_parameter<std::string>("abso_energy_mode", "CdTe");
}
HistogramCompton::~HistogramCompton()
{
}

int HistogramCompton::mod_bgnrun()
{
    cout << "HistogramCompton::mod_bgnrun() start" << endl;
    auto output_filename = get_parameter<std::string>( "output_file" );
    
    output_file = new TFile( output_filename.c_str(), "recreate" );
    if ( output_file==nullptr || output_file->IsZombie() )
	return anl::ANL_NG;

    // histos = new std::vector<int,histos_each_layer*>();
    std::vector<int> detid_list = { 0, 1, 2, 3, 4 };
    for ( auto detid : detid_list ) {
    	histos[ detid ] = new histos_each_layer(detid);
	histos_ecut[ detid ] = new histos_each_layer(detid, "_ecut");
    }

    evs::define("Si_CdTe_2Hits_Not_Fluor_and_Ene_Consis");
    evs::define("Reasonable_Angle_Geom");
    evs::define("Reasonable_Angle_Kine");
    evs::define("Si_CdTe_Compton_Event");    
    
    evs::define("Si1_CdTe1_2Hits_Before_Cut");
    evs::define("Si1_CdTe2_2Hits_Before_Cut");
    evs::define("Si1_CdTe3_2Hits_Before_Cut");
    evs::define("Si2_CdTe1_2Hits_Before_Cut");
    evs::define("Si2_CdTe2_2Hits_Before_Cut");
    evs::define("Si2_CdTe3_2Hits_Before_Cut");
    evs::define("Si1_CdTe1_2Hits_After_Cut");
    evs::define("Si1_CdTe2_2Hits_After_Cut");
    evs::define("Si1_CdTe3_2Hits_After_Cut");
    evs::define("Si2_CdTe1_2Hits_After_Cut");
    evs::define("Si2_CdTe2_2Hits_After_Cut");
    evs::define("Si2_CdTe3_2Hits_After_Cut");
    
    th2_energy_vs_energy = new histo2d_energy_vs_energy();

    static const int nbins = 2000;
    static const double xmin = 0.0;
    static const double xmax = 1000.0;    

    th1_total_energy_spectra_comtpon
	= new TH1D( "total_compton_detid",
		    "Si+CdTe Compton detid;keV",
		    nbins*0.25, xmin, xmax );    

    th2_hit_pattern = new histo2d_hitpattern();
    th2_hit_pattern_ecut = new histo2d_hitpattern("_ecut");

    parameter.energy_window_half = get_parameter<double>("energy_window_half");
    
    auto elist
	= split_to_double( get_parameter<std::string>("incident_energy_list") );
    std::sort( elist.begin(), elist.end() );
    parameter.incident_energy_list = elist;    

    for ( auto epeak : elist ) {
	
	auto h = new histos_energy_peak( epeak, parameter.energy_window_half );
	histos_peaks.emplace_back( h );	

	TString evs_name =
	    Form("Si+CdTe_In_E-window_of_%5.1fkeV", h->GetEnergy() );	
	evs::define((std::string)evs_name);
    }    

    auto orig_x = get_parameter<double>("source_origin_x");
    auto orig_y = get_parameter<double>("source_origin_y");
    auto orig_z = get_parameter<double>("source_origin_z");
    parameter.source_origin = TVector3( orig_x, orig_y, orig_z );

    auto cdte_emode = get_parameter<std::string>("cdte_energy_mode");
    if ( cdte_emode=="Anode" || cdte_emode=="anode" ||
	 cdte_emode=="Al-side" || cdte_emode=="Y-side" || cdte_emode=="y-side" ) {
	parameter.cdte_energy_mode = ANODE;
	cout << "> Use Energy of Anode-sideas CdTe Energy" << endl;
    }
    else {
	parameter.cdte_energy_mode = AVERAGE;
	cout << "> Use Average Energy of both-sides as CdTe Energy" << endl;
    }

    auto abs_emode = get_parameter<std::string>("abso_energy_mode");
    if ( abs_emode=="CdTe" ) {
	parameter.abso_energy_mode = CDTE;
	cout << "> Use CdTe Energy as Absorption" << endl;	
    }
    else {
	parameter.abso_energy_mode = PEAK_EXT_SI;
	cout << "> Use Energy of (Epeak-Si) as Absorption" << endl;
    }
    
    return anl::ANL_OK;
}

int HistogramCompton::mod_ana()
{
    auto detid  = bnk::getv<int>   ( "detid_lv3_mod" );
    auto epi    = bnk::getv<float>( "epi_lv3_mod"   );
    auto epi_x  = bnk::getv<float>( "epi_x_lv3" );
    auto epi_y  = bnk::getv<float>( "epi_y_lv3" );
    auto pos_x  = bnk::getv<float>( "pos_x_lv3" );
    auto pos_y  = bnk::getv<float>( "pos_y_lv3" );
    auto pos_z  = bnk::getv<float>( "pos_z_lv3" );
    // auto init_x = bnk::get<double> ( "init_x"    );
    // auto init_y = bnk::get<double> ( "init_y"    );
    // auto init_z = bnk::get<double> ( "init_z"    );
    
    auto strip_x = bnk::getv<int>   ( "strip_x_lv3" );
    auto strip_y = bnk::getv<int>   ( "strip_y_lv3" );

    auto hitid = bnk::getv<int>( "hitid_over_ethre" );

    auto nhits_all = (int)detid.size();
    
    for ( int ihit=0; ihit<nhits_all; ++ihit ) {
	
    	auto hist = histos[ detid[ihit] ];

    	hist->th2_energy_spectra_each_channels
    	    ->Fill( strip_x[ihit], epi_x[ihit] );
	hist->th2_energy_spectra_each_channels
    	    ->Fill( strip_y[ihit], epi_y[ihit] );

	auto epi_ave  = ( epi_y[ihit]+epi_x[ihit] ) * 0.5;
	auto epi_diff = ( epi_y[ihit]-epi_x[ihit] ) * 0.5;
	hist->th2_energy_correlation->Fill( epi_ave, epi_diff );	
    }
        
    if ( nhits_all==2 ) {

	std::vector<int>    det = { detid[0], detid[1] };
	std::vector<double> ene = { epi[0],   epi[1]   };       
	
	if ( det[0]>det[1] ) {
	    std::swap( det[0], det[1] );
	    std::swap( ene[0], ene[1] ); 
	}

	th2_hit_pattern->Fill2Hits( det[0], det[1] );
	
	if ( det[0]==0 && det[1]==2 ) {
	    evs::set("Si1_CdTe1_2Hits_Before_Cut");	    
	    th2_energy_vs_energy->histo
		["th2_eplot_si1_cdte1"]->Fill( ene[0], ene[1]);
	}	
	else if ( det[0]==0 && det[1]==3 ) {
	    evs::set("Si1_CdTe2_2Hits_Before_Cut");
	    th2_energy_vs_energy->histo
		["th2_eplot_si1_cdte2"]->Fill( ene[0], ene[1]);
	}
	else if ( det[0]==0 && det[1]==4 ) {
	    evs::set("Si1_CdTe3_2Hits_Before_Cut");
	    th2_energy_vs_energy->histo
		["th2_eplot_si1_cdte3"]->Fill( ene[0], ene[1]);
	}
	else if ( det[0]==1 && det[1]==2 ) {
	    evs::set("Si2_CdTe1_2Hits_Before_Cut");
	    th2_energy_vs_energy->histo
		["th2_eplot_si2_cdte1"]->Fill( ene[0], ene[1]);
	}
	else if ( det[0]==1 && det[1]==3 ) {
	    evs::set("Si2_CdTe2_2Hits_Before_Cut");
	    th2_energy_vs_energy->histo
		["th2_eplot_si2_cdte2"]->Fill( ene[0], ene[1]);		    
	}
	else if ( det[0]==1 && det[1]==4 ) {
	    evs::set("Si2_CdTe3_2Hits_Before_Cut");
	    th2_energy_vs_energy->histo
		["th2_eplot_si2_cdte3"]->Fill( ene[0], ene[1]);	    
	}
    }
    else if ( nhits_all==3 ) {
	
	th2_hit_pattern->Fill3Hits( detid[0], detid[1], detid[2] );
	
    }    
    
    auto nhits_over_thre = (int)hitid.size();
    if ( nhits_over_thre<1 ) return anl::ANL_OK;    
    
    if ( evs::get( "Si_CdTe_Coin_After_Cut" )==false ) return anl::ANL_OK;
    
    if ( nhits_over_thre==2 ) {
	std::vector<double> ene = { epi[ hitid[0] ], epi[ hitid[1] ] };
	if ( detid[ hitid[0] ]>detid[ hitid[1] ] ) 
	    std::swap( ene[0], ene[1] ); 

	th2_hit_pattern_ecut->Fill2Hits( detid[hitid[0]], detid[hitid[1]] );

	
	if ( evs::get("Hit_on_Si1_After_Cut") &&
	     evs::get("Hit_on_CdTe1_After_Cut") ) {
	    th2_energy_vs_energy->histo
		["th2_eplot_si1_cdte1_ecut"]->Fill( ene[0], ene[1]);
	    evs::set("Si1_CdTe1_2Hits_After_Cut");
	}

	else if ( evs::get("Hit_on_Si1_After_Cut") &&
		  evs::get("Hit_on_CdTe2_After_Cut") ) {
	    th2_energy_vs_energy->histo
		["th2_eplot_si1_cdte2_ecut"]->Fill( ene[0], ene[1]);
	    evs::set("Si1_CdTe2_2Hits_After_Cut");
	}

	else if ( evs::get("Hit_on_Si1_After_Cut") &&
		  evs::get("Hit_on_CdTe3_After_Cut") ) {
	    th2_energy_vs_energy->histo
		["th2_eplot_si1_cdte3_ecut"]->Fill( ene[0], ene[1]);
	    evs::set("Si1_CdTe3_2Hits_After_Cut");
	}

	else if ( evs::get("Hit_on_Si2_After_Cut") &&
		  evs::get("Hit_on_CdTe1_After_Cut") ) {
	    th2_energy_vs_energy->histo
		["th2_eplot_si2_cdte1_ecut"]->Fill( ene[0], ene[1]);
	    evs::set("Si2_CdTe1_2Hits_After_Cut");
	}

	else if ( evs::get("Hit_on_Si2_After_Cut") &&
		  evs::get("Hit_on_CdTe2_After_Cut") ) {
	    th2_energy_vs_energy->histo
		["th2_eplot_si2_cdte2_ecut"]->Fill( ene[0], ene[1]);
	    evs::set("Si2_CdTe2_2Hits_After_Cut");
	}

	else if ( evs::get("Hit_on_Si2_After_Cut") &&
		  evs::get("Hit_on_CdTe3_After_Cut") ) {
	    th2_energy_vs_energy->histo
		["th2_eplot_si2_cdte3_ecut"]->Fill( ene[0], ene[1]);
	    evs::set("Si2_CdTe3_2Hits_After_Cut");
	}
    }
    else if ( nhits_over_thre==3 ) {

	std::vector<std::tuple<int,double>> hits
	    = { { detid[ hitid[0] ], epi[ hitid[0] ] },
		{ detid[ hitid[1] ], epi[ hitid[1] ] },
		{ detid[ hitid[2] ], epi[ hitid[2] ] }  };
	
	std::sort( hits.begin(), hits.end(),
		   []( auto ha, auto hb ) {
		       auto [ da, ea ] = ha;
		       auto [ db, eb ] = hb;
		       return da < db;
		   });

	th2_hit_pattern_ecut->Fill3Hits( detid[hitid[0]],
					 detid[hitid[1]], detid[hitid[2]] );
	
    }

    if ( evs::get( "Si_CdTe_2Hits_After_Cut" )==false ) return anl::ANL_OK;
    
    std::vector<double> ene = { epi[ hitid[0] ], epi[ hitid[1] ] };
    if ( detid[ hitid[0] ]>detid[ hitid[1] ] ) 
	std::swap( ene[0], ene[1] ); 
    // th1_total_energy_spectra_comtpon->Fill( ene[0]+ene[1] );	        
    
    for ( int ihit=0; ihit<nhits_over_thre; ++ihit ) {
    	int id = hitid[ihit];
    	auto hist = histos_ecut[ detid[id] ];
	if ( hist==nullptr ) {
	    continue;
	}	
    	hist->th2_energy_spectra_each_channels
    	    ->Fill( strip_x[id], epi_x[id] );
    	hist->th2_energy_spectra_each_channels
    	    ->Fill( strip_y[id], epi_y[id] );

	auto epi_ave  = ( epi_y[id]+epi_x[id] ) * 0.5;
	auto epi_diff = ( epi_y[id]-epi_x[id] ) * 0.5;
	hist->th2_energy_correlation->Fill( epi_ave,   epi_diff );
	hist->th2_diff_vs_cathode->   Fill( epi_x[id], epi_diff );
	hist->th2_diff_vs_anode->     Fill( epi_y[id], epi_diff );	
    }        
    
    if ( evs::get( "Not_Include_Fluor_Hits" )==false ) return anl::ANL_OK;
    if ( evs::get( "Ene_Consistent_Abso_Hits" )==false ) return anl::ANL_OK;
    evs::set("Si_CdTe_2Hits_Not_Fluor_and_Ene_Consis");
    
    auto epi_x_si = bnk::get<double>("epi_x_si");
    // auto epi_y_si = bnk::get<double>("epi_y_si");
    auto epi_si = epi_x_si;
    auto epi_x_cdte = bnk::get<double>("epi_x_cdte");
    auto epi_y_cdte = bnk::get<double>("epi_y_cdte");
    auto epi_cdte = epi_y_cdte;
    auto pos_x_si = bnk::get<double>("pos_x_si");
    auto pos_y_si = bnk::get<double>("pos_y_si");
    auto pos_z_si = bnk::get<double>("pos_z_si");
    auto pos_x_cdte = bnk::get<double>("pos_x_cdte");
    auto pos_y_cdte = bnk::get<double>("pos_y_cdte");
    auto pos_z_cdte = bnk::get<double>("pos_z_cdte");    
    
    if ( parameter.cdte_energy_mode==AVERAGE )
	epi_cdte = ( epi_x_cdte + epi_y_cdte ) * 0.5;
    
    auto epi_total_compton = epi_si + epi_cdte;
    bnk::put<double>( "epi_total_compton", epi_total_compton );
    
    TVector3 pos_orig = parameter.source_origin;
    TVector3 pos_scat( pos_x_si, pos_y_si, pos_z_si );
    TVector3 pos_abso( pos_x_cdte, pos_y_cdte, pos_z_cdte );   
    auto theta_geometric = angle_of_3points( pos_orig, pos_scat, pos_abso );
    // bnk::put<double>( "theta_geometric", theta_geometric );
    
    static const double theta_max = 150.0;
    if ( theta_geometric<0.0 || theta_max<theta_geometric ) return anl::ANL_OK;    
    evs::set( "Reasonable_Angle_Geom" );

    auto theta_kinetic = compton_angle( epi_si, epi_cdte );
    if ( theta_kinetic<0.0 || theta_max<theta_kinetic ) return anl::ANL_OK;
    evs::set( "Reasonable_Angle_Kine" );

    th1_total_energy_spectra_comtpon->Fill( epi_total_compton );
    
    for ( auto h : histos_peaks ) {
	
    	if ( h->IsMatchEnergy(epi_total_compton) ) {	    
	    
    	    auto epi_scat = epi_si;
    	    auto epi_abso = epi_cdte;
    	    if ( parameter.abso_energy_mode==PEAK_EXT_SI )
    		epi_abso = h->GetEnergy() - epi_si;
	    
    	    auto theta_kinetic_peak = compton_angle( epi_scat, epi_abso );
    	    // bnk::put<double>( "theta_kinetic", theta_kinetic );
	    
    	    if ( theta_kinetic_peak<0.0 || theta_max<theta_kinetic_peak )
    	    	continue;	    
    	    evs::set( "Si_CdTe_Compton_Event" );

    	    auto arm = theta_geometric - theta_kinetic_peak;
	    // auto arm  = bnk::get<double>( "angular_resolution_measure" );
    	    // bnk::put<double>( "angular_resolution_measure", arm );

	    if ( std::fabs(arm)>25.0 ) {
		cout << "det=" << detid[ hitid[0] ] << "," << detid[ hitid[1] ] << endl;
		cout << "epi=" << epi[ hitid[0] ] << "," << epi[ hitid[1] ] << endl;
		// cout << "epi_si=" << epi_si << ", epi_cdte=" << epi_cdte << endl;
		cout << "epi_scat=" << epi_scat << ", epi_abso=" << epi_abso << endl;
		cout << "epi_x_cdte=" << epi_x_cdte << ", epi_y_cdte=" << epi_y_cdte << endl;
		cout << "pos_orig=";
		pos_orig.Print();
		cout << "pos_scat=";
		pos_scat.Print();
		cout << "pos_abso=";
		pos_abso.Print();
		cout << "theta_g =" << theta_geometric << endl;
		cout << "theta_k =" << theta_kinetic << endl;
		cout << "theta_kp=" << theta_kinetic_peak << endl;
	    }
	    
    	    TString evs_name =
    		Form("Si+CdTe_In_E-window_of_%5.1fkeV", h->GetEnergy() );
    	    evs::set((std::string)evs_name);
	    
    	    h->th2_scat_vs_arm->Fill( arm, epi_si );
    	    h->th2_geom_vs_kine->Fill( theta_geometric, theta_kinetic );

    	}
    }
    
    // // if ( evs::get("Si_CdTe_Compton_Event")==false ) return anl::ANL_OK;
    // if ( evs::get("Fluor_Hits_on_Si")==true ) return anl::ANL_OK; 
    
    // // auto epi_total_compton = bnk::get<double>( "epi_total_compton" );
    // theta_kinetic   = bnk::get<double>( "theta_kinetic"   );
    // theta_geometric = bnk::get<double>( "theta_geometric" );
    
    // if ( theta_geometric<0.0 || theta_max<theta_geometric ) return anl::ANL_OK;
    // if ( theta_kinetic<0.0 || theta_max<theta_kinetic ) return anl::ANL_OK;
    
    // auto arm  = bnk::get<double>( "angular_resolution_measure" );
    // // th1_total_energy_spectra_comtpon->Fill( epi_total_compton );
    
    // for ( auto h : histos_peaks ) {
    // 	if ( h->IsMatchEnergy(epi_total_compton) ) {
    // 	    auto epi_si = bnk::get<double>("epi_si");
    // 	    // cout << arm << endl;
    // 	    h->th2_scat_vs_arm->Fill( arm, epi_si );
    // 	    h->th2_geom_vs_kine->Fill( theta_geometric, theta_kinetic );

    // 	    TString evs_name =
    // 		Form("Si+CdTe_In_E-window_of_%5.1fkeV", h->GetEnergy() );
    // 	    evs::set((std::string)evs_name);
    // 	}
    // }
    
    return anl::ANL_OK;
}

int HistogramCompton::mod_endrun()
{
    output_file->cd();

    for ( auto [ detid, hist ] : histos ) {
	if ( hist!=nullptr ) hist->Write();
    }

    for ( auto [ detid, hist ] : histos_ecut ) {
	if ( hist!=nullptr ) hist->Write();
    }    
    
    th2_energy_vs_energy->Write();

    th2_hit_pattern->Write();
    th2_hit_pattern_ecut->Write();

    th1_total_energy_spectra_comtpon->Write();

    for ( auto h : histos_peaks )
	h->Write();
    
    output_file->Close();
    
    return anl::ANL_OK;
}

