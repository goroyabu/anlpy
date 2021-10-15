/***
    @file   FilterComptree.hpp
    @date   2021/10/14
    @author Goro Yabu
***/

#include "FilterComptree.hpp"

#include <iostream>
using std::cout;
using std::endl;

#include <bnk.hpp>
#include <evs.hpp>

#include <TROOT.h>
#include <TSeqCollection.h>

FilterComptree::FilterComptree()
    : anl::VANL_Module("FilterComptree", "20211015"),
    input_file(nullptr),
    input_tree(nullptr),
    output_file(nullptr),
    output_tree(nullptr)
{
    define_parameter<std::string>("input_file", "input.root");
    define_parameter<std::string>("input_tree", "comptree");
    define_parameter<std::string>("output_file", "output.root");
    define_parameter<std::string>("output_tree", "comptree");

    define_parameter<int>("enable_reject_fluor", 1);

    define_parameter<double>("source_posx", 0.0);
    define_parameter<double>("source_posy", 0.0);
    define_parameter<double>("source_posz", 0.0);
    define_parameter<double>("detector_z_position", -41.0);

    define_parameter<double>("e_window_begin", 100.0);
    define_parameter<double>("e_window_end", 300.0);
    define_parameter<double>("e_threshold_si", 5.0);
    define_parameter<double>("e_threshold_cdte", 5.0);

    define_parameter<double>("theta_max_degree", 150.0);
    define_parameter<double>("eigen_ratio_threshold", 10.0);
    define_parameter<double>("pixel_ratio_threshold", 2.0);

}

FilterComptree::~FilterComptree()
{
    if ( this->output_file->IsOpen() ) {
        this->output_file->Close();
        cout << " - Closed : " << this->get_parameter<std::string>("output_file") << endl;
    }
    if ( this->input_file->IsOpen() ) {
        this->input_file->Close();
        cout << " - Closed : " << this->get_parameter<std::string>("input_file") << endl;
    }
}

