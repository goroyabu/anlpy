/***
    @file   ProjectConeETCC.cpp
    @date
    @author User
    @detail Automatically generated by mkanlproject
***/

#include "ProjectConeETCC.hpp"

#include <iostream>
using std::cout;
using std::endl;

#include <bnk.hpp>
#include <evs.hpp>

#include <TROOT.h>
#include <TSeqCollection.h>
#include <TCanvas.h>

ProjectConeETCC::ProjectConeETCC()
    : anl::VANL_Module("ProjectConeETCC", "0.1"),
    input_file(nullptr),
    input_tree(nullptr),
    output_file(nullptr),
    output_tree(nullptr),
    h1_cone_filling_ratio(nullptr),
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
ProjectConeETCC::~ProjectConeETCC()
{
}

int ProjectConeETCC::mod_bgnrun()
{
    auto ifname = get_parameter<std::string>("input_file");
    input_file = new TFile( ifname.c_str() );
    if ( !input_file || input_file->IsZombie() ) {
        cout << "Opening " << ifname << " is failed." << endl;
        return anl::ANL_NG;
    }
    cout << ifname << " is opened." << endl;

    auto itname = get_parameter<std::string>("input_tree");
    input_tree = (TTree*)input_file->Get( itname.c_str() );
    if ( !input_tree ) {
        cout << itname << " is not found." << endl;
        return anl::ANL_NG;
    }
    cout << itname << " is loaded." << endl;

    event.SetBranchAddress(input_tree);

    auto ofname = get_parameter<std::string>("output_file");

    TSeqCollection * list = gROOT->GetListOfFiles();
    auto opened = list->FindObject( ofname.c_str() );

    if (
        !!opened
        && opened->ClassName()==(TString)"TFile"
        && ((TFile*)opened)->IsWritable()
    ) {
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
        image = new TH3F(
            (TString)"response"+copyid.c_str(),
            "response;X(mm);Y(mm);Z(mm)",
            x_nbins, x_min, x_max,
            y_nbins, y_min, y_max,
            z_nbins, z_min, z_max );
        h1_cone_filling_ratio = new TH1D(
            (TString)"cone_filling_ratio_"+copyid.c_str(),
            "cone_filling_ratio;Z(mm)", z_nbins, z_min, z_max );
    }

    if ( DefineBranch( output_tree )!=anl::ANL_OK ) return anl::ANL_NG;

    evs::define("Si-CdTe 2hits event");
    evs::define("E-consistent Si-CMOS");
    evs::define("SC-2hits in E-window");
    evs::define("SC-2hits in Theta-range");

    e_window_begin = get_parameter<double>("e_window_begin");
    e_window_end   = get_parameter<double>("e_window_end");
    cout << " - Energy-window : " << e_window_begin << ", " << e_window_end << "keV" << endl;

    auto cone_thick_deg = get_parameter<double>("cone_thick_deg");
    cone_thick_rad = cone_thick_deg*TMath::Pi()/180.0;
    tangent_cone_thick = TMath::Tan( cone_thick_rad/2.35 );
    cout << " - Thickness of Cone : " << cone_thick_deg << "(deg)" << endl;

    distance_index_omega = -1.0*get_parameter<double>("distance_index");
    cout << " - Distance-factor Omega : " << distance_index_omega << endl;

    e_threshold_si = get_parameter<double>("e_threshold_si");
    e_threshold_cdte = get_parameter<double>("e_threshold_cdte");
    cout << " - Threshold for Si-DSD   : " << e_threshold_si << "keV" << endl;
    cout << " - Threshold for CdTe-DSD : " << e_threshold_cdte << "keV" << endl;

    this->theta_max_degree = get_parameter<double>("theta_max_degree");
    cout << " - Maximum scattering angle : " << this->theta_max_degree << "(deg)" << endl;

    this->detector_z_position = get_parameter<double>("detector_z_position");
    cout << " - Detector-Z : " << this->detector_z_position << "mm" << endl;

    auto deg = get_parameter<double>("rotation_around_vertical_deg");
    cout << " - Rotation around Y-axis : " << deg << "(deg)" << endl;

    if ( deg!=0.0 )
        this->rotation_around_vertical_rad = deg/180.0*TMath::Pi();
    else
        this->rotation_around_vertical_rad = 0.0;
    // cout << "Rotatation Angle around Y-axis is set to " << rotation_around_vertical_rad << " [rad]" << endl;

    this->enable_reject_fluor = get_parameter<int>("enable_reject_fluor");
    this->enable_normalize_cone = get_parameter<int>("enable_normalize_cone");

    int nbin_param2 = 32;
    double min_param2 = -0.2;
    double max_param2 = 0.1;
    const static double cdteposz_param2[32] = {
        -8.71105, -8.63401, -8.59368, -8.56371, -8.45148,
        -8.40101, -8.29345, -8.25342, -8.15028, -8.10914,
        -8.05395, -7.9456, -7.84244, -7.76908, -7.63907,
        -7.51551, -7.40043, -7.34394, -7.21731,  -7.14637,
        -7.04336, -6.95543, -6.93367, -6.91075, -6.87694,
        -6.75941, -6.75941, -6.75941, -6.75941, -6.75941,
        -6.75941, -6.75941
    };
    this->param2_cdtez = new TH1D("param2_cdtez","param2_cdtez", nbin_param2, min_param2, max_param2);
    for ( int iparam2_cdtez=0; iparam2_cdtez<nbin_param2; ++iparam2_cdtez )
    {
        this->param2_cdtez->SetBinContent(iparam2_cdtez+1, cdteposz_param2[iparam2_cdtez]);
    }

    this->si_ee_up = new TF1( "si_ee_up", "1.3743723 * x - 0.54678582", 0.0, 1000.0 );
    this->si_ee_lo = new TF1( "si_ee_lo", "1.1449777 * x - 0.88609000", 0.0, 1000.0 );

    return anl::ANL_OK;
}

