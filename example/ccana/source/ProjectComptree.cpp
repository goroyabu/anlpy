/***
    @file   ProjectComptree.cpp
    @date
    @author User
    @detail Automatically generated by mkanlproject
***/

#include "ProjectComptree.hpp"

#include <iostream>
using std::cout;
using std::endl;

#include <bnk.hpp>
#include <evs.hpp>

#include <TROOT.h>
#include <TSeqCollection.h>
#include <TCanvas.h>

ProjectComptree::ProjectComptree()
    : anl::VANL_Module("ProjectComptree", "20211025b"),
    output_file(nullptr),
    output_tree(nullptr),
    image(nullptr),
    image_etcc(nullptr),
    h1_cone_filling_ratio(nullptr),
    etrack_calc_dedx(nullptr)
{
    // define_parameter<std::string>("input_file", "input.root");
    // define_parameter<std::string>("input_tree", "comptree");
    define_parameter<std::string>("output_file", "output.root");
    define_parameter<std::string>("output_tree", "resptree");

    define_parameter<double>("cone_thick_deg", 0.5);
    define_parameter<double>("arc_length_deg", 10.0);

    define_parameter<double>("distance_index", 2.0);
    define_parameter<int>("enable_normalize_cone", 1);
    // define_parameter<double>("rotation_around_vertical_deg", 0.0);

    define_parameter<int>("xaxis_nbins", 70);
    define_parameter<double>("xaxis_minimum", -35.0);
    define_parameter<double>("xaxis_maximum", 35.0);

    define_parameter<int>("yaxis_nbins", 70);
    define_parameter<double>("yaxis_minimum", -35.0);
    define_parameter<double>("yaxis_maximum", 35.0);

    define_parameter<int>("zaxis_nbins", 40);
    define_parameter<double>("zaxis_minimum", -10.0);
    define_parameter<double>("zaxis_maximum", 10.0);

    define_parameter<int>("use_polar_coordinate", false);
}
ProjectComptree::~ProjectComptree()
{
    if ( this->output_file->IsOpen() ) {
        this->output_file->Close();
        cout << " - Closed : " << this->get_parameter<std::string>("output_file") << endl;
    }
}