int FilterComptree::mod_bgnrun()
{
    cout << " --- " << this->mod_name2() << "::mod_bgnrun" << endl;
    cout << endl;


    // ### ROOT FILE IO ###
    auto ifname = get_parameter<std::string>("input_file");
    this->input_file = new TFile( ifname.c_str() );
    if ( !this->input_file || this->input_file->IsZombie() ) {
        cout << "Opening " << ifname << " is failed." << endl;
        return anl::ANL_NG;
    }
    cout << ifname << " is opened." << endl;

    auto itname = get_parameter<std::string>("input_tree");
    this->input_tree = (TTree*)this->input_file->Get( itname.c_str() );
    if ( !this->input_tree ) {
        cout << itname << " is not found." << endl;
        return anl::ANL_NG;
    }
    cout << itname << " is loaded." << endl;

    this->event.SetBranchAddress(this->input_tree);
    evs::put("ANL_totalevents", this->event.GetEntries());

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
    this->output_tree = this->input_tree->CloneTree(0);
    this->output_tree->SetNameTitle( otname.c_str(), otname.c_str() );
    this->define_branch( this->output_tree );
    this->define_histogram();
    // ### ROOT FILE IO END ###

    // ### DEFINE Z & ENERGY CORRECTION FUNCTION ###
    this->param2_cdtez = this->define_zparam_function();
    this->si_ee_up = new TF1( "si_ee_up", "1.3743723 * x - 0.54678582", 0.0, 1000.0 );
    this->si_ee_lo = new TF1( "si_ee_lo", "1.1449777 * x - 0.88609000", 0.0, 1000.0 );

    // ### DEFINE EVS ###
    evs::define("Si_CdTe_2Hits_Event");
    evs::define("E_Consistent_Si_CMOS");
    evs::define("SC_2hits_In_E_Window");
    evs::define("SC_2hits_In_Theta_Range");
    evs::define("Direction_Is_Reconstruted");
    evs::define("Eigen_Value_Is_Over_Threshold");
    evs::define("Pixel_Ratio_Is_Over_Threshold");

    // ### DEFINE BNK ###
    bnk::define<int>( "num_hits" );
    bnk::define<double>( "hit1_energy" );
    bnk::define<double>( "hit1_posx" );
    bnk::define<double>( "hit1_posy" );
    bnk::define<double>( "hit1_posz" );
    bnk::define<double>( "hit2_energy" );
    bnk::define<double>( "hit2_posx" );
    bnk::define<double>( "hit2_posy" );
    bnk::define<double>( "hit2_posz" );
    bnk::define<double>( "totalenergy" );
    bnk::define<double>( "theta_kine" );
    bnk::define<double>( "theta_geom" );
    bnk::define<double>( "phi_esti" );
    bnk::define<double>( "phi_geom" );
    bnk::define<double>( "arm" );
    bnk::define<double>( "spd" );

    // ### ANALYSIS PARAMETERS ###
    cout << endl;
    this->enable_reject_fluor = get_parameter<int>("enable_reject_fluor");

    auto source_posx = this->get_parameter<double>("source_posx");
    auto source_posy = this->get_parameter<double>("source_posy");
    auto source_posz = this->get_parameter<double>("source_posz");
    this->source_position = TVector3( source_posx, source_posy, source_posz );
    cout << " - Given Source Position : (" << source_posx << ", " << source_posy << ", " << source_posz << ")" << endl;

    this->detector_z_position = get_parameter<double>("detector_z_position");
    cout << " - Detector-Z : " << this->detector_z_position << "mm" << endl;

    e_window_begin = get_parameter<double>("e_window_begin");
    e_window_end   = get_parameter<double>("e_window_end");
    cout << " - Energy-window : " << e_window_begin << ", " << e_window_end << "keV" << endl;

    e_threshold_si = get_parameter<double>("e_threshold_si");
    e_threshold_cdte = get_parameter<double>("e_threshold_cdte");
    cout << " - Threshold for Si-DSD   : " << e_threshold_si << "keV" << endl;
    cout << " - Threshold for CdTe-DSD : " << e_threshold_cdte << "keV" << endl;

    this->theta_max_degree = get_parameter<double>("theta_max_degree");
    cout << " - Maximum scattering angle : " << this->theta_max_degree << "(deg)" << endl;

    this->eigen_ratio_threshold = get_parameter<double>("eigen_ratio_threshold");
    this->pixel_ratio_threshold = get_parameter<double>("pixel_ratio_threshold");
    cout << " - Threshold for Eigen value ratio : " << this->eigen_ratio_threshold << endl;
    cout << " - Threshold for Pixel value ratio : " << this->pixel_ratio_threshold << endl;

    cout << endl;
    return anl::ANL_OK;
}