int ProjectConeETCC::mod_ana()
{
    if ( !this->event.Next() ) return anl::ANL_LOOP;
    return anl::ANL_OK;

    auto nhit_si = this->event.merged_si_nhit;
    auto nhit_ct = this->event.n_reconst;

    if ( nhit_si != 1 || nhit_ct != 1 )
        return anl::ANL_SKIP;
    else
        evs::set("Si-CdTe 2hits event");

    auto si = ProjectConeETCC::Hit(
            this->event.init_pos_cmos_detx,
            this->event.init_pos_cmos_dety,
            this->event.init_pos_cmos_detz,
            this->event.merged_epi_si[0]
        );
    auto cdte = ProjectConeETCC::Hit(
            this->event.cdte_detx[0],
            this->event.cdte_dety[0],
            this->event.cdte_detz[0],
            this->event.reconst_epi[0]
        );

    if ( si.Energy()<=0.0 || cdte.Energy()<=0.0 )
        return anl::ANL_SKIP;
    // evs::set("Si-CdTe 2hits event");

    auto si_energy_on_va = this->event.epi_total;
    if ( si_energy_on_va < this->si_ee_lo->Eval( si.Energy() )
        || this->si_ee_up->Eval( si.Energy() ) < si_energy_on_va )
        return anl::ANL_SKIP;
    else
        evs::set("E-consistent Si-CMOS");

    if ( !this->IsInEnergyWindow( si.Energy()+cdte.Energy() ) )
        return anl::ANL_SKIP;
    else
        evs::set("SC-2hits in E-window");

    if ( !IsInThetaRange( si, cdte ) )
        return anl::ANL_SKIP;
    else
        evs::set("SC-2hits in Theta-range");

    if ( is_event_list_only ) return anl::ANL_OK;

    // auto is_filled_voxels = this->projection( image, si, cdte );
    // if ( is_filled_voxels ) this->fill();

    return anl::ANL_OK;
}

int ProjectConeETCC::mod_endrun()
{
    cout << mod_name2() << "::mod_endrun" << endl;

    output_file->cd();
    output_tree->Write();
    output_file->Close();

    input_file->Close();
    return anl::ANL_OK;
}

int ProjectConeETCC::DefineBranch(TTree* tree)
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

    // if ( event.ExistBranch("coin_eventid") )
    //     output_tree->Branch( "coin_eventid", &event.coin_eventid, "coin_eventid/L" );
    //
    // if ( event.ExistBranch("coin_delta_t") )
    //     output_tree->Branch( "coin_delta_t", &event.coin_delta_t, "coin_delta_t/I" );

    if ( is_event_list_only ) return anl::ANL_OK;

    if ( !tree->Branch( "response", "TH3F", &image ) ) {
        cout << "Creating a branch of TH3F is failed." << endl;
        return anl::ANL_NG;
    }

    return anl::ANL_OK;
}
int ComptreeEvent::SetBranchAddress(TTree* tree)
{
    auto branch_list = { "merged_si_nhit", "n_reconst", "epi_total", "reconstructed", "phi_cmos_det" };
    for ( auto branch : branch_list )
        if ( !this->ExistBranch(tree, branch) ) return anl::ANL_NG;

    static const int alloc_size = 10;
    this->epi1.resize(alloc_size);
    this->epi2.resize(alloc_size);
    this->reconst_epi.resize(alloc_size);
    this->cdte_detx.resize(alloc_size);
    this->cdte_dety.resize(alloc_size);
    this->cdte_detz.resize(alloc_size);

    tree->SetBranchAddress( "merged_si_nhit", &(this->merged_si_nhit) );
    tree->SetBranchAddress( "merged_epi_si", this->merged_epi_si.data() );
    tree->SetBranchAddress( "n_reconst", &(this->n_reconst) );
    tree->SetBranchAddress( "epi1", this->epi1.data() );
    tree->SetBranchAddress( "epi2", this->epi2.data() );
    tree->SetBranchAddress( "reconst_epi", this->reconst_epi.data() );
    tree->SetBranchAddress( "cdte_detx", this->cdte_detx.data() );
    tree->SetBranchAddress( "cdte_dety", this->cdte_dety.data() );
    tree->SetBranchAddress( "cdte_detz", this->cdte_detz.data() );
    tree->SetBranchAddress( "epi_total", &(this->epi_total) );
    tree->SetBranchAddress( "reconstructed", &(this->reconstructed) );
    tree->SetBranchAddress( "init_pos_cmos_detx", &(this->init_pos_cmos_detx) );
    tree->SetBranchAddress( "init_pos_cmos_dety", &(this->init_pos_cmos_dety) );
    tree->SetBranchAddress( "init_pos_cmos_detz", &(this->init_pos_cmos_detz) );
    tree->SetBranchAddress( "phi_cmos_det", &(this->phi_cmos_det) );

    this->nentries = tree->GetEntries();
    this->current_entry = -1;
    this->tree = tree;

    return 0;
}
