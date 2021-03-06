/**
   @file   ProjectCone3D.cpp
   @date   2020/05/07
   @author 
   @detail Automatically generated by make_anlpy_project.sh 1.0.0
**/

#include "ProjectCone3D.hpp"

#include <iostream>
using std::cout;
using std::endl;

#include <bnk.hpp>
#include <evs.hpp>

#include <TROOT.h>
#include <TSeqCollection.h>
#include <TCanvas.h>

ProjectCone3D::ProjectCone3D()
    : anl::VANL_Module("ProjectCone3D", "0.1"),
      input_file(nullptr), input_tree(nullptr),
      output_file(nullptr), output_tree(nullptr),
      image(nullptr)
{
    define_parameter<std::string>("input_file", "input.root");
    define_parameter<std::string>("input_tree", "hittree");
    define_parameter<std::string>("output_file", "output.root");
    define_parameter<std::string>("output_tree", "resptree");
    define_parameter<double>("e_window_begin", 100.0);
    define_parameter<double>("e_window_end", 300.0);
    define_parameter<double>("cone_thick_deg", 0.5);
    define_parameter<double>("distance_index", 2.0);
    define_parameter<double>("e_threshold_si", 5.0);
    define_parameter<double>("e_threshold_cdte", 5.0);

    define_parameter<int>("nbins_1axis", 100);
    define_parameter<double>("axis_minimum", -25.0);
    define_parameter<double>("axis_maximum", 25.0);

    define_parameter<int>("xaxis_nbins", 70);
    define_parameter<double>("xaxis_minimum", -35.0);
    define_parameter<double>("xaxis_maximum", 35.0);

    define_parameter<int>("yaxis_nbins", 70);
    define_parameter<double>("yaxis_minimum", -35.0);
    define_parameter<double>("yaxis_maximum", 35.0);
    
    define_parameter<int>("zaxis_nbins", 40);
    define_parameter<double>("zaxis_minimum", -10.0);
    define_parameter<double>("zaxis_maximum", 10.0);
    
    define_parameter<double>("theta_max_degree", 150.0);
    define_parameter<double>("detector_z_position", -41.0);
    define_parameter<int>("event_list_only", 0);
    define_parameter<double>("rotation_around_vertical_deg", 0.0);
    define_parameter<int>("enable_reject_fluor", 1);
    define_parameter<int>("enable_normalize_cone", 1);
}
ProjectCone3D::~ProjectCone3D()
{
}