int FilterComptree::mod_ana()
{
    if ( !this->event.Next() ) return anl::ANL_LOOP;

    auto nhit_si = this->event.merged_si_nhit;
    auto nhit_ct = this->event.n_reconst;

    if ( nhit_si==1 && nhit_ct==1 ) evs::set("Si_CdTe_2Hits_Event");
    else return anl::ANL_SKIP;

    auto si = ComptonHit(
            this->event.init_pos_cmos_detx,
            this->event.init_pos_cmos_dety,
            this->event.init_pos_cmos_detz,
            this->event.merged_epi_si[0],
            this->event.phi_cmos_det
        );
    auto cdte = ComptonHit(
            this->event.cdte_detx[0],
            this->event.cdte_dety[0],
            this->event.cdte_detz[0],
            this->event.reconst_epi[0],
            this->event.phi_cmos_det
        );

    auto energy_sum = si.Energy()+cdte.Energy();
    this->num_hits = nhit_si + nhit_ct;
    this->hit1_detector = 0;
    this->hit1_energy = si.Energy();
    this->hit1_posx = si.Postion().X();
    this->hit1_posy = si.Postion().Y();
    this->hit1_posz = si.Postion().Z();
    this->hit2_detector = 1;
    this->hit2_energy = cdte.Energy();
    this->hit2_posx = cdte.Postion().X();
    this->hit2_posy = cdte.Postion().Y();
    this->hit2_posz = cdte.Postion().Z();
    this->totalenergy = energy_sum;

    bnk::put<int>( "num_hits", this->num_hits );
    bnk::put<double>( "hit1_energy", this->hit1_energy );
    bnk::put<double>( "hit1_posx", this->hit1_posx );
    bnk::put<double>( "hit1_posy", this->hit1_posy );
    bnk::put<double>( "hit1_posz", this->hit1_posz );
    bnk::put<double>( "hit2_energy", this->hit2_energy );
    bnk::put<double>( "hit2_posx", this->hit2_posx );
    bnk::put<double>( "hit2_posy", this->hit2_posy );
    bnk::put<double>( "hit2_posz", this->hit2_posz );
    bnk::put<double>( "totalenergy", this->totalenergy );

    auto vec_norm_vertical = TVector3( 0, 1, 0 );
    auto scat  = si.Postion();
    auto abso  = cdte.Postion();
    auto vec_cone_axis = (scat - abso).Unit();
    auto angle_theta_rad = this->ComptonTheta( si.Energy(), cdte.Energy() );
    auto angle_phi_rad = TVector2::Phi_mpi_pi( si.Phi() + TMath::Pi()*0.5 );
    this->theta_kine = angle_theta_rad;
    this->phi_esti = angle_phi_rad;

    auto vec_norm_z = TVector3( 0, 0, -1 );
    auto source_to_scat = scat - this->source_position;
    auto vec_norm_inci = source_to_scat;
    vec_norm_inci = vec_norm_inci.Unit();
    auto scat_to_abso = abso - scat;
    auto angle_theta_rad_geom = source_to_scat.Angle(scat_to_abso);
    auto arm = (angle_theta_rad - angle_theta_rad_geom)/TMath::Pi()*180.0;
    this->theta_geom = angle_theta_rad_geom;
    this->angle_inci = vec_norm_inci.Angle( vec_norm_z );
    bnk::put<double>( "theta_kine", this->theta_kine );
    bnk::put<double>( "theta_geom", this->theta_geom );
    bnk::put<double>( "arm", arm );

    this->vector_incident[0] = vec_norm_inci.X();
    this->vector_incident[1] = vec_norm_inci.Y();
    this->vector_incident[2] = vec_norm_inci.Z();
    this->vector_photon[0] = vec_cone_axis.X();
    this->vector_photon[1] = vec_cone_axis.Y();
    this->vector_photon[2] = vec_cone_axis.Z();
    auto vec_norm_elec = this->ElectronVector( vec_norm_inci, vec_cone_axis, si.Energy(), cdte.Energy() );
    this->vector_electron[0] = vec_norm_elec.X();
    this->vector_electron[1] = vec_norm_elec.Y();
    this->vector_electron[2] = vec_norm_elec.Z();
    auto [ alpha, beta ] = this->VectorToAlphaBeta( vec_norm_elec );
    this->angle_electron_on_plane = alpha;
    this->angle_electron_vertical = beta;

    auto vec_axis_to_src_plane = vec_cone_axis.Unit();
    vec_axis_to_src_plane *= this->source_position.Z() - scat.Z();
    auto pos_axis_on_src_plane = scat + vec_axis_to_src_plane;
    auto vec_phi_on_src_plane = this->source_position - pos_axis_on_src_plane;
    auto angle_phi_rad_geom = TVector2::Phi_mpi_pi( vec_phi_on_src_plane.DeltaPhi( vec_norm_vertical ) );
    auto spd = (angle_phi_rad - angle_phi_rad_geom)/TMath::Pi()*180;
    this->phi_geom = angle_phi_rad_geom;
    bnk::put<double>( "phi_esti", this->phi_esti );
    bnk::put<double>( "phi_geom", this->phi_geom );
    bnk::put<double>( "spd", spd );

    this->beta_vs_alpha_angle_electron->Fill(
        this->angle_electron_on_plane/TMath::Pi()*180.0,
        this->angle_electron_vertical/TMath::Pi()*180.0
    );
    this->angle_distribution_electron->Fill(
        TMath::Cos(this->angle_electron_on_plane)*this->angle_electron_vertical/TMath::Pi()*180,
        TMath::Sin(this->angle_electron_on_plane)*this->angle_electron_vertical/TMath::Pi()*180
    );

    auto si_energy_on_va = this->event.epi_total;
    if ( this->si_ee_lo->Eval( si.Energy() < si_energy_on_va )
        &&  si_energy_on_va < this->si_ee_up->Eval( si.Energy() )) {
        evs::set("E_Consistent_Si_CMOS");
        this->is_consis_si_energy = true;
    }
    else this->is_consis_si_energy = false;

    if ( this->IsInEnergyWindow( energy_sum ) ) {
        evs::set("SC_2hits_In_E_Window");
        this->is_in_energy_range = true;
    }
    else this->is_in_energy_range = false;

    if ( this->IsInThetaRange( si.Energy(), cdte.Energy()) ) {
        evs::set("SC_2hits_In_Theta_Range");
        this->is_in_theta_range = true;
    }
    else this->is_in_theta_range = false;

    if ( this->event.reconstructed == true )
        evs::set("Direction_Is_Reconstruted");

    if ( this->eigen_ratio_threshold < this->event.eigen_ratio ){
        evs::set("Eigen_Value_Is_Over_Threshold");
        this->is_over_thre_eigen = true;
    }
    else this->is_over_thre_eigen = false;

    if ( this->pixel_ratio_threshold <
        this->event.sum_pixel_value_around_end/this->event.sum_pixel_value_around_init ) {
        evs::set("Pixel_Ratio_Is_Over_Threshold");
        this->is_over_thre_pixel_ratio = true;
    }
    else this->is_over_thre_pixel_ratio = false;

    this->output_tree->Fill();

    return anl::ANL_OK;
}