int ProjectComptree::mod_bgnrun()
{
    cout << " --- ProjectComptree::mod_bgnrun" << endl;
    cout << endl;

    auto ofname = get_parameter<std::string>("output_file");

    TSeqCollection * list = gROOT->GetListOfFiles();
    auto opened = list->FindObject( ofname.c_str() );

    if (
        !!opened
        && opened->ClassName()==(TString)"TFile"
        && ((TFile*)opened)->IsWritable()
    ) {
        this->output_file = (TFile*)opened;
        this->output_file->cd();
        cout << ofname << " is already opened." << endl;
    }
    else {
        this->output_file = new TFile( ofname.c_str(), "recreate" );
        if ( !this->output_file || this->output_file->IsZombie() ) {
            cout << "Creating " << ofname << " is failed." << endl;
            return anl::ANL_NG;
        }
        cout << ofname << " is created." << endl;
    }

    auto otname = get_parameter<std::string>("output_tree");
    this->output_tree = new TTree( otname.c_str(), otname.c_str() );
    // this->output_tree->SetNameTitle( otname.c_str(), otname.c_str() );

    auto copyid = get_parameter<std::string>("copyid");
    // this->is_event_list_only = get_parameter<int>("event_list_only");

    auto x_nbins = get_parameter<int>("xaxis_nbins");
    auto x_min   = get_parameter<double>("xaxis_minimum");
    auto x_max   = get_parameter<double>("xaxis_maximum");

    auto y_nbins = get_parameter<int>("yaxis_nbins");
    auto y_min   = get_parameter<double>("yaxis_minimum");
    auto y_max   = get_parameter<double>("yaxis_maximum");

    auto z_nbins = get_parameter<int>("zaxis_nbins");
    auto z_min   = get_parameter<double>("zaxis_minimum");
    auto z_max   = get_parameter<double>("zaxis_maximum");

    this->is_used_polar_coordinate = get_parameter<int>("use_polar_coordinate");

    if ( this->is_used_polar_coordinate == false ) {
        this->image = new TH3F(
            (TString)"response"+copyid.c_str(),
            "response;X(mm);Y(mm);Z(mm)",
            x_nbins, x_min, x_max,
            y_nbins, y_min, y_max,
            z_nbins, z_min, z_max );

        this->image_etcc = (TH3F*)this->image->Clone();
        this->image_etcc->SetNameTitle(
            (TString)"response_etcc"+copyid.c_str(),
            "response_etcc;X(mm);Y(mm);Z(mm)"
        );
    }
    else {
        this->image = new TH3F(
            (TString)"response"+copyid.c_str(),
            "response as polar coordinate;sin#theta*cos#phi;sin#theta*sin#phi;radius(mm)",
            x_nbins, -0.5*TMath::Pi(), 0.5* TMath::Pi(),
            y_nbins, -0.5*TMath::Pi(), 0.5* TMath::Pi(),
            z_nbins, z_min, z_max );

        this->image_etcc = (TH3F*)this->image->Clone();
        this->image_etcc->SetNameTitle(
            (TString)"response_etcc"+copyid.c_str(),
            "response_etcc as polar coordinate;sin#theta*cos#phi;sin#theta*sin#phi;radius(mm)"
        );
    }

    this->h1_cone_filling_ratio = new TH1D(
        (TString)"cone_filling_ratio_"+copyid.c_str(),
        "cone_filling_ratio;Z(mm)", z_nbins, z_min, z_max );

    const static double pixel_pitch = 0.02; //mm
    this->etrack_calc_dedx = new TH2D( "track_image", "etrack",
        110, -pixel_pitch*(10-0.5), pixel_pitch*(100-0.5),
        110, -pixel_pitch*(10-0.5), pixel_pitch*(100-0.5)
        );

    this->h1_arm_distribution = new TH1D( "arm_dist", "ARM;(deg)",
        360, -180.5, 179.5
    );
    this->h1_spd_distribution = new TH1D( "spd_dist", "SPD;(deg)",
        360, -180.5, 179.5
    );
    this->h2_arm_vs_spd = new TH2D( "arm_vs_spd", ";SPD;ARM",
        360, -180.5, 179.5,
        360, -180.5, 179.5
    );
    this->h2_arm_distribition = new TH2D( "arm_dist_2d", ";Kine;Geom",
        360, -180.5, 179.5,
        360, -180.5, 179.5
    );
    this->h2_spd_distribition = new TH2D( "spd_dist_2d", ";Esti;Geom;",
        360, -180.5, 179.5,
        360, -180.5, 179.5
    );
    this->h2_energy_hit1_vs_hit2 = new TH2D(
        "h2_energy_hit1_vs_hit2",
        ";Si(hit1_energy,keV);CdTe(hit2_energy,keV)",
        250, -2, 998,
        250, -2, 998
    );
    this->h2_energy_hit1_vs_hit2_comp = new TH2D(
        "h2_energy_hit1_vs_hit2_comp",
        "only Compton;Si(hit1_energy,keV);CdTe(hit2_energy,keV)",
        250, -2, 998,
        250, -2, 998
    );
    this->h1_totalenergy = new TH1D(
        "h1_totalenergy",
        ";Si+CdTe(totalenergy,keV)",
        1000, -0.5, 999.5
    );
    this->h1_totalenergy_comp = new TH1D(
        "h1_totalenergy_comp",
        "only Compton;Si+CdTe(totalenergy,keV)",
        1000, -0.5, 999.5
    );

    if ( DefineBranch( this->output_tree )!=anl::ANL_OK ) return anl::ANL_NG;

    cout << endl;

    auto cone_thick_deg = get_parameter<double>("cone_thick_deg");
    cone_thick_rad = cone_thick_deg*TMath::Pi()/180.0;
    tangent_cone_thick = TMath::Tan( cone_thick_rad/2.35 );
    cout << " - Thickness of Cone : " << cone_thick_deg << "(deg)" << endl;

    auto arc_length_deg = get_parameter<double>("arc_length_deg");
    this->arc_length_rad = arc_length_deg*TMath::Pi()/180.0;
    this->arc_length_sigma = this->arc_length_rad/2.35;
    cout << " - Length of Arc : " << arc_length_deg << "(deg), ";
    cout << this->arc_length_sigma << "(rad)" << endl;

    distance_index_omega = -1.0*get_parameter<double>("distance_index");
    cout << " - Distance-factor Omega : " << distance_index_omega << endl;

    cout << endl;
    cout << endl;
    return anl::ANL_OK;
}