int ProjectCone3D::mod_bgnrun()
{
    // auto nbins = get_parameter<int>("nbins_1axis");
    // auto xmin = get_parameter<double>("axis_minimum");
    // auto xmax = get_parameter<double>("axis_maximum");
    
    // cout << "current_dir=" << gDirectory->GetName() << endl;
    
    auto ifname = get_parameter<std::string>("input_file");
    input_file = new TFile( ifname.c_str() );
    if ( !input_file || input_file->IsZombie() ) {
	cout << "Opening " << ifname << " is faild." << endl;
	return anl::ANL_NG;
    }
    cout << ifname << " is opened." << endl;

    auto itname = get_parameter<std::string>("input_tree");
    input_tree = (TTree*)input_file->Get( itname.c_str() );
    if ( !input_tree ) {
	cout << itname << " is not found." << endl;
	return anl::ANL_NG;
    }

    event.set_branch_address(input_tree);	
    
    auto ofname = get_parameter<std::string>("output_file");

    TSeqCollection * list = gROOT->GetListOfFiles();
    auto opened = list->FindObject( ofname.c_str() );
    // if ( !!opened ) {
    // 	cout << opened->GetName() << " " << opened->ClassName() << endl;
    // }
    // else cout << "null" << endl;
    
    if ( !!opened && opened->ClassName()==(TString)"TFile"
	 && ((TFile*)opened)->IsWritable() ) {
	output_file = (TFile*)opened;
	output_file->cd();
	cout << ofname << " is already opened." << endl;	
    }
    else {        
	output_file = new TFile( ofname.c_str(), "recreate" );
	if ( !output_file || output_file->IsZombie() ) {
	    cout << "Creating " << ofname << " is failed." << endl;
	    return anl::ANL_NG;
	}
	cout << ofname << " is created." << endl;
    }
    
    auto otname = get_parameter<std::string>("output_tree");
    output_tree = new TTree( otname.c_str(), otname.c_str() );

    auto copyid = get_parameter<std::string>("copyid");
    is_event_list_only = get_parameter<int>("event_list_only");

    auto x_nbins = get_parameter<int>("xaxis_nbins");
    auto x_min   = get_parameter<double>("xaxis_minimum");
    auto x_max   = get_parameter<double>("xaxis_maximum");
    
    auto y_nbins = get_parameter<int>("yaxis_nbins");
    auto y_min   = get_parameter<double>("yaxis_minimum");
    auto y_max   = get_parameter<double>("yaxis_maximum");
    
    auto z_nbins = get_parameter<int>("zaxis_nbins");
    auto z_min   = get_parameter<double>("zaxis_minimum");
    auto z_max   = get_parameter<double>("zaxis_maximum");
    
    if ( !is_event_list_only ) {
	image = new TH3F( (TString)"response"+copyid.c_str(),
			  "response;X(mm);Y(mm);Z(mm)",
			  x_nbins, x_min, x_max, y_nbins, y_min, y_max, z_nbins, z_min, z_max );
	h1_cone_filling_ratio
	    = new TH1D( (TString)"cone_filling_ratio_"+copyid.c_str(),
			"cone_filling_ratio;Z(mm)", z_nbins, z_min, z_max );
    }
    
    if ( define_branch( output_tree )!=anl::ANL_OK ) return anl::ANL_NG;
    
    evs::define("Si-CdTe 2hits event");
    // evs::define("SC-2hits w/o flour");
    evs::define("SC-2hits in E-window");
    evs::define("SC-2hits in Theta-range");

    e_window_begin = get_parameter<double>("e_window_begin");
    e_window_end = get_parameter<double>("e_window_end");
    cone_thick_rad = get_parameter<double>("cone_thick_deg")*TMath::Pi()/180.0;
    tangent_cone_thick = TMath::Tan( cone_thick_rad/2.35 );
    distance_index_omega = -1.0*get_parameter<double>("distance_index");
    e_threshold_si = get_parameter<double>("e_threshold_si");
    e_threshold_cdte = get_parameter<double>("e_threshold_cdte");
    theta_max_degree = get_parameter<double>("theta_max_degree");
    detector_z_position = get_parameter<double>("detector_z_position");

    auto deg = get_parameter<double>("rotation_around_vertical_deg");

    if ( deg!=0.0 )
	rotation_around_vertical_rad = deg/180.0*TMath::Pi();
    else
	rotation_around_vertical_rad = 0.0;

    cout << "Rotatation Angle around Y-axis is set to " << rotation_around_vertical_rad << " [rad]" << endl;
    
    enable_reject_fluor = get_parameter<int>("enable_reject_fluor");
    enable_normalize_cone = get_parameter<int>("enable_normalize_cone");
    
    return anl::ANL_OK;
}

int ProjectCone3D::mod_ana()
{
    if ( !event.next() ) return anl::ANL_LOOP;
    
    auto [ si, cdte ] = get_sc2hit_event();

    if ( si.Energy()<=0.0 || cdte.Energy()<=0.0 ) return anl::ANL_SKIP;

    evs::set("Si-CdTe 2hits event");

    // if ( has_flour(si) || has_flour(cdte) ) 
    // 	return anl::ANL_SKIP;        
    // evs::set("SC-2hits w/o flour");
    
    if ( !is_in_energy_range( si.Energy()+cdte.Energy() ) )
	return anl::ANL_SKIP;

    evs::set("SC-2hits in E-window");

    // if ( use_cdte_energy_for_abso==false && is_event_list_only==false )
    // 	cdte.e = peak_energy - si.e;

    if ( !is_in_theta_range( si, cdte ) )
	return anl::ANL_SKIP;           

    evs::set("SC-2hits in Theta-range");
    
    if ( is_event_list_only ) return anl::ANL_OK;
    
    auto is_filled_voxels = this->projection( image, si, cdte );
    if ( is_filled_voxels ) this->fill();
        
    return anl::ANL_OK;
}