int FilterComptree::mod_endrun()
{
    cout << mod_name2() << "::mod_endrun" << endl;

    this->output_file->cd();

    this->output_tree->Write();
    this->beta_vs_alpha_angle_electron->Write();
    this->angle_distribution_electron->Write();

    // this->output_file->Close();
    // cout << " - Closed : " << this->get_parameter<std::string>("output_file") << endl;
    // this->input_file->Close();
    // cout << " - Closed : " << this->get_parameter<std::string>("input_file") << endl;

    return anl::ANL_OK;
}

int FilterComptree::define_branch(TTree* tree)
{
    tree->Branch( "num_hits", &num_hits, "num_hits/I" );
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

    tree->Branch( "is_consis_si_energy", &is_consis_si_energy, "is_consis_si_energy/O" );
    tree->Branch( "is_in_energy_range", &is_in_energy_range, "is_in_energy_range/O" );
    tree->Branch( "is_in_theta_range", &is_in_theta_range, "is_in_theta_range/O" );
    tree->Branch( "is_over_thre_eigen", &is_over_thre_eigen, "is_over_thre_eigen/O" );
    tree->Branch( "is_over_thre_pixel_ratio", &is_over_thre_pixel_ratio, "is_over_thre_pixel_ratio/O" );

    tree->Branch( "theta_kine", &theta_kine, "theta_kine/F" );
    tree->Branch( "theta_geom", &theta_geom, "theta_geom/F" );
    // tree->Branch( "theta_elec", &theta_elec, "theta_elec/F" );
    tree->Branch( "phi_esti", &phi_esti, "phi_esti/F" );
    tree->Branch( "phi_geom", &phi_geom, "phi_geom/F" );
    tree->Branch( "de_over_dx", &de_over_dx, "de_over_dx/F" );

    tree->Branch( "angle_inci", &angle_inci, "angle_inci/F" );
    tree->Branch( "prod_inci_phi", &prod_inci_phi, "prod_inci/F" );
    tree->Branch( "sum_epi_around_init",
        &sum_epi_around_init, "sum_epi_around_init/F");
    tree->Branch( "sum_epi_forward_init",
        &sum_epi_forward_init, "sum_epi_forward_init/F");

    vector_incident.resize(3);
    vector_photon.resize(3);
    vector_electron.resize(3);
    tree->Branch( "vector_incident", vector_incident.data(), "vector_incident[3]/F" );
    tree->Branch( "vector_photon", vector_photon.data(), "vector_photon[3]/F" );
    tree->Branch( "vector_electron", vector_electron.data(), "vector_electron[3]/F" );
    tree->Branch( "angle_electron_on_plane", &angle_electron_on_plane, "angle_electron_on_plane/F" );
    tree->Branch( "angle_electron_vertical", &angle_electron_vertical, "angle_electron_vertical/F" );

    return anl::ANL_OK;
}
int FilterComptree::define_histogram()
{
    this->beta_vs_alpha_angle_electron = new TH2D(
        "beta_vs_alpha_angle_electron", ";alpha(deg);beta(deg)",
        360, -180, 180, 360, 0, 90
    );
    this->angle_distribution_electron = new TH2D(
        "angle_distribution_electron", ";cos(alpha)*beta;sin(alpha)*beta",
        180, -90, 90, 180, -90, 90
    );
    return anl::ANL_OK;
}
TH1D* FilterComptree::define_zparam_function()
{
    const static int nbin_param2 = 32;
    const static double min_param2 = -0.2;
    const static double max_param2 = 0.1;
    const static double cdteposz_param2[32] = {
        -8.71105, -8.63401, -8.59368, -8.56371, -8.45148,
        -8.40101, -8.29345, -8.25342, -8.15028, -8.10914,
        -8.05395, -7.9456, -7.84244, -7.76908, -7.63907,
        -7.51551, -7.40043, -7.34394, -7.21731,  -7.14637,
        -7.04336, -6.95543, -6.93367, -6.91075, -6.87694,
        -6.75941, -6.75941, -6.75941, -6.75941, -6.75941,
        -6.75941, -6.75941
    };
    auto histo = new TH1D("param2_cdtez","param2_cdtez", nbin_param2, min_param2, max_param2);
    for ( int iparam2_cdtez=0; iparam2_cdtez<nbin_param2; ++iparam2_cdtez )
    {
        histo->SetBinContent(iparam2_cdtez+1, cdteposz_param2[iparam2_cdtez]);
    }
    return histo;
}