int ProjectComptree::mod_ana()
{
    if ( evs::get("Si_CdTe_2Hits_Event")==false )
        return anl::ANL_SKIP;

    if ( evs::get("E_Consistent_Si_CMOS")==false )
        return anl::ANL_SKIP;

    this->hit1_energy = bnk::get<double>("hit1_energy");
    this->hit2_energy = bnk::get<double>("hit2_energy");
    this->totalenergy = bnk::get<double>("totalenergy");
    this->h2_energy_hit1_vs_hit2->Fill( hit1_energy, hit2_energy );
    this->h1_totalenergy->Fill( totalenergy );

    if ( evs::get("SC_2hits_In_Theta_Range")==false )
        return anl::ANL_SKIP;
    else {
        this->h2_energy_hit1_vs_hit2_comp->Fill( hit1_energy, hit2_energy );
        this->h1_totalenergy_comp->Fill( totalenergy );
    }

    if ( evs::get("SC_2hits_In_E_Window")==false )
        return anl::ANL_SKIP;

    this->theta_kine = bnk::get<double>("theta_kine");
    this->theta_geom = bnk::get<double>("theta_geom");
    auto arm = bnk::get<double>("arm");
    this->h1_arm_distribution->Fill( arm );
    this->h2_arm_distribition->Fill(
        theta_kine*TMath::Pi()*180, theta_geom*TMath::Pi()*180 );

    if ( evs::get("Direction_Is_Reconstruted")==false )
        return anl::ANL_SKIP;

    this->phi_esti = bnk::get<double>("phi_esti");
    this->phi_geom = bnk::get<double>("phi_geom");
    auto spd = bnk::get<double>("spd");
    this->h1_spd_distribution->Fill( spd );
    this->h2_spd_distribition->Fill(
        phi_esti*TMath::Pi()*180, phi_geom*TMath::Pi()*180 );
    this->h2_arm_vs_spd->Fill( spd, arm );

    if ( evs::get("Eigen_Value_Is_Over_Threshold")==false )
        return anl::ANL_SKIP;

    if ( evs::get("Pixel_Ratio_Is_Over_Threshold")==false )
        return anl::ANL_SKIP;

    this->num_hits = bnk::get<int>("num_hits");
    this->hit1_detector = 0;
    this->hit1_posx = bnk::get<double>("hit1_posx");
    this->hit1_posy = bnk::get<double>("hit1_posy");
    this->hit1_posz = bnk::get<double>("hit1_posz");
    this->hit2_detector = 1;
    this->hit2_posx = bnk::get<double>("hit2_posx");
    this->hit2_posy = bnk::get<double>("hit2_posy");
    this->hit2_posz = bnk::get<double>("hit2_posz");

    this->Projection();
    // this->CalcComptonEvent( si, cdte );
    // auto is_filled_voxels = this->Projection( si, cdte );
    // if ( is_filled_voxels )
    this->output_tree->Fill();

    return anl::ANL_OK;
}