int ProjectCone3D::mod_endrun()
{
    cout << mod_name2() << "::mod_endrun" << endl;
    output_file->cd();
    output_tree->Write();
    // output_file->Close();
    input_file->Close();
    return anl::ANL_OK;
}

TH1D* ProjectCone3D::cone_filling_ratio
(TH3F* image, const TVector3& scat, const TVector3& abso, double scat_angle_deg)
{
    auto vec_cone_axis = scat - abso;
    // auto scat_angle_deg = eval_theta( si.Energy(), cdte.Energy() );

    auto vec_genline_origin = vec_cone_axis;
    auto vertical = vec_cone_axis.Cross( TVector3(0,1,0) );
    vec_genline_origin.Rotate( scat_angle_deg, vertical );

    static const int nplot = 1000;
    double deltaphi = 2*TMath::Pi()/nplot;

    // auto zaxis = image->GetZaxis();
    this->h1_cone_filling_ratio->Reset();
    auto nbins = this->h1_cone_filling_ratio->GetXaxis()->GetNbins();
    
    for ( int iphi=0; iphi<nplot; ++iphi ) {

	auto phi = deltaphi*iphi;
	auto vec_genline = vec_genline_origin;
	vec_genline.Rotate( phi, vec_cone_axis );

	for ( int zbin=1; zbin<nbins+1; zbin++ ) {

	    auto z = h1_cone_filling_ratio->GetXaxis()->GetBinCenter( zbin );
	    auto deltaz_scat_to_xyplane = z - scat.Z();
	    
	    auto vec_genline_scat_to_xyplane = vec_genline;
	    vec_genline_scat_to_xyplane *= deltaz_scat_to_xyplane/vec_genline.Z();

	    auto pos_genline_on_xyplane = scat + vec_genline_scat_to_xyplane;
	    
	    if ( is_in_image(image, pos_genline_on_xyplane) )
		h1_cone_filling_ratio->Fill( z, (double)1/nplot );
	    
	}	
    }

    // h1_cone_filling_ratio->Draw();
    // gPad->SaveAs("test.png");
    
    // h1_cone_filling_ratio->Scale( (double)1/nplot );
    return h1_cone_filling_ratio;
}
double ProjectCone3D::scale_by_filling_ratio(TH3F* th3, TH1D* ratio)
{
    auto nbinsx = th3->GetXaxis()->GetNbins();
    auto nbinsy = th3->GetYaxis()->GetNbins();
    auto nbinsz = th3->GetZaxis()->GetNbins();
    auto nbins_th1 = ratio->GetXaxis()->GetNbins();
    if ( nbinsz != nbins_th1 ) return -1;
    
    for ( int zbin=1; zbin<=nbinsz; ++zbin ) {
	
	auto z = th3->GetZaxis()->GetBinCenter(zbin);
	double integral = 0.0;
	
	for ( int xbin=1; xbin<=nbinsx; ++xbin ) 
	    for ( int ybin=1; ybin<=nbinsy; ++ybin ) 
		integral += th3->GetBinContent( xbin, ybin, zbin );

	if ( integral == 0.0 ) continue;
	auto factor = 1/integral;//get_bin_content(ratio, z)/integral;
	auto rate = get_bin_content(ratio, z);
	if ( factor==0.0 || rate==0.0 ) continue; 

	// cout << integral << " " << factor << " " << rate << endl;
	
	for ( int xbin=1; xbin<=nbinsx; ++xbin ) 
	    for ( int ybin=1; ybin<=nbinsy; ++ybin ) 
		th3->SetBinContent( xbin, ybin, zbin,
				    th3->GetBinContent( xbin, ybin, zbin )
				    *factor*rate );	    	    	    
    }
    
    return 0;
}