int ComptreeReader::SetBranchAddress(TTree* tree)
{
    auto branch_list = { "merged_si_nhit", "n_reconst", "epi_total", "reconstructed", "phi_cmos_det" };
    for ( auto branch : branch_list )
        if ( !this->ExistBranch(tree, branch) ) return anl::ANL_NG;

    static const int alloc_size = 150;
    this->merged_epi_si.resize(alloc_size);
    this->epi1.resize(alloc_size);
    this->epi2.resize(alloc_size);
    this->reconst_epi.resize(alloc_size);
    this->cdte_detx.resize(alloc_size);
    this->cdte_dety.resize(alloc_size);
    this->cdte_detz.resize(alloc_size);
    this->epi_pixel_value.resize(alloc_size);
    this->cmos_detx.resize(alloc_size);
    this->cmos_dety.resize(alloc_size);

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
    tree->SetBranchAddress( "n_pixel", &(this->n_pixel) );
    tree->SetBranchAddress( "epi_pixel_value", this->epi_pixel_value.data() );
    tree->SetBranchAddress( "cmos_detx", this->cmos_detx.data() );
    tree->SetBranchAddress( "cmos_dety", this->cmos_dety.data() );
    tree->SetBranchAddress( "min_cmos_detx", &(this->min_cmos_detx) );
    tree->SetBranchAddress( "min_cmos_dety", &(this->min_cmos_dety) );
    tree->SetBranchAddress( "sum_pixel_value_around_init", &(this->sum_pixel_value_around_init) );
    tree->SetBranchAddress( "sum_pixel_value_around_end", &(this->sum_pixel_value_around_end) );
    tree->SetBranchAddress( "eigen_ratio", &(this->eigen_ratio) );
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