int ProjectComptree::mod_endrun()
{
    cout << mod_name2() << "::mod_endrun" << endl;

    this->output_file->cd();

    this->output_tree->Write();

    this->h1_arm_distribution->Write();
    this->h1_spd_distribution->Write();
    this->h2_arm_vs_spd->Write();
    this->h2_arm_distribition->Write();
    this->h2_spd_distribition->Write();

    this->h2_energy_hit1_vs_hit2->Write();
    this->h2_energy_hit1_vs_hit2_comp->Write();
    this->h1_totalenergy->Write();
    this->h1_totalenergy_comp->Write();

    // this->output_file->Close();
    // cout << " - Closed : " << this->get_parameter<std::string>("output_file") << endl;

    return anl::ANL_OK;
}

int ProjectComptree::DefineBranch(TTree* tree)
{
    // tree->Branch( "ti", &ti, "ti/i" );
    // tree->Branch( "livetime", &livetime, "livetime/i" );
    // tree->Branch( "unixtime", &unixtime, "unixtime/i" );
    // tree->Branch( "ext1pps", &ext1pps, "ext1pps/i" );
    // tree->Branch( "msec_counter", &msec_counter, "msec_counter/i" );

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
    tree->Branch( "totalenergy", &totalenergy, "totalenergy/F" );

    tree->Branch( "theta_kine", &theta_kine, "theta_kine/F" );
    tree->Branch( "theta_geom", &theta_geom, "theta_geom/F" );
    tree->Branch( "phi_esti", &phi_esti, "phi_esti/F" );
    tree->Branch( "phi_geom", &phi_geom, "phi_geom/F" );
    // tree->Branch( "de_over_dx", &de_over_dx, "de_over_dx/F" );

    tree->Branch( "angle_electron_on_plane", &angle_electron_on_plane, "angle_electron_on_plane/F" );
    tree->Branch( "angle_electron_vertical", &angle_electron_vertical, "angle_electron_vertical/F" );

    tree->Branch( "track_image", "TH2D", &(this->etrack_calc_dedx) );
    tree->Branch( "response", "TH3F", &image );
    tree->Branch( "response_etcc", "TH3F", &image_etcc );

    return anl::ANL_OK;
}
bool ProjectComptree::Projection()//const Hit& si, const Hit& cdte)
{
    this->image->Reset();
    this->image_etcc->Reset();

    auto nvoxels = this->image->GetNcells();
    auto vec_norm_vertical = TVector3( 0, 1, 0 );

    auto scat = TVector3( this->hit1_posx, this->hit1_posy, this->hit1_posz );
    auto abso = TVector3( this->hit2_posx, this->hit2_posy, this->hit2_posz );
    auto vec_cone_axis = scat - abso;

    auto angle_theta_rad = this->theta_kine;
    auto angle_theta_rad_geom = this->theta_geom;
    auto angle_phi_rad = this->phi_esti;
    auto angle_phi_rad_geom = this->phi_geom;

    int n_of_filled_voxels = 0;

    for ( int i=0; i<nvoxels; ++i ) {
        if ( image->IsBinOverflow(i) || image->IsBinUnderflow(i) ) continue;

        auto voxel = TVector3();
        if ( this->is_used_polar_coordinate )
            voxel = this->VoxelCenterPolar(image, i);
        else
            voxel = this->VoxelCenter(image, i);

        auto vec_scat2vox = voxel - scat;
        auto fVecAxisRotateGeneratingline = vec_cone_axis.Cross(vec_scat2vox);
        auto vec_genline_nearest_to_vox = vec_cone_axis;

        if ( 0.001<=fVecAxisRotateGeneratingline.Mag() ){
            vec_genline_nearest_to_vox.Rotate(angle_theta_rad, fVecAxisRotateGeneratingline);
        }

        /* The event is rejected if the direction of the generating line
        is not oriented to the front of the camera. */
        auto scale = vec_scat2vox.Dot( vec_genline_nearest_to_vox.Unit() );
        if( scale<=0.0 ) continue;

        vec_genline_nearest_to_vox *= scale/vec_genline_nearest_to_vox.Mag();

        double mag_vec_nearest_to_vox = vec_genline_nearest_to_vox.Mag();
        double perpen_dist = ( vec_scat2vox - vec_genline_nearest_to_vox ).Mag();
        double sigma = mag_vec_nearest_to_vox*this->tangent_cone_thick;

        if( sigma*3<perpen_dist ) continue;

        auto weight = TMath::Power( mag_vec_nearest_to_vox, distance_index_omega )
            *TMath::Exp( -0.5* TMath::Power( perpen_dist/sigma, 2 ) );

        if ( weight<=0 ) continue;
        image->SetBinContent( i, image->GetBinContent(i)+weight );

        auto vec_axis_projected = TVector3( vec_cone_axis.Unit() );
        double mag_vec_axis_projected = vec_axis_projected.Dot( vec_genline_nearest_to_vox );
        vec_axis_projected *= mag_vec_axis_projected;

        auto vec_phi_axis_to_surface = vec_genline_nearest_to_vox - vec_axis_projected;
        auto angle_phi_voxel = TVector2::Phi_mpi_pi( vec_phi_axis_to_surface.DeltaPhi( vec_norm_vertical ) );

        auto delta_phi = angle_phi_rad - angle_phi_voxel;
        if ( fabs(delta_phi)<this->arc_length_sigma*3 ) {
            weight *= TMath::Exp( -0.5* TMath::Power( delta_phi/this->arc_length_sigma, 2 ) );
            image_etcc->SetBinContent( i, image_etcc->GetBinContent(i)+weight );
            ++n_of_filled_voxels;
        }

    }

    const static int threshold_n_of_filled_voxels = 10;

    if ( n_of_filled_voxels<=threshold_n_of_filled_voxels ) return false;

    if ( this->enable_normalize_cone==false ) return true;

    auto filling_ratio = this->GetFillingRatio(image, scat, abso, angle_theta_rad);
    this->ScaleByFillingRatio( image, filling_ratio );

    filling_ratio = this->GetFillingRatio(image_etcc, scat, abso, angle_theta_rad);
    this->ScaleByFillingRatio( image_etcc, filling_ratio );

    return true;
}
TH1D* ProjectComptree::GetFillingRatio
(TH3F* image, const TVector3& scat, const TVector3& abso, double angle_theta_rad)
{
    auto vec_cone_axis = scat - abso;
    // auto angle_theta_rad = eval_theta( si.Energy(), cdte.Energy() );

    auto vec_genline_origin = vec_cone_axis;
    auto vertical = vec_cone_axis.Cross( TVector3(0,1,0) );
    vec_genline_origin.Rotate( angle_theta_rad, vertical );

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

            auto z = this->h1_cone_filling_ratio->GetXaxis()->GetBinCenter( zbin );
            auto deltaz_scat_to_xyplane = z - scat.Z();

            auto vec_genline_scat_to_xyplane = vec_genline;
            vec_genline_scat_to_xyplane *= deltaz_scat_to_xyplane/vec_genline.Z();

            auto pos_genline_on_xyplane = scat + vec_genline_scat_to_xyplane;

            if ( this->IsVectorInside(image, pos_genline_on_xyplane) )
                this->h1_cone_filling_ratio->Fill( z, (double)1/nplot );

        }
    }
    return this->h1_cone_filling_ratio;
}
double ProjectComptree::ScaleByFillingRatio(TH3F* th3, TH1D* ratio)
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

        auto factor = 1/integral;
        auto rate = this->GetBinContent(ratio, z);
        if ( factor==0.0 || rate==0.0 ) continue;

        for ( int xbin=1; xbin<=nbinsx; ++xbin ) {
            for ( int ybin=1; ybin<=nbinsy; ++ybin ) {
                th3->SetBinContent( xbin, ybin, zbin,
                th3->GetBinContent( xbin, ybin, zbin )
                *factor*rate );
            }
        }
    }

    return 0;
}