bool ProjectCone3D::projection(TH3F* image, const hit& si, const hit& cdte)
{
    // cout << "projection" << endl;
    image->Reset();
    auto nvoxels = image->GetNcells();
    
    auto scat  = si.Postion();
    auto abso  = cdte.Postion();    
    
    auto vec_cone_axis = scat - abso; //fHitPos[0] - fHitPos[1];
    auto scat_angle_deg = eval_theta( si.Energy(), cdte.Energy() );
    //Double_t armrad = armdeg*TMath::Pi()/180.0;
    //fDthetaCompcone = armrad/2.35;
    //fTanDthetaCompcone = TMath::Tan(fDthetaCompcone);
    
    // bool is_filled_voxels = false;
    int n_of_filled_voxels = 0;
    
    for ( int i=0; i<nvoxels; ++i ) {
	if ( image->IsBinOverflow(i) || image->IsBinUnderflow(i) )
	    continue;
	
	TVector3 voxel = voxel_center(image, i);
	
	auto vec_scat2vox = voxel - scat;
	auto fVecAxisRotateGeneratingline = vec_cone_axis.Cross(vec_scat2vox);
	auto vec_genline_nearest_to_vox = vec_cone_axis;

	if ( fVecAxisRotateGeneratingline.Mag()>=0.0001 ){
	    vec_genline_nearest_to_vox.Rotate(scat_angle_deg, fVecAxisRotateGeneratingline);
	}
	
	//auto fVecGeneratingLineUni = vec_genline_nearest_to_vox.Unit();
	/* The event is rejected if the direction of the generating line
	   is not oriented to the front of the camera. */
	auto scale = vec_scat2vox.Dot( vec_genline_nearest_to_vox.Unit() );
	if( scale<=0.0 ) continue;
	vec_genline_nearest_to_vox *= scale/vec_genline_nearest_to_vox.Mag();

	// if ( vec_scat2vox.Dot( vec_genline_nearest_to_vox.Unit() )<=0 )
	//     continue;	
	
	Double_t mag_vec_nearest_to_vox = vec_genline_nearest_to_vox.Mag();
	Double_t perpen_dist = ( vec_scat2vox - vec_genline_nearest_to_vox ).Mag();
	Double_t sigma = mag_vec_nearest_to_vox*tangent_cone_thick;

	if( perpen_dist>sigma*3 ) continue;
	
	auto weight = TMath::Power( mag_vec_nearest_to_vox, distance_index_omega )
	    *TMath::Exp( -0.5* TMath::Power( perpen_dist/sigma, 2 ) );
	
	if ( weight<=0 ) continue;
	image->SetBinContent( i, image->GetBinContent(i)+weight );
	// is_filled_voxels = true;
	++n_of_filled_voxels;
	
	//image->Fill( voxel.x, voxel.y, voxel.z, weight );
	//fHistBranch->Fill(voxx,voxy,voxz,weight);	
    }
    // if ( is_filled_voxels ) output_tree->Fill();

    const static int threshold_n_of_filled_voxels = 10;
    
    if ( n_of_filled_voxels<=threshold_n_of_filled_voxels )
	return false;

    if ( enable_normalize_cone==false )
	return true;
    
    auto filling_ratio = cone_filling_ratio(image, scat, abso, scat_angle_deg);
    
    scale_by_filling_ratio( image, filling_ratio );
    
    return true;
}

int ProjectCone3D::define_branch(TTree* tree)
{
    tree->Branch( "ti", &ti, "ti/i" );
    tree->Branch( "livetime", &livetime, "livetime/i" );
    tree->Branch( "unixtime", &unixtime, "unixtime/i" );
    tree->Branch( "ext1pps", &ext1pps, "ext1pps/i" );
    tree->Branch( "msec_counter", &msec_counter, "msec_counter/i" );
    
    tree->Branch( "num_hits", &num_hits, "num_hits/I" );
    tree->Branch( "externalCLK", &externalCLK, "externalCLK/i" );
    tree->Branch( "first_internalCLK", &first_internalCLK, "internalCLK/i" );
    tree->Branch( "hit1_detector", &hit1_detector, "hit1_detector/S" );
    tree->Branch( "hit1_energy", &hit1_energy, "hit1_energy/F" );
    tree->Branch( "hit1_posx", &hit1_posx, "hit1_posx/F" );
    tree->Branch( "hit1_posy", &hit1_posy, "hit1_posy/F" );
    tree->Branch( "hit1_posz", &hit1_posz, "hit1_posz/F" );
    tree->Branch( "hit2_detector", &hit2_detector, "hit2_detector/S" );
    tree->Branch( "hit2_energy", &hit2_energy, "hit2_energy/F" );
    tree->Branch( "hit2_posx", &hit2_posx, "hit2_posx/F" );
    tree->Branch( "hit2_posy", &hit2_posy, "hit2_posy/F" );
    tree->Branch( "hit2_posz", &hit2_posz, "hit2_posz/F" );
    tree->Branch( "hit3_detector", &hit3_detector, "hit3_detector/S" );
    tree->Branch( "hit3_energy", &hit3_energy, "hit3_energy/F" );
    tree->Branch( "hit3_posx", &hit3_posx, "hit3_posx/F" );
    tree->Branch( "hit3_posy", &hit3_posy, "hit3_posy/F" );
    tree->Branch( "hit3_posz", &hit3_posz, "hit3_posz/F" );
    tree->Branch( "totalenergy", &totalenergy, "totalenergy/F" );
    
    if ( event.exist_branch("coin_eventid") ) 
	output_tree->Branch( "coin_eventid", &event.coin_eventid, "coin_eventid/L" );
    
    if ( event.exist_branch("coin_delta_t") ) 
	output_tree->Branch( "coin_delta_t", &event.coin_delta_t, "coin_delta_t/I" );

    if ( is_event_list_only ) return anl::ANL_OK;
    
    if ( !tree->Branch( "response", "TH3F", &image ) ) {
	cout << "Creating a branch of TH3F is failed." << endl;
	return anl::ANL_NG;
    }
    
    return anl::ANL_OK;
}

std::tuple<ProjectCone3D::hit, ProjectCone3D::hit> ProjectCone3D::get_sc2hit_event()
{
    ti = event.ti;
    livetime = event.livetime;
    unixtime = event.unixtime;
    ext1pps = event.ext1pps;
    msec_counter = event.msec_counter;
    externalCLK = event.ext1pps;
    first_internalCLK = event.msec_counter;
    
    int n_si = 0; int n_cdte = 0;
    hit si; hit cdte;
    
    for ( int i=0; i<event.nhit_lv3; ++i ) {
	
	if ( is_cdte(event.detid_lv3[i])
	     && event.epi_y_lv3[i]>=e_threshold_cdte ){
	    
	    cdte.detid = event.detid_lv3[i];
	    cdte.e = event.epi_y_lv3[i];
	    cdte.x = event.pos_x_lv3[i];
	    cdte.y = event.pos_y_lv3[i];
	    cdte.z = event.pos_z_lv3[i] + detector_z_position;
	    ++n_cdte;
	    
	}
	else if ( is_si(event.detid_lv3[i])
		  && event.epi_x_lv3[i]>=e_threshold_si
		  && is_rejected_as_fluor( event.epi_x_lv3[i] )==false ) {
	    
	    si.detid = event.detid_lv3[i];
	    si.e = event.epi_x_lv3[i];
	    si.x = event.pos_x_lv3[i];
	    si.y = event.pos_y_lv3[i];
	    si.z = event.pos_z_lv3[i] + detector_z_position;
	    ++n_si;
	    
	}	
    }    

    // if ( rotation_around_vertical_rad!=0.0 ) {
	
    TVector3 cdte_pos(cdte.x, cdte.y, cdte.z);
    cdte_pos.RotateY( rotation_around_vertical_rad );
    cdte.x = cdte_pos.x();
    cdte.y = cdte_pos.y();
    cdte.z = cdte_pos.z();
    
    TVector3 si_pos(si.x, si.y, si.z);
    si_pos.RotateY( rotation_around_vertical_rad );
    si.x = si_pos.x();
    si.y = si_pos.y();
    si.z = si_pos.z();
    // cdte.x = -1.0 * cdte.x;
    // cdte.z = -1.0 * cdte.z;
    // si.x = -1.0 * si.x;
    // si.z = -1.0 * si.z;
    
    //}
    
    num_hits = n_si + n_cdte;
    hit1_detector = si.detid;
    hit1_energy = si.e;
    hit1_posx = si.x;
    hit1_posy = si.y;
    hit1_posz = si.z;
    hit2_detector = cdte.detid;
    hit2_energy = cdte.e;
    hit2_posx = cdte.x;
    hit2_posy = cdte.y;
    hit2_posz = cdte.z;
    hit3_detector = -1;
    hit3_energy = -1.0;
    hit3_posx = 0.0;
    hit3_posy = 0.0;
    hit3_posz = 0.0;
    totalenergy = si.e + cdte.e;
    
    if ( n_si==1 && n_cdte==1 ) return std::make_tuple( si, cdte );
    return std::make_tuple( hit(), hit() );
}

bool ProjectCone3D::hittree_event::exist_branch(TTree* tree, TString key)
{
    if ( !tree->FindBranch(key) ) {
	cout << "TBranch " << key << " is not found." << endl;
	return false;
    }
    return true;
}
int ProjectCone3D::hittree_event::set_branch_address(TTree* tree)
{
    tree->SetBranchAddress( "ti", &ti );
    tree->SetBranchAddress( "livetime", &livetime );
    tree->SetBranchAddress( "unixtime", &unixtime );
    tree->SetBranchAddress( "ext1pps", &ext1pps );
    tree->SetBranchAddress( "msec_counter", &msec_counter );
    
    if ( !exist_branch(tree, "nhit_lv3")  ) return anl::ANL_NG;
    if ( !exist_branch(tree, "detid_lv3") ) return anl::ANL_NG;
    if ( !exist_branch(tree, "epi_lv3")   ) return anl::ANL_NG;
    if ( !exist_branch(tree, "epi_x_lv3") ) return anl::ANL_NG;
    if ( !exist_branch(tree, "epi_y_lv3") ) return anl::ANL_NG;
    if ( !exist_branch(tree, "pos_x_lv3") ) return anl::ANL_NG;
    if ( !exist_branch(tree, "pos_y_lv3") ) return anl::ANL_NG;
    if ( !exist_branch(tree, "pos_z_lv3") ) return anl::ANL_NG;

    static const int n = 10;
    detid_lv3.resize(n);
    epi_lv3.resize(n);
    epi_x_lv3.resize(n);
    epi_y_lv3.resize(n);
    pos_x_lv3.resize(n);
    pos_y_lv3.resize(n);
    pos_z_lv3.resize(n);
    
    tree->SetBranchAddress( "nhit_lv3",  &nhit_lv3        );
    tree->SetBranchAddress( "detid_lv3", detid_lv3.data() );
    tree->SetBranchAddress( "epi_lv3",   epi_lv3.data()   );
    tree->SetBranchAddress( "epi_x_lv3", epi_x_lv3.data() );
    tree->SetBranchAddress( "epi_y_lv3", epi_y_lv3.data() );
    tree->SetBranchAddress( "pos_x_lv3", pos_x_lv3.data() );
    tree->SetBranchAddress( "pos_y_lv3", pos_y_lv3.data() );
    tree->SetBranchAddress( "pos_z_lv3", pos_z_lv3.data() );

    if ( exist_branch(tree, "coin_eventid") )
	tree->SetBranchAddress( "coin_eventid", &coin_eventid );
    if ( exist_branch(tree, "coin_delta_t") )
	tree->SetBranchAddress( "coin_delta_t", &coin_delta_t );
    
    nentries = tree->GetEntries();
    current_entry = -1;
    this->tree = tree;    
    return 0;
}
bool ProjectCone3D::hittree_event::next()
{
    ++current_entry;
    if ( current_entry>=nentries ) return false;
    tree->GetEntry(current_entry);
    return true;
